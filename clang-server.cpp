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
#include <pthread.h>
#include <vector>

class CompilationThread
{
public:
    CompilationThread()
    {
        ++count;
    }

    ~CompilationThread()
    {
        --count;
    }

    void run()
    {
//        printf("Thread %ld: compilation %d %s\n", m_thread, m_clientPid, m_str);
        printf("Thread %ld: compiling %d %d\n", m_thread, m_clientPid, m_argc);
        usleep(10000000);
        finalize();
    }

    bool start(char *str)          
    { 
        if(!setStr(str)) {
            printf("Parsing error in %s", str);
            return false;
        }
        return pthread_create(&m_thread, NULL, CompilationThread::thread_func, (void*)this) == 0;
    }

    bool wait()
    {
        return pthread_join(m_thread, NULL) == 0;
    }

    void finalize()
    {
        if(m_clientPid > 0) {
            printf("Server: killing %d\n", m_clientPid);
            kill(m_clientPid, SIGUSR1);
        }
        free(m_str);
        m_str = 0;
        delete [] m_argv;
        m_argv = 0;
    }

private:
    CompilationThread(const CompilationThread& copy); // copy constructor denied
    static void *thread_func(void *d)   
    { 
        ((CompilationThread *)d)->run(); 
    }
    
    bool setStr(char *str)
    {
        if(!str)
            return false;
        //printf("Thread %d: setStr %s\n", m_thread, str);
        m_str = strdup(str);
        if(!m_str)
            return false;
        int i = 0;
        char *argc_start = 0;
        for(char *c = m_str; *c != '\0'; ++c) {
            if(*c != ';')
                continue;
            *c = '\0';
            if(i == 0) {
                m_clientPid = atoi(m_str);
                if(m_clientPid <= 0)
                    return false;
                m_cwd = c+1;
            } else if(i == 1) {
                argc_start = c+1;
            } else if(i == 2) {
                m_argc = atoi(argc_start);
                if(!m_argc)
                    return false;
                m_argv = new char*[m_argc];
                m_argv[0] = c+1;
            } else {
                // i >= 3, m_argv[0] was assigned above
                m_argv[i-2] = c+1;
            }            
            ++i;
        }
//        printf("Thread %ld: %d, %s, %d, %s, %s, %s\n", m_thread, m_clientPid, m_cwd, m_argc, m_argv[0], m_argv[1], m_argv[m_argc-1]);
        return true;
    }
    
    pthread_t m_thread;
    char *m_str;
    int m_clientPid;
    char *m_cwd;
    int m_argc;
    char **m_argv;

    static int count;
};
 
#define BUF_SIZE 10240
const int MAX_WAIT = 600;
int CompilationThread::count = 0;

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

