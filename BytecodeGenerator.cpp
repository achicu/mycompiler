/*
 *  BytecodeGenerator.cpp
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/11/09.
 *
 */

#include "BytecodeGenerator.h"
#include "Disassembler.h"
#include "Interpreter.h"
#include "OpCodes.h"

Register* LocalPropertyAccessor::EmitLoad(BytecodeGenerator* generator, Register* dst)
{
    return m_register.Ptr();
}

Register* LocalPropertyAccessor::EmitSave(BytecodeGenerator* generator, Register* src, Register* dst)
{
    if (src != m_register.Ptr())
    {
        generator->EmitBytecode(op_assign);
        generator->EmitRegister(m_register.Ptr());
        generator->EmitRegister(src);
    }
    return m_register.Ptr();
}

Register* ParentPropertyAccessor::EmitLoad(BytecodeGenerator* generator, Register* dst)
{
    if (!dst) dst = generator->NewTempRegister().Ptr();
    
    generator->EmitBytecode(op_load_scope);
    generator->EmitRegister(dst);
    generator->EmitConstantInt(m_scopeNumber);
    generator->EmitConstantInt(m_registerNumber);
    
    dst->SetType(GetType());
    
    return dst;
}

Register* ParentPropertyAccessor::EmitSave(BytecodeGenerator* generator, Register* src, Register* dst)
{
    if (src->GetType() != GetType())
    {
        printf("invalid type\n");
        exit(1);
    }

    generator->EmitBytecode(op_save_scope);
    generator->EmitRegister(src);
    generator->EmitConstantInt(m_scopeNumber);
    generator->EmitConstantInt(m_registerNumber);
    
    return src;
}

bool Scope::HasLocalProperty(std::string& name) const
{
    PropertyMap::const_iterator iter = m_properties.find(name);
    return (iter != m_properties.end());
}

PassRef<Accessor> Scope::GetProperty(std::string& name) const
{
    PropertyMap::const_iterator iter = m_properties.find(name);
    if (iter != m_properties.end())
        return AdoptRef<Accessor>(new LocalPropertyAccessor((*iter).second->GetType(), (*iter).second->GetRegister()));
    
    if (m_parentScope.Ptr())
    {
        // need to get the property and set it into an accessor
        int scopeNumber = 1;
        Property* property = m_parentScope->LookupProperty(name, scopeNumber);
        if (property)
            return AdoptRef<Accessor>(new ParentPropertyAccessor( property->GetType(), scopeNumber, property->GetRegister()->Number() ));
    }

    return 0;
}

Property* Scope::LookupProperty(std::string& name, int& scopeNumber) const
{
    PropertyMap::const_iterator iter = m_properties.find(name);
    if (iter != m_properties.end())
        return (*iter).second.Ptr();
    
    if (m_parentScope.Ptr())
    {
        ++scopeNumber;
        
        return m_parentScope->LookupProperty(name, scopeNumber);
    }
    
    return 0;
}

bool BuiltinType::CoerceArgsIfNeeded(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* &reg1, Register* &reg2)
{
    if (type2 != this)
    {
        // have to convert me or the other to my type
        if (!type2->IsBuiltin())
        {
            printf("Error: trying to do %s %s %s\n", Name().c_str(), BinaryOpcodeToString(op), type2->Name().c_str());
            exit(1);
        }
        
        if (static_cast<BuiltinType*>(type2)->GetPriority() > GetPriority())
        {
            // coerce me to the other
            reg1 = generator->Coerce(reg1, type2);
            return false;
        }
        else
        {
            // coerce the other to me
            reg2 = generator->Coerce(reg2, this);
        }
    }
    
    return true;
}

Register* Type::EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst)
{
    // arbitrary type operators not supported, yet
    printf("Error: trying to do %s %s %s\n", Name().c_str(), BinaryOpcodeToString(op), type2->Name().c_str());
    exit(1);
}

Register* Type::EmitUnaryOpBytecode(BytecodeGenerator* generator, UnaryOpcode op, Register* reg1, Register* dst)
{
    // arbitrary type operators not supported, yet
    printf("Error: trying to do %s %s\n", UnaryOpcodeToString(op), Name().c_str());
    exit(1);
}

Register* Type::EmitAssignOpBytecode (BytecodeGenerator* generator, AssignOpcode op, Accessor* accessor, ArenaNode* node2, Register* dst)
{
    // arbitrary type operators not supported, yet
    printf("Error: trying to do %s %s\n", AssignOpcodeToString(op), Name().c_str());
    exit(1);
}

Register* IntType::EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst)
{
    if (!CoerceArgsIfNeeded(generator, type2, op, reg1, reg2))
    {
        // reverse if cannot do it
        return type2->EmitBinaryOpBytecode(generator, type2, op, reg1, reg2, dst);
    }
    
    assert(reg1->GetType() == reg2->GetType());
    dst->SetType(this);
    
    switch(op)
    {
        case binary_op_plus:
            generator->EmitBytecode(op_int_plus);
        break;
        case binary_op_multiply:
            generator->EmitBytecode(op_int_multiply);
        break;
        case binary_op_minus:
            generator->EmitBytecode(op_int_minus);
        break;
        case binary_op_divide:
            generator->EmitBytecode(op_int_divide);
        break;
        case binary_op_less:
            generator->EmitBytecode(op_int_less);
        break;
        case binary_op_less_or_equal:
            generator->EmitBytecode(op_int_less_or_equal);
        break;
        case binary_op_more:
            generator->EmitBytecode(op_int_more);
        break;
        case binary_op_more_or_equal:
            generator->EmitBytecode(op_int_more_or_equal);
        break;
        case binary_op_equal:
            generator->EmitBytecode(op_int_equals);
        break;
        default:
            printf("%s operation not supported on ints\n", BinaryOpcodeToString(op));
            exit(1);
    }
    
    generator->EmitRegister(dst);
    generator->EmitRegister(reg1);
    generator->EmitRegister(reg2);
    
    return dst;
}

