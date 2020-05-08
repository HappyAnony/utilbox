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
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <conf/util_json_parser.h>


#define   JSON_FIELD_NUM_MAX       (4)
#define   JSON_DICT_LEN_MAX        (200)
#define   JSON_ARRAY_DEPTH_MAX     (2)

static int parse_json_oid(const char *json_oid, struct json_parser *json_parser)
{
    if (!json_oid || !json_parser) return -1;
    char *path   = NULL;
    char *pPre   = NULL;
    char *pCur   = NULL;
    char *pTmp   = NULL;
    int   localFieldNum = 0;            /* 字段数 */
    int   localArrayIndex = 0;          /* 数组嵌套层次 */
    int   fullFlag = 0;                 /* 数组符号[]配对标志 */
    int   cpyFlag  = 1;                 /* 没有数组符号[]标志 */

    struct json_field* json_field = json_parser->json_field;

    path = strdup(json_oid);
    if (!path) return -1;

    /* fieldStr不能以.或[或]开头, 不能以.或[结尾 */
    pTmp = path;
    if (*pTmp == '.' || *pTmp == '[' || *pTmp == ']'
        ||
        *(pTmp = path + strlen(path) - 1) == '.'
        ||
        *(pTmp = path + strlen(path) - 1) == '[')
    {
        free(path);
        return -1;
    }

    pPre = path;
    pCur = pPre;

    while('\0' != *pCur)
    {
        /* 以].符号作为字段分隔符, ]在下面操作中会被置为'\0' */
        if (*pCur == '.'
            &&
            *(pTmp = pCur - 1) == '\0')
        {
            if (++localFieldNum >= json_parser->attr.json_field_num
                ||
                *(pTmp = pCur + 1) == '\0')
            {
                free(path);
                return -1;
            }
            localArrayIndex = 0;   /* 解析新字段, 重置该变量 */
            cpyFlag         = 1;   /* 解析新字段, 重置该标志 */
            pCur++;
            pPre = pCur;
            continue;
        }

        /* 开始解析[]数组 */
        if (*pCur == '[')
        {
            if (*(pTmp = pCur - 1) == '.'
                ||
                *(pTmp = pCur + 1) == ']'
                ||
                *(pTmp = pCur + 1) == '.')
            {
                free(path);
                return -1;
            }

            cpyFlag = 0;
            *pCur = '\0';
            /* 解析[][]数组嵌套时, [被置为'\0' */
            if (*(pTmp = pCur - 1) != '\0')
            {
                strncpy(json_field[localFieldNum].json_dict, pPre, json_parser->attr.json_dict_length - 1);
                json_field[localFieldNum].exist = true;
            }
            pCur++;
            pPre = pCur;

            do
            {
                if (localArrayIndex >= json_parser->attr.json_array_depth)
                {
                    free(path);
                    return -1;
                }
                /* 数组符号[]配置成功 */
                if (*pCur == ']')
                {
                    *pCur = '\0';
                    json_field[localFieldNum].json_arry[localArrayIndex].index = atoi(pPre);
                    json_field[localFieldNum].json_arry[localArrayIndex].exist = true;
                    localArrayIndex++;
                    fullFlag = 1;
                    break;
                }
                pCur++;
            } while(*pCur != '\0');

            if (!fullFlag)
            {
                free(path);
                return -1;
            }
            else
            {
                fullFlag = 0;
            }
        }
        pCur++;
    }
    if (cpyFlag && pCur - pPre > 0)
    {
        strncpy(json_field[localFieldNum].json_dict, pPre, json_parser->attr.json_dict_length - 1);
        json_field[localFieldNum].exist = true;
    }

    free(path);
    return 0;
}

struct json_object *json_search_object(struct json_object *container, const char *field)
{
    if (util_json_object_is_error(container) || field == NULL) return NULL;

    char *path = NULL;
    char *p    = NULL;
    char *q    = NULL;
    const char *delim = ".";
    struct json_object *obj = NULL;
    struct json_object *container_obj = NULL;
    char *strtok_save;

    path = strdup(field);
    if (!path) return NULL;

