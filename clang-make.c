/*
 * clang-make - Make utility wrapper for use with clang-client and clang-server
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

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int create_named_pipe(char **s)
{
    while (*s = (tempnam("/tmp", "clangmake"))) {
        if (mknod(*s, S_IFIFO|0600, 0) == 0)
            break;
        if (errno != EEXIST) {
            /* error */
            return 1;
        }
    }
    if (!*s) {
        /* error */
        return 2;
    }
    return 0;
}

int main(int argc, char **argv)
{
    int res;
    pid_t serverpid;
    char *pipename;
    char *makecmd;
    char defaultmake[] = "make";
    const char clangservercmd[] = "clang-server";

    // Check command line
    if(argc > 1) {
        if (!strncmp(argv[1], "-h", 2)) {
            printf("Usage: %s [make_command] <make_arguments>\n"
                   "If no make_command is specified, \"make\" is assumed.\n",
                   argv[0]);
            return 0;
        }
        makecmd = argv[1];
    } else {
        makecmd = defaultmake;
    }
    
    // Create temporary pipe
    res = create_named_pipe(&pipename);
    if(res != 0) {
        fprintf(stderr, "Cannot create named pipe!\n");
        return res;
    }
    
    // Set environment variable for clang-client
    if(setenv("CLANGSERVERPIPE", pipename, 1) == -1) {
        fprintf(stderr, "Cannot set CLANGSERVERPIPE variable!\n");
        return 3;
    }        

    // Start clang-server
    serverpid = fork();
    if(serverpid == 0) {
        printf("Forked!\n");
        if(execlp(clangservercmd, clangservercmd, (char*)0) == -1) {
            printf("Failed to start %s: %s\n", clangservercmd, strerror(errno));
        }
    } else {
        // Run make utility
        res = system(makecmd);
        if(res > 0) {
            fprintf(stderr, "%s failed!\n", makecmd);
            // Shutdown clang-server
            kill(serverpid, SIGTERM);
            // Remove pipe
            remove(pipename);
            return 5;
        }
    }
    free(pipename);
    return 0;
}
