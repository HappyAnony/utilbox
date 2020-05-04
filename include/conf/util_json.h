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




/***********************************************将c基本数据类型转换成jsonc基本数据类型对象***************************************************************/
/* 将int类型转换成json_type_int类型对象
 * i                  : int
 * return             : struct json_object *(整数对象)
 * */
#define JsonIntFromInt(i)                                                   json_object_new_int(i)

/* 将double类型转换成json_type_double类型对象
 * d                  : double
 * return             : struct json_object *(浮点对象)
 * */
#define JsonDoubleFromDouble(d)                                             json_object_new_double(d)

/* 将string类型转换成json_type_string类型对象
 * s                  : string
 * return             : struct json_object *(字符串对象)
 * */
#define JsonStringFromString(s)                                             json_object_new_string(s)

/* 将boolean类型转换成json_type_boolean类型对象
 * b                  : boolean
 * return             : struct json_object *(布尔对象)
 * */
#define JsonBooleanFromBoolean(b)                                           json_object_new_boolean(b)

/* 获得s的部分字符串, 并转换成json_type_string类型对象
 * s                  : string
 * l                  : 字符串截取长度
 * return             : struct json_object *(字符串对象)
 * */
#define JsonStrlenFromStrlen(s, l)                                          json_object_new_string_len(s, l)



/***********************************************将jsonc基本数据类型对象转换成c基本数据类型***************************************************************/
/* 将json_type_object字典类型对象转换成struct lh_table
 * jsonObject         : struct json_object *(字典对象)
 * return             : struct lh_table *(jsonc库维护的key/value节点组成的双向链表)
 * */
#define JsonObjectToTable(jsonObject)                                       json_object_get_object(jsonObject)

/* 将json_type_array数组类型对象转换成struct array_list
 * jsonObject         : struct json_object *(数组对象)
 * return             : struct array_list *(jsonc库维护的二维数组)
 * */
#define JsonArrayToArray(jsonObject)                                        json_object_get_array(jsonObject)

/* 将json_type_boolean布尔类型对象转换成boolean
 * jsonObject         : struct json_object *(布尔对象)
 * return             : boolean(jsonc库维护的unsigned char)
 * */
#define JsonBooleanToBoolean(jsonObject)                                    json_object_get_boolean(jsonObject)

/* 将json_type_int整数类型对象转换成int
 * jsonObject         : struct json_object *(整数对象)
 * return             : int
 * */
#define JsonIntToInt(jsonObject)                                            json_object_get_int(jsonObject)

/* 将json_type_double浮点类型对象转换成double
 * jsonObject         : struct json_object *(浮点对象)
 * return             : double
 * */
#define JsonDoubleToDouble(jsonObject)                                      json_object_get_double(jsonObject)

/* 将json_type_string字符串类型对象转换成string
 * jsonObject         : struct json_object *(字符串对象)
 * return             : string
 * */
#define JsonStringToString(jsonObject)                                      json_object_get_string(jsonObject)


#define   JSON_FIELD_STRLEN_MAX       (350)     /* 通用字符串长度最大值 */
#define   JSON_FIELD_STRLEN_MID       (101)     /* 通用字符串长度适中值(已和UI前端约定,文本输入最大长度为100) */
#define   JSON_FIELD_STRLEN_MIN       (30)      /* 通用字符串长度最小值 */
#define   JSON_NODE_STRLEN_MAX        (150)     /* 节点字串长度 */
#define   JSON_FIELD_STRLEN           (200)     /* json节点字符串最大长度 */
#define   JSON_ARRAY_DEPTH            (2)       /* json数组嵌套层数, 可配置 */
#define   JSON_FIELD_NUM_MAX          (4)       /* a.b.c[0]为1个field;a.b.c[0].c[0]为2个field */
typedef struct json_array_index
{
    BOOL           exist;          /* 索引是否存在 */
    UINT32         index;          /* 索引id */
} JSON_ARRAY_INDEX;

typedef struct json_field
{
    struct
    {
        BOOL                 exist;                                    /* 字段是否存在 */
        char                 jsonDictField[JSON_FIELD_STRLEN];         /* 字典字段 */
        JSON_ARRAY_INDEX     jsonArrayFiled[JSON_ARRAY_DEPTH];         /* 数组字段 */
    } field[JSON_FIELD_NUM_MAX];
} JSON_FIELD;

