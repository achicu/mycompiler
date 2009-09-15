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

class RefCode: public CollectorRef
{
public:
    RefCode(MethodEnv* methodEnv)
        : m_methodEnv (methodEnv)
    {
        assert(methodEnv->GetRefCode() == 0);
        methodEnv->SetRefCode(this);
    }
    
    ~RefCode()
    {
        m_methodEnv->SetRefCode(0);
    }

    virtual Type* GetType() const;
    
    MethodEnv* GetMethodEnv() const { return m_methodEnv.Ptr(); }
    
private:
    RefPtr<MethodEnv> m_methodEnv; // global data will keep this alive
};

Type* RefString::GetType() const
{
    return Heap::CellBlock(this)->heap->GetGlobalData()->GetStringType();
}

Type* RefCode::GetType() const
{
    return Heap::CellBlock(this)->heap->GetGlobalData()->GetCodeType();
}

Type* RefObject::GetType() const { return m_type; }
Type* RefVector::GetType() const { return m_type; }

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

RefVector::RefVector(VectorType* type, int size)
    : m_type(type)
    , m_size(size)
{
    assert(m_type);
    m_bufferSize = type->GetElementSize() * size;
    m_buffer = new char[m_bufferSize];
    memset(m_buffer, 0, m_bufferSize);
}

RefVector::~RefVector()
{
    delete [] m_buffer;
}
    
void RefVector::Mark()
{
    assert(m_type);
    m_type->MarkObject(this);
    CollectorRef::Mark();
}