    p             = path;
    container_obj = container;

    q = strtok_r(p, delim, &strtok_save);
    while (q) {
        obj = util_json_object_get_object_by_key(container_obj, q);
        if(!util_json_object_is_error(obj)) {
            container_obj = obj;
        } else {
            free(path);
            return NULL;
        }

        q = strtok_r(NULL, delim, &strtok_save);
    }

    free(path);
    return obj;
}

struct json_object *json_field_search_object(struct json_parser* json_parser, const char *json_oid)
{
    if (util_json_object_is_error(json_parser->json_object) || json_oid == NULL) return NULL;
    struct json_object *obj = NULL;
    struct json_object *tmp_obj = json_parser->json_object;

    /* parse the JSON-OID and store */
    if (0 != parse_json_oid(json_oid, json_parser)) return NULL;

    /* get @obj */
    unsigned int field_item = 0;
    unsigned int array_item = 0;
    for (field_item = 0; field_item < json_parser->attr.json_field_num; field_item++)
    {
        if (!json_parser->json_field[field_item].exist) continue;

        if (NULL == (obj = json_search_object(tmp_obj, json_parser->json_field[field_item].json_dict)))
            return obj;
        tmp_obj = obj;

        for (array_item = 0; array_item < json_parser->attr.json_array_depth; array_item++)
        {
            if (json_parser->json_field[field_item].json_arry[array_item].exist) {
                if (tmp_obj->o_type == json_type_array) {
                    obj = JsonArrayGetFieldByIdx(tmp_obj, json_parser->json_field[field_item].json_arry[array_item].index);
                    if (util_json_object_is_error(obj)) return NULL;
                    tmp_obj = obj;
                } else {
                    return NULL;
                }
            }
        }
    }
    return obj;
}

struct json_object *json_search_parent_object(struct json_object *container, const char *field, char *node, int bufsize)
{
    if (util_json_object_is_error(container) || field == NULL || node == NULL) return NULL;

    char *last_delim  = NULL;
    char *parent_path = NULL;
    int  parent_path_len = 0;
    struct json_object *parent_obj = NULL;

    last_delim = strrchr(field, '.');
    memset(node, 0, bufsize);
    if (last_delim) {
        parent_path_len = last_delim - field;
        parent_path = calloc(parent_path_len + 1, sizeof(char));
        if (!parent_path) return NULL;

        strncpy(parent_path, field, parent_path_len);
        strncpy(node, last_delim + 1, bufsize - 1); /* Last node */

        parent_obj = json_search_object(container, parent_path);
        free(parent_path);
        return parent_obj;
    } else {
        strncpy(node, field, bufsize - 1);
        return container;
    }
}

struct json_object *Json_field_search_parent_object(struct json_parser* json_parser, const char *json_oid, void *member, ...)
{
    if (util_json_object_is_error(json_parser->json_object) || field == NULL || node == NULL) return NULL;

    struct json_object *parent_obj  = NULL;
    struct json_object *tmp_obj     = json_parser->json_object;
    va_list ap;
    int  buffer_len        = 0;
    int  array_flag        = 0;
    int  array_index       = 0;
    int  field_len = strlen(json_oid) + 1;
    char tmp_node[field_len];
    memset(tmp_node, 0, sizeof(tmp_node));

    /* parse the JSON-OID and store */
    if (0 != parse_json_oid(json_oid, json_parser)) return NULL;