/***********************************************utiljson.c封装的基本操作api***************************************************************/

/**
 * @brief Search the object of specify field in container
 * (遍历字典对象获取指定路径中(a.b.c)末节点(c)对应的对象)
 *
 * @par Usage:
 *
 * @code
 * JsonSearchObject(container, "a.b.c");
 * @endcode
 *
 * @param container---The container of the fields          (传入参数: 字典对象)
 * @param field-------The field to search, delimited by "."(传入参数: 仅支持a.b.c;不支持a.b.c[0])
 *
 * @return struct json_object *----NULL或者c对应的jsonc对象(字典/数组/字符串/整数/浮点数对象)
 */
struct json_object *JsonSearchObject(struct json_object *container, const char *field);


/**
 * @brief Search the object of specify field in container
 * (遍历字典对象获取指定路径中(a.b.c[0].d)末节点(d)对应的对象, 支持数组以及数组嵌套)
 *
 * @par Usage:
 *
 * @code
 * JsonSearchObject(container, "a.b.c[0].d");
 * @endcode
 *
 * @param container---The container of the fields          (传入参数: 字典对象)
 * @param field-------The field to search, delimited by "."(传入参数: 支持a.b.c/a.b.c[0]/a.b.c[0].d/a.b.c[0][0].d)
 *
 * @return struct json_object *----NULL或者d对应的jsonc对象(字符串/整数/浮点数对象)
 */
struct json_object *JsonSearchObjectEnhance(struct json_object *container, const char *field);

/**
 * @brief Seach the parent object of specify field. The field is delimited by '.'\n
 *              This function returns the node name.
 * (遍历字典对象获取指定路径中(a.b.c)末节点的根节点(b)对应的对象(字典对象),并返回c的内容字串
 *
 * @par Usage:
 *
 * @code
 * struct json_object *obj;
 * char node[100];
 * obj = JsonSearchParentObject(container, "a.b.c", node, sizeof(node));
 * //Now we get json object of "a.b" field, and node=="c"
 * @endcode
 *
 * @param container----The json object container that the field included (传入参数: 字典对象)
 * @param field--------The field in json object                          (传入参数: 仅支持a.b.c;不支持a.b.c[0])
 * @param node---------return the content of c node                      (传出参数: 返回c的内容字串)
 * @param bufsize------declare the size of node param                    (传入参数: node参数buffer size)
 *
 * @return struct json_object *----NULL或者b对应的jsonc对象(字典对象), a和b节点不存在则返回NULL
 */
struct json_object *JsonSearchParentObject(struct json_object *container, const char *field, char *node, int bufsize);


/**
 * @brief Seach the parent object of specify field. The field is delimited by '.'\n
 *              This function returns the node name.
 * (遍历字典对象获取指定路径中(a.b.c)末节点的根节点(b)对应的对象(字典对象),并返回c的内容字串
 *  获取指定路径中(a.b.c[0])末节点的根节点(c)对应的对象(数组对象),并返回数组索引
 *  获取指定路径中(a.b.c[0].d)末节点的根节点([])对应的对象(字典对象对象),并返回d的内容子串)
 *
 * @par Usage:
 *
 * @code
 * struct json_object *obj;
 * char node[100];
 * obj = JsonSearchParentObject(container, "a.b.c[0].d", node, sizeof(node));
 * //Now we get json object of "a.b.c[]" field, and node=="d"
 * @endcode
 *
 * @param container----The json object container that the field included (传入参数: 字典对象)
 * @param field--------The field in json object                          (传入参数: 支持a.b.c/a.b.c[0]/a.b.c[0].d/a.b.c[0][0].d)
 * @param node---------return the content of c node                      (传出参数: 返回末节点内容字串或数组索引)
 * @param bufsize------declare the size of node param                    (可变参数: node为子串时的buffer size)
 *
 * @return struct json_object *----NULL或者b对应的jsonc对象(字典/数组对象), a和b节点不存在则返回NULL
 */
struct json_object *JsonSearchParentObjectEnhance(struct json_object *container, const char *field, void *node, ...);

