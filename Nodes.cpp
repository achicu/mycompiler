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

    if (m_typeIdentifier.Ptr())
        o << m_typeIdentifier->ToString() << " ";
    
    if (m_identifierList.Ptr())
        o << m_identifierList->ToString();
    
    o << " " << LocationToString();
    
    o << "]";
    
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

