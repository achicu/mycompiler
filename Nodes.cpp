/*
 *  Nodes.cpp
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/10/09.
 *
 */

#include "Nodes.h"
#include <string>
#include <sstream>
#include "ByteCodeGenerator.h"
#include "OpCodes.h"

// ============= FloatValueNode =============

FloatValueNode::FloatValueNode(double value)
    : m_value(value)
{
}

std::string FloatValueNode::ToString() const
{
    std::ostringstream o;
    o << "(" << m_value << "f " << LocationToString() << ")";
	return o.str();
}

Register* FloatValueNode::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    assert(dst);
    
    generator->EmitBytecode(op_load_float_constant);
    generator->EmitRegister(dst);
    generator->EmitConstantFloat(m_value);
    
    dst->SetType(generator->GetGlobalData()->GetFloatType());
    
    return dst;
}

// ============= IntegerValueNode =============

IntegerValueNode::IntegerValueNode(int value)
    : m_value(value)
{
}

std::string IntegerValueNode::ToString() const
{
    std::ostringstream o;
    o << "(" << m_value << ' ' << LocationToString() << ")";
	return o.str();
}

Register* IntegerValueNode::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    assert(dst);
    
    generator->EmitBytecode(op_load_int_constant);
    generator->EmitRegister(dst);
    generator->EmitConstantInt(m_value);
    
    dst->SetType(generator->GetGlobalData()->GetIntType());
    
    return dst;
}


// ============= StringValueNode =============

StringValueNode::StringValueNode(char* value)
    : m_value(value + 1, strlen(value) - 2)
{
}

std::string StringValueNode::ToString() const
{
    std::ostringstream o;
    o << "(\"" << m_value << "\" " << LocationToString() << ")";
	return o.str();
}

Register* StringValueNode::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    assert(dst);
    
    generator->EmitBytecode(op_load_string_constant);
    generator->EmitRegister(dst);
    generator->EmitConstantString(m_value);
    
    dst->SetType(generator->GetGlobalData()->GetStringType());
    
    return dst;
}

// ============= IdentifierNode =============

class MethodAccessor: public Accessor
{
public:
    MethodAccessor(Type* type, std::string name)
        : Accessor(type)
        , m_name(name)
    {
    }
    
    virtual Register* EmitLoad(BytecodeGenerator* generator, Register* dst) 
    {
        RefPtr<Register> where (dst ? dst : generator->NewTempRegister());
        
        generator->EmitBytecode(op_load_method);
        generator->EmitRegister(where.Ptr());
        generator->EmitConstantString(m_name);
        
        return where.Ptr();
    }
    
    virtual Register* EmitSave(BytecodeGenerator* generator, Register* src, Register* dst)
    {
        return 0;
    }

private:
    std::string m_name;
};

IdentifierNode::IdentifierNode(char* value)
    : m_value(value)
{
}

std::string IdentifierNode::ToString() const
{
    std::ostringstream o;
    o << "<<" << m_value << ">>";
	return o.str();
}

Register* IdentifierNode::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    assert(dst);
    if (m_value == "null")
    {
        generator->EmitBytecode(op_load_null);
        generator->EmitRegister(dst);
        dst->SetType(generator->GetGlobalData()->GetNullType());
        return dst;
    }
    
    MethodEnv* definedMethod = generator->GetGlobalData()->GetDefinedMethod(m_value);
    if (definedMethod)
    {
        generator->EmitBytecode(op_load_method);
        generator->EmitRegister(dst);
        generator->EmitConstantString(m_value);
        dst->SetType(generator->GetGlobalData()->GetCodeType());
        
        return dst;
    }
    
    PassRef<Accessor> accessor = generator->GetProperty(m_value);
    if (!accessor.Ptr())
    {
        printf("Property not found %s\n", m_value.c_str());
        exit(1);
    }

    dst->SetIgnored();
    return accessor->EmitLoad(generator, dst);
}

PassRef<Accessor> IdentifierNode::GetAccessor(BytecodeGenerator* generator)
{
    MethodEnv* definedMethod = generator->GetGlobalData()->GetDefinedMethod(m_value);
    if (definedMethod)
    {
        return AdoptRef<Accessor>(new MethodAccessor(generator->GetGlobalData()->GetCodeType(), m_value));
    }
    
    PassRef<Accessor> accessor = generator->GetProperty(m_value);
    if (!accessor.Ptr())
    {
        printf("Property not found %s\n", m_value.c_str());
        exit(1);
    }
    return accessor;
}


// ============ ExpressionNode ============

std::string ExpressionNode::ToString() const
{
	return std::string("[ExpressionNode ") + LocationToString() + "]";
}

// ============ CallNode ============

std::string CallNode::ToString() const
{
    std::ostringstream o;
    o << "(Call ";
    
    if (m_name.Ptr())
        o << m_name->ToString();
    
    o << " with ";
    
    if (m_arguments.Ptr())
        o << m_arguments->ToString();
    
    o << " " << LocationToString();
    
    o << ")";
	return o.str();
}

