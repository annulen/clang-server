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

#ifndef COMPILATIONTHREAD_H
#define COMPILATIONTHREAD_H

#include <pthread.h>
#include <semaphore.h>
#include <cstdio>

extern sem_t sem;

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

    void run();

    bool start(char *str)          
    {
        if(!setStr(str)) {
            printf("Parsing error in %s", str);
            return false;
        }
        printf("Thread waiting...\n");
        sem_wait(&sem);
        printf("Thread starting...\n");
        return pthread_create(&m_thread, NULL, CompilationThread::thread_func, (void*)this) == 0;
    }

    bool wait()
    {
        return pthread_join(m_thread, NULL) == 0;
    }

    void finalize();

    static int count;
private:
    CompilationThread(const CompilationThread& copy); // copy constructor denied
    bool setStr(char *str);
    static void *thread_func(void *d)   
    { 
        ((CompilationThread *)d)->run(); 
    }
    
    pthread_t m_thread;
    char *m_str;
    int m_clientPid;
    char *m_cwd;
    int m_argc;
    char **m_argv;

};

#endif // COMPILATIONTHREAD_H