    /* get @parent_obj */
    unsigned int field_item = 0;
    unsigned int array_item = 0;
    for (field_item = 0; field_item < json_parser->attr.json_field_num; field_item++)
    {
        if (!json_parser->json_field[field_item].exist) continue;

        if (field_item != 0) {
            if (tmp_obj->o_type == json_type_array) {
                parent_obj = util_json_array_get_object_by_idx(tmp_obj, array_index);

                if(!util_json_object_is_error(parent_obj)) {
                    tmp_obj = parent_obj;
                    array_flag = 0;
                } else {
                    return NULL;
                }
            }  else {
                return NULL;
            }
        }

        memset(tmp_node, 0, sizeof(field_len));
        if (NULL == (parent_obj = json_search_parent_object(tmp_obj, json_parser->json_field[field_item].json_dict, tmp_node, field_len)))
            return parent_obj;
        tmp_obj = parent_obj;

        for (array_item = 0; array_item < json_parser->attr.json_array_depth; array_item++)
        {
            if (json_parser->json_field[field_item].json_arry[array_item].exist) {
                /* parent obj is dict obj */
                if (array_item == 0) {
                    parent_obj = util_json_object_get_object_by_key(tmp_obj, tmp_node);
                }
                /* parent obj is array obj */
                else if (tmp_obj->o_type == json_type_array) {
                    parent_obj = util_json_array_get_object_by_idx(tmp_obj, array_index);
                } else {
                    return NULL;
                }

                if(!util_json_object_is_error(parent_obj)) {
                    tmp_obj = parent_obj;
                    array_index = json_parser->json_field[field_item].json_arry[array_item].index;
                    array_flag = 1;
                } else {
                    return NULL;
                }
            }
        }

    }

    /* return the @parent_obj and assign @member value */
    if (array_flag) {
        *(int *)member    = array_index;
    } else {
        va_start(ap, member);
        buffer_len = va_arg(ap, int);
        memset((char *)member, 0, buffer_len);
        snprintf((char *)member, buffer_len, "%s", tmp_node);
        va_end (ap);
    }
    return parent_obj;
}

struct json_object *json_new_parent_object(struct json_object *container, const char *field, char *node, int bufsize)
{
    if (util_json_object_is_error(container) || field == NULL || node == NULL) return NULL;

    char *last_delim = NULL;
    char *path       = NULL;
    char *p          = NULL;
    char *q          = NULL;
    struct json_object *obj = NULL;
    struct json_object *parent_obj = NULL;
    int    new_obj_start = 0;
    char   *strtok_save = NULL;

    path = strdup(field);
    if (path == NULL) return NULL;

    parent_obj = container;
    p = path;
    last_delim = strrchr(path, '.');
    if (last_delim == NULL) {
        strncpy(node, path, bufsize - 1);
        free(path);
        return parent_obj;
    }

    q = strtok_r(p, ".", &strtok_save);
    while (q)
    {
        /* lastDelim is points to the '.' delimiter, but q points to the node name */
        if (q == last_delim + 1) {
            /* reach to the last node */
            strncpy(node, q, bufsize - 1);
            free(path);
            return parent_obj;
        }

        if (!new_obj_start) {
            obj = util_json_object_get_object_by_key(parent_obj, q);
            if (util_json_object_is_error(obj)) {
                new_obj_start = 1;
                obj = util_json_object_new_object();
                if (util_json_object_is_error(obj)) {
                    free(path);
                    return NULL;
                }
                util_json_object_add_object_by_key(parent_obj, q, obj);
            }
            parent_obj = obj;
        } else {
            obj = util_json_object_new_object();
            if (util_json_object_is_error(obj) {
                free(path);
                return NULL;
            }

            util_json_object_add_object_by_key(parent_obj, q, obj);
            parent_obj = obj;
        }

        q = strtok_r(NULL, ".", &strtok_save);
    }

    return parent_obj;
}

struct json_object *json_field_new_parent_object(struct json_parser* json_parser, const char *json_oid, void *member, ...)
{
    if (util_json_object_is_error(json_parser->json_object) || json_oid == NULL || member == NULL) return NULL;

    struct json_object *parent_obj  = NULL;
    struct json_object *tmp_obj     = json_parser->json_object;
    struct json_object *new_obj     = NULL;

    va_list ap;
    int  buffer_len  = 0;
    int  array_flag  = 0;
    int  array_index = 0;
    int  field_len   = strlen(json_oid) + 1;
    char tmp_node[field_len];
    memset(tmp_node, 0, sizeof(tmp_node));

    /* parse the JSON-OID and store */
    if (0 != parse_json_oid(json_oid, json_parser)) return NULL;

