
#include "form/muldiv.h"

#include "form/generic.h"
#include "parse.h"

const struct formation rv32m[] = {
    {"mul", &form_rtype, &parse_rtype, {4, OP_OP, 0x0, 0x1}},
    {"mulh", &form_rtype, &parse_rtype, {4, OP_OP, 0x1, 0x1}},
    {"mulhsu", &form_rtype, &parse_rtype, {4, OP_OP, 0x2, 0x1}},
    {"mulhu", &form_rtype, &parse_rtype, {4, OP_OP, 0x3, 0x1}},
    {"div", &form_rtype, &parse_rtype, {4, OP_OP, 0x4, 0x1}},
    {"divu", &form_rtype, &parse_rtype, {4, OP_OP, 0x5, 0x1}},
    {"rem", &form_rtype, &parse_rtype, {4, OP_OP, 0x6, 0x1}},
    {"remu", &form_rtype, &parse_rtype, {4, OP_OP, 0x7, 0x1}},
    
    END_FORMATION
}

const struct formation rv64m[] = {
    {"mulw", &form_rtype, &parse_rtype, {4, OP_OP32, 0x0, 0x1}},
    {"divw", &form_rtype, &parse_rtype, {4, OP_OP32, 0x4, 0x1}},
    {"divuw", &form_rtype, &parse_rtype, {4, OP_OP32, 0x5, 0x1}},
    {"remw", &form_rtype, &parse_rtype, {4, OP_OP32, 0x6, 0x1}},
    {"remuw", &form_rtype, &parse_rtype, {4, OP_OP32, 0x7, 0x1}},

    END_FORMATION
}