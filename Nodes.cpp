/*
 *  Nodes.cpp
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/10/09.
 *
 */
#include "Platform.h"

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
        printf("Property not found %s %s\n", m_value.c_str(), LocationToString().c_str());
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
        printf("Property not found %s %s\n", m_value.c_str(), LocationToString().c_str());
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
                printf("vector initialization needs 1 size argument %s\n", LocationToString().c_str());
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
            printf("cannot initialize builtin types %s\n", LocationToString().c_str());
            exit(1);
        }
    }
        
    PassRef<Accessor> codeObjectAccessor = generator->GetProperty(name);
    if (codeObjectAccessor.Ptr())
    {
        if (!(codeObjectAccessor->GetType() == generator->GetGlobalData()->GetCodeType()))
        {
            printf("cannot call non-code property %s\n", LocationToString().c_str());
        }
        
        if (m_arguments.Ptr() != 0 && m_arguments->size() != 0)
        {
            printf("cannot send arguments to code objects %s\n", LocationToString().c_str());
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
        printf("invalid arguments count %s\n", LocationToString().c_str());
        exit(1);
    }
    
    if (m_arguments.Ptr())
    {
        // create all the registers at once, so that we have them in order
        std::vector<RefPtr<Register> > argumentRegisters;
        for (unsigned i=0; i<m_arguments->size(); ++i)
            argumentRegisters.push_back(generator->NewTempRegister());
        
        if (!reg.Ptr())
            reg = argumentRegisters.at(0).Ptr();
        
        for (unsigned i=0; i<m_arguments->size(); ++i)
        {
            Register* nice_to_have_dst = argumentRegisters.at(i).Ptr();
            RefPtr<Register> actual_dst = m_arguments->at(i)->EmitBytecode(generator, nice_to_have_dst);
            
            if (actual_dst->GetType() != typeList->at(i).Ptr())
            {
                actual_dst = generator->Coerce(actual_dst.Ptr() , typeList->at(i).Ptr());
            }
            
            if (actual_dst.Ptr() != nice_to_have_dst)
            {
                // it returned another register
                // have to copy it over here
                generator->EmitBytecode(op_assign);
                generator->EmitRegister(nice_to_have_dst);
                generator->EmitRegister(actual_dst.Ptr());
                nice_to_have_dst->SetType(actual_dst->GetType());
            }
        }
        
        generator->CleanupRegisters();
    }
    else
    {
        if (typeList->size() > 0)
        {
            printf("invalid arguments count %s\n", LocationToString().c_str());
            exit(1);
        }
    }
    
    generator->EmitBytecode(op_call_method);
    if (!reg.Ptr())
        reg = generator->NewTempRegister();
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
    
    RefPtr<Register> reg1 = generator->NewTempRegister();
    reg1 = m_node1->EmitBytecode(generator, reg1.Ptr());
    
    RefPtr<Register> reg2 = generator->NewTempRegister();
    reg2 = m_node2->EmitBytecode(generator, reg2.Ptr());
    
    Type* type1 = reg1->GetType();
    Type* type2 = reg2->GetType();
    assert(type1);
    assert(type2);
    
    dst = type1->EmitBinaryOpBytecode(generator, type2, m_op, reg1.Ptr(), reg2.Ptr(), dst);

    // this will always generate a new refernce 
    // no need to increment ref here
    /*if (dst->GetType()->IsRefCounted())
    {
        generator->EmitIncRef(dst);
    }*/
    
    return dst;
}

// ============ AssignOpNode ============

std::string AssignOpNode::ToString() const
{
    std::ostringstream o;
    o << "(";

    o << AssignOpcodeToString(m_op) << " ";

    if (m_node1.Ptr())
        o << m_node1->ToString() << " ";
    
    if (m_node2.Ptr())
        o << m_node2->ToString() << " ";
    
    o << ")";
    
    return o.str();
}

