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




/***********************************************��c������������ת����jsonc�����������Ͷ���***************************************************************/
/* ��int����ת����json_type_int���Ͷ���
 * i                  : int
 * return             : struct json_object *(��������)
 * */
#define JsonIntFromInt(i)                                                   json_object_new_int(i)

/* ��double����ת����json_type_double���Ͷ���
 * d                  : double
 * return             : struct json_object *(�������)
 * */
#define JsonDoubleFromDouble(d)                                             json_object_new_double(d)

/* ��string����ת����json_type_string���Ͷ���
 * s                  : string
 * return             : struct json_object *(�ַ�������)
 * */
#define JsonStringFromString(s)                                             json_object_new_string(s)

/* ��boolean����ת����json_type_boolean���Ͷ���
 * b                  : boolean
 * return             : struct json_object *(��������)
 * */
#define JsonBooleanFromBoolean(b)                                           json_object_new_boolean(b)

/* ���s�Ĳ����ַ���, ��ת����json_type_string���Ͷ���
 * s                  : string
 * l                  : �ַ�����ȡ����
 * return             : struct json_object *(�ַ�������)
 * */
#define JsonStrlenFromStrlen(s, l)                                          json_object_new_string_len(s, l)



/***********************************************��jsonc�����������Ͷ���ת����c������������***************************************************************/
/* ��json_type_object�ֵ����Ͷ���ת����struct lh_table
 * jsonObject         : struct json_object *(�ֵ����)
 * return             : struct lh_table *(jsonc��ά����key/value�ڵ���ɵ�˫������)
 * */
#define JsonObjectToTable(jsonObject)                                       json_object_get_object(jsonObject)

/* ��json_type_array�������Ͷ���ת����struct array_list
 * jsonObject         : struct json_object *(�������)
 * return             : struct array_list *(jsonc��ά���Ķ�ά����)
 * */
#define JsonArrayToArray(jsonObject)                                        json_object_get_array(jsonObject)

/* ��json_type_boolean�������Ͷ���ת����boolean
 * jsonObject         : struct json_object *(��������)
 * return             : boolean(jsonc��ά����unsigned char)
 * */
#define JsonBooleanToBoolean(jsonObject)                                    json_object_get_boolean(jsonObject)

/* ��json_type_int�������Ͷ���ת����int
 * jsonObject         : struct json_object *(��������)
 * return             : int
 * */
#define JsonIntToInt(jsonObject)                                            json_object_get_int(jsonObject)

/* ��json_type_double�������Ͷ���ת����double
 * jsonObject         : struct json_object *(�������)
 * return             : double
 * */
#define JsonDoubleToDouble(jsonObject)                                      json_object_get_double(jsonObject)

/* ��json_type_string�ַ������Ͷ���ת����string
 * jsonObject         : struct json_object *(�ַ�������)
 * return             : string
 * */
#define JsonStringToString(jsonObject)                                      json_object_get_string(jsonObject)


#define   JSON_FIELD_STRLEN_MAX       (350)     /* ͨ���ַ����������ֵ */
#define   JSON_FIELD_STRLEN_MID       (101)     /* ͨ���ַ�����������ֵ(�Ѻ�UIǰ��Լ��,�ı�������󳤶�Ϊ100) */
#define   JSON_FIELD_STRLEN_MIN       (30)      /* ͨ���ַ���������Сֵ */
#define   JSON_NODE_STRLEN_MAX        (150)     /* �ڵ��ִ����� */
#define   JSON_FIELD_STRLEN           (200)     /* json�ڵ��ַ�����󳤶� */
#define   JSON_ARRAY_DEPTH            (2)       /* json����Ƕ�ײ���, ������ */
#define   JSON_FIELD_NUM_MAX          (4)       /* a.b.c[0]Ϊ1��field;a.b.c[0].c[0]Ϊ2��field */
typedef struct json_array_index
{
    BOOL           exist;          /* �����Ƿ���� */
    UINT32         index;          /* ����id */
} JSON_ARRAY_INDEX;

