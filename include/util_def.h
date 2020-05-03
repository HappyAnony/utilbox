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
#ifndef __UTIL_DEF_H__
#define __UTIL_DEF_H__

/*
 * offsetof_field - return the offset from @TYPE head to @MEMBER field
 *
 * @TYPE: the structure containing the @MEMBER field
 * @MEMBER: the field name
 * */
#define offsetof_field(TYPE, MEMBER)  ((size_t)&((TYPE *)0)->MEMBER)


/*
 * sizeof_field - return the size of @MEMBER field embed in @TYPE structure
 *
 * @TYPE: the structure containing the @MEMBER field
 * @MEMBER: the field name
 * */
#define sizeof_field(TYPE, MEMBER) sizeof((((TYPE *)0)->MEMBER))


/*
 * entry_of - cast a member of a structure out to the structure address
 * @ptr: the pointer to the @member
 * @type: the type of structure containing of the @member
 * @member: the name of the member embed in the @type structure
 * */
#define entry_of(ptr, type, member) ({ \
    void *__tmptr = (void*)(ptr);        \
    ((type *)(__tmptr - offsetof_field(type, member))) ;})

#endif /* __UTIL_DEF_H__ */