Register* IntType::EmitAssignOpBytecode (BytecodeGenerator* generator, AssignOpcode op, Accessor* accessor, ArenaNode* node2, Register* dst)
{
    dst->SetType(this);
    
    RefPtr<Register> source (accessor->EmitLoad(generator, 0));
    RefPtr<Register> newValue;
    
    switch(op)
    {
        case assign_op_plusplus_prefix:
            generator->EmitBytecode(op_int_plus_one);
            generator->EmitRegister(source.Ptr());
            dst = source.Ptr();
        break;
        case assign_op_plusplus_sufix:
        {
            // let the user get the old value
            RefPtr<Register> r (generator->NewTempRegister());
            r->SetType(this);
            
            generator->EmitBytecode(op_assign);
            generator->EmitRegister(r.Ptr());
            generator->EmitRegister(source.Ptr());
            dst = r.Ptr();
            
            generator->EmitBytecode(op_int_plus_one);
            generator->EmitRegister(source.Ptr());
        }
        break;
        case assign_op_minusminus_prefix:
            generator->EmitBytecode(op_int_minus_one);
            generator->EmitRegister(source.Ptr());
            dst = source.Ptr();
        break;
        case assign_op_minusminus_sufix:
        {
            // let the user get the old value
            RefPtr<Register> r( generator->NewTempRegister() );
            r->SetType(this);
            
            generator->EmitBytecode(op_assign);
            generator->EmitRegister(r.Ptr());
            generator->EmitRegister(source.Ptr());
            dst = r.Ptr();
            
            generator->EmitBytecode(op_int_minus_one);
            generator->EmitRegister(source.Ptr());
        }
        break;
        default:
            printf(" assign %s operation not supported on ints\n", AssignOpcodeToString(op));
            exit(1);
    }
    
    accessor->EmitSave(generator, source.Ptr(), 0)->SetIgnored();
    
    return dst;
}

Register* IntType::EmitUnaryOpBytecode(BytecodeGenerator* generator, UnaryOpcode op, Register* reg1, Register* dst)
{
    dst->SetType(this);
    
    switch(op)
    {
        case unary_op_not:
            generator->EmitBytecode(op_int_not);
            generator->EmitRegister(dst);
            generator->EmitRegister(reg1);
    
        break;
        default:
            printf(" unary %s operation not supported on ints\n", UnaryOpcodeToString(op));
            exit(1);
    }
    
    return dst;
}


Register* FloatType::EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst)
{
    if (!CoerceArgsIfNeeded(generator, type2, op, reg1, reg2))
    {
        // reverse if cannot do it
        return type2->EmitBinaryOpBytecode(generator, type2, op, reg1, reg2, dst);
    }
    
    assert(reg1->GetType() == reg2->GetType());
    dst->SetType(this);
    
    switch(op)
    {
        case binary_op_plus:
            generator->EmitBytecode(op_float_plus);
        break;
        case binary_op_multiply:
            generator->EmitBytecode(op_float_multiply);
        break;
        case binary_op_minus:
            generator->EmitBytecode(op_float_minus);
        break;
        case binary_op_divide:
            generator->EmitBytecode(op_float_divide);
        break;
        case binary_op_less:
            generator->EmitBytecode(op_float_less);
            dst->SetType(generator->GetGlobalData()->GetIntType());
        break;
        case binary_op_less_or_equal:
            generator->EmitBytecode(op_float_less_or_equal);
            dst->SetType(generator->GetGlobalData()->GetIntType());
        break;
        case binary_op_more:
            generator->EmitBytecode(op_float_more);
            dst->SetType(generator->GetGlobalData()->GetIntType());
        break;
        case binary_op_more_or_equal:
            generator->EmitBytecode(op_float_more_or_equal);
            dst->SetType(generator->GetGlobalData()->GetIntType());
        break;
        case binary_op_equal:
            generator->EmitBytecode(op_float_equals);
            dst->SetType(generator->GetGlobalData()->GetIntType());
        break;
        default:
            printf("%s operation not supported on floats\n", BinaryOpcodeToString(op));
            exit(1);
    }
    
    generator->EmitRegister(dst);
    generator->EmitRegister(reg1);
    generator->EmitRegister(reg2);
    
    return dst;
}

Register* FloatType::EmitAssignOpBytecode (BytecodeGenerator* generator, AssignOpcode op, Accessor* accessor, ArenaNode* node2, Register* dst)
{
    dst->SetType(this);
    
    RefPtr<Register> source (accessor->EmitLoad(generator, 0));
    RefPtr<Register> newValue;
    
    switch(op)
    {
        case assign_op_plusplus_prefix:
            generator->EmitBytecode(op_float_plus_one);
            generator->EmitRegister(source.Ptr());
            dst = source.Ptr();
        break;
        case assign_op_plusplus_sufix:
        {
            // let the user get the old value
            RefPtr<Register> r (generator->NewTempRegister());
            r->SetType(this);
            
            generator->EmitBytecode(op_assign);
            generator->EmitRegister(r.Ptr());
            generator->EmitRegister(source.Ptr());%0