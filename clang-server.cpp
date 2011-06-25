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

#include "compilationthread.h"

#include <cstdio>
#include <cstdlib>
#include <limits.h>
#include <signal.h>
#include <cstring>
#include <string>
#include <vector>

#define BUF_SIZE 10240
const int MAX_WAIT = 600;
int CompilationThread::count = 0;
sem_t *sem;

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

int main(int argc, char **argv)
{
    FILE *input;
    char buf[BUF_SIZE];
    char c;
    std::vector<CompilationThread*> compileThreads;

    //printf("CLANGSERVERPIPE=%s\n", getenv("CLANGSERVERPIPE"));
    printf("Server start\n");
    input = fopen(getenv("CLANGSERVERPIPE"), "r");
    if(!input) {
        printf("Fatal error: CLANGSERVERPIPE not specified!\n");
        return 1;
    }
    signal(SIGTERM, sigterm_handler);
    signal(SIGSEGV, sigsegv_handler);
    if((sem = sem_open("/clang-server-thread-limit", O_CREAT | O_EXCL, 600, 3)) == SEM_FAILED) {
        perror("sem_open");
        return 2;
    }


    compileThreads.reserve(16);
    while(true) {
        while(fgets(buf, BUF_SIZE, input)) {
            if(strncmp(buf, "END", 3) == 0)
                goto the_end;
            CompilationThread *thread = new CompilationThread;
            if(!thread->start(buf)) {
                printf("Server: Failed to start compilation!\n");
                thread->finalize();
                delete thread;
            } else {
                compileThreads.push_back(thread);
            }
        }
        printf("Server: EOF\n");
        freopen(getenv("CLANGSERVERPIPE"), "r", input);
        printf("Server: Reopened\n");
    }
the_end:

    // Cleanup threads
    for(int i=0; i<compileThreads.size(); ++i) {
        if(!compileThreads[i]->wait()) {
            fprintf(stderr, "Failed to join thread\n");
            compileThreads[i]->finalize();
        }
        delete compileThreads[i];
    }
    fclose(input);
    printf("Server stop\n");
    return 0;
}

