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
#ifndef __UTIL_LIST_H__
#define __UTIL_LIST_H__

#include <util_types.h>
#include <util_def.h>

/*
 * Simple doubly linked list implementation
 * */

#define LIST_NODE_INIT(name)  { &(name), &(name) }

#define LIST_NODE_ASSIGN(name) \
        struct list_node name = LIST_INIT(name)

/*
 * init_list_node - Initialize a list_node structure
 * @node: list_node structure to be initialized
 *
 * Initializes the list_node to point to itself. If it is a list header,
 * the result is an empty list.
 * */

static inline void init_list_node(struct list_node *node)
{
    if (node) node->prev = node->next = node;
}

#define LIST_NODE_MALLOC(name)                                                      \
({                                                                                  \
    struct list_node* name = (struct list_node *)malloc(sizeof(struct list_node));  \
    init_list_node(name);                                                           \
    name;                                                                           \
})

#define LIST_NODE_FREE(name)  if (name) free(name)


/* Insert a new node between two known consecutive nodes */
static inline void __util_list_add(struct list_node *new, struct list_node *prev,
                                   struct list_node *next)
{
    if (new && prev && next) {
        next->prev = new;
        new->next  = next;
        prev->next = new;
        new->prev  = prev;
    }
}

/*
 * util_list_insert - add a new node to list head
 * @head: list head node
 * @new: new list node to be added
 *
 * Insert a new node after the specified head node,
 * which is the equal of adding to the list head
 * This is good for implementing stacks(first in last out)
 * */
static inline void util_list_insert(struct list_node *head, struct list_node *new)
{
    if (head && new) __util_list_add(new, head, head->next);
}

/*
 * util_list_add_tail - add a new node to list tail
 * @head: list head node
 * @new: new list node to be added
 *
 * Insert a new node before the specified head node,
 * which is the equal of adding to the list tail
 * This is good for implementing queues(firt in first out)
 * */
static inline void util_list_add_tail(struct list_node *head, struct list_node *new)
{
    if (head && new) __util_list_add(new, head->prev, head);
}


/*
 * Delete a list node by making the prev&next nodes point to each other
 * */
static inline void __util_list_del(struct list_node *prev, struct list_node *next)
{
    if (prev && next) {
        next->prev = prev;
        prev->next = next;
    }
}

/*
 * util_list_del - delete node from list
 * @node: the node to delete from the list
 * */
static inline void util_list_del(struct list_node *node)
{
    if (node) __util_list_del(node->prev, node->next);
}

/*
 * util_list_del_init - delete node from list and reinitialize it
 * @node: the node tp delete from the list
 * */
static inline void util_list_del_init(struct list_node *node)
{
    if (node) {
        __util_list_del(node->prev, node->next);
        init_list_node(node);
    }
}

/*
 * util_list_move_head - delete @node from one list and as @head's head
 * @head: the head node that will precede below node
 * @node: the node to move
 * */
static inline void util_list_move_head(struct list_node *head,
                                       struct list_node *node)
{
    util_list_del(node);
    util_list_insert(head, node);
}

/*
 * util_list_move_tail - delete @node from ont list and as @head's tail
 * @head: the head node that will follow below ndoe
 * @node: the node to move
 * */
static inline void util_list_move_tail(struct list_node *head,
                                       struct list_node *node)
{
    util_list_del(node);
    util_list_add_tail(head, node);
}

/*
 * __util_list_replace - replace old node by new node
 * @old: the old node to be replaced
 * @new: the new node to be insert
 * */
static inline void __util_list_replace(struct list_node *old,
                                       struct list_node *new)
{
    if (old && new) {
        new->next = old->next;
        new->next->prev = new;
        new->prev = old->prev;
        new->prev->next = new;
    }
}

static inline void util_list_replace(struct list_node *old,
                                     struct list_node *new)
{
    __util_list_replace(old, new);
}

/*
 * util_list_replace_init - replace old node by new node and reinitialize the old node
 * @old: the old node to be replaced
 * @new: the new node to be insert
 * */
static inline void util_list_replace_init(struct list_node *old,
                                          struct list_node *new)
{
    __util_list_replace(old, new);
    init_list_node(old);
}

/*
 * util_list_swap - exchange the position of node1 and node2
 * @node1: the location to place node2
 * @node2: the location to place node1*/
static inline void util_list_swap(struct list_node *node1,
                                  struct list_node *node2)
{
    if (node1 && node2) {
        struct list_node *pos = node2->prev;
        util_list_del(node2);
        util_list_replace(node1, node2);
        if (pos == node1)
            pos = node2;
        util_list_insert(pos, node1);
    }
}


/*
 * util_list_is_empty - test whether a list is empty
 * @head: the head node of list to test
 * */
