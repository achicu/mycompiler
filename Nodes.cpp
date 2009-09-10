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
    o << "(" << m_value << "f)";
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
    o << "(" << m_value << ")";
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
    o << "(\"" << m_value << "\")";
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
	return "[ExpressionNode]";
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
    
    o << ")";
	return o.str();
}

// ============ StatementNode ============

std::string StatementNode::ToString() const
{
	return "[StatementNode]";
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
    
    o << "]";
    
    return o.str();
}

// ============ MethodStatement ============

std::string MethodStatement::ToString() const
{
    std::ostringstream o;
    o << "[MethodStatement ";

    if (m_identifier.Ptr())
        o << m_identifier->ToString() << " code ";

    if (m_nodes.Ptr())
        o << m_nodes->ToString();
    
    o << "]";
    
    return o.str();
}

// ============ VarDeclarationStatement ============

std::string VarDeclarationStatement::ToString() const
{
    std::ostringstream o;
    o << "[Var ";

    if (m_typeIdentifier.Ptr())
        o << m_typeIdentifier->ToString() << " ";

    if (m_nameIdentifier.Ptr())
        o << m_nameIdentifier->ToString();
    
    o << "]";
    
    return o.str();
}


// ============ TemplateVarDeclarationStatement ============

std::string TemplateVarDeclarationStatement::ToString() const
{
    std::ostringstream o;
    o << "[TemplateVar ";

    if (m_typeIdentifier.Ptr())
        o << m_typeIdentifier->ToString() << " ";
    
    if (m_identifierList.Ptr())
        o << m_identifierList->ToString() << " ";    

    if (m_nameIdentifier.Ptr())
        o << m_nameIdentifier->ToString();
    
    o << "]";
    
    return o.str();
}

