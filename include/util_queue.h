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
#ifndef __UTIL_QUEUE_H__
#define __UTIL_QUEUE_H__

#include <util_types.h>

/*
 * Simple circular queue implementation
 * */

 #define CQUEUE_INIT(length)            \
 {                                      \
     .length  = length,                 \
     .alloc   = 0,                      \
     .front   = 0,                      \
     .rear    = 0                       \
 }

#define CQUEUE_ASSIGN(name, length)     \
        struct cqueue name = CQUEUE_INIT(length)


/*
 * init_cqueue - Initialize a circular queue
 * @cqueue: cqueue structure to be initialized
 * @length: assign the length of circular queue
 * */
static inline void init_cqueue(struct cqueue *cqueue, unsigned int length)
{
    if (cqueue) {
        cqueue->length = length;
        cqueue->alloc  = 0;
        cqueue->front  = 0;
        cqueue->rear   = 0;
    }
}

static inline int util_cqueue_length(struct cqueue *cqueue)
{
	if (cqueue) {
		return ((cqueue->rear - cqueue->front + cqueue->length) % cqueue->length);
	}
	return -1;
}

static inline int util_cqueue_alloc(struct cqueue *cqueue)
{
	if (cqueue) {
		return (cqueue->alloc % cqueue->length);
	}
	return -1;
}

static inline int util_cqueue_idle(struct cqueue *cqueue)
{
	if (cqueue) {
		return ((cqueue->length - cqueue->alloc) % cqueue->length);
	}
	return -1;
}

static inline bool util_cqueue_is_full(struct cqueue *cqueue)
{
	return ((cqueue) ? ((cqueue->rear + 1) % cqueue->length == cqueue->front) : false);
}

static inline bool util_cqueue_is_empty(struct cqueue *cqueue)
{
	return ((cqueue) ? cqueue->rear == cqueue->front : false);
}

static inline int util_cqueue_push(struct cqueue *cqueue)
{
	if (cqueue) {
		if (util_cqueue_is_full(cqueue)) return -1;
		cqueue->alloc++;
		return (cqueue->rear = (cqueue->rear + 1) % cqueue->length);
	}
	return -1;
}

static inline int util_cqueue_pop(struct cqueue *cqueue)
{
	if (cqueue) {
		if (util_cqueue_is_empty(cqueue)) return -1;
		cqueue->alloc--;
		cqueue->front = (cqueue->front + 1) % cqueue->length;
		return cqueue->front - 1;
	}
	return -1;
}

static inline int util_cqueue_front(struct cqueue *cqueue)
{
	return ((cqueue) ? cqueue->front : -1);
}

static inline int util_cqueue_rear(struct cqueue *cqueue)
{
	return ((cqueue) ? cqueue->rear : -1);
}

#endif /* __UTIL_QUEUE_H__ */