static inline bool util_list_is_empty(const struct list_node *head)
{
    return (head ? head->next == head : false);
}

static inline bool util_list_is_empty_careful(const struct list_node *head)
{
    if (!head) return false;
    struct list_node *next = head->next;
    return ((next == head) && (next == head->prev));
}

/*
 * util_list_is_full - test whether a list is full of @size
 * @head: the head node of list to test
 * @size: the size to test
 * */
static inline bool util_list_is_full(const struct list_node *head, unsigned int size)
{
    if (!head) return false;
    struct list_node *node = NULL;
    for (node = head->next; size; size--) {
        if (node == head)
            return false;
        else
            node = node->next;
    }
    return true;
}

/*
 * util_list_is_first - test whether @node is the first node in list @head
 * @head: the head node of list
 * @node: the node to test
 * */
static inline bool util_list_is_first(const struct list_node *head,
                                      const struct list_node *node)
{
    return ((head && node) ? head->next == node : false);
}

/*
 * util_list_is_last - test whether @node is the last node in list @head
 * @head: the head node of list
 * @node: the node to test
 * */
static inline bool util_list_is_last(const struct list_node *head,
                                     const struct list_node *node)
{
    return ((head && node) ? head->prev == node : false);
}


/*
 * util_list_is_singular - test whether a list has only one node
 * @head: the head node of list to test
 * */
static inline bool util_list_is_singular(const struct list_node *head)
{
    if (!head) return false;
    return (!util_list_is_empty(head) && (head->next == head->prev));
}



/*
 * util_list_first - find the first node of list and return the node
 * @head: the head node of list
 *
 * Return NULL whren the list is empty
 * */
static inline struct list_node* util_list_first(struct list_node *head)
{
    if (!head) return NULL;
    return (util_list_is_empty(head) ? NULL : head->next);
}

/*
 * util_list_pop_first - pop the first node of list and return the node
 * @head: the head node of list
 *
 * Return NULL when the list is empty
 * */
static inline struct list_node* util_list_pop_first(struct list_node *head)
{
    if (!head) return NULL;
    struct list_node *first = head->next;
    return ((first == head) ? NULL : (util_list_del(first), first));
}


/*
 * util_list_last - find the last node of list and return the node
 * @head: the head node of list
 *
 * Return NULL when the list is empty
 * */
static inline struct list_node* util_list_last(struct list_node *head)
{
    if (!head) return NULL;
    return (util_list_is_empty(head) ? NULL : head->prev);
}

/*
 * util_list_pop_last - pop the last node of list and return the node
 * @head: the head node of list
 *
 * Return NULL when the list is empty
 *  */
static inline struct list_node* util_list_pop_last(struct list_node *head)
{
    if (!head) return NULL;
    struct list_node *last = head->prev;
    return ((last == head) ? NULL : (util_list_del(last), last));
}

/*
 * util_list_prev - find the prev node of @node in @head list
 * @head: the head node of list
 * @node: the target node
 *
 * Return NULL when the list is empty
 * Return the tail node when @node is NULL
 * */
static inline struct list_node* util_list_prev(struct list_node *head,
                                               struct list_node *node)
{
    if (!head) return NULL;
    if (!node) node = head;
    return (util_list_is_empty(head) ? NULL : node->prev);
}

/*
 * util_list_next - find the next node of @node in @head list
 * @head: the head node of list
 * @node: the target node
 *
 * Return NULL when the list is empty
 * Return the first node when @node is NULL
 * */
static inline struct list_node* util_list_next(struct list_node *head,
                                               struct list_node *node)
{
    if (!head) return NULL;
    if (!node) node = head;
    return (util_list_is_empty(head) ? NULL : node->next);
}

/*
 * util_list_rotate_left - move the list to the left
 * @head: the head node of list
 * */
static inline void util_list_rotate_left(struct list_node *head)
{
    struct list_node *first = NULL;
    if (!util_list_is_empty(head)) {
        first = head->next;
        util_list_move_tail(head, first);
    }
}

/*
 * util_list_rotate_right - move the list to the right
 * @head: the head node of list
 * */
static inline void util_list_rotate_right(struct list_node *head)
{
    struct list_node *last = NULL;
    if (!util_list_is_empty(head)) {
        last = head->prev;
        util_list_move_head(head, last);
    }
}

static inline void __util_list_join(const struct list_node *list,
                                    struct list_node *prev,
                                    struct list_node *next)
{
    if (list && prev && next) {
        struct list_node *first = list->next;
        struct list_node *last  = list->prev;

        first->prev = prev;
        prev->next  = first;

        last->next  = next;
        next->prev  = last;
    }
}

