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

#include <stdio.h>
#include <utilboxConfig.h>

#if (UTIL_CONF_ENABLE)
#include <conf/util_json.h>
#include <conf/util_json_parser.h>
#endif

#if (UTIL_SYSTEM_ENABLE)
#include <system/util_shell_exec.h>
#endif

static void utilbox_banner()
{
    printf("Welcome to utilbox!\n");
}

int main()
{
    utilbox_banner();

#if (UTIL_CONF_ENABLE)
    struct json_parser* json_parser = NULL;
    json_parser = json_parser_create(NULL);
    json_parser_free(json_parser);
#endif

#if (UTIL_SYSTEM_ENABLE)
    char cmd_str[1024] = {};
    char res_str[1024] = {};
    snprintf(cmd_str, sizeof(cmd_str), "ls");
    if (0 == popen_exec_shell_cmd(cmd_str, res_str, sizeof(res_str)))
        printf("%s\n", res_str);
    else
        printf("%s exec fail\n", cmd_str);
#endif

    return 0;
}
