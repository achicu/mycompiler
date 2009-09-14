/*
 *  Interpreter.cpp
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/12/09.
 *
 */

#include "Interpreter.h"

#include "BytecodeGenerator.h"
#include "OpCodes.h"
#include "Collector.h"
#include <sstream>

struct BytecodeMetaData
{
    const char* name;
    void* label;
    int length;
};

RefObject::RefObject(ObjectType* type)
    : m_type(type)
{
    int size = type->ObjectSize();
    m_buffer = new char[size];
    memset(m_buffer, 0, size);
}

RefObject::~RefObject()
{
    delete [] m_buffer;
}
    
void RefObject::Mark()
{
    m_type->MarkObject(this);
    CollectorRef::Mark();
}

static const int maximumReentrancy = 10000;

class ReentrancyCheck
{
public:
    ReentrancyCheck()
    {
        ++ s_reentrancy;
    }
    ~ReentrancyCheck()
    {
        -- s_reentrancy;
    }
    
    bool CanEnter() const { return s_reentrancy < maximumReentrancy; }
    
private:
    static int s_reentrancy;
};

int ReentrancyCheck::s_reentrancy = 0;

void Interpret(GlobalData* globalData, RegisterValue* registers, std::vector<Bytecode>* buffer)
{
    ReentrancyCheck checker;
    if (!checker.CanEnter())
    {
        printf("stack overflow\n");
        exit(1);
    }

    static BytecodeMetaData bytecodeList[op_last];
    static bool initialized = false;
    if (!initialized)
    {
        #define INITBYTECODE(_opcode, ignored, _length) \
            bytecodeList[_opcode].name= #_opcode; \
            bytecodeList[_opcode].label = &&OPCODE_##_opcode; \
            bytecodeList[_opcode].length = _length + 1; \

        OPCODES(INITBYTECODE)
        #undef INITBYTECODE
        
        initialized = true;
    }
    
    BytecodeMetaData* meta;
    Bytecode byte;
    
    #define GOTONEXT() { \
        vPC = vPCNext; \
        if (vPC >= buffer->size()) goto finished; \
        assert(buffer->at(vPC).Code < op_last); \
        meta = &bytecodeList[buffer->at(vPC).Code]; \
        /*printf("%s\n", meta->name);*/ \
        byte = buffer->at(vPC); \
        vPCNext += meta->length; \
        goto *meta->label; }
    
    #define OPCODE(opcode) OPCODE_##opcode: {
    #define NEXT() \
        /* for (int z=0; z<registersCount; ++z) printf("register: %d\t\tasInt:%d\t\t\t\tasFloat:%lf\t\t\tasReference:%p\n", z, registers[z].asInt, registers[z].asFloat, registers[z].asReference);*/ \
        GOTONEXT() }
    
    #define V(j) (buffer->at(vPC + j))
    #define RAT(j) (registers[j])
    #define R(j) (RAT(V(j).RegisterNumber))
    
    int vPCNext=0;
    int vPC = 0;
    
    {
        GOTONEXT()
        
        /*#define GENERATOR(_opcode, ignored, ignored2) \
            OPCODE(_opcode) \
            NEXT() \
        
        OPCODES(GENERATOR)*/
        
        OPCODE(op_load_float_constant)
            R(1).asFloat = globalData->GetConstantFloat(V(2).ConstantFloatIndex);
        NEXT()
        OPCODE(op_load_int_constant)
            R(1).asInt = V(2).ConstantInt;
        NEXT()
        OPCODE(op_load_string_constant)
            R(1).asReference = new RefString(globalData->GetConstantString(V(2).ConstantStringIndex));
        NEXT()
        OPCODE(op_int_plus)
            R(1).asInt = R(2).asInt + R(3).asInt;
        NEXT()
        OPCODE(op_int_plus_one)
            R(1).asInt ++;
        NEXT()
        OPCODE(op_int_minus_one)
            R(1).asInt --;
        NEXT()
        OPCODE(op_int_minus)
            R(1).asInt = R(2).asInt - R(3).asInt;
        NEXT()
        OPCODE(op_int_multiply)
            R(1).asInt = R(2).asInt * R(3).asInt;
        NEXT()
        OPCODE(op_int_divide)
            if (R(3).asInt != 0)
                R(1).asInt = R(2).asInt / R(3).asInt;
            else
                R(1).asInt = 0;
        NEXT()
        OPCODE(op_int_less)
            R(1).asInt = (R(2).asInt < R(3).asInt) ? 1 : 0;
        NEXT()
        OPCODE(op_int_more)
            R(1).asInt = (R(2).asInt > R(3).asInt) ? 1 : 0;
        NEXT()
        OPCODE(op_int_less_or_equal)
            R(1).asInt = (R(2).asInt <= R(3).asInt) ? 1 : 0;
        NEXT()
        OPCODE(op_int_more_or_equal)
            R(1).asInt = (R(2).asInt >= R(3).asInt) ? 1 : 0;
        NEXT()
        OPCODE(op_int_equals)
            R(1).asInt = (R(2).asInt == R(3).asInt) ? 1 : 0;
        NEXT()
        OPCODE(op_float_plus)
            R(1).asFloat = R(2).asFloat + R(3).asFloat;
        NEXT()
        OPCODE(op_float_plus_one)
            R(1).asFloat ++;
        NEXT()
        OPCODE(op_float_minus_one)
            R(1).asFloat --;
        NEXT()
        OPCODE(op_float_minus) 
            R(1).asFloat = R(2).asFloat - R(3).asFloat;
        NEXT()
        OPCODE(op_float_multiply)
            R(1).asFloat = R(2).asFloat * R(3).asFloat;
        NEXT()
        OPCODE(op_float_divide)
            if (R(3).asFloat != 0)
                R(1).asFloat = R(2).asFloat / R(3).asFloat;
            else
                R(1).asFloat = 0;
        NEXT()
        OPCODE(op_float_less)
            R(1).asInt = (R(2).asFloat < R(3).asFloat) ? 1 : 0;
        NEXT()
        OPCODE(op_float_more)
            R(1).asInt = (R(2).asFloat > R(3).asFloat) ? 1 : 0;
        NEXT()
        OPCODE(op_float_less_or_equal)
            R(1).asInt = (R(2).asFloat <= R(3).asFloat) ? 1 : 0;
        NEXT()
        OPCODE(op_float_more_or_equal)
            R(1).asInt = (R(2).asFloat >= R(3).asFloat) ? 1 : 0;
        NEXT()
        OPCODE(op_float_equals)
            R(1).asInt = (R(2).asFloat == R(3).asFloat) ? 1 : 0;
        NEXT()
        OPCODE(op_string_plus)
            R(1).asReference = new RefString(static_cast<RefString*>(R(2).asReference)->Value + static_cast<RefString*>(R(3).asReference)->Value);
        NEXT()
        OPCODE(op_coerce_int_float)
            R(1).asFloat = R(2).asInt;
        NEXT()
        OPCODE(op_coerce_int_string)
            std::ostringstream o;
            o << R(2).asInt;
            R(1).asReference = new RefString(o.str());
        NEXT()
        OPCODE(op_coerce_float_int)
            R(1).asInt = (int)R(2).asFloat;
        NEXT()
        OPCODE(op_coerce_float_string)
            std::ostringstream o;
            o << R(2).asFloat;
            R(1).asReference = new RefString(o.str());
        NEXT()
        OPCODE(op_coerce_string_int)
            CollectorRef* ref = R(2).asReference;
            assert (ref);
            R(1).asInt = atoi(static_cast<RefString*>(ref)->Value.c_str());
        NEXT()
        OPCODE(op_coerce_string_float)
            CollectorRef* ref = R(2).asReference;
            assert (ref);
            R(1).asFloat = atof(static_cast<RefString*>(ref)->Value.c_str());
        NEXT()
        OPCODE(op_assign)
            R(1) = R(2);
        NEXT()
        OPCODE(op_debug_int)
            printf("%d\n", R(1).asInt);
        NEXT()
        OPCODE(op_debug_float)
            printf("%lf\n", R(1).asFloat);
        NEXT()
        OPCODE(op_debug_string)
            printf("%s\n", static_cast<RefString*>(R(1).asReference)->Value.c_str());
        NEXT()

        OPCODE(op_debug_object)
            Type* type = globalData->GetDefinedType(globalData->GetConstantString(V(1).ConstantStringIndex));
            assert(type->IsObjectType());
            static_cast<ObjectType*>(type)->DebugObject(globalData, static_cast<RefObject*>(R(2).asReference));
        NEXT()

        OPCODE(op_init_ref)
            R(1).asReference = 0;
        NEXT()
        OPCODE(op_jmp_if_true)
            if (R(1).asInt)
            {
                vPCNext = V(2).ConstantInt;
            }
        NEXT()
        OPCODE(op_jmp_if_false)
            if (!R(1).asInt)
            {
                vPCNext = V(2).ConstantInt;
            }
        NEXT()
        OPCODE(op_jmp)
            vPCNext = V(1).ConstantInt;
        NEXT()
        OPCODE(op_int_not)
            R(1).asInt = !R(2).asInt;
        NEXT()
        OPCODE(op_float_not)
            R(1).asFloat = !R(2).asFloat;
        NEXT()
        
        OPCODE(op_save_scope)
            //fixme: this only works for one global scope
            RegisterValue* globalRegister = globalData->GetRegisterFile()->GetBlock();
            globalRegister[V(2).ConstantInt] = R(1);
        NEXT()
        
        OPCODE(op_load_scope)
            RegisterValue* globalRegister = globalData->GetRegisterFile()->GetBlock();
            R(1) = globalRegister[V(2).ConstantInt];
        NEXT()
        
        OPCODE(op_call_method)
            MethodEnv* methodEnv = globalData->GetMethod(globalData->GetConstantString(V(2).ConstantStringIndex));
            methodEnv->Run(&R(1));
        NEXT()
        
        OPCODE(op_init_object)
            Type* type = globalData->GetDefinedType(globalData->GetConstantString(V(2).ConstantStringIndex));
            assert(type->IsObjectType());
            RefObject* refObject = new RefObject(static_cast<ObjectType*>(type));
            R(1).asReference = refObject;
        NEXT()
        
        OPCODE(op_load_int_object_property)
            if (!R(2).asReference)
            {
                printf("null reference\n");
                exit(1);
            }
            R(1).asInt = static_cast<RefObject*>(R(2).asReference)->ReadAtOffset<int>(V(3).ConstantInt);
        NEXT()
        OPCODE(op_save_int_object_property)
            if (!R(1).asReference)
            {
                printf("null reference\n");
                exit(1);
            }
            static_cast<RefObject*>(R(1).asReference)->WriteAtOffset<int>(V(3).ConstantInt, R(2).asInt);
        NEXT()
        
        OPCODE(op_load_float_object_property)
            if (!R(2).asReference)
            {
                printf("null reference\n");
                exit(1);
            }
            R(1).asFloat = static_cast<RefObject*>(R(2).asReference)->ReadAtOffset<double>(V(3).ConstantInt);
        NEXT()
        OPCODE(op_save_float_object_property)
            if (!R(1).asReference)
            {
                printf("null reference\n");
                exit(1);
            }
            static_cast<RefObject*>(R(1).asReference)->WriteAtOffset<double>(V(3).ConstantInt, R(2).asFloat);
        NEXT()
        
        OPCODE(op_load_ref_object_property)
            if (!R(2).asReference)
            {
                printf("null reference\n");
                exit(1);
            }
            R(1).asReference = static_cast<RefObject*>(R(2).asReference)->ReadAtOffset<CollectorRef*>(V(3).ConstantInt);
        NEXT()
        
        OPCODE(op_save_ref_object_property)
            if (!R(1).asReference)
            {
                printf("null reference\n");
                exit(1);
            }
            static_cast<RefObject*>(R(1).asReference)->WriteAtOffset<CollectorRef*>(V(3).ConstantInt, R(2).asReference);
        NEXT()

        
finished:
        return;
    }
        
    #undef R
    #undef J
    #undef OPCODE
    #undef NEXT
}