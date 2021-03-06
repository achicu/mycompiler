/*
 *  Disassembler.cpp
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/12/09.
 *
 */

#include "Platform.h"

#include "Disassembler.h"

#include "BytecodeGenerator.h"
#include "OpCodes.h"

struct BytecodeMetaData
{
    const char* name;
    const char* structure;
    int length;
};

void Disassemble(GlobalData* globalData, const std::vector<Bytecode>* buffer)
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
    for(unsigned i=0; i<buffer->size(); ++i)
    {
        assert(buffer->at(i).Code < op_last);
        BytecodeMetaData* meta = &bytecodeList[buffer->at(i).Code];
        printf("%d:\t%s", i, meta->name);
        
        const char* structure = meta->structure; 
        while(*structure)
        {
            i++;
            Bytecode byte = buffer->at(i);
            switch(*structure)
            {
                case 'r':
                    printf("\n\tregister %d", byte.RegisterNumber);
                break;
                case 's':
                    printf("\n\tconstant string index %d \"%s\"", byte.ConstantStringIndex, globalData->GetConstantString(byte.ConstantStringIndex).c_str());
                break;
                case 'i':
                    printf("\n\tconstant int %d", byte.ConstantInt);
                break;
                case 'f':
                    printf("\n\tconstant float index %d %lf", byte.ConstantFloatIndex, globalData->GetConstantFloat(byte.ConstantFloatIndex));
                break;
            }
            ++ structure;
        }
        printf("\n");
    }
}
