/*
 * clang-client - Client of Clang compilation server
 * Copyright (c) 2011 Konstantin Tokarev.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 * 
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void sigterm_handler(int sig) {
    printf("Client exit\n");
    signal(sig, SIG_IGN);
}

int main(int argc, char **argv)
{
    FILE *serverpipe;
    char cmd[PIPE_BUF];
    int i;

    printf("Client start\n");
    sprintf(cmd, "%d;", getpid());
    if(strstr(argv[0], "clang++"))
        strcat(cmd, "++");
    strcat(cmd, ";");
    for(i=1; i<argc; ++i) {
        if(strlen(cmd)+strlen(argv[i])+1 > PIPE_BUF) {
            printf("Fatal error: too many args!\n");
            return 2;
        }
        strcat(cmd, argv[i]);
        strcat(cmd, ";");
    }
    
    //printf("CLANGSERVERPIPE=%s\n", getenv("CLANGSERVERPIPE"));
    serverpipe = fopen(getenv("CLANGSERVERPIPE"), "w");
    if(!serverpipe) {
        printf("Fatal error: CLANGSERVERPIPE not specified!\n");
        return 1;
    }
    printf("Client opened pipe\n");
/*    if(strlen(cmd) > PIPE_BUF) {
        printf("Fatal error: %s command line is longer than %d characters\n"
               "Cannot perform atomic write to Unix pipe\n",
               argv[0], PIPE_BUF);
        return 2;
    }*/
    fprintf(serverpipe, "%s\n", cmd);
    fclose(serverpipe);
    signal(SIGTERM, sigterm_handler);
    pause();
    return 0;
}

