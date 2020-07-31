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

#include <signal.h>
#include <trace/util_execinfo.h>

/*
 * 0 for backtrace dump
 * 1 for gdb dump
 */
static int dump_flag = 0;

static void gdb_dump()
{
	system("gdbcore.sh > /dev/null 2&>1");
}

static void backtrace_dump()
{
	void dump_printf(char *dump_info)
	{
		if (dump_info)
			printf("%s\n", dump_info);
	}
	util_stack_dump(dump_printf);
}

static void signal_dump_handler()
{
	if (dump_flag == 0)
		backtrace_dump();
	else
		gdb_dump();
	exit(-1);
}

static void signal_exit_handler()
{
	exit(0);
}
void util_exception_catch()
{
	/* ignore */
	signal(SIGPIPE, SIG_IGN);

	/* exit */
	signal(SIGTERM, signal_exit_handler);
	signal(SIGINT,  signal_exit_handler);

	/* dump */
	signal(SIGBUS,  signal_dump_handler);
	signal(SIGSEGV, signal_dump_handler);
	signal(SIGFPE,  signal_dump_handler);
	signal(SIGABRT, signal_dump_handler);
}
