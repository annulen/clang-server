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
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>

#define BUF_SIZE 10240

void sig_handler(int sig) {
    printf("Client %d exit\n", getpid());
    signal(SIGUSR1, sig_handler);
}

int main(int argc, char **argv)
{
    int fifo, lock;
    char cmd[BUF_SIZE];
    char cwd[4096];
    int need_write_lock = 1;
    pid_t mypid;
    int i;

    signal(SIGUSR1, sig_handler);
    mypid = getpid();
    fprintf(stderr, "Client %d start\n", mypid);
    getcwd(cwd, 4096);
    if(!cwd) {
        fprintf(stderr, "Fatal error: cannot getcwd()!\n");
        return 1;
    }
    sprintf(cmd, "%d;%s", mypid, cwd);
    for(i=0; i<argc; ++i) {
        if(strlen(cmd)+strlen(argv[i])+1 > BUF_SIZE) {
            fprintf(stderr, "Fatal error: argument list is too long!\n");
            return 2;
        }
        strcat(cmd, ";");
        strcat(cmd, argv[i]);
    }
    strcat(cmd, "\n");

    //printf("CLANGSERVERPIPE=%s\n", getenv("CLANGSERVERPIPE"));
    if((fifo = open(getenv("CLANGSERVERPIPE"), O_WRONLY)) == -1) {
        fprintf(stderr, "Fatal error: cannot open CLANGSERVERPIPE!\n");
        return 1;
    }
    
    /* Need atomic write! */
/*    fprintf(stderr, "Client %d waiting lock\n", mypid);
    if(flock(fifo, LOCK_EX) == -1) {
        fprintf(stderr, "Fatal error: cannot lock pipe!\n");
        return 3;
    }
    fprintf(stderr, "Client %d lock\n", mypid);*/
    write(fifo, cmd, strlen(cmd));
//    fprintf(stderr, "Client %d sleep\n", mypid);
//    usleep(1000000);

/*    if(flock(fifo, LOCK_UN) == -1) {
        fprintf(stderr, "Fatal error: cannot unlock pipe!\n");
        return 4;
    }
    fprintf(stderr, "Client %d unlock\n", mypid);*/
    close(fifo);
    //fprintf(stderr, "Client %d closed pipe\n", mypid);
    pause();
    return 0;
}

