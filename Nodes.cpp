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
    generator->EmitBytecode(op_call_method);
    generator->EmitConstantString(m_name->Value());
    
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
    
    return type1->EmitAssignOpBytecode(generator, m_op, accessor.Ptr(), dst);
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
    
    if (accessor->GetType() != reg2->GetType())
    {
        generator->CoerceInPlace(reg2.Ptr(), accessor->GetType());
    }
    
    assert(reg2->GetType() == accessor->GetType());
    
    if (accessor->GetType()->IsRefCounted())
    {
        generator->EmitIncRef(reg2.Ptr()); // increment first

        RefPtr<Register> valueRegister (accessor->EmitLoad(generator, 0));
        generator->EmitDecRef(valueRegister.Ptr());
    }
    
    return accessor->EmitSave(generator, reg2.Ptr(), dst);
}


// ============ AccessorNode ============

std::string AccessorNode::ToString() const
{
    std::ostringstream o;
    o << "[AccessorNode ";

    if (m_identifier.Ptr())
        o << m_identifier->ToString() << " offset ";

    if (m_nodes.Ptr())
        o << m_nodes->ToString();
    
    o << " " << LocationToString();
    
    o << "]";
    
    return o.str();
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

std::string TypeNode::CompleteTypeName() const
{
    std::ostringstream o;

    assert(m_typeIdentifier.Ptr());
    
    o << m_typeIdentifier->Value();
    
    if (m_typeNodeList.Ptr())
    {
        o << "<";
        for (int i=0; i<m_typeNodeList->size(); ++i)
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
        
        if (accessor->GetType() != initializedValue->GetType())
        {
            generator->CoerceInPlace(initializedValue.Ptr(), accessor->GetType());
        }
        
        if (accessor->GetType()->IsRefCounted())
        {
            generator->EmitIncRef(initializedValue.Ptr());
            
            RefPtr<Register> valueRegister (accessor->EmitLoad(generator, 0));
            generator->EmitDecRef(valueRegister.Ptr());
        }
        
        accessor->EmitSave(generator, initializedValue.Ptr(), 0);
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

// ============ ExpressionStatement ============

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
    
    if (generator->GetGlobalData()->GetIntType() == dst->GetType())
    {
        generator->EmitBytecode(op_debug_int);
    }
    else if (generator->GetGlobalData()->GetFloatType() == dst->GetType())
    {
        generator->EmitBytecode(op_debug_float);
    }
    else if (generator->GetGlobalData()->GetStringType() == dst->GetType())
    {
        generator->EmitBytecode(op_debug_string);
    }
    else
    {
        assert(dst->GetType());
        printf("debug failed for type %s\n", dst->GetType()->Name().c_str());
    }
    
    generator->EmitRegister(dst);
    
    return dst;
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
    RefPtr<Register> expressionReg = dst ? dst : generator->NewTempRegister().Ptr();
    
    assert(m_expression.Ptr());
    expressionReg = m_expression->EmitBytecode(generator, expressionReg.Ptr());
    
    if (expressionReg->GetType() != generator->GetGlobalData()->GetIntType())
    {
        generator->CoerceInPlace(expressionReg.Ptr(), generator->GetGlobalData()->GetIntType());
    }
    
    generator->CleanupRegisters();
    
    generator->EmitBytecode(op_jmp_if_false);
    generator->EmitRegister(expressionReg.Ptr());
    int patchJumpLabel = generator->GetLabel();
    generator->EmitConstantInt(0);
    
    assert(m_ifBranch.Ptr());
    
    for (int i=0; i<m_ifBranch->size(); i++)
    {
        generator->EmitNode(m_ifBranch->at(i).Ptr());
    }
    
    generator->CleanupRegisters();
    
    // patch the jump
    int jumpLabel = generator->GetLabel();
    generator->PatchConstantInt(patchJumpLabel, jumpLabel);
    
    if (m_elseBranch.Ptr())
    {
        for (int i=0; i<m_elseBranch->size(); i++)
        {
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

    assert (m_expression.Ptr());
    o << m_expression->ToString() << ") ";
        
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
    RefPtr<Register> expressionReg = dst ? dst : generator->NewTempRegister().Ptr();
    
    generator->CleanupRegisters();
    
    int startLabel = generator->GetLabel();
    
    assert(m_expression.Ptr());
    expressionReg = m_expression->EmitBytecode(generator, expressionReg.Ptr());
    
    if (expressionReg->GetType() != generator->GetGlobalData()->GetIntType())
    {
        generator->CoerceInPlace(expressionReg.Ptr(), generator->GetGlobalData()->GetIntType());
    }
    
    generator->CleanupRegisters();
    
    generator->EmitBytecode(op_jmp_if_false);
    generator->EmitRegister(expressionReg.Ptr());
    int patchJumpLabel = generator->GetLabel();
    generator->EmitConstantInt(0);
    
    assert(m_whileBranch.Ptr());
    for (int i=0; i<m_whileBranch->size(); i++)
    {
        generator->EmitNode(m_whileBranch->at(i).Ptr());
    }
    
    generator->CleanupRegisters();
    
    generator->EmitBytecode(op_jmp);
    generator->EmitConstantInt(startLabel);
    
    // patch the jump
    int jumpLabel = generator->GetLabel();
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