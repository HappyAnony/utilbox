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

#include <conf/util_json.h>

bool util_json_object_is_error(struct json_object* json_object)
{
    return (is_error(json_object) || json_object == NULL) ? true : false;
}

bool util_json_object_is_type(struct json_object* json_object, enum json_type type)
{
    if (util_json_object_is_error(json_object)) return false;
    return json_object_is_type(json_object, type) ? true : false;
}

enum json_type util_json_object_get_type(struct json_object* json_object)
{
    if (util_json_object_is_error(json_object)) return json_type_null;
    return json_object_get_type(json_object);
}

struct json_object* util_json_object_new_object()
{
    return json_object_new_object();
}

struct json_object* util_json_object_new_array()
{
    return json_object_new_array();
}

int util_json_object_free(struct json_object* json_object)
{
    json_object_put(json_object);
    json_object = NULL;
    return 0;
}

char* util_json_object_to_string(struct json_object *json_object)
{
    if (util_json_object_is_error(json_object)) return NULL;
    return json_object_to_json_string(json_object);
}

struct json_object* util_json_string_to_object(char *json_string)
{
    if (!json_string) return NULL;
    return json_tokener_parse(json_string);
}

int util_json_object_del_object_by_key(struct json_object *json_object, char *key)
{
    if (util_json_object_is_error(json_object)) return -1;
    if (!key) return -1;
    if (json_object->o_type == json_type_object)
    {
        json_object_object_del(json_object, key);
    }
    return 0;
}

int util_json_object_add_object_by_key(struct json_object *json_object, char *key, struct json_object *val)
{
    if (util_json_object_is_error(json_object)) return -1;
    if (util_json_object_is_error(val)) return -1;
    if (!key) return -1;
    if (json_object->o_type == json_type_object)
    {
        json_object_object_add(json_object, key, val);
    }
    return 0;
}

struct json_object* util_json_object_get_object_by_key(struct json_object *json_object, char *key)
{
    if (util_json_object_is_error(json_object)) return NULL;
    if (!key) return NULL;
    if (json_object->o_type != json_type_object) return NULL;
    return json_object_object_get(json_object, key);
}

int util_json_array_length(struct json_object *json_object)
{
	if (util_json_object_is_error(json_object)) return -1;
	if (json_object->o_type != json_type_array) return -1;
	return json_object_array_length(json_object);
}

int util_json_array_add_object_by_value(struct json_object *json_object, struct json_object *value)
{
	if (util_json_object_is_error(json_object)) return -1;
	if (util_json_object_is_error(value)) return -1;
	if (json_object->o_type != json_type_array) return -1;
	return json_object_array_add(json_object, value);
}

struct json_object* util_json_array_get_object_by_idx(struct json_object *json_object, int idx)
{
	if (util_json_object_is_error(json_object)) return NULL;
	if (json_object->o_type != json_type_array) return NULL;
	return json_object_array_get_idx(json_object, idx);
}

int util_json_array_update_object_by_idx(struct json_object *json_object, int idx, struct json_object *value)
{
	if (util_json_object_is_error(json_object)) return -1;
	if (util_json_object_is_error(value)) return -1;
	if (json_object->o_type != json_type_array) return -1;
	return json_object_array_put_idx(json_object, idx, value);
}

struct json_object* util_json_object_parse_bool(bool b)
{
    return json_object_new_boolean(b);
}

struct json_object* util_json_object_parse_int(int i)
{
    return json_object_new_int(i);
}

struct json_object* util_json_object_parse_double(double d)
{
    return json_object_new_double(d);
}

struct json_object* util_json_object_parse_string(char *s)
{
    if (!s) return NULL;
#if 0
    return json_object_new_string(s);
#else
    return json_tokener_parse(s);
#endif
}

struct json_object* util_json_object_parse_string_slice(char *s, int len)
{
    if (!s) return NULL;
    return json_object_new_string_len(s, len);
}

bool util_json_object_dump_bool(struct json_object* json_object)
{
    return json_object_get_boolean(json_object);
}

int util_json_object_dump_int(struct json_object* json_object)
{
    return json_object_get_int(json_object);
}

double util_json_object_dump_double(struct json_object* json_object)
{
    return json_object_get_double(json_object);
}

char* util_json_object_dump_string(struct json_object* json_object)
{
    if (util_json_object_is_error(json_object)) return NULL;
#if 0
    return json_object_get_string(json_object);
#else
    return json_object_to_json_string(json_object);
#endif
}

struct lh_table* util_json_object_dump_table(struct json_object* json_object)
{
    return json_object_get_object(json_object);
}

struct array_list* util_json_object_dump_array(struct json_object* json_object)
{
    return json_object_get_array(json_object);
}
