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
#ifndef __UTIL_RBTREE_H__
#define __UTIL_RBTREE_H__

#include <util_types.h>

/*
 * Simple red-black tree implementation
 * */

#define RBTREE_INIT(rbtree, compare_callback)	\
{												\
	.nil  = &rbtree._nil,						\
	.nil = {									\
		.color  = RB_BLACK,						\
		.lchild = &rbtree.nil,					\
		.rchild = &rbtree.nil,					\
		.parent = &rbtree.nil,					\
	},											\
	.root = &rbtree.nil,						\
	.compare = compare_callback					\
}

#define RBTREE_ASSIGN(name, compare_callback)	\
		struct rb_tree name = RBTREE_INIT(name, compare_callback)


static inline void init_rbtree(struct rb_tree *rbtree, int (*compare)(void *, void *))
{
	if (rbtree) {
		rbtree->nil  = &rbtree->_nil;
		rbtree->nil->color  = RB_BLACK;
		rbtree->nil->lchild = rbtree->nil;
		rbtree->nil->rchild = rbtree->nil;
		rbtree->nil->parent = rbtree->nil;
		rbtree->root = rbtree->nil;
		rbtree->compare = compare;
	}
}

static inline void util_rbtree_walk_inorder(struct rb_tree *rbtree, struct rb_node *rbnode, void(*print)(void *))
{
	if (rbnode != rbtree->nil) {
		util_rbtree_walk_inorder(rbtree, rbnode->lchild, print);
		if (print) print(rbnode->key);
		util_rbtree_walk_inorder(rbtree, rbnode->rchild, print);
	}
}

static inline struct rb_node* util_rbtree_search_recursion(struct rb_tree *rbtree, struct rb_node *rbnode, void *key)
{
	if ((rbnode == rbtree->nil) || (rbtree->compare(key, rbnode->key)))
		return rbnode;
	if (rbtree->compare(key, rbnode->key) < 0) {
		return util_rbtree_search_recursion(rbtree, rbnode->lchild, key);
	} else {
		return util_rbtree_search_recursion(rbtree, rbnode->rchild, key);
	}
}

static inline struct rb_node* util_rbtree_search_circular(struct rb_tree *rbtree, struct rb_node *rbnode, void *key)
{
	while ((rbnode != rbtree->nil) && (rbtree->compare(key, rbnode->key) != 0)) {
		if (rbtree->compare(key, rbnode->key)) {
			rbnode = rbnode->lchild;
		} else {
			rbnode = rbnode->rchild;
		}
	}
	return rbnode;
}

static inline struct rb_node* util_rbtree_minimum(struct rb_tree *rbtree, struct rb_node *rbnode)
{
	if (rbnode != rbtree->nil) {
		while (rbnode->lchild != rbtree->nil) {
			rbnode = rbnode->lchild;
		}
	}
	return rbnode;
}

static inline struct rb_node* util_rbtree_maximum(struct rb_tree *rbtree, struct rb_node *rbnode)
{
	if (rbnode != rbtree->nil) {
		while (rbnode->rchild != rbtree->nil) {
			rbnode = rbnode->rchild;
		}
	}
	return rbnode;
}

static inline struct rb_node* util_rbtree_successor(struct rb_tree *rbtree, struct rb_node *rbnode)
{
	if (rbnode->rchild != rbtree->nil) {
		return util_rbtree_minimum(rbtree, rbnode->rchild);
	}
	struct rb_node *successor = rbnode->parent;
	while ((successor != rbtree->nil) && (rbnode == successor->rchild)) {
		rbnode    = successor;
		successor = successor->parent;
	}
	return successor;
}

static inline struct rb_node* util_rbtree_successor_key(struct rb_tree *rbtree, struct rb_node *rbnode, void *key)
{
	if (rbnode == rbtree->nil) return rbnode;
	while (true) {
		if (rbtree->compare(key, rbnode->key) < 0) {
			if (rbnode->lchild != rbtree->nil) {
				rbnode = rbnode->lchild;
			} else {
				return rbnode;
			}
		} else {
			if (rbnode->rchild != rbtree->nil) {
				rbnode = rbnode->rchild;
			} else {
				return util_rbtree_successor(rbtree, rbnode);
			}
		}
	}
}

