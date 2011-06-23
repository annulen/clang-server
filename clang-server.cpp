/*
 * clang-server - Simple compilation server for clang 
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

#include <cstdio>
#include <cstdlib>
#include <limits.h>
#include <signal.h>
#include <cstring>
#include <string>
/*#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>*/

using namespace std;

const int MAX_WAIT = 600;

void sigterm_handler(int sig) {
    printf("Stopping compilation server...\n");
    // Stop current jobs
    // List queued jobs
    // Kill all waiting clients
    exit(0);
}
void sigsegv_handler(int sig) {
    printf("server crashed\n");
    // Kill all waiting clients
    exit(0);
}

/*void waitForNewData(int fd)
{
    fd_set rfds;
    struct timeval tv;
    int retval;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_sec = MAX_WAIT;
    tv.tv_usec = 0;

    retval = select(fd+1, &rfds, NULL, NULL, &tv);

    if (retval)
        printf("Данные доступны.\n");
    else
        printf("Данные не появились в течение 600 секунд.\n");
}*/

void process(char *str)
{
    printf("Server: %s", str);
    char *token = strtok(str, ";");
    if(token) {
//    printf("Server: killing %s\n", token);
        kill(atol(token), SIGTERM);
    }
}

int main(int argc, char **argv)
{
    FILE *input;
    string args;
    char buf[PIPE_BUF];
    char c;

    //printf("CLANGSERVERPIPE=%s\n", getenv("CLANGSERVERPIPE"));
    printf("Server start\n");
    input = fopen(getenv("CLANGSERVERPIPE"), "r");
    if(!input) {
        printf("Fatal error: CLANGSERVERPIPE not specified!\n");
        return 1;
    }
/*    do {
        if(!fgets(buf, 1024, input))
            return;
    } while((c = fgetc (stream)) != EOF);*/
    signal(SIGTERM, sigterm_handler);
    signal(SIGSEGV, sigsegv_handler);
    while(true) {
        while(fgets(buf, PIPE_BUF, input)) 
            process(buf);
        //while ((c = fgetc (input)) != EOF)
          //  putchar(c);
        printf("Server: EOF\n");
    //    waitForNewData(fileno(input));
        freopen(getenv("CLANGSERVERPIPE"), "r", input);
        printf("Server: Reopened\n");
    }
    
    fclose(input);
    printf("Server stop\n");
    return 0;
}