/**
 * @brief Malloc the new parent object of the node if the parent path doesn't exist. If the \n
 *              parent path esists, just return its location
 * (遍历字典对象获取指定路径中(a.b.c)末节点的根节点(b)对应的对象,并返回c的内容字串, 如果a或b节点不存在, 则创建之)
 *
 * @par Usage:
 *
 * @code
 * char node[100];
 * struct json_object *obj;
 *
 * obj = JsonNewObject ();
 * JsonNewParentObject(obj, "a.b.c.d", node, sizeof(node));
 * printf ("obj:%s\n", JsonToString(obj));
 * JsonFreeObject (obj);
 * @endcode
 *
 * @param container----The json object container that the field included(传入参数: 字典对象)
 * @param field--------The field in json object                         (传入参数: 仅支持a.b.c;不支持a.b.c[0])
 * @param node---------The node name to get out                         (传出参数: char *返回c的内容字串)
 * @param bufsize------The buffer size                                  (传入参数: node参数buffer size)
 *
 * @return struct json_object *----NULL或者b对应的jsonc对象(字典/数组/字符串/整数/浮点数对象), a和b节点不存在则创建添加之
 */
struct json_object *JsonNewParentObject(struct json_object *container, const char *field, char *node, int bufsize);

/**
 * @brief Malloc the new parent object of the node if the parent path doesn't exist. If the \n
 *              parent path esists, just return its location
 * (遍历字典对象获取指定路径中(a.b.c)末节点的根节点(b)对应的对象,并返回c的内容字串, 如果a或b节点不存在, 则创建之
 *  获取指定路径中(a.b.c[0])末节点的根节点(c)对应的对象(数组对象),并返回数组索引,如果a或b节点不存在, 则创建之
 *  获取指定路径中(a.b.c[0].d)末节点的根节点([])对应的对象(字典对象对象),并返回d的内容子串, 如果a或b节点不存在, 则创建之)
 *
 * @par Usage:
 *
 * @code
 * char node[100];
 * struct json_object *obj;
 *
 * obj = JsonNewObject();
 * JsonNewParentObject(obj, "a.b.c.d[0].e", node, sizeof(node));
 * printf ("obj:%s\n", JsonToString(obj));
 * JsonFreeObject (obj);
 * @endcode
 *
 * @param container----The json object container that the field included(传入参数: 字典对象)
 * @param field--------The field in json object                         (传入参数: 支持a.b.c/a.b.c[0]/a.b.c[0].d/a.b.c[0][0].d)
 * @param node---------The node name to get out                         (传出参数: 返回末节点内容字串或数组索引)
 * @param bufsize------The buffer size                                  (可变参数: node为子串时的buffer size)
 *
 * @return struct json_object *----NULL或者b对应的jsonc对象(字典/数组对象), a和b节点不存在则创建添加之
 */
struct json_object *JsonNewParentObjectEnhance(struct json_object *container, const char *field, void *node, ...);


/**
 * @brief Get the value of specify field in container, the format of field is "obj.subobj.field"\n
 *              YOU SHOULD pay more attention when the type is json_type_string, because the function\n
 *              DOES NOT check the buffer length.
 * (遍历container对象获取给定路径(a.b.c或a.b.c[0])中末节点(c)对应的对象
 *  该对象可能是整数、字符串、浮点数、数组(如果是数组根据路径后面的索引获取对应元素)
 *  最后根据传入的type返回c基本类型数据给value(当value用来返回字符串时, 其字符串的buffer size可以通过后面的变参传入)
 *  此处value用来返回整数, 字符串, 浮点数;成功返回0, 失败返回-1
 *  支持a.b.c/a.b.c[0]/a.b.c[0][0]/a.b.c[0].e/a.b.c[0][0].e/a.b.c[0].c[0]等)
 *
 * @par Usage:
 *
 * @code
 * char value[100];
 * JsonGetField(container, "obj.field", value, json_type_string, 100);
 * @endcode
 *
 * @param container The json object container that the field included  (传入参数: 字典对象)
 * @param field The field in json object                               (传入参数: 支持a.b.c和a.b.c[0]以及a.b.c[0].a以及数组嵌套)
 * @param value The value to get out                                   (传出参数: void *value可以返回整数、字符串、浮点数)
 * @param type The type of value                                       (传入参数: value参数对应的jsonc数据类型, 仅支持整数/字符串/浮点数)
 * @param ... When the type is 'string', the extended argument -- the length of the buffer allocated to value must be\n
 *        provided                                                     (可变传入参数:当value需要返回字符串时, 指定其buffer size)
 *
 * @return                                                             (成功返回0;失败返回-1)
 */
