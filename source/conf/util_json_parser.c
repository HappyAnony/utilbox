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

#include <stdlib.h>
#include <conf/util_json_parser.h>


#define   JSON_FIELD_NUM_MAX       (4)
#define   JSON_DICT_LEN_MAX        (200)
#define   JSON_ARRAY_DEPTH_MAX     (2)


static int json_parser_new(struct json_parser* json_parser, json_op op, struct json_op_attr *op_attr)
{
    if (!json_parser) return -1;
    if (!op_attr) return -1;

    if (op_attr->op_type == JSON_OP_NEW_DICT)
        json_parser->json_object = util_json_object_new_object();
    else
    if (op_attr->op_type == JSON_OP_NEW_ARRAY)
        json_parser->json_object = util_json_object_new_array();

    return 0;
}

static struct {
    json_op     op_type;
    int (*callback)(struct json_parser* json_parser, json_op op, struct json_op_attr *op_attr);
} json_parser_reactor[JSON_OP_NUM] = {
        {JSON_OP_NEW         ,         json_parser_new}
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