    /* generate @parent_obj */
    unsigned int field_item = 0;
    unsigned int array_item = 0;
    for (field_item = 0; field_item < json_parser->attr.json_field_num; field_item++)
    {
        if (json_parser->json_field[field_item].exist) continue;

        if (field_item != 0) {
            if (tmp_obj->o_type == json_type_array) {
                parent_obj = util_json_array_get_object_by_idx(tmp_obj, array_index);
                if(util_json_object_is_error(parent_obj)) {
                    new_obj = util_json_object_new_object();
                    if (util_json_object_is_error(new_obj)) return NULL;
                    util_json_array_update_object_by_idx(tmp_obj, array_index, new_obj);
                    tmp_obj = new_obj;
                    array_flag = 0;
                } else {
                    tmp_obj = parent_obj;
                    array_flag = 0;
                }
            } else {
                return NULL;
            }
        }

        memset(tmp_node, 0, sizeof(field_len));
        if (NULL == (parent_obj = json_new_parent_object(tmp_obj, json_parser->json_field[field_item].json_dict, tmp_node, field_len)))
            return parent_obj;
        tmp_obj = parent_obj;

        for (array_item = 0; array_item < json_parser->attr.json_array_depth; array_item++)
        {
            if (json_parser->json_field[field_item].json_arry[array_item].exist) {
                /* parent obj is dict obj */
                if (array_item == 0) {
                    parent_obj = util_json_object_get_object_by_key(tmp_obj, tmp_node);
                    if(util_json_object_is_error(parent_obj))
                    {
                        new_obj = util_json_object_new_array();
                        if (util_json_object_is_error(new_obj)) return NULL;
                        util_json_object_add_object_by_key(tmp_obj, tmp_node, new_obj);
                        parent_obj = new_obj;
                    }
                }
                /* parent obj is array obj */
                else if (tmp_obj->o_type == json_type_array) {
                    parent_obj = util_json_array_get_object_by_idx(tmp_obj, array_index);
                    if(util_json_object_is_error(parent_obj)) {
                        new_obj = util_json_object_new_array();
                        if (util_json_object_is_error(new_obj)) return NULL;
                        util_json_array_update_object_by_idx(tmp_obj, array_index, new_obj);
                        parent_obj = new_obj;
                    }
                } else {
                    return NULL;
                }


                if(!util_json_object_is_error(parent_obj)) {
                    tmp_obj = parent_obj;
                    array_index = json_parser->json_field[field_item].json_arry[array_item].index;
                    array_flag = 1;
                } else {
                    return NULL;
                }
            }
        }

    }

