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
#ifndef __UTIL_JSON_H__
#define __UTIL_JSON_H__

#include <util_types.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_object_private.h>
#include <json-c/json_util.h>


/* util_json_object_is_error - test whether struct json_object @json_object is legal
 * @json_object: &struct json_object pointer which is json_type_array or json_type_object type
 * @return: true for illegal, false for legal
 * */
bool util_json_object_is_error(struct json_object* json_object);


/* util_json_object_is_type - test whether @json_object is @type
 * @json_object: &struct json_object pointer
 * @typ: enum json_type
 *       1.json_type_null
 *       2.json_type_boolean
 *       3.json_type_double
 *       4.json_type_int
 *       5.json_type_object(be similar to dict)
 *       6.json_type_array
 *       7.json_type_string
 * @return: true for matching; false for mismatching
 * */
bool util_json_object_is_type(struct json_object* json_object, enum json_type type);



/* util_json_object_get_type - get the enum json_type of @json_object
 * @jsonObject: &struct json_object pointer
 * @return: enum json_type
 *          1.json_type_null
 *          2.json_type_boolean
 *          3.json_type_double
 *          4.json_type_int
 *          5.json_type_object(be similar to dict)
 *          6.json_type_array
 *          7.json_type_string
 * */
enum json_type util_json_object_get_type(struct json_object* json_object);



/* util_json_object_new_object - new a struct json_object object which is json_type_object type
 * @return: &struct json_object pointer which is json_type_object type
 *
 * &struct json_object pointer can be tested whether is legal by util_json_object_is_error
 * &struct json_object pointer can be freed by util_json_object_free
 * */
struct json_object* util_json_object_new_object();



/* util_json_object_new_array - new a struct json_object object which is json_type_array type
 * @return: &struct json_object pointer which is json_type_array type
 *
 * &struct json_object pointer can be tested whether is legal by util_json_object_is_error
 * &struct json_object pointer can be freed by util_json_object_free
 * */
struct json_object* util_json_object_new_array();



/* util_json_object_free - free a struct json_object object which is json_type_array or json_type_object type
 * @json_object: &struct json_object pointer which is json_type_array or json_type_object type
 * @return: zero for success, non-zero for fail
 * */
int util_json_object_free(struct json_object* json_object);



/* util_json_object_to_string - convert the struct json_object object which is json_type_array or json_type_object type to string
 * @json_object: &struct json_object pointer which is json_type_array or json_type_object type
 * @return: char *
 *
 * Return NULL when convert fail
 * */
char* util_json_object_to_string(struct json_object *json_object);



/* util_json_string_to_object - convert the json string to the struct json_object object which is json_type_array or json_type_object type
 * @json_string: char *
 * @return: &struct json_object pointer which is json_type_array or json_type_object type
 *
 * &struct json_object pointer can be tested whether is legal by util_json_object_is_error
 * &struct json_object pointer can be freed by util_json_object_free
 * */
struct json_object* util_json_string_to_object(char *json_string);




/* util_json_object_del_object_by_key - delete the object corresponding to @key in @json_object
 * @json_object: &struct json_object pointer which is json_type_object type
 * @key: char *
 * @return: zero for success, non-zero for fail
 * */
int util_json_object_del_object_by_key(struct json_object *json_object, char *key);



/* util_json_object_add_object_by_key - add the @key-@val pair in @json_object
 * @json_object: &struct json_object pointer which is json_type_object type
 * @key: char *
 * @value: &struct json_object pointer which is any type
 * @return: zero for success, non-zero for fail
 * */
int util_json_object_add_object_by_key(struct json_object *json_object, char *key, struct json_object *val);



/* util_json_object_get_object_by_key - get the object corresponding to @key in @json_object
 * @json_object: &struct json_object pointer which is json_type_object type
 * @key: char *
 * @return: &struct json_object pointer which is any type
 *
 * &struct json_object pointer can be tested whether is legal by util_json_object_is_error
 * &struct json_object pointer can be freed by util_json_object_free
 * */
struct json_object* util_json_object_get_object_by_key(struct json_object *json_object, char *key);




/* util_json_object_array_length - get the length of struct json_object object which is json_type_array type
 * @json_object: &struct json_object pointer which is json_type_array type
 * @return: the length value for success, -1 for fail
 * */
int util_json_array_length(struct json_object *json_object);



/* util_json_object_array_add_by_value - add a any type of @value object to @json_object object which is json_type_array type
 * @json_object: &struct json_object pointer which is json_type_array type
 * @value: any type of &struct json_object pointer
 * @return: the subscript index of @value object in array object for success, -1 for fail
 *
 * */
int util_json_array_add_object_by_value(struct json_object *json_object, struct json_object *value);



