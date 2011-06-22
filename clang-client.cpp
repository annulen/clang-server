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

#include <cstdio>
#include <cstdlib>
#include <limits.h>
#include <string.h>
#include <sstream>

using namespace std;

int main(int argc, char **argv)
{
    FILE *serverpipe;
    ostringstream out;
    const char *cmd;

    printf("Client start\n");
    //printf("CLANGSERVERPIPE=%s\n", getenv("CLANGSERVERPIPE"));
    serverpipe = fopen(getenv("CLANGSERVERPIPE"), "w");
    if(!serverpipe) {
        printf("Fatal error: CLANGSERVERPIPE not specified!\n");
        return 1;
    }
    printf("Client opened pipe\n");
    for(int i=0; i<argc; ++i) {
        out << argv[i] << ' ';
    }
    cmd = out.str().c_str();
    if(strlen(cmd) > PIPE_BUF) {
        printf("Fatal error: %s command line is longer than %d characters\n"
               "Cannot perform atomic write to Unix pipe\n",
               argv[0], PIPE_BUF);
        return 2;
    }
    fprintf(serverpipe, "%s\n", cmd);
    fclose(serverpipe);
    printf("Client exit\n");
    return 0;
}