    /* return the @parent_obj and assign @member value */
    if (array_flag) {
        *(int *)member    = array_index;
    } else {
        va_start(ap, member);
        buffer_len = va_arg(ap, int);
        memset((char *)member, 0, buffer_len);
        snprintf((char *)member, buffer_len, "%s", tmp_node);
        va_end (ap);
    }
    return parent_obj;
}

static int json_parser_new(struct json_parser* json_parser, json_op op, struct json_op_attr *op_attr)
{
    if (!json_parser) return -1;
    if (!op_attr) return -1;

    switch (op_attr->op_type)
    {
    	case JSON_OP_NEW_DICT:
    		json_parser->json_object = util_json_object_new_object();
    		break;
    	case JSON_OP_NEW_ARRAY:
    		json_parser->json_object = util_json_object_new_array();
    		break;
    	default:
    		break;
    }

    return 0;
}
static int json_parser_parse(struct json_parser* json_parser, json_op op, struct json_op_attr *op_attr)
{
    if (!json_parser) return -1;
    if (!op_attr) return -1;

    switch (op_attr->op_type)
    {
    	case JSON_OP_PARSE_BOOL:
    		json_parser->json_object = util_json_object_parse_bool(op_attr->op_data.parse_data.json_bool);
    		break;
    	case JSON_OP_PARSE_INT32:
    		json_parser->json_object = util_json_object_parse_int(op_attr->op_data.parse_data.json_int);
    		break;
    	case JSON_OP_PARSE_DOUBLE:
    		json_parser->json_object = util_json_object_parse_double(op_attr->op_data.parse_data.json_double);
    		break;
    	case JSON_OP_PARSE_STRING:
    		json_parser->json_object = util_json_object_parse_string(op_attr->op_data.parse_data.json_string);
    		break;
    	case JSON_OP_PARSE_FILE:
    		break;
    	default:
    		break;
    }

    if (util_json_object_is_error(json_parser->json_object))
    	return -1;
	return 0;
}
static int json_field_add(struct json_parser* json_parser, char *json_oid, void *value, enum json_type type)
{
    if (util_json_object_is_error(json_parser->json_object) || json_oid == NULL || value == NULL) return -1;

    struct json_object *parent_obj = NULL;
    struct json_object *member_obj = NULL;
    bool              is_array_obj = false;
    char   *member_key             = NULL;
    int    member_index            = 0;
    int    oid_len                 = 0;
    int    ret                     = 0;

    /* store JSON-OID */
    oid_len = strlen(json_oid) + 1;
    member_key = calloc(1, oid_len);
    if (!member_key) return -1;
    memcpy(member_key, json_oid, oid_len);

    /* test whether @member_obj is array object */
    if (*(member_key + oid_len - 2) == ']') is_array_obj = true;
    memset(member_key, 0, oid_len);

    do {
        /* generate the @parent_obj */
        if (is_array_obj) {
            parent_obj = json_field_new_parent_object(json_parser->json_object, json_oid, &member_index);
            if (util_json_object_is_error(parent_obj)
                ||
                parent_obj->o_type != json_type_array)
            {
                ret = -1;
                break;
            }
        } else {
            parent_obj = json_field_new_parent_object(json_parser->json_object, json_oid, member_key, oid_len);
            if (util_json_object_is_error(parent_obj)
                ||
                parent_obj->o_type != json_type_object)
            {
                ret = -1;
                break;
            }
        }

        /* generate the @member_obj */
        switch (type)
        {
            case json_type_boolean:
                member_obj = util_json_object_parse_bool(*(bool *)value);
                break;
            case json_type_int:
                member_obj = util_json_object_parse_int(*(int *)value);
                break;
            case json_type_double:
                member_obj = util_json_object_parse_double(*(double *)value);
                break;
            case json_type_string:
                member_obj = util_json_object_parse_string((char *)value);
                break;
            default :
                ret = -1;
                break;
        }
        if (ret == -1 || util_json_object_is_error(member_obj))
        {
            ret = -1;
            break;
        }

        /* add the @member_obj in @parent_obj*/
        if (is_array_obj)
            util_json_array_update_object_by_idx(parent_obj, member_index, member_obj);
        else
            util_json_object_add_object_by_key(parent_obj, member_key, member_obj);
    } while(0);

    free(member_key);
    return ret;
}
static int json_parser_add(struct json_parser* json_parser, json_op op, struct json_op_attr *op_attr)
{
    if (!json_parser) return -1;
    if (!op_attr) return -1;
	return 0;
}
static int json_parser_get(struct json_parser* json_parser, json_op op, struct json_op_attr *op_attr)
{
    if (!json_parser) return -1;
    if (!op_attr) return -1;
	return 0;
}
static int json_parser_update(struct json_parser* json_parser, json_op op, struct json_op_attr *op_attr)
{
    if (!json_parser) return -1;
    if (!op_attr) return -1;
	return 0;
}
static int json_parser_delete(struct json_parser* json_parser, json_op op, struct json_op_attr *op_attr)
{
    if (!json_parser) return -1;
    if (!op_attr) return -1;
	return 0;
}
static int json_parser_dump(struct json_parser* json_parser, json_op op, struct json_op_attr *op_attr)
{
    if (!json_parser) return -1;
    if (!op_attr) return -1;

    if (util_json_object_is_error(json_parser->json_object)) return -1;

    switch (op_attr->op_type)
    {
    	case JSON_OP_DUMP_BOOL:
    		op_attr->op_data.dump_data.json_bool   = util_json_object_dump_bool(json_parser->json_object);
    		break;
    	case JSON_OP_DUMP_INT32:
    		op_attr->op_data.dump_data.json_int    = util_json_object_dump_int(json_parser->json_object);
    		break;
    	case JSON_OP_DUMP_DOUBLE:
    		op_attr->op_data.dump_data.json_double = util_json_object_dump_double(json_parser->json_object);
    		break;
    	case JSON_OP_DUMP_STRING:
    		snprintf(op_attr->op_data.dump_data.json_string, op_attr->op_data.dump_data.string_len,
    				"%s", util_json_object_dump_string(json_parser->json_object));
    		break;
    	case JSON_OP_DUMP_FILE:
    		break;
    	default:
    		break;
    }
	return 0;
}
static struct {
    json_op     op_type;
    int (*callback)(struct json_parser* json_parser, json_op op, struct json_op_attr *op_attr);
} json_parser_reactor[JSON_OP_NUM] = {
        {JSON_OP_NEW           ,         json_parser_new   },
		{JSON_OP_PARSE         ,         json_parser_parse },
		{JSON_OP_ADD           ,         json_parser_add   },
		{JSON_OP_GET           ,         json_parser_get   },
		{JSON_OP_UPDATE        ,         json_parser_update},
		{JSON_OP_DELETE        ,         json_parser_delete},
		{JSON_OP_DUMP          ,         json_parser_dump  }
};

struct json_parser* json_parser_create(struct json_parser_attr *attr)
{
    struct json_parser *json_parser = NULL;