typedef struct json_field
{
    struct
    {
        BOOL                 exist;                                    /* �ֶ��Ƿ���� */
        char                 jsonDictField[JSON_FIELD_STRLEN];         /* �ֵ��ֶ� */
        JSON_ARRAY_INDEX     jsonArrayFiled[JSON_ARRAY_DEPTH];         /* �����ֶ� */
    } field[JSON_FIELD_NUM_MAX];
} JSON_FIELD;

/***********************************************utiljson.c��װ�Ļ�������api***************************************************************/

/**
 * @brief Search the object of specify field in container
 * (�����ֵ�����ȡָ��·����(a.b.c)ĩ�ڵ�(c)��Ӧ�Ķ���)
 *
 * @par Usage:
 *
 * @code
 * JsonSearchObject(container, "a.b.c");
 * @endcode
 *
 * @param container---The container of the fields          (�������: �ֵ����)
 * @param field-------The field to search, delimited by "."(�������: ��֧��a.b.c;��֧��a.b.c[0])
 *
 * @return struct json_object *----NULL����c��Ӧ��jsonc����(�ֵ�/����/�ַ���/����/����������)
 */
struct json_object *JsonSearchObject(struct json_object *container, const char *field);


/**
 * @brief Search the object of specify field in container
 * (�����ֵ�����ȡָ��·����(a.b.c[0].d)ĩ�ڵ�(d)��Ӧ�Ķ���, ֧�������Լ�����Ƕ��)
 *
 * @par Usage:
 *
 * @code
 * JsonSearchObject(container, "a.b.c[0].d");
 * @endcode
 *
 * @param container---The container of the fields          (�������: �ֵ����)
 * @param field-------The field to search, delimited by "."(�������: ֧��a.b.c/a.b.c[0]/a.b.c[0].d/a.b.c[0][0].d)
 *
 * @return struct json_object *----NULL����d��Ӧ��jsonc����(�ַ���/����/����������)
 */
struct json_object *JsonSearchObjectEnhance(struct json_object *container, const char *field);

/**
 * @brief Seach the parent object of specify field. The field is delimited by '.'\n
 *              This function returns the node name.
 * (�����ֵ�����ȡָ��·����(a.b.c)ĩ�ڵ�ĸ��ڵ�(b)��Ӧ�Ķ���(�ֵ����),������c�������ִ�
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
 * @param container----The json object container that the field included (�������: �ֵ����)
 * @param field--------The field in json object                          (�������: ��֧��a.b.c;��֧��a.b.c[0])
 * @param node---------return the content of c node                      (��������: ����c�������ִ�)
 * @param bufsize------declare the size of node param                    (�������: node����buffer size)
 *
 * @return struct json_object *----NULL����b��Ӧ��jsonc����(�ֵ����), a��b�ڵ㲻�����򷵻�NULL
 */
struct json_object *JsonSearchParentObject(struct json_object *container, const char *field, char *node, int bufsize);


/**
 * @brief Seach the parent object of specify field. The field is delimited by '.'\n
 *              This function returns the node name.
 * (�����ֵ�����ȡָ��·����(a.b.c)ĩ�ڵ�ĸ��ڵ�(b)��Ӧ�Ķ���(�ֵ����),������c�������ִ�
 *  ��ȡָ��·����(a.b.c[0])ĩ�ڵ�ĸ��ڵ�(c)��Ӧ�Ķ���(�������),��������������
 *  ��ȡָ��·����(a.b.c[0].d)ĩ�ڵ�ĸ��ڵ�([])��Ӧ�Ķ���(�ֵ�������),������d�������Ӵ�)
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
 * @param container----The json object container that the field included (�������: �ֵ����)
 * @param field--------The field in json object                          (�������: ֧��a.b.c/a.b.c[0]/a.b.c[0].d/a.b.c[0][0].d)
 * @param node---------return the content of c node                      (��������: ����ĩ�ڵ������ִ�����������)
 * @param bufsize------declare the size of node param                    (�ɱ����: nodeΪ�Ӵ�ʱ��buffer size)
 *
 * @return struct json_object *----NULL����b��Ӧ��jsonc����(�ֵ�/�������), a��b�ڵ㲻�����򷵻�NULL
 */