static inline struct rb_node* util_rbtree_precursor(struct rb_tree *rbtree, struct rb_node *rbnode)
{
	if (rbnode->lchild != rbtree->nil) {
		return util_rbtree_maximum(rbtree, rbnode->lchild);
	}
	struct rb_node *precursor = rbnode->parent;
	while ((precursor != rbtree->nil) && (rbnode == precursor->lchild)) {
		rbnode    = precursor;
		precursor = precursor->parent;
	}
	return precursor;
}

static inline struct rb_node* util_rbtree_precursor_key(struct rb_tree *rbtree, struct rb_node *rbnode, void *key)
{
	if (rbnode == rbtree->nil) return rbnode;
	while (true) {
		if (rbtree->compare(key, rbnode->key) > 0) {
			if (rbnode->rchild != rbtree->nil) {
				rbnode = rbnode->rchild;
			} else {
				return rbnode;
			}
		} else {
			if (rbnode->lchild != rbtree->nil) {
				rbnode = rbnode->lchild;
			} else {
				return util_rbtree_precursor(rbtree, rbnode);
			}
		}
	}
}


static inline void util_rbtree_left_rotate(struct rb_tree *rbtree, struct rb_node *rbnode)
{
    struct rb_node *tmp;

    tmp = rbnode->rchild;
    rbnode->rchild = tmp->lchild;
    if (tmp->lchild != rbtree->nil) {
        tmp->lchild->parent = rbnode;
    }
    tmp->parent = rbnode->parent;
    if (rbnode->parent == rbtree->nil) {
    	rbtree->root = tmp;
    } else if (rbnode == rbnode->parent->lchild) {
    	rbnode->parent->lchild = tmp;
    } else {
    	rbnode->parent->rchild = tmp;
    }
    tmp->lchild = rbnode;
    rbnode->parent = tmp;
}
static inline void util_rbtree_right_rotate(struct rb_tree *rbtree, struct rb_node *rbnode)
{
    struct rb_node *tmp;

    rbnode = tmp->lchild;
    tmp->lchild = rbnode->rchild;
    if (rbnode->rchild != rbtree->nil) {
    	rbnode->rchild->parent = tmp;
    }
    rbnode->parent = tmp->parent;
    if (tmp->parent == rbtree->nil) {
    	rbtree->root = rbnode;
    } else if (tmp == tmp->parent->rchild) {
    	tmp->parent->rchild = rbnode;
    } else {
    	tmp->parent->lchild = rbnode;
    }
    rbnode->rchild = tmp;
    tmp->parent = rbnode;
}

static inline void util_rbtree_insert_rotate(struct rb_tree *rbtree, struct rb_node *rbnode)
{
    struct rb_node *tmp;

    while (rbnode->parent->color == RB_RED) {
        if (rbnode->parent == rbnode->parent->parent->lchild) {
        	tmp = rbnode->parent->parent->rchild;
            if (tmp->color == RB_RED) {
            	rbnode->parent->color = RB_BLACK;
            	tmp->color = RB_BLACK;
                rbnode->parent->parent->color = RB_RED;
                rbnode = rbnode->parent->parent;
            } else {
                if (rbnode == rbnode->parent->rchild) {
                	rbnode = rbnode->parent;
                	util_rbtree_left_rotate(rbtree, rbnode);
                }
                rbnode->parent->color = RB_BLACK;
                rbnode->parent->parent->color = RB_RED;
                util_rbtree_right_rotate(rbtree, rbnode->parent->parent);
            }
        } else {
        	tmp = rbnode->parent->parent->lchild;
            if (tmp->color == RB_RED) {
            	rbnode->parent->color = RB_BLACK;
            	tmp->color = RB_BLACK;
                rbnode->parent->parent->color = RB_RED;
                rbnode = rbnode->parent->parent;
            } else {
                if (rbnode == rbnode->parent->lchild) {
                	rbnode = rbnode->parent;
                	util_rbtree_right_rotate(rbtree, rbnode);
                }
                rbnode->parent->color = RB_BLACK;
                rbnode->parent->parent->color = RB_RED;
                util_rbtree_left_rotate(rbtree, rbnode->parent->parent);
            }
        }
    }
    rbtree->root->color = RB_BLACK;
}
static inline void util_rbtree_insert(struct rb_tree *rbtree, struct rb_node *rbnode)
{
    struct rb_node *x, *y;

    y = rbtree->nil;
    x = rbtree->root;
    while (x != rbtree->nil) {
        y = x;
        if (rbtree->compare(rbnode->key, x->key) < 0) {
            x = x->lchild;
        } else {
            x = x->rchild;
        }
    }
    rbnode->parent = y;
    if (y == rbtree->nil) {
    	rbtree->root = rbnode;
    } else if (rbtree->compare(rbnode->key, y->key) < 0) {
        y->lchild = rbnode;
    } else {
        y->rchild = rbnode;
    }
    rbnode->lchild = rbtree->nil;
    rbnode->rchild = rbtree->nil;
    rbnode->color = RB_RED;
    util_rbtree_insert_rotate(rbtree, rbnode);
}