Register* AssignOpNode::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    assert(dst);
    assert(m_node1.Ptr());
    
    RefPtr<Accessor> accessor = m_node1->GetAccessor(generator);
    assert(accessor.Ptr());

    Type* type1 = accessor->GetType();
    assert(type1);
    
    return type1->EmitAssignOpBytecode(generator, m_op, accessor.Ptr(), m_node2.Ptr(), dst);
}

// ============ UnaryOpNode ============

std::string UnaryOpNode::ToString() const
{
    std::ostringstream o;
    o << "(";

    o << UnaryOpcodeToString(m_op) << " ";

    if (m_node1.Ptr())
        o << m_node1->ToString() << " ";
    
    o << ")";
    
    return o.str();
}

Register* UnaryOpNode::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    assert(dst);
    assert(m_node1.Ptr());
    
    RefPtr<Register> reg1 = generator->NewTempRegister();
    reg1 = m_node1->EmitBytecode(generator, reg1.Ptr());
        
    Type* type1 = reg1->GetType();
    assert(type1);
    
    dst = type1->EmitUnaryOpBytecode(generator, m_op, reg1.Ptr(), dst);

    // this will always generate a new refernce
    // no need to increment ref here    
    /*if (dst->GetType()->IsRefCounted())
    {
        generator->EmitIncRef(dst);
    }*/
    
    return dst;
}

// ============ AssignNode ============

std::string AssignNode::ToString() const
{
    std::ostringstream o;
    o << "(";

    if (m_node1.Ptr())
        o << m_node1->ToString() << " ";
        
    o << "= ";

    if (m_node2.Ptr())
        o << m_node2->ToString();
    
    o << " " << LocationToString();
    
    o << ")";
    
    return o.str();
}

Register* AssignNode::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    assert(m_node1.Ptr());
    assert(m_node2.Ptr());
    
    RefPtr<Register> reg2 = generator->NewTempRegister();
    reg2 = m_node2->EmitBytecode(generator, reg2.Ptr());
    
    RefPtr<Accessor> accessor( m_node1->GetAccessor(generator) );
    assert(accessor.Ptr());
    
    if (!reg2->GetType())
    {
        printf("void type assignment %s\n", LocationToString().c_str());
        exit(1);
    }
    
    if (accessor->GetType() != reg2->GetType())
    {
        reg2 = generator->Coerce(reg2.Ptr(), accessor->GetType());
    }
    
    assert(reg2->GetType() == accessor->GetType());
        
    return accessor->EmitSave(generator, reg2.Ptr(), dst);
}


// ============ AccessorNode ============

std::string AccessorNode::ToString() const
{
    std::ostringstream o;
    o << "[AccessorNode ";

    if (m_node.Ptr())
        o << m_node->ToString() << " offset ";

    if (m_offsetNode.Ptr())
        o << m_offsetNode->ToString();
    
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

PassRef<Accessor> AccessorNode::GetAccessor(BytecodeGenerator* generator)
{
    RefPtr<Accessor> accessor = m_node->GetAccessor(generator);
    if (!accessor.Ptr())
    {
        printf("invalid referenced object %s\n", LocationToString().c_str());
        exit(1);
    }
    
    RefPtr<Register> dst ( generator->NewTempRegister() );
    dst = accessor->EmitLoad( generator, dst.Ptr() );
    
    assert(dst->GetType());
    if (!dst->GetType()->IsVectorRef())
    {
        printf("invalid offset accessor getter on non vector type %s\n", LocationToString().c_str());
        exit(1);
    }
    
    RefPtr<Register> offsetRegister ( generator->NewTempRegister() );
    offsetRegister = m_offsetNode->EmitBytecode(generator, offsetRegister.Ptr());
    
    VectorType* objectType = static_cast<VectorType*>(dst->GetType());
    return objectType->GetRegisterAccessor(offsetRegister.Ptr(), dst.Ptr());
}

Register* AccessorNode::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    assert(dst);
    
    PassRef<Accessor> accessor = GetAccessor(generator);
    dst->SetIgnored();
    
    return accessor->EmitLoad(generator, dst);
}


