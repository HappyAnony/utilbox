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
#ifndef __UTIL_JSON_PARSER_H__
#define __UTIL_JSON_PARSER_H__

#include <conf/util_json.h>


/*
 * Simple json-parser implementation based on json-c library
 *
 * Json-parser parses and encapsulates json data as JSON-OID to JSON-OBJECT
 *
 * JSON-OBJECT: struct json_object object in json-c library
 *
 * JSON-OID: similar to SNMP-OID
 * For example:
 * {
 *     "a" : {
 *        "b" : [
 *           {
 *              "c" : [d,e]
 *           },
 *           {
 *              "f" : [[g]]
 *           }
 *        ]
 *     }
 * }
 * the JSON-OID corresponding to c is a.b[0].c
 * the JSON-OID corresponding to e is a.b[0].c[1]
 * the JSON-OID corresponding to g is a.b[1].f[0][0]
 *
 *
 * JSON-OID is consists of JSON-FIELD
 * For example: a.b[1].f[0][0]
 *      a.b[1] is the first JSON-FIELD
 *      f[0][0] is the second JSON-FIELD
 *
 *
 * JSON-FIELD is consists of JSON-DICT and JSON_ARRAY
 * For example: a.b[1]
 *      a.b is JSON-DICT
 *      [1] is JSON-ARRAY
 *
 * There is a length limit on JSON-DICT, the length of a.b is three
 * There is a depth limit on JSON-ARRAY, the depth of [0][0] is two
 * */

struct json_parser_attr {
    /* The maximum number of fields in JSON-OID supported by the json-parser
     * defalut value is defined by JSON_FIELD_NUM_MAX macro*/
    unsigned int json_field_num;

    /* The maximum length of JSON-DICT in JSON-FIELD supported by the json-parser
     * defalut value is defined by JSON_DICT_LEN_MAX macro*/
    unsigned int json_dict_length;

    /* The maximum depth of JSON-ARRAY in JSON-FIELD supported by the json-parser
     * defalut value is defined by JSON_ARRAY_DEPTH_MAX macro*/
    unsigned int json_array_depth;
};

struct json_array {
    bool         exist;
    unsigned int index;
};

struct json_field {
    bool               exist;
    char               *json_dict;
    struct json_array  *json_arry;
};

struct json_parser {
    /* the attribution of json-parser*/
    struct json_parser_attr   attr;

    /* parse the JSON-OID and save the JSON-FIELD */
    struct json_field         *json_field;

    /* point to the JSON-OBJECT generated from parsing json-data or creating */
    struct json_object        *json_object;
};


typedef enum {
    /* create a struct json_object JSON-OBJECT */
    JSON_OP_NEW,

    /* parse the json-data(json-string or json-file and so on) and generate the struct json_object JSON-OBJECT */
    JSON_OP_PARSE,

    /* add field to JSON-OBJECT by JSON-OID */
    JSON_OP_ADD,

    /* get the value of field in JSON-OBJECT by JSON-OID */
    JSON_OP_GET,

    /* update the value of field in JSON-OBJECT by JSON-OID */
    JSON_OP_UPDATE,

    /* delete the value of field in JSON-OBJECT by JSON-OID */
    JSON_OP_DELETE,

    /* dump the struct json_object JSON-OBJECT to json-data, such as json-string or json-file */
    JSON_OP_DUMP,

    JSON_OP_NUM
} json_op;

typedef enum {
    JSON_OP_NEW_DICT        = 0x0 ,           /* create {} JSON-OBJECT */
    JSON_OP_NEW_ARRAY             ,           /* create [] JSON-OBJECT */

    JSON_OP_PARSE_BOOL      = 0x10,
    JSON_OP_PARSE_INT32           ,
    JSON_OP_PARSE_DOUBLE          ,
    JSON_OP_PARSE_STRING          ,
    JSON_OP_PARSE_FILE            ,

    JSON_OP_ADD_BOOL        = 0x20,
    JSON_OP_ADD_INT32             ,
    JSON_OP_ADD_DOUBLE            ,
    JSON_OP_ADD_STRING            ,

    JSON_OP_GET_BOOL        = 0x30,
    JSON_OP_GET_INT32             ,
    JSON_OP_GET_DOUBLE            ,
    JSON_OP_GET_STRING            ,

    JSON_OP_UPDATE_BOOL     = 0x40,
    JSON_OP_UPDATE_INT32          ,
    JSON_OP_UPDATE_DOUBLE         ,
    JSON_OP_UPDATE_STRING         ,

    JSON_OP_DELETE_BOOL     = 0x50,
    JSON_OP_DELETE_INT32          ,
    JSON_OP_DELETE_DOUBLE         ,
    JSON_OP_DELETE_STRING         ,

    JSON_OP_DUMP_BOOL       = 0x60,
    JSON_OP_DUMP_INT32            ,
    JSON_OP_DUMP_DOUBLE           ,
    JSON_OP_DUMP_STRING           ,
    JSON_OP_DUMP_FILE             ,

    JSON_OP_OBJ_NUM
} json_op_obj;
struct json_op_attr{
    json_op_obj     op_type;
    union {

    }               op_data;
};

struct json_parser* json_parser_create(struct json_parser_attr *attr);
int json_parser_ctl(struct json_parser* json_parser, json_op op, struct json_op_attr *op_attr);
int json_parser_free(struct json_parser* json_parser);

#endif /* __UTIL_JSON_PARSER_H__ */
