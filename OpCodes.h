/*
 *  OpCodes.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/12/09.
 *
 */

#define OPCODES(MACRO) \
    MACRO(op_load_float_constant, "rf", 2) \
    MACRO(op_load_int_constant, "ri", 2) \
    MACRO(op_load_string_constant, "rs", 2) \
    MACRO(op_int_plus, "rrr", 3) \
    MACRO(op_int_minus, "rrr", 3) \
    MACRO(op_int_multiply, "rrr", 3) \
    MACRO(op_int_divide, "rrr", 3) \
    MACRO(op_float_plus, "rrr", 3) \
    MACRO(op_float_minus, "rrr", 3) \
    MACRO(op_float_multiply, "rrr", 3) \
    MACRO(op_float_divide, "rrr", 3) \
    MACRO(op_string_plus, "rrr", 3) \
    MACRO(op_coerce_int_float, "r", 1) \
    MACRO(op_coerce_int_string, "r", 1) \
    MACRO(op_coerce_float_int, "r", 1) \
    MACRO(op_coerce_float_string, "r", 1) \
    MACRO(op_coerce_string_int, "r", 1) \
    MACRO(op_coerce_string_float, "r", 1) \
    MACRO(op_assign, "rr", 2) \
    MACRO(op_debug_int, "r", 1) \
    MACRO(op_debug_float, "r", 1) \
    MACRO(op_debug_string, "r", 1) \
    MACRO(op_inc_ref, "r", 1) \
    MACRO(op_dec_ref, "r", 1) \
    MACRO(op_init_ref, "r", 1) \
    MACRO(op_jmp_if_true, "ri", 2) \

#define ENUM_MACRO(opcode, ignore, ignore2) opcode,
enum OpCode
{
    OPCODES(ENUM_MACRO)
    op_last
};
#undef ENUM_MACRO