/*
 * util_list_join - join @list to the head of @head
 * @head: the head node of list to place @list
 * @list: the head node of list to add to @head
 * */
static inline void util_list_join(struct list_node *head,
                                  const struct list_node *list)
{
    if (!util_list_is_empty(list))
        __util_list_join(list, head, head->next);
}

/*
 * util_list_join_tail - join @list to the tail of @head
 * @head: the head node of list to place @list
 * @list: the head node of list to add to @head
 * */
static inline void util_list_join_tail(struct list_node *head,
                                       const struct list_node *list)
{
    if (!util_list_is_empty(list))
        __util_list_join(list, head->prev, head);
}

/*
 * util_list_for_each - iterate over a list
 * @head: the &struct list_head head node of list
 * @pos: the &struct list_head node to use as a loop cursor
 *
 * Iterate list from head node when @pos is null
 * Iterate list from @pos node when @pos is not null
 * */
#define util_list_for_each(head, pos) \
    if (!pos) pos = head->next; \
    for (; pos != head; pos = pos->next)


/*
 * util_list_for_each_prev - iterate over a list backward
 * @head: the &struct list_head head node of list
 * @pos: the &struct list_head node to use as a loop cursor
 *
 * Iterate list from tail node when @pos is null
 * Iterate list from @pos node when @pos is not null
 * */
#define util_list_for_each_prev(head, pos) \
    if (!pos) pos = head->prev; \
    for (; pos != head; pos = pos->prev)


/*
 * util_list_entry - get the structure address containing of @member
 * @ptr: the &struct list_node pointer
 * @type: the type of the structure containing of @member
 * @member: the name of struct list_node not-pointer member embed in @type structure
 * */
#define util_list_entry(ptr, type, member) \
    entry_of(ptr, type, member)


/*
 * util_list_first_entry - get the structure address containing of first node from a list
 * @ptr: the head node pointer from a list
 * @type: the type of the structure containing of @member
 * @member: the name of struct list_node not-pointer member embed in @type structure
 *
 * Return NULL when @ptr is NULL
 * Return NULL when list is empty
 * */
#define util_list_first_entry(ptr, type, member) \
    ((!(ptr) || util_list_is_empty(ptr)) ? NULL : entry_of((ptr)->next, type, member))


/*
 * util_list_last_entry - get the structure address containing of last node from a list
 * @ptr: the tail node pointer from a list
 * @type: the type of the structure containing of @member
 * @member: the name of struct list_node not-pointer member embed in @type structure
 *
 * Return NULL when @ptr is NULL
 * Return NULL whten list is empty
 * */
#define util_list_last_entry(ptr, type, member) \
    ((!(ptr) || util_list_is_empty(ptr)) ? NULL : entry_of((ptr)->prev, type, member))


/*
 * util_list_next_entry - get the next element of @pos in list
 * @pos: the structure pointer to cursor
 * @member: the name of struct list_node not-pointer member embed in the @pos structure
 *
 * Return NULL when @pos is NULL
 * */
#define util_list_next_entry(pos, member) \
    (!(pos) ? NULL : util_list_entry((pos)->member.next, typeof(*(pos)), member))


/*
 * util_list_prev_entry - get the prev element of @pos in list
 * @pos: the structure pointer to cursor
 * @member: the name of struct list_node not-pointer member embed in the @pos structure
 *
 * Return NULL when @pos is NULL
 * */
#define util_list_prev_entry(pos, member) \
    ((!pos) ? NULL : util_list_entry((pos)->member.prev, typeof(*(pos)), member))


/*
 * util_list_for_each_entry - iterate over list of given type
 * @pos: the structure pointer to use as a loop cursor
 * @head: the head node pointer of list
 * @member: the name of struct list_node not-pointer member embed in the @pos structure
 *
 * Iterate list from head node when @pos is null
 * Iterate list from @pos node when @pos is not null
 * */
#define util_list_for_each_entry(pos, head, member) \
    if (!pos) pos = util_list_first_entry(head, typeof(*pos), member); \
    for (; &pos->member != (head); \
         pos = util_list_next_entry(pos, member))


/*
 * util_list_for_each_entry_prev - iterate over list of given type backward
 * @pos: the structure pointer to use as a loop cursor
 * @head: the head node pointer of list
 * @member: the name of struct list_node not-pointer member embed in the @pos structure
 *
 * Iterate list from tail node when @pos is null
 * Iterate list from @pos node when @pos is not null
 * */
#define util_list_for_each_entry_prev(pos, head, member) \
    if (!pos) pos = util_list_last_entry(head, typeof(*pos), member);  \
    for (; &pos->member != (head); \
         pos = util_list_prev_entry(pos, member))

#endif /* __UTIL_LIST_H__ */
