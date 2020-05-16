/*
 * Copyright (c) 2020-2030, HappyAnony <13545108786@163.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <system/util_shell_exec.h>

int system_exec_shell_cmd(const char *cmd)
{
    if (cmd) {
        int ret = 0;
        ret = system(cmd);
        if ((-1 != ret) && WIFEXITED(ret) && (0 == WEXITSTATUS(ret)))
            return 0;
        else
            return -1;
    }
    return -1;
}

int popen_exec_shell_cmd(const char *cmd, char *res, unsigned int len)
{
    if (cmd == NULL || res == NULL) return -1;

    FILE    *fp = NULL;
    char    local[1024] = {0};

    if ((fp = popen(cmd, "r")) == NULL) return -1;

    if (fread(local, 1, sizeof(local) - 1, fp) <= 0) {
        pclose(fp);
        return -1;
    }

    /* escape the blank space */
    while (*local == ' ') snprintf(local, sizeof(local) - 1, "%s", local + 1);

    /* escape \r and \n */
    char *tmp = NULL;
    if ((tmp = strchr(local, '\r')) != NULL) *tmp = '\0';
    if ((tmp = strchr(local, '\n')) != NULL) *tmp = '\0';
    if (res) snprintf(res, len - 1, "%s", local);

    pclose(fp);

    return 0;
}
