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
    MACRO(op_plus, "rrr", 3) \
    MACRO(op_minus, "rrr", 3) \
    MACRO(op_multiply, "rrr", 3) \
    MACRO(op_divide, "rrr", 3)

#define ENUM_MACRO(opcode, ignore, ignore2) opcode,
enum OpCode
{
    OPCODES(ENUM_MACRO)
    op_last
};
#undef ENUM_MACRO