// ============ DotNode ============

std::string DotNode::ToString() const
{
    std::ostringstream o;
    o << "[DotNode ";

    if (m_node.Ptr())
        o << m_node->ToString() << ".";
    
    if (m_identifier.Ptr())
        o << m_identifier->ToString();
    
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

PassRef<Accessor> DotNode::GetAccessor(BytecodeGenerator* generator)
{
    RefPtr<Accessor> accessor = m_node->GetAccessor(generator);
    if (!accessor.Ptr())
    {
        printf("invalid referenced object %s\n", LocationToString().c_str());
        exit(1);
    }
    
    RefPtr<Register> dst ( generator->NewTempRegister() );
    dst = accessor->EmitLoad( generator, dst.Ptr() );
    
    assert(dst->GetType());
    if (!dst->GetType()->IsObjectType())
    {
        printf("invalid property getter on non object type %s\n", LocationToString().c_str());
        exit(1);
    }
    
    ObjectType* objectType = static_cast<ObjectType*>(dst->GetType());
    return objectType->GetPropertyAccessor(m_identifier->Value(), dst.Ptr());
}

Register* DotNode::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    assert(dst);
    
    RefPtr<Accessor> leftAccessor = m_node->GetAccessor(generator);
    if (!leftAccessor.Ptr())
    {
        printf("invalid referenced object %s\n", LocationToString().c_str());
        exit(1);
    }
    
    RefPtr<Register> leftDst ( generator->NewTempRegister() );
    leftDst = leftAccessor->EmitLoad( generator, leftDst.Ptr() );
    
    assert(leftDst->GetType());
    if (leftDst->GetType()->IsVectorRef())
    {
        if (m_identifier->Value() == "size")
        {
            // get the size of the vector
            generator->EmitBytecode(op_vector_size);
            generator->EmitRegister(dst);
            generator->EmitRegister(leftDst.Ptr());
            dst->SetType(generator->GetGlobalData()->GetIntType());
            return dst;
        }
        
        printf("invalid property getter on vector object %s\n", LocationToString().c_str());
        exit(1);
    }
    
    if (!leftDst->GetType()->IsObjectType())
    {
        printf("invalid property getter on non object type %s\n", LocationToString().c_str());
        exit(1);
    }
    
    ObjectType* objectType = static_cast<ObjectType*>(leftDst->GetType());
    PassRef<Accessor> accessor = objectType->GetPropertyAccessor(m_identifier->Value(), leftDst.Ptr());
    
    return accessor->EmitLoad(generator, dst);
}

// ============ ArgumentNode ============

std::string ArgumentNode::ToString() const
{
    std::ostringstream o;
    o << "[ArgumentNode ";
    
    if (m_typeNode.Ptr())
        o << m_typeNode->ToString() << " ";

    if (m_identifier.Ptr())
        o << m_identifier->ToString() << " code ";
    
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}


// ============ MethodNode ============

