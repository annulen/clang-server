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
#include <cstring>
#include <signal.h>
#include <unistd.h>
 
bool CompilationThread::setStr(char *str)
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

void CompilationThread::run()
{
    //        printf("Thread %ld: compilation %d %s\n", m_thread, m_clientPid, m_str);
    printf("Thread %ld: compiling %d %d\n", m_thread, m_clientPid, m_argc);
    usleep(10000000);
    finalize();
}

void CompilationThread::finalize()
{
    if(m_clientPid > 0) {
        printf("Server: killing %d\n", m_clientPid);
        kill(m_clientPid, SIGUSR1);
    }
    free(m_str);
    m_str = 0;
    delete [] m_argv;
    m_argv = 0;
    sem_post(sem);
}