struct json_object *JsonSearchParentObjectEnhance(struct json_object *container, const char *field, void *node, ...);

/**
 * @brief Malloc the new parent object of the node if the parent path doesn't exist. If the \n
 *              parent path esists, just return its location
 * (�����ֵ�����ȡָ��·����(a.b.c)ĩ�ڵ�ĸ��ڵ�(b)��Ӧ�Ķ���,������c�������ִ�, ���a��b�ڵ㲻����, �򴴽�֮)
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
 * @param container----The json object container that the field included(�������: �ֵ����)
 * @param field--------The field in json object                         (�������: ��֧��a.b.c;��֧��a.b.c[0])
 * @param node---------The node name to get out                         (��������: char *����c�������ִ�)
 * @param bufsize------The buffer size                                  (�������: node����buffer size)
 *
 * @return struct json_object *----NULL����b��Ӧ��jsonc����(�ֵ�/����/�ַ���/����/����������), a��b�ڵ㲻�����򴴽����֮
 */
struct json_object *JsonNewParentObject(struct json_object *container, const char *field, char *node, int bufsize);

/**
 * @brief Malloc the new parent object of the node if the parent path doesn't exist. If the \n
 *              parent path esists, just return its location
 * (�����ֵ�����ȡָ��·����(a.b.c)ĩ�ڵ�ĸ��ڵ�(b)��Ӧ�Ķ���,������c�������ִ�, ���a��b�ڵ㲻����, �򴴽�֮
 *  ��ȡָ��·����(a.b.c[0])ĩ�ڵ�ĸ��ڵ�(c)��Ӧ�Ķ���(�������),��������������,���a��b�ڵ㲻����, �򴴽�֮
 *  ��ȡָ��·����(a.b.c[0].d)ĩ�ڵ�ĸ��ڵ�([])��Ӧ�Ķ���(�ֵ�������),������d�������Ӵ�, ���a��b�ڵ㲻����, �򴴽�֮)
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
 * @param container----The json object container that the field included(�������: �ֵ����)
 * @param field--------The field in json object                         (�������: ֧��a.b.c/a.b.c[0]/a.b.c[0].d/a.b.c[0][0].d)
 * @param node---------The node name to get out                         (��������: ����ĩ�ڵ������ִ�����������)
 * @param bufsize------The buffer size                                  (�ɱ����: nodeΪ�Ӵ�ʱ��buffer size)
 *
 * @return struct json_object *----NULL����b��Ӧ��jsonc����(�ֵ�/�������), a��b�ڵ㲻�����򴴽����֮
 */
struct json_object *JsonNewParentObjectEnhance(struct json_object *container, const char *field, void *node, ...);


/**
 * @brief Get the value of specify field in container, the format of field is "obj.subobj.field"\n
 *              YOU SHOULD pay more attention when the type is json_type_string, because the function\n
 *              DOES NOT check the buffer length.
 * (����container�����ȡ����·��(a.b.c��a.b.c[0])��ĩ�ڵ�(c)��Ӧ�Ķ���
 *  �ö���������������ַ�����������������(������������·�������������ȡ��ӦԪ��)
 *  �����ݴ����type����c�����������ݸ�value(��value���������ַ���ʱ, ���ַ�����buffer size����ͨ������ı�δ���)
 *  �˴�value������������, �ַ���, ������;�ɹ�����0, ʧ�ܷ���-1
 *  ֧��a.b.c/a.b.c[0]/a.b.c[0][0]/a.b.c[0].e/a.b.c[0][0].e/a.b.c[0].c[0]��)
 *
 * @par Usage:
 *
 * @code
 * char value[100];
 * JsonGetField(container, "obj.field", value, json_type_string, 100);
 * @endcode
 *
 * @param container The json object container that the field included  (�������: �ֵ����)
 * @param field The field in json object                               (�������: ֧��a.b.c��a.b.c[0]�Լ�a.b.c[0].a�Լ�����Ƕ��)
 * @param value The value to get out                                   (��������: void *value���Է����������ַ�����������)
 * @param type The type of value                                       (�������: value������Ӧ��jsonc��������, ��֧������/�ַ���/������)
 * @param ... When the type is 'string', the extended argument -- the length of the buffer allocated to value must be\n
 *        provided                                                     (�ɱ䴫�����:��value��Ҫ�����ַ���ʱ, ָ����buffer size)
 *
 * @return                                                             (�ɹ�����0;ʧ�ܷ���-1)
 */