static inline void util_rbtree_transplant(struct rb_tree *rbtree, struct rb_node *u, struct rb_node *v)
{
    if (u->parent == rbtree->nil) {
    	rbtree->root = v;
    } else if (u == u->parent->lchild) {
        u->parent->lchild = v;
    } else {
        u->parent->rchild = v;
    }
    v->parent = u->parent;
}

static inline void util_rbtree_delete_rotate(struct rb_tree *rbtree, struct rb_node *rbnode)
{
    struct rb_node *tmp;

    while ((rbnode != rbtree->root) && (rbnode->color == RB_BLACK)) {
        if (rbnode == rbnode->parent->lchild) {
        	tmp = rbnode->parent->rchild;
            if (tmp->color == RB_RED) {
            	tmp->color = RB_BLACK;
                rbnode->parent->color = RB_RED;
                util_rbtree_left_rotate(rbtree, rbnode->parent);
                tmp = rbnode->parent->rchild;
            }
            if ((tmp->lchild->color == RB_BLACK) && (tmp->rchild->color == RB_BLACK)) {
            	tmp->color = RB_RED;
                rbnode = rbnode->parent;
            } else {
                if (tmp->rchild->color == RB_BLACK) {
                	tmp->lchild->color = RB_BLACK;
                	tmp->color = RB_RED;
                    util_rbtree_right_rotate(rbtree, tmp);
                    tmp = rbnode->parent->rchild;
                }
                tmp->color = rbnode->parent->color;
                rbnode->parent->color = RB_BLACK;
                tmp->rchild->color = RB_BLACK;
                util_rbtree_left_rotate(rbtree, rbnode->parent);
                rbnode = rbtree->root;
            }
        } else {
        	tmp = rbnode->parent->lchild;
            if (tmp->color == RB_RED) {
            	tmp->color = RB_BLACK;
                rbnode->parent->color = RB_RED;
                util_rbtree_right_rotate(rbtree, rbnode->parent);
                tmp = rbnode->parent->lchild;
            }
            if ((tmp->rchild->color == RB_BLACK) && (tmp->lchild->color == RB_BLACK)) {
            	tmp->color = RB_RED;
                rbnode = rbnode->parent;
            } else {
                if (tmp->lchild->color == RB_BLACK) {
                	tmp->rchild->color = RB_BLACK;
                	tmp->color = RB_RED;
                    util_rbtree_left_rotate(rbtree, tmp);
                    tmp = rbnode->parent->lchild;
                }
                tmp->color = rbnode->parent->color;
                rbnode->parent->color = RB_BLACK;
                tmp->lchild->color = RB_BLACK;
                util_rbtree_right_rotate(rbtree, rbnode->parent);
                rbnode = rbtree->root;
            }
        }
    }
    rbnode->color = RB_BLACK;
}
static inline void util_rbtree_delete(struct rb_tree *rbtree, struct rb_node *rbnode)
{
    struct rb_node *x, *y;
    rb_color color;

    y = rbnode;
    color = y->color;
    if (rbnode->lchild == rbtree->nil) {
        x = rbnode->rchild;
        util_rbtree_transplant(rbtree, rbnode, rbnode->rchild);
    } else if (rbnode->rchild == rbtree->nil) {
        x = rbnode->lchild;
        util_rbtree_transplant(rbtree, rbnode, rbnode->lchild);
    } else {
        y = util_rbtree_minimum(rbtree, rbnode->rchild);
        color = y->color;
        x = y->rchild;
        if (y->parent == rbnode) {
            x->parent = y;
        } else {
            util_rbtree_transplant(rbtree, y, y->rchild);
            y->rchild = rbnode->rchild;
            y->rchild->parent = y;
        }
        util_rbtree_transplant(rbtree, rbnode, y);
        y->lchild = rbnode->lchild;
        y->lchild->parent = y;
        y->color = rbnode->color;
    }
    if (color == RB_BLACK) {
        util_rbtree_delete_rotate(rbtree, x);
    }
}


#endif /* __UTIL_RBTREE_H__ */
