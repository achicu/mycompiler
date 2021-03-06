/*
 *  OpCodes.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/12/09.
 *
 */

#ifndef OPCODES_H
#define OPCODES_H

#define OPCODES(MACRO) \
    MACRO(op_load_float_constant, "rf", 2) \
    MACRO(op_load_int_constant, "ri", 2) \
    MACRO(op_load_string_constant, "rs", 2) \
    MACRO(op_load_null, "r", 1) \
    MACRO(op_load_method, "rs", 2) \
    MACRO(op_int_plus, "rrr", 3) \
    MACRO(op_int_minus, "rrr", 3) \
    MACRO(op_int_multiply, "rrr", 3) \
    MACRO(op_int_divide, "rrr", 3) \
    MACRO(op_int_less, "rrr", 3) \
    MACRO(op_int_more, "rrr", 3) \
    MACRO(op_int_less_or_equal, "rrr", 3) \
    MACRO(op_int_more_or_equal, "rrr", 3) \
    MACRO(op_int_equals, "rrr", 3) \
    MACRO(op_int_minus_one, "r", 1) \
    MACRO(op_int_plus_one, "r", 1) \
    MACRO(op_float_plus, "rrr", 3) \
    MACRO(op_float_minus, "rrr", 3) \
    MACRO(op_float_multiply, "rrr", 3) \
    MACRO(op_float_divide, "rrr", 3) \
    MACRO(op_float_less, "rrr", 3) \
    MACRO(op_float_more, "rrr", 3) \
    MACRO(op_float_less_or_equal, "rrr", 3) \
    MACRO(op_float_more_or_equal, "rrr", 3) \
    MACRO(op_float_equals, "rrr", 3) \
    MACRO(op_float_minus_one, "r", 1) \
    MACRO(op_float_plus_one, "r", 1) \
    MACRO(op_string_plus, "rrr", 3) \
    MACRO(op_coerce_int_float, "rr", 2) \
    MACRO(op_coerce_int_string, "rr", 2) \
    MACRO(op_coerce_float_int, "rr", 2) \
    MACRO(op_coerce_float_string, "rr", 2) \
    MACRO(op_coerce_string_int, "rr", 2) \
    MACRO(op_coerce_string_float, "rr", 2) \
    MACRO(op_coerce_string_code, "rr", 2) \
    MACRO(op_coerce_code_string, "rr", 2) \
    MACRO(op_code_plus_string, "rr", 2) \
    MACRO(op_assign, "rr", 2) \
    MACRO(op_debug_int, "r", 1) \
    MACRO(op_debug_float, "r", 1) \
    MACRO(op_debug_string, "r", 1) \
    MACRO(op_debug_object, "sr", 2) \
    MACRO(op_debug_vector, "sr", 2) \
    MACRO(op_debug_code, "s", 1) \
    MACRO(op_read_int, "r", 1) \
    MACRO(op_read_float, "r", 1) \
    MACRO(op_read_string, "r", 1) \
    MACRO(op_init_ref, "r", 1) \
    MACRO(op_jmp_if_true, "ri", 2) \
    MACRO(op_jmp_if_false, "ri", 2) \
    MACRO(op_jmp, "i", 1) \
    MACRO(op_int_not, "rr", 2) \
    MACRO(op_float_not, "rr", 2) \
    MACRO(op_load_scope, "rii", 3) \
    MACRO(op_save_scope, "rii", 3) \
    MACRO(op_call_method, "rs", 2) \
    MACRO(op_call_code, "r", 1) \
    MACRO(op_init_object, "rs", 2) \
    MACRO(op_load_int_object_property, "rri", 3) \
    MACRO(op_save_int_object_property, "rri", 3) \
    MACRO(op_load_float_object_property, "rri", 3) \
    MACRO(op_save_float_object_property, "rri", 3) \
    MACRO(op_load_ref_object_property, "rri", 3) \
    MACRO(op_save_ref_object_property, "rri", 3) \
    MACRO(op_init_vector, "rrs", 3) \
    MACRO(op_vector_size, "rr", 2) \
    MACRO(op_load_int_vector_property, "rrr", 3) \
    MACRO(op_save_int_vector_property, "rrr", 3) \
    MACRO(op_load_float_vector_property, "rrr", 3) \
    MACRO(op_save_float_vector_property, "rrr", 3) \
    MACRO(op_load_ref_vector_property, "rrr", 3) \
    MACRO(op_save_ref_vector_property, "rrr", 3) \
    MACRO(op_dynamic_cast, "rrs", 3) \
    MACRO(op_ref_equal, "rrr", 3) \
    MACRO(op_string_equal, "rrr", 3) \
    MACRO(op_debug_line, "i", 1)
    

#define ENUM_MACRO(opcode, ignore, ignore2) opcode,
enum OpCode
{
    OPCODES(ENUM_MACRO)
    op_last
};
#undef ENUM_MACRO

#endif // OPCODES_H