/* util_json_array_get_object_by_idx - get the object corresponding to @idx subscript index in @json_object
 * @json_object: &struct json_object pointer which is json_type_array type
 * @idx: the subscript index in @json_object array object
 * &return: struct json_object pointer which is any type
 *
 * &struct json_object pointer can be tested whether is legal by util_json_object_is_error
 * &struct json_object pointer can be freed by util_json_object_free
 * */
struct json_object* util_json_array_get_object_by_idx(struct json_object *json_object, int idx);


/* util_json_array_update_object_by_idx - replace the object corresponding to @idx subscript index in @json_object with @value object
 * @json_object: &struct json_object pointer which is json_type_array type
 * @idx: the subscript index in @json_object array object
 * @value: any type of &struct json_object pointer
 * @return: zero for success, non-zero for fail
 *
 * */
int util_json_array_update_object_by_idx(struct json_object *json_object, int idx, struct json_object *value);



/* util_json_object_parse_bool - parse bool @b and return the corresponding struct_object object
 * @b: bool data
 * @return: &struct json_object pointer
 *
 * &struct json_object pointer can be tested whether is legal by util_json_object_is_error
 * &struct json_object pointer can be freed by util_json_object_free
 * */
struct json_object* util_json_object_parse_bool(bool b);



/* util_json_object_parse_int - parse unsigned int @i and return the corresponding struct_object object
 * @i: int data
 * @return: &struct json_object pointer
 *
 * &struct json_object pointer can be tested whether is legal by util_json_object_is_error
 * &struct json_object pointer can be freed by util_json_object_free
 * */
struct json_object* util_json_object_parse_int(int i);



/* util_json_object_parse_double - parse double @d and return the corresponding struct_object object
 * @d: double data
 * @return: &struct json_object pointer
 *
 * &struct json_object pointer can be tested whether is legal by util_json_object_is_error
 * &struct json_object pointer can be freed by util_json_object_free
 * */
struct json_object* util_json_object_parse_double(double d);



/* util_json_object_parse_string - parse string @s and return the corresponding struct_object object
 * @s: char* data
 * @return: &struct json_object pointer
 *
 * &struct json_object pointer can be tested whether is legal by util_json_object_is_error
 * &struct json_object pointer can be freed by util_json_object_free
 * */
struct json_object* util_json_object_parse_string(char *s);



/* util_json_object_parse_string_slice - parse the [0, @len-1] slice of string @s and return the corresponding struct_object object
 * @s: char* data
 * @len: string slice length
 * @return: &struct json_object pointer
 *
 * &struct json_object pointer can be tested whether is legal by util_json_object_is_error
 * &struct json_object pointer can be freed by util_json_object_free
 * */
struct json_object* util_json_object_parse_string_slice(char *s, int len);



/*
 * util_json_object_parse_file - parse the @filename json file and return the corresponding struct_object object
 * @filename: the json file name
 * @return: &struct json_object pointer
 *
 * &struct json_object pointer can be tested whether is legal by util_json_object_is_error
 * &struct json_object pointer can be freed by util_json_object_free
 * */
struct json_object* util_json_object_parse_file(const char *filename);



/* util_json_object_dump_bool - convert @json_object to bool data
 * @json_object: &struct json_object pointer
 * @return: bool data
 *
 * Return false when @json_object is NULL
 * */
bool util_json_object_dump_bool(struct json_object* json_object);



/* util_json_object_dump_int - convert @json_object to int data
 * @json_object: &struct json_object pointer
 * @return: int data
 *
 * Return 0 when @json_object is NULL
 * */
int util_json_object_dump_int(struct json_object* json_object);



/* util_json_object_dump_double - convert @json_object to double data
 * @json_object: &struct json_object pointer
 * @return: double data
 *
 * Return 0.0 when @json_object is NULL
 * */
double util_json_object_dump_double(struct json_object* json_object);



/* util_json_object_dump_string - convert @json_object to double data
 * @json_object: &struct json_object pointer
 * @return: char* data
 *
 * Return NULL when @json_object is NULL
 * */
char* util_json_object_dump_string(struct json_object* json_object);



/* util_json_object_dump_file - dump @json_object to @filename json file
 * @json_object: &struct json_object pointer
 * @filename: the json file name
 * @return: zero for success, non-zero for fail
 *
 * Return -1 when @json_object is NULL
 * */
int util_json_object_dump_file(struct json_object* json_object, const char* filename);



/* util_json_object_dump_table - convert @json_object to struct lh_table object
 * @json_object: &struct json_object pointer
 * @return: &struct lh_table pointer
 *
 * Return NULL when @json_object is NULL
 * */
struct lh_table* util_json_object_dump_table(struct json_object* json_object);



/* util_json_object_dump_array - convert @json_object to struct array_list object
 * @json_object: &struct json_object pointer
 * @return: &struct array_list pointer
 *
 * Return NULL when @json_object is NULL
 * */
struct array_list* util_json_object_dump_array(struct json_object* json_object);

#endif /* __UTIL_JSON_H__ */