int JsonGetField(struct json_object *container, const char *field, void *value, enum json_type type, ...);

int JsonGetFieldUrl(struct json_object *container, const char *name, void *value, enum json_type type, ...);


/**
 * @brief Set the value of specify field in container, the format of field is "obj.subobj.field"
 * (将container对象中a.b.c路径的c节点对应的对象值替换成value转换成的对象
 * 支持a.b.c/a.b.c[0]/a.b.c[0][0]/a.b.c[0].e/a.b.c[0][0].e/a.b.c[0].c[0]等)
 * @par Usage:
 *
 * @code
 * JsonSetField(container, "obj.field", "value", json_type_string);
 * @endcode
 *
 * @param container The json object container that the field included (传入参数:字典对象)
 * @param field The field in json object                              (传入参数:支持a.b.c和a.b.c[0]以及a.b.c[0].a以及数组嵌套)
 * @param value The value to set                                      (传入参数:void *value可以传入整数、字符串、浮点数)
 * @param type The type of value                                      (传入参数:value对应的jsonc数据类型, 仅支持整数/字符串/浮点数)
 *
 * @return                                                            (成功返回0;失败返回-1)
 */
int JsonSetField(struct json_object *container, const char *field, const void *value, enum json_type type);


/**
 * @brief Assign a filed with the specify value of specify field in container\n
 *              the format of field is "obj.subobj.field"
 * (将container对象中a.b.c路径的c节点对应的对象值替换成value转换成的对象, a或b节点不存在, 则创建之
 * 支持a.b.c/a.b.c[0]/a.b.c[0][0]/a.b.c[0].e/a.b.c[0][0].e/a.b.c[0].c[0]等)
 * @par Usage:
 *
 * @code
 * JsonNewField(container, "obj.field", "value", json_type_string);
 * @endcode
 *
 * @param container The json object container that the field included (传入参数:字典对象)
 * @param field The field in json object                              (传入参数:支持a.b.c和a.b.c[0]以及a.b.c[0].a以及数组嵌套)
 * @param value The value to set                                      (传入参数:void *value可以传入整数、字符串、浮点数)
 * @param type The type of value                                      (传入参数:value对应的jsonc数据类型, 仅支持整数/字符串/浮点数)
 *
 * @return                                                            (成功返回0;失败返回-1)
 */
int JsonNewField(struct json_object *container, const char *field, const void *value, enum json_type type);


/**
 * @brief Get the value of specify array field in container, the format of field is "obj.subobj.field"\n
 *              When the "type" is json_type_string, please call New2DArray to malloc a string array\n
 *              and Free2DArray in the end, these two function is in the util2darray.h interface.
 * (遍历container对象获取给定路径(a.b.c)中末节点(c)对应的对象, 该对象必须是数组类型
 *  然后根据传入的type返回c基本类型数据组成的数组给value
 *  value是使用New2DArray申请的二维数组, 数组大小由size参数传入
 *  如果value二维数组存储的是字符串, 则字符串buffer size由后面的变参传入
 *  成功时返回value数组中实际包含的元素个数, 失败返回-1)
 * @par Usage:
 *
 * @code
 * char **strArray;
 * int arrayLen;
 *
 * //String Array
 * New2DArray(&strArray, 10, 100);
 * arrayLen = JsonGetFieldArray(jsonInObject, "obj.strarr", (void **)strArray, json_type_string, 100);
 * //Check arrayLen and use strArray[idx] to get the strings
 * Free2DArray(&strArray, 10);
 *
 * //Integer Array
 * int intArray[3] = {0};
 * arrayLen = JsonGetFieldArray(jsonInObject, "obj.intarr", (void **)&intArray, json_type_int);
 * //Check arrayLen and use intArray[idx] to get the integers
 * @endcode
 *
 * @param container The json object container that the field included  (传入参数:字典对象)
 * @param field The field in json object                               (传入参数:仅支持a.b.c;不支持a.b.c[0]==后面可以增加对数组嵌套的解析)
 * @param value The value to get out                                   (传出参数:void **value可以返回整数、字符串、浮点数数组)
 * @param type The type of value                                       (传入参数:value数组元素对应的jsonc数据类型, 仅支持整数/字符串/浮点数)
 * @param ... When the type is 'string', the extended argument -- the length of the buffer allocated to value must be\n
 *        provided                                                     (可变传入参数:当value是字符串数组时, 指定其每个元素buffer size)
 *
 * @return                                                             (成功时返回value数组中实际包含的元素个数, 失败返回-1)
 */