int JsonGetField(struct json_object *container, const char *field, void *value, enum json_type type, ...);

int JsonGetFieldUrl(struct json_object *container, const char *name, void *value, enum json_type type, ...);


/**
 * @brief Set the value of specify field in container, the format of field is "obj.subobj.field"
 * (��container������a.b.c·����c�ڵ��Ӧ�Ķ���ֵ�滻��valueת���ɵĶ���
 * ֧��a.b.c/a.b.c[0]/a.b.c[0][0]/a.b.c[0].e/a.b.c[0][0].e/a.b.c[0].c[0]��)
 * @par Usage:
 *
 * @code
 * JsonSetField(container, "obj.field", "value", json_type_string);
 * @endcode
 *
 * @param container The json object container that the field included (�������:�ֵ����)
 * @param field The field in json object                              (�������:֧��a.b.c��a.b.c[0]�Լ�a.b.c[0].a�Լ�����Ƕ��)
 * @param value The value to set                                      (�������:void *value���Դ����������ַ�����������)
 * @param type The type of value                                      (�������:value��Ӧ��jsonc��������, ��֧������/�ַ���/������)
 *
 * @return                                                            (�ɹ�����0;ʧ�ܷ���-1)
 */
int JsonSetField(struct json_object *container, const char *field, const void *value, enum json_type type);


/**
 * @brief Assign a filed with the specify value of specify field in container\n
 *              the format of field is "obj.subobj.field"
 * (��container������a.b.c·����c�ڵ��Ӧ�Ķ���ֵ�滻��valueת���ɵĶ���, a��b�ڵ㲻����, �򴴽�֮
 * ֧��a.b.c/a.b.c[0]/a.b.c[0][0]/a.b.c[0].e/a.b.c[0][0].e/a.b.c[0].c[0]��)
 * @par Usage:
 *
 * @code
 * JsonNewField(container, "obj.field", "value", json_type_string);
 * @endcode
 *
 * @param container The json object container that the field included (�������:�ֵ����)
 * @param field The field in json object                              (�������:֧��a.b.c��a.b.c[0]�Լ�a.b.c[0].a�Լ�����Ƕ��)
 * @param value The value to set                                      (�������:void *value���Դ����������ַ�����������)
 * @param type The type of value                                      (�������:value��Ӧ��jsonc��������, ��֧������/�ַ���/������)
 *
 * @return                                                            (�ɹ�����0;ʧ�ܷ���-1)
 */
int JsonNewField(struct json_object *container, const char *field, const void *value, enum json_type type);