std::string MethodNode::ToString() const
{
    std::ostringstream o;
    o << "[MethodNode ";
    
    if (m_typeNode.Ptr())
        o << m_typeNode->ToString() << " ";

    if (m_identifier.Ptr())
        o << m_identifier->ToString() << " code ";

    if (m_nodes.Ptr())
        o << m_nodes->ToString();
    
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

// ============ StructNode ============

std::string StructNode::ToString() const
{
    std::ostringstream o;
    o << "[StructNode ";

    if (m_extends.Ptr())
        o << "extends " << m_extends->ToString() << " ";

    if (m_identifier.Ptr())
        o << m_identifier->ToString() << " with ";

    if (m_nodes.Ptr())
        o << m_nodes->ToString();
    
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

// ============ TypeNode ============

std::string TypeNode::ToString() const
{
    std::ostringstream o;
    o << "[TypeNode ";

    assert(m_typeIdentifier.Ptr());
    o << m_typeIdentifier->ToString() << " ";
    
    if (m_typeNodeList.Ptr())
        o << m_typeNodeList->ToString();
    
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

std::string TypeNode::GetName() const
{
    assert(m_typeIdentifier.Ptr());
    return m_typeIdentifier->Value();
}

std::string TypeNode::CompleteTypeName() const
{
    std::ostringstream o;

    assert(m_typeIdentifier.Ptr());
    
    o << m_typeIdentifier->Value();
    
    if (m_typeNodeList.Ptr())
    {
        o << "<";
        for (unsigned i=0; i<m_typeNodeList->size(); ++i)
        {
            assert(m_typeNodeList->at(i).Ptr());
            if (i != 0)
                o << ", ";
            o << m_typeNodeList->at(i)->CompleteTypeName();
        }
        o << ">";
    }
    
    return o.str();
}

// ============ VarStatement ============

std::string VarStatement::ToString() const
{
    std::ostringstream o;
    o << "[Var ";

    if (m_typeNode.Ptr())
        o << m_typeNode->ToString() << " ";

    if (m_nameIdentifier.Ptr())
        o << m_nameIdentifier->ToString();
    
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

Register* VarStatement::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    assert(dst == 0);
    assert(m_nameIdentifier.Ptr());
    if (m_initializer.Ptr())
    {
        RefPtr<Register> initializedValue = generator->NewTempRegister();
        initializedValue = m_initializer->EmitBytecode(generator, initializedValue.Ptr());
        
        PassRef<Accessor> accessor = generator->GetProperty(m_nameIdentifier->Value());
        assert(accessor.Ptr());
        
        if (!initializedValue->GetType())
        {
            printf("void type assignment %s\n", LocationToString().c_str());
            exit(1);
        }
        
        if (accessor->GetType() != initializedValue->GetType())
        {
            initializedValue = generator->Coerce(initializedValue.Ptr(), accessor->GetType());
        }
                
        accessor->EmitSave(generator, initializedValue.Ptr(), 0);
    }
    else
    {
        // we used to auto initialize variables
        /*PassRef<Accessor> accessor = generator->GetProperty(m_nameIdentifier->Value());
        assert(accessor.Ptr());
        
        if (accessor->GetType()->IsObjectType())
        {
            // this will not generate any bytecode
            RefPtr<Register> valueRegister (accessor->EmitLoad(generator, 0));
            ObjectType* objectType = static_cast<ObjectType*>(accessor->GetType());
            
            generator->EmitBytecode(op_init_object);
            generator->EmitRegister(valueRegister.Ptr());
            generator->EmitConstantString(objectType->Name());
        }*/
    }
    
    return 0;
}

// ============ ExpressionStatement ============

std::string ExpressionStatement::ToString() const
{
    std::ostringstream o;
    o << "[ExpressionStatement ";

    if (m_expression.Ptr())
        o << m_expression->ToString();
        
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

Register* ExpressionStatement::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    assert(m_expression.Ptr());
    
    if (!dst)
        dst = generator->NewTempRegister().Ptr();
    
    return m_expression->EmitBytecode(generator, dst);
}

// ============ ReturnStatement ============

std::string ReturnStatement::ToString() const
{
    std::ostringstream o;
    o << "[ReturnStatement ";

    if (m_expression.Ptr())
        o << m_expression->ToString();
        
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

Register* ReturnStatement::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    if (m_expression.Ptr())
    {
        RefPtr<Register> reg = generator->NewTempRegister();
        reg = m_expression->EmitBytecode(generator, reg.Ptr());
        
        static std::string returnValue("$ReturnValue");
        RefPtr<Accessor> returnValueAccessor = generator->GetProperty(returnValue, true);
        if (!returnValueAccessor.Ptr())
        {
            printf("No return type defined %s\n", LocationToString().c_str());
            exit(1);
        }
        
        if (returnValueAccessor->GetType() != reg->GetType())
        {
            reg = generator->Coerce(reg.Ptr(), returnValueAccessor->GetType());
        }
        
        returnValueAccessor->EmitSave(generator, reg.Ptr(), 0);
    }
    
    return dst;
}

// ============ ContinueStatement ============

std::string ContinueStatement::ToString() const
{
    std::ostringstream o;
    o << "[ContinueStatement ";
        
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

Register* ContinueStatement::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    generator->EmitContinue();
    
    return 0;
}

// ============ BreakStatement ============

std::string BreakStatement::ToString() const
{
    std::ostringstream o;
    o << "[BreakStatement ";
        
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

Register* BreakStatement::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    generator->EmitBreak();
    
    return 0;
}

// ============ DebugStatement ============

std::string DebugStatement::ToString() const
{
    std::ostringstream o;
    o << "[DebugStatement ";

    if (m_expression.Ptr())
        o << m_expression->ToString();
        
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

Register* DebugStatement::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    assert(m_expression.Ptr());
    
    if (!dst)
        dst = generator->NewTempRegister().Ptr();
    
    dst = m_expression->EmitBytecode(generator, dst);
    
    Type* type = dst->GetType();
    
    if (!type)
    {
        printf("cannot debug type void %s\n", LocationToString().c_str());
        exit(1);
    }
    else if (generator->GetGlobalData()->GetIntType() == type)
    {
        generator->EmitBytecode(op_debug_int);
    }
    else if (generator->GetGlobalData()->GetFloatType() == type)
    {
        generator->EmitBytecode(op_debug_float);
    }
    else if (generator->GetGlobalData()->GetStringType() == type)
    {
        generator->EmitBytecode(op_debug_string);
    }
    else if (generator->GetGlobalData()->GetCodeType() == type)
    {
        generator->EmitBytecode(op_debug_code);
    }
    else if (type->IsObjectType())
    {
        generator->EmitBytecode(op_debug_object);
        generator->EmitConstantString(type->Name());
    }
    else if (type->IsVectorRef())
    {
        generator->EmitBytecode(op_debug_vector);
        generator->EmitConstantString(type->Name());
    }
    else
    {
        printf("debug failed for type \"%s\" %s\n", type->Name().c_str(), LocationToString().c_str());
        exit(1);
    }
    
    generator->EmitRegister(dst);
    
    return dst;
}

// ============ ReadStatement ============

std::string ReadStatement::ToString() const
{
    std::ostringstream o;
    o << "[ReadStatement ";

    if (m_expression.Ptr())
        o << m_expression->ToString();
        
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

Register* ReadStatement::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    assert(m_expression.Ptr());
    
    RefPtr<Accessor> accessor = m_expression->GetAccessor(generator);
    
    Type* type = accessor->GetType();
    
    RefPtr<Register> reg = generator->NewTempRegister();
    
    if (!type)
    {
        printf("cannot read type void %s\n", LocationToString().c_str());
        exit(1);
    }
    else if (generator->GetGlobalData()->GetIntType() == type)
    {
        generator->EmitBytecode(op_read_int);
    }
    else if (generator->GetGlobalData()->GetFloatType() == type)
    {
        generator->EmitBytecode(op_read_float);
    }
    else if (generator->GetGlobalData()->GetStringType() == type)
    {
        generator->EmitBytecode(op_read_string);
    }
    else
    {
        printf("read failed for type \"%s\" %s\n", type->Name().c_str(), LocationToString().c_str());
        exit(1);
    }
    
    generator->EmitRegister(reg.Ptr());
    
    accessor->EmitSave(generator, reg.Ptr(), 0);
    
    return 0;
}


// ============ IfStatement ============

std::string IfStatement::ToString() const
{
    std::ostringstream o;
    o << "[If (";

    assert (m_expression.Ptr());
    o << m_expression->ToString() << ") ";
        
    o << "{ ";

    assert (m_ifBranch.Ptr());
    o << m_ifBranch->ToString();
    
    o << " }";
    
    if (m_elseBranch.Ptr())
    {
        o << " else { ";
        o << m_elseBranch->ToString();
        o << " }";
    }
    
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

Register* IfStatement::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    RefPtr<Register> expressionReg = generator->NewTempRegister().Ptr();
    
    assert(m_expression.Ptr());
    expressionReg = m_expression->EmitBytecode(generator, expressionReg.Ptr());
    
    if (expressionReg->GetType() != generator->GetGlobalData()->GetIntType())
    {
        expressionReg = generator->Coerce(expressionReg.Ptr(), generator->GetGlobalData()->GetIntType());
    }
    
    generator->CleanupRegisters();
    
    generator->EmitBytecode(op_jmp_if_false);
    generator->EmitRegister(expressionReg.Ptr());
    int patchJumpLabel = generator->GetLabel();
    generator->EmitConstantInt(0);
    
    assert(m_ifBranch.Ptr());
    
    for (unsigned i=0; i<m_ifBranch->size(); i++)
    {
        if (m_ifBranch->at(i).Ptr())
            generator->EmitNode(m_ifBranch->at(i).Ptr());
    }
    
    generator->CleanupRegisters();
    
    // patch the jump
    int jumpLabel = generator->GetLabel();
    generator->PatchConstantInt(patchJumpLabel, jumpLabel);
    
    if (m_elseBranch.Ptr())
    {
        for (unsigned i=0; i<m_elseBranch->size(); i++)
        {
            if (m_elseBranch->at(i).Ptr())
                generator->EmitNode(m_elseBranch->at(i).Ptr());
        }
        generator->CleanupRegisters();
    }
    
    return 0;
}

// ============ WhileStatement ============

std::string WhileStatement::ToString() const
{
    std::ostringstream o;
    o << "[While (";

    if (m_expression.Ptr())
        o << m_expression->ToString();

    o << ") ";
        
    o << "{ ";

    assert (m_whileBranch.Ptr());
    o << m_whileBranch->ToString();
    
    o << " }";
        
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

Register* WhileStatement::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    BreakOrContinueHelper breakOrContinueHelper(generator);
    
    generator->CleanupRegisters();
    
    int startLabel = generator->GetLabel();
    int patchJumpLabel = -1;
    
    breakOrContinueHelper.SetContinueLabel(startLabel);
    
    if (m_expression.Ptr())
    {
        RefPtr<Register> expressionReg = generator->NewTempRegister().Ptr();
        
        expressionReg = m_expression->EmitBytecode(generator, expressionReg.Ptr());
        
        if (expressionReg->GetType() != generator->GetGlobalData()->GetIntType())
        {
            expressionReg = generator->Coerce(expressionReg.Ptr(), generator->GetGlobalData()->GetIntType());
        }
        
        generator->CleanupRegisters();
        
        generator->EmitBytecode(op_jmp_if_false);
        generator->EmitRegister(expressionReg.Ptr());
        patchJumpLabel = generator->GetLabel();
        generator->EmitConstantInt(0);
    }
    
    assert(m_whileBranch.Ptr());
    for (unsigned i=0; i<m_whileBranch->size(); i++)
    {
        if (m_whileBranch->at(i).Ptr())
            generator->EmitNode(m_whileBranch->at(i).Ptr());
    }
    
    generator->CleanupRegisters();
    
    generator->EmitBytecode(op_jmp);
    generator->EmitConstantInt(startLabel);
    
    // patch the jump
    int jumpLabel = generator->GetLabel();
    breakOrContinueHelper.SetBreakLabel(jumpLabel);
    if (patchJumpLabel != -1)
        generator->PatchConstantInt(patchJumpLabel, jumpLabel);
    
    return 0;
}

// ============ ForStatement ============

std::string ForStatement::ToString() const
{
    std::ostringstream o;
    o << "[For (";

    if (m_expression1.Ptr())
        o << m_expression1->ToString();
    o << ", ";

    if (m_expression2.Ptr())
        o << m_expression2->ToString();
    o << ", ";

    if (m_expression3.Ptr())
        o << m_expression3->ToString();
    o << ", ";

    o << ") ";
        
    o << "{ ";

    assert (m_forBranch.Ptr());
    o << m_forBranch->ToString();
    
    o << " }";
        
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
}

Register* ForStatement::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    BreakOrContinueHelper breakOrContinueHelper(generator);
    
    // generate the first node
    if (m_expression1.Ptr())
    {
        m_expression1->EmitBytecode(generator, generator->NewTempRegister().Ptr());
    }
    
    generator->CleanupRegisters();
    
    int startLabel = generator->GetLabel();
    int patchJumpLabel = -1;
    
    if (m_expression2.Ptr())
    {
        RefPtr<Register> expressionReg = generator->NewTempRegister().Ptr();

        expressionReg = m_expression2->EmitBytecode(generator, expressionReg.Ptr());
        
        if (expressionReg->GetType() != generator->GetGlobalData()->GetIntType())
        {
            expressionReg = generator->Coerce(expressionReg.Ptr(), generator->GetGlobalData()->GetIntType());
        }
        
        generator->CleanupRegisters();
    
        generator->EmitBytecode(op_jmp_if_false);
        generator->EmitRegister(expressionReg.Ptr());
        patchJumpLabel = generator->GetLabel();
        generator->EmitConstantInt(0);
    }
    
    assert(m_forBranch.Ptr());
    for (unsigned i=0; i<m_forBranch->size(); i++)
    {
        if (m_forBranch->at(i).Ptr())
            generator->EmitNode(m_forBranch->at(i).Ptr());
    }
    
    generator->CleanupRegisters();
    
    breakOrContinueHelper.SetContinueLabel(generator->GetLabel());
    
    if (m_expression3.Ptr())
    {
        m_expression3->EmitBytecode(generator, generator->NewTempRegister().Ptr());
    }
    
    generator->CleanupRegisters();
    
    generator->EmitBytecode(op_jmp);
    generator->EmitConstantInt(startLabel);
    
    // patch the jump
    int jumpLabel = generator->GetLabel();
    breakOrContinueHelper.SetBreakLabel(jumpLabel);
    
    if (patchJumpLabel != -1)
        generator->PatchConstantInt(patchJumpLabel, jumpLabel);
    
    return 0;
}

// ===================

const char* AssignOpcodeToString(AssignOpcode opcode)
{
    switch(opcode)
    {
        case assign_op_plusplus_prefix: return "assign_op_plusplus_prefix";
        case assign_op_minusminus_prefix: return "assign_op_minusminus_prefix";
        case assign_op_plusplus_sufix: return "assign_op_plusplus_sufix";
        case assign_op_minusminus_sufix: return "assign_op_minusminus_sufix";
    }
    
    assert(false);
    return "undefined";
};

const char* UnaryOpcodeToString(UnaryOpcode opcode)
{
    switch(opcode)
    {
        case unary_op_not: return "unary_op_not";
    }
    
    assert(false);
    return "undefined";
};

const char* BinaryOpcodeToString(BinaryOpcode opcode)
{
    switch(opcode)
    {
        case binary_op_plus: return "binary_op_plus";
        case binary_op_minus: return "binary_op_minus";
        case binary_op_multiply: return "binary_op_multiply";
        case binary_op_divide: return "binary_op_divide";
        case binary_op_less: return "binary_op_less";
        case binary_op_more: return "binary_op_more";
        case binary_op_equal: return "binary_op_equal";
        case binary_op_less_or_equal: return "binary_op_less_or_equal";
        case binary_op_more_or_equal: return "binary_op_more_or_equal";
        case binary_op_bit_and: return "binary_op_bit_and";
        case binary_op_bit_or: return "binary_op_bit_or";
        case binary_op_bit_xor: return "binary_op_bit_xor";
    }
    
    assert(false);
    return "undefined";
}