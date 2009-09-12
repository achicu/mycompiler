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
    
    return dst;
}


// ============= StringValueNode =============

StringValueNode::StringValueNode(char* value)
    : m_value(value)
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

    o << m_op << " ";

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
    m_node1->EmitBytecode(generator, reg1.Ptr());
    
    RefPtr<Register> reg2 = generator->NewTempRegister();
    m_node2->EmitBytecode(generator, reg2.Ptr());
    
    switch(m_op)
    {
        case '+':
            generator->EmitBytecode(op_plus);
        break;
        case '*':
            generator->EmitBytecode(op_multiply);
        break;
        case '-':
            generator->EmitBytecode(op_minus);
        break;
        case '\\':
            generator->EmitBytecode(op_divide);
        break;
        default:
            assert(false);
    }
    
    generator->EmitRegister(dst);
    generator->EmitRegister(reg1.Ptr());
    generator->EmitRegister(reg2.Ptr());
    
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
    
    m_expression->EmitBytecode(generator, dst);
    
    return dst;
}
