/*
 *  Disassembler.cpp
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/12/09.
 *  Copyright 2009 Adobe Systems Romania SRL. All rights reserved.
 *
 */

#include "Disassembler.h"

#include "BytecodeGenerator.h"
#include "OpCodes.h"

struct BytecodeMetaData
{
    const char* name;
    const char* structure;
    int length;
};

void Disassemble(std::vector<Bytecode>* buffer)
{
    static BytecodeMetaData bytecodeList[op_last];
    static bool initialized = false;
    if (!initialized)
    {
        #define INITBYTECODE(_opcode, _structure, _length) \
            bytecodeList[_opcode].name= #_opcode; \
            bytecodeList[_opcode].structure = _structure; \
            bytecodeList[_opcode].length = _length; \

        OPCODES(INITBYTECODE)
        #undef INITBYTECODE
        
        initialized = true;
    }
    for(int i=0; i<buffer->size(); ++i)
    {
        assert(buffer->at(i).Code < op_last);
        BytecodeMetaData* meta = &bytecodeList[buffer->at(i).Code];
        printf(" %s", meta->name);
        
        const char* structure = meta->structure; 
        while(*structure)
        {
            i++;
            Bytecode byte = buffer->at(i);
            switch(*structure)
            {
                case 'r':
                    printf(", register %d", byte.RegisterNumber);
                break;
                case 's':
                    printf(", constant string index %d", byte.ConstantStringIndex);
                break;
                case 'i':
                    printf(", constant int %d", byte.ConstantInt);
                break;
                case 'f':
                    printf(", constant float index %d", byte.ConstantFloatIndex);
                break;
            }
            ++ structure;
        }
        printf("\n");
    }
}