/**
 * @brief Get the value of specify array field in container, the format of field is "obj.subobj.field"\n
 *              When the "type" is json_type_string, please call New2DArray to malloc a string array\n
 *              and Free2DArray in the end, these two function is in the util2darray.h interface.
 * (����container�����ȡ����·��(a.b.c)��ĩ�ڵ�(c)��Ӧ�Ķ���, �ö����������������
 *  Ȼ����ݴ����type����c��������������ɵ������value
 *  value��ʹ��New2DArray����Ķ�ά����, �����С��size��������
 *  ���value��ά����洢�����ַ���, ���ַ���buffer size�ɺ���ı�δ���
 *  �ɹ�ʱ����value������ʵ�ʰ�����Ԫ�ظ���, ʧ�ܷ���-1)
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
 * @param container The json object container that the field included  (�������:�ֵ����)
 * @param field The field in json object                               (�������:��֧��a.b.c;��֧��a.b.c[0]==����������Ӷ�����Ƕ�׵Ľ���)
 * @param value The value to get out                                   (��������:void **value���Է����������ַ���������������)
 * @param type The type of value                                       (�������:value����Ԫ�ض�Ӧ��jsonc��������, ��֧������/�ַ���/������)
 * @param ... When the type is 'string', the extended argument -- the length of the buffer allocated to value must be\n
 *        provided                                                     (�ɱ䴫�����:��value���ַ�������ʱ, ָ����ÿ��Ԫ��buffer size)
 *
 * @return                                                             (�ɹ�ʱ����value������ʵ�ʰ�����Ԫ�ظ���, ʧ�ܷ���-1)
 */
int JsonGetFieldArray(struct json_object *container, const char *field, void **value, int size, enum json_type type, ...);

/**
 * @brief Set the value of specify field in container, the format of field is "obj.subobj.field"
 *              When the "type" is json_type_string, please call New2DArray to malloc a string array\n
 *              and Free2DArray in the end, these two function is in the util2darray.h interface.
 * (��container������a.b.c·����c�ڵ��Ӧ���������ֵ�滻��value��ά����ת���ɵ��������
 *  ���value��ά���鳤��lenС��c�ڵ�������󳤶�, ���滻ǰlen������Ԫ�ض���
 *  ���value��ά���鳤��len����c�ڵ�������󳤶�, �򷵻ش���)
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
 * @param container The json object container that the field included  (�������:�ֵ����)
 * @param field The field in json object                               (�������:��֧��a.b.c;��֧��a.b.c[0]==����������Ӷ�����Ƕ�׵Ľ���)
 * @param value The value to set                                       (�������:void **value���Է����������ַ���������������)
 * @param type The type of value                                       (�������:value����Ԫ�ض�Ӧ��jsonc��������, ��֧������/�ַ���/������)
 * @param size The elment count of array                               (�������:value����Ԫ�صĸ���)
 *
 * @return                                                             (�ɹ�ʱ����value������ʵ�ʰ�����Ԫ�ظ���, ʧ�ܷ���-1)
 */
int JsonSetFieldArray(struct json_object *container, const char *field, void **value, enum json_type type, int size);


/**
 * @brief Assign an array with specify the value of specify field in container, \n
 *              the format of field is "obj.subobj.field".\n
 *              When the "type" is json_type_string, please call New2DArray to malloc a string array\n
 *              and Free2DArray in the end, these two function is in the util2darray.h interface.
 * (��container������a.b.c·����c�ڵ��Ӧ���������ֵ��ȫ�滻��value��ά����ת���ɵ��������, a��b�ڵ㲻����, �򴴽�֮)
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
 * @param container The json object container that the field included   (�������:�ֵ����)
 * @param field The field in json object                                (�������:��֧��a.b.c;��֧��a.b.c[0]==����������Ӷ�����Ƕ�׵Ľ���)
 * @param value The value to set                                        (�������:void **value���Է����������ַ���������������)
 * @param type The type of value                                        (�������:value����Ԫ�ض�Ӧ��jsonc��������, ��֧������/�ַ���/������)
 * @param size The elment count of array                                (�������:value����Ԫ�صĸ���)
 *
 * @return                                                              (�ɹ�ʱ����value������ʵ�ʰ�����Ԫ�ظ���, ʧ�ܷ���-1)
 */
int JsonNewFieldArray(struct json_object *container, const char *field, void **value, enum json_type type, int size);


#endif /* __UTIL_JSON_H__ */