void RefVector::CheckSpace(int offset)
{
    if (offset < 0 || m_bufferSize <= offset)
    {
        printf("index out of bounds %d - %d\n", offset, m_bufferSize);
        exit(1);
    } 
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
        OPCODE(op_load_null)
            R(1).asReference = 0;
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
            if (!ref)
            {
                printf ("null string reference\n");
                exit(1);
            }
            R(1).asInt = atoi(static_cast<RefString*>(ref)->Value.c_str());
        NEXT()
        OPCODE(op_coerce_string_float)
            CollectorRef* ref = R(2).asReference;
            if (!ref)
            {
                printf ("null string reference\n");
                exit(1);
            }
            R(1).asFloat = atof(static_cast<RefString*>(ref)->Value.c_str());
        NEXT()
        OPCODE(op_coerce_string_code)

            CollectorRef* ref = R(2).asReference;
            if (!ref)
            {
                printf ("null string reference\n");
                exit(1);
            }
            
            std::istringstream o(static_cast<RefString*>(ref)->Value);
            RefPtr<ArenaNode> result;
            
            Arena parser(&o);
            result = parser.Parse();
            
            if (!result.Ptr())
            {
                printf("parse error\n");
                exit(1);
            }
            
            std::ostringstream name;
            name << std::string("$method") << globalData->GetNextMethodId();
            
            BytecodeGenerator generator(globalData, static_cast<StatementList*> (result.Ptr()), name.str());
            RefPtr<MethodEnv> methodEnv = generator.Generate();
            methodEnv->SetSourceCode( parser.GetSourceCode() );
            R(1).asReference = new RefCode(methodEnv.Ptr());

        NEXT()
        
        OPCODE(op_code_plus_string)
            CollectorRef* refString = R(2).asReference;
            if (!refString)
            {
                printf ("null string reference\n");
                exit(1);
            }
            
            CollectorRef* refCode = R(1).asReference;
            if (!refCode)
            {
                printf ("null code reference\n");
                exit(1);
            }
            
            std::istringstream o(static_cast<RefString*>(refString)->Value);

            RefPtr<ArenaNode> result;
            
            Arena parser(&o);
            result = parser.Parse();
            
            if (!result.Ptr())
            {
                printf("parse error\n");
                exit(1);
            }
            
            std::ostringstream name;
            name << std::string("$method") << globalData->GetNextMethodId();
            
            RefPtr<MethodEnv> methodEnv = globalData->GetMethod(name.str());
            methodEnv->SetSourceCode( parser.GetSourceCode() );
            
            RefPtr<MethodEnv> lastMethodEnv = static_cast<RefCode*>(refCode)->GetMethodEnv()->GetLast();
            lastMethodEnv->SetNext(methodEnv.Ptr());
            
            BytecodeGenerator generator(methodEnv.Ptr(), static_cast<StatementList*> (result.Ptr()), lastMethodEnv->GetLocalScope());
            generator.Generate();
            
        NEXT()
        
        
        OPCODE(op_coerce_code_string)
            CollectorRef* ref = R(2).asReference;
            if (!ref)
            {
                printf ("null code reference\n");
                exit(1);
            }
            R(1).asReference = new RefString(static_cast<RefCode*>(ref)->GetMethodEnv()->GetSourceCode());
        NEXT()
        
        OPCODE(op_load_method)
            MethodEnv* methodEnv = globalData->GetDefinedMethod(globalData->GetConstantString(V(2).ConstantStringIndex));
            if (methodEnv->GetRefCode())
                R(1).asReference = methodEnv->GetRefCode();
            else
                R(1).asReference = new RefCode(methodEnv);
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
        
        OPCODE(op_debug_vector)
            Type* type = globalData->GetDefinedType(globalData->GetConstantString(V(1).ConstantStringIndex));
            assert(type->IsVectorRef());
            static_cast<VectorType*>(type)->DebugObject(globalData, static_cast<RefVector*>(R(2).asReference));
        NEXT()
        
        OPCODE(op_debug_code)
            CollectorRef* ref = R(1).asReference;
            if (!ref)
            {
                printf ("null\n");
            }
            else
            {
                static_cast<RefCode*>(ref)->GetMethodEnv()->Debug();
            }
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
        
        OPCODE(op_call_code)
            if (!R(1).asReference)
            {
                printf("null code reference\n");
                exit(1);
            }
            MethodEnv* methodEnv = static_cast<RefCode*>(R(1).asReference)->GetMethodEnv();
            methodEnv->Run(globalData->GetRegisterFile()->GetLastUsed() + 1);
        NEXT()
        
        OPCODE(op_init_object)
            Type* type = globalData->GetDefinedType(globalData->GetConstantString(V(2).ConstantStringIndex));
            assert(type->IsObjectType());
            R(1).asReference = new RefObject(static_cast<ObjectType*>(type));
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

        OPCODE(op_init_vector)
            Type* type = globalData->GetDefinedType(globalData->GetConstantString(V(3).ConstantStringIndex));
            assert(type->IsVectorRef());
            R(1).asReference = new RefVector(static_cast<VectorType*>(type), R(2).asInt);
        NEXT()
        
        OPCODE(op_vector_size)
            if (!R(2).asReference)
            {
                printf("null vector reference\n");
                exit(1);
            }
            
            R(1).asInt = static_cast<RefVector*>(R(2).asReference)->GetSize();
        NEXT()
        
        OPCODE(op_load_int_vector_property)
            if (!R(2).asReference)
            {
                printf("null vector reference\n");
                exit(1);
            }
            R(1).asInt = static_cast<RefVector*>(R(2).asReference)->ReadAtOffset<int>(R(3).asInt * sizeof(int));
        NEXT()
        OPCODE(op_save_int_vector_property)
            if (!R(1).asReference)
            {
                printf("null vector reference\n");
                exit(1);
            }
            static_cast<RefVector*>(R(1).asReference)->WriteAtOffset<int>(R(3).asInt * sizeof(int), R(2).asInt);
        NEXT()
        
        OPCODE(op_load_float_vector_property)
            if (!R(2).asReference)
            {
                printf("null vector reference\n");
                exit(1);
            }
            R(1).asFloat = static_cast<RefVector*>(R(2).asReference)->ReadAtOffset<double>(R(3).asInt * sizeof(double));
        NEXT()
        OPCODE(op_save_float_vector_property)
            if (!R(1).asReference)
            {
                printf("null vector reference\n");
                exit(1);
            }
            static_cast<RefVector*>(R(1).asReference)->WriteAtOffset<double>(R(3).asInt * sizeof(double), R(2).asFloat);
        NEXT()
        
        OPCODE(op_load_ref_vector_property)
            if (!R(2).asReference)
            {
                printf("null vector reference\n");
                exit(1);
            }
            R(1).asReference = static_cast<RefVector*>(R(2).asReference)->ReadAtOffset<CollectorRef*>(R(3).asInt * sizeof(CollectorRef*));
        NEXT()
        
        OPCODE(op_save_ref_vector_property)
            if (!R(1).asReference)
            {
                printf("null vector reference\n");
                exit(1);
            }
            static_cast<RefVector*>(R(1).asReference)->WriteAtOffset<CollectorRef*>(R(3).asInt * sizeof(CollectorRef*), R(2).asReference);
        NEXT()
        
        OPCODE(op_dynamic_cast)
            Type* type = globalData->GetDefinedType(globalData->GetConstantString(V(3).ConstantStringIndex));
            assert(type->IsCollectorRef());
            R(1).asReference = (R(2).asReference->GetType()->InheritsFrom(type)) ? R(2).asReference : 0;
        NEXT()
        
        OPCODE(op_ref_equal)
            R(1).asInt = (R(2).asReference == R(3).asReference) ? 1 : 0;
        NEXT()
        
        OPCODE(op_string_equal)
            if (!R(2).asReference)
                printf("null string reference\n");
            if (!R(3).asReference)
                printf("null string reference\n");
            R(1).asInt = (static_cast<RefString*>(R(3).asReference)->Value == static_cast<RefString*>(R(3).asReference)->Value) ? 1 : 0;
        NEXT()
        
finished:
        return;
    }
        
    #undef R
    #undef J
    #undef OPCODE
    #undef NEXT
}