int JsonGetFieldArray(struct json_object *container, const char *field, void **value, int size, enum json_type type, ...);

/**
 * @brief Set the value of specify field in container, the format of field is "obj.subobj.field"
 *              When the "type" is json_type_string, please call New2DArray to malloc a string array\n
 *              and Free2DArray in the end, these two function is in the util2darray.h interface.
 * (将container对象中a.b.c路径的c节点对应的数组对象值替换成value二维数组转换成的数组对象
 *  如果value二维数组长度len小于c节点数组对象长度, 则替换前len个数组元素对象
 *  如果value二维数组长度len大于c节点数组对象长度, 则返回错误)
 * @par Usage:
 *
 * @code
 * char **strArray;
 * int arrayLen;
 *
 * //String Array
 * New2DArray(&strArray, 3, 100);
 * for (i = 0; i < 3; ++i)
 * {
 *      sprintf(strArray[i], "str%d", i);
 * }
 * arrayLen = JsonSetFieldArray(jsonInObject, "obj.strarr", (void **)strArray, json_type_string, 3);
 * Free2DArray(&strArray, 3);
 *
 * //Integer Array
 * int intArray[3];
 * intArray[0] = 0xc;
 * intArray[1] = 0x0;
 * intArray[2] = 0x1;
 * arrayLen = JsonSetFieldArray(jsonInObject, "obj.intarr", (void **)&intArray, json_type_int, 3);
 * @endcode
 *
 * @param container The json object container that the field included  (传入参数:字典对象)
 * @param field The field in json object                               (传入参数:仅支持a.b.c;不支持a.b.c[0]==后面可以增加对数组嵌套的解析)
 * @param value The value to set                                       (传入参数:void **value可以返回整数、字符串、浮点数数组)
 * @param type The type of value                                       (传入参数:value数组元素对应的jsonc数据类型, 仅支持整数/字符串/浮点数)
 * @param size The elment count of array                               (传入参数:value数组元素的个数)
 *
 * @return                                                             (成功时返回value数组中实际包含的元素个数, 失败返回-1)
 */
int JsonSetFieldArray(struct json_object *container, const char *field, void **value, enum json_type type, int size);


/**
 * @brief Assign an array with specify the value of specify field in container, \n
 *              the format of field is "obj.subobj.field".\n
 *              When the "type" is json_type_string, please call New2DArray to malloc a string array\n
 *              and Free2DArray in the end, these two function is in the util2darray.h interface.
 * (将container对象中a.b.c路径的c节点对应的数组对象值完全替换成value二维数组转换成的数组对象, a或b节点不存在, 则创建之)
 * @par Usage:
 *
 * @code
 * char **strArray;
 * int arrayLen;
 *
 * //String Array
 * New2DArray (&strArray, 3, 100);
 * for (i = 0; i < 3; ++i)
 * {
 *  sprintf (strArray[i], "str%d", i);
 * }
 * arrayLen = JsonNewFieldArray (jsonInObject, "obj.strarr", (void **)strArray, json_type_string, 3);
 * Free2DArray (&strArray, 3);
 *
 * //Integer Array
 * int intArray[3];
 * intArray[0] = 0xc;
 * intArray[1] = 0x0;
 * intArray[2] = 0x1;
 * JsonNewFieldArray (jsonInObject, "obj.intarr", (void **)&intArray, json_type_int, 3);
 * @endcode
 *
 * @param container The json object container that the field included   (传入参数:字典对象)
 * @param field The field in json object                                (传入参数:仅支持a.b.c;不支持a.b.c[0]==后面可以增加对数组嵌套的解析)
 * @param value The value to set                                        (传入参数:void **value可以返回整数、字符串、浮点数数组)
 * @param type The type of value                                        (传入参数:value数组元素对应的jsonc数据类型, 仅支持整数/字符串/浮点数)
 * @param size The elment count of array                                (传入参数:value数组元素的个数)
 *
 * @return                                                              (成功时返回value数组中实际包含的元素个数, 失败返回-1)
 */
int JsonNewFieldArray(struct json_object *container, const char *field, void **value, enum json_type type, int size);


#endif /* __UTIL_JSON_H__ */