    do {
        json_parser = calloc(1, sizeof(struct json_parser));
        if (!json_parser) break;

        /* attribution assignment */
        json_parser->attr.json_field_num    = ((attr && attr->json_field_num  ) ? attr->json_field_num   : JSON_FIELD_NUM_MAX  );
        json_parser->attr.json_dict_length  = ((attr && attr->json_dict_length) ? attr->json_dict_length : JSON_DICT_LEN_MAX   );
        json_parser->attr.json_array_depth  = ((attr && attr->json_array_depth) ? attr->json_array_depth : JSON_ARRAY_DEPTH_MAX);

        /* alloc memory for JSON-FIELD */
        struct json_field *json_field_memory = (struct json_field *)calloc(json_parser->attr.json_field_num,
                sizeof(struct json_field));
        char              *json_dict_memory  = (char *)calloc(json_parser->attr.json_field_num * json_parser->attr.json_dict_length,
                sizeof(char));
        struct json_array *json_array_memory = (struct json_array *)calloc(json_parser->attr.json_field_num * json_parser->attr.json_array_depth,
                sizeof(struct json_array));
        if (!json_field_memory || !json_dict_memory || !json_array_memory) {
            if (json_field_memory) free(json_field_memory);
            if (json_dict_memory)  free(json_dict_memory);
            if (json_array_memory) free(json_array_memory);
            free(json_parser);
            json_parser = NULL;
            break;
        }

        json_parser->json_field = json_field_memory;
        unsigned int field = 0;
        for (field = 0; field < json_parser->attr.json_field_num; field++) {
            json_parser->json_field[field].json_dict = json_dict_memory +
                    (field * (json_parser->attr.json_dict_length));
            json_parser->json_field[field].json_arry = json_array_memory +
                    (field * (json_parser->attr.json_array_depth));
        }
    } while(0);

    return json_parser;
}
int json_parser_ctl(struct json_parser* json_parser, json_op op, struct json_op_attr *op_attr)
{
    if (op >= JSON_OP_NUM) return -1;
    return json_parser_reactor[op].callback(json_parser, op, op_attr);
}
int json_parser_free(struct json_parser* json_parser)
{
    if (json_parser) {
        if (json_parser->json_field[0].json_dict) free(json_parser->json_field[0].json_dict);
        if (json_parser->json_field[0].json_arry) free(json_parser->json_field[0].json_arry);
        if (json_parser->json_field) free(json_parser->json_field);
        if (!util_json_object_is_error(json_parser->json_object))
            util_json_object_free(json_parser->json_object);
        if (json_parser) free(json_parser);
    }
    return 0;
}