Register* CallNode::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    std::string name = m_name->CompleteTypeName();
    
    // if this is a name of a type, we have to initialize the object here
    Type* type = generator->GetGlobalData()->GetDefinedType(name);
    if (type)
    {
        RefPtr<Register> reg (dst ? dst : generator->NewTempRegister());
        
        if (type->IsObjectType())
        {
            generator->EmitBytecode(op_init_object);
            generator->EmitRegister(reg.Ptr());
            generator->EmitConstantString(type->Name());
            
            reg->SetType(type);
            
            return reg.Ptr();
        }
        else if (type->IsVectorRef())
        {
            if (!m_arguments.Ptr() || m_arguments->size() != 1)
            {
                printf("vector initialization needs 1 size argument\n");
                exit(1);
            }
            
            RefPtr<Register> sizeReg = generator->NewTempRegister();
            
            sizeReg = m_arguments->at(0)->EmitBytecode(generator, sizeReg.Ptr());
            
            if (sizeReg->GetType() != generator->GetGlobalData()->GetIntType())
            {
                sizeReg = generator->Coerce(sizeReg.Ptr(), generator->GetGlobalData()->GetIntType());
            }
            
            generator->EmitBytecode(op_init_vector);
            generator->EmitRegister(reg.Ptr());
            generator->EmitRegister(sizeReg.Ptr());
            generator->EmitConstantString(type->Name());
            
            reg->SetType(type);
            
            return reg.Ptr();
        }
        else
        {
            printf("cannot initialize builtin types");
            exit(1);
        }
    }
        
    PassRef<Accessor> codeObjectAccessor = generator->GetProperty(name);
    if (codeObjectAccessor.Ptr())
    {
        if (!(codeObjectAccessor->GetType() == generator->GetGlobalData()->GetCodeType()))
        {
            printf("cannot call non-code property\n");
        }
        
        if (m_arguments.Ptr() != 0 && m_arguments->size() != 0)
        {
            printf("cannot send arguments to code objects\n");
            exit(1);
        }
        
        RefPtr<Register> reg = codeObjectAccessor->EmitLoad(generator, 0);
        
        generator->EmitBytecode(op_call_code);
        generator->EmitRegister(reg.Ptr());
        
        return 0;
    }


    // this where the result will come (if we have one), otherwise it will be start of the next function frame
    RefPtr<Register> reg;

    MethodEnv* methodEnv = generator->GetGlobalData()->GetMethod(name);
    
    if (methodEnv->GetReturnType())
    {
        reg = generator->NewTempRegister();
        reg->SetType(methodEnv->GetReturnType());
        dst = reg.Ptr();
    }
    
    const TypeList* typeList = methodEnv->GetArgumentsType();
    if (typeList->size() > 0 && ((!m_arguments.Ptr()) || (m_arguments->size() != typeList->size())))
    {
        printf("invalid arguments count");
        exit(1);
    }
    
    if (m_arguments.Ptr())
    {
        // create all the registers at once, so that we have them in order
        std::vector<RefPtr<Register> > argumentRegisters;
        for (int i=0; i<m_arguments->size(); ++i)
            argumentRegisters.push_back(generator->NewTempRegister());
        if (!reg.Ptr())
            reg = argumentRegisters.at(0).Ptr();
        
        for (int i=0; i<m_arguments->size(); ++i)
        {
            Register* nice_to_have_dst = argumentRegisters.at(i).Ptr();
            RefPtr<Register> actual_dst = m_arguments->at(i)->EmitBytecode(generator, nice_to_have_dst);
            if (actual_dst.Ptr() != nice_to_have_dst)
            {
                // it returned another register
                // have to copy it over here
                generator->EmitBytecode(op_assign);
                generator->EmitRegister(nice_to_have_dst);
                generator->EmitRegister(actual_dst.Ptr());
                nice_to_have_dst->SetType(actual_dst->GetType());
            }
            
            if (nice_to_have_dst->GetType() != typeList->at(i).Ptr())
            {
                printf("invalid argument number %d\n", i);
                exit(1);
            }
        }
        
        generator->CleanupRegisters();
    }
    else
    {
        if (typeList->size() > 0)
        {
            printf("invalid arguments count");
            exit(1);
        }
    }
    
    generator->EmitBytecode(op_call_method);
    generator->EmitRegister(reg.Ptr());
    generator->EmitConstantString(name);
    
    return dst;
}

// ============ StatementNode ============

std::string StatementNode::ToString() const
{
	return std::string("[StatementNode ") + LocationToString() + "]";
}

// ============ BinaryOpNode ============

std::string BinaryOpNode::ToString() const
{
    std::ostringstream o;
    o << "(";

    if (m_node1.Ptr())
        o << m_node1->ToString() << " ";

    o << BinaryOpcodeToString(m_op) << " ";

    if (m_node2.Ptr())
        o << m_node2->ToString();
    
    o << " " << LocationToString();
    
    o << ")";
    
    return o.str();
}

Register* BinaryOpNode::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    assert(dst);
    assert(m_node1.Ptr());
    assert(m_node2.Ptr());
    
    RefPtr<Register> reg1 = generator