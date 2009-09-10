/*
 *  Nodes.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/10/09.
 *  Copyright 2009 Adobe Systems Romania SRL. All rights reserved.
 *
 */

#ifndef NODES_H
#define NODES_H

#include "ParserArena.h"
#include <string>
#include <vector>
#include <sstream>

template <typename Type>
class ArenaList: public ArenaNode, public std::vector<Type>
{
public:
    virtual std::string ToString() const;
};

template <typename Type>
std::string ArenaList<Type>::ToString() const
{
    std::ostringstream o;
    o << " (";

    for (int i=0; i<ArenaList<Type>::size(); ++i)
    {
        if (ArenaList<Type>::at(i).Ptr())
        {
            if (i > 0)
                o << ", ";
            o << ArenaList<Type>::at(i)->ToString();
        }
    }
    o << ") ";
	return o.str();
}

class StatementNode;
class IdentifierNode;

typedef ArenaList<RefPtr<ArenaNode> > NodeList;
typedef ArenaList<RefPtr<StatementNode> > StatementList;
typedef ArenaList<RefPtr<IdentifierNode> > IdentifierList;

class ExpressionNode: public ArenaNode
{
public:
    virtual std::string ToString() const;
};

class FloatValueNode: public ArenaNode
{
public:
    FloatValueNode(double value);
    
    virtual std::string ToString() const;
    
private:
    double m_value;

};

class IntegerValueNode: public ArenaNode
{
public:
    IntegerValueNode(int value);
    
    virtual std::string ToString() const;

private:
    int m_value;
};

class StringValueNode: public ArenaNode
{
public:
    StringValueNode(char* name);
    
    virtual std::string ToString() const;
    
private:
    std::string m_value;
};

class IdentifierNode: public ArenaNode
{
public:
    IdentifierNode(char* name);
    
    virtual std::string ToString() const;
    
private:
    std::string m_value;
};

class CallNode: public ArenaNode
{
public:
    CallNode(ArenaNode* name, NodeList* arguments)
        : m_name(name)
        , m_arguments(arguments)
    {
    }
    
    virtual std::string ToString() const;

private:
    RefPtr<ArenaNode> m_name;
    RefPtr<NodeList> m_arguments;
};

class StatementNode: public ArenaNode
{
public:
    virtual std::string ToString() const;
};

class BinaryOpNode: public ArenaNode
{
public:
    BinaryOpNode(char op, ArenaNode* node1, ArenaNode* node2)
        : m_op(op)
        , m_node1(node1)
        , m_node2(node2)
    {
    }

    virtual std::string ToString() const;

private:
    char m_op;
    RefPtr<ArenaNode> m_node1;
    RefPtr<ArenaNode> m_node2;
};

class AssignNode: public StatementNode
{
public:
    AssignNode(ArenaNode* node1, ArenaNode* node2)
        : m_node1(node1)
        , m_node2(node2)
    {
    }

    virtual std::string ToString() const;
    
private:
    RefPtr<ArenaNode> m_node1;
    RefPtr<ArenaNode> m_node2;
};

class AccessorNode: public ArenaNode
{
public:
    AccessorNode(ArenaNode* identifier, NodeList* nodes)
        : m_identifier(identifier)
        , m_nodes(nodes)
    {
    }
    
    virtual std::string ToString() const;
    
private:
    RefPtr<ArenaNode> m_identifier;
    RefPtr<NodeList> m_nodes;
};

class DotNode: public ArenaNode
{
public:
    DotNode(ArenaNode* node, IdentifierNode* identifier)
        : m_node(node)
        , m_identifier(identifier)
    {
    }
    
    virtual std::string ToString() const;
    
private:
    RefPtr<ArenaNode> m_node;
    RefPtr<IdentifierNode> m_identifier;
};

class MethodStatement: public StatementNode
{
public:
    MethodStatement(IdentifierNode* identifier, StatementList* nodes)
        : m_identifier(identifier)
        , m_nodes(nodes)
    {
    }
    
    virtual std::string ToString() const;
    
private:
    RefPtr<IdentifierNode> m_identifier;
    RefPtr<StatementList> m_nodes;
};

class VarDeclarationStatement: public StatementNode
{
public:
    VarDeclarationStatement(IdentifierNode* typeIdentifier, IdentifierNode* nameIdentifier, ArenaNode* initializer)
        : m_typeIdentifier(typeIdentifier)
        , m_nameIdentifier(nameIdentifier)
        , m_initializer(initializer)
    {
    }
    
    virtual std::string ToString() const;
    
private:
    RefPtr<IdentifierNode> m_typeIdentifier;
    RefPtr<IdentifierNode> m_nameIdentifier;
    RefPtr<ArenaNode> m_initializer;
};

class TemplateVarDeclarationStatement: public StatementNode
{
public:
    TemplateVarDeclarationStatement(IdentifierNode* typeIdentifier, IdentifierList* identifierList, IdentifierNode* nameIdentifier)
        : m_typeIdentifier(typeIdentifier)
        , m_identifierList(identifierList)
        , m_nameIdentifier(nameIdentifier)
    {
    }
    
    virtual std::string ToString() const;
    
private:
    RefPtr<IdentifierNode> m_typeIdentifier;
    RefPtr<IdentifierList> m_identifierList;
    RefPtr<IdentifierNode> m_nameIdentifier;
};


#endif // NODES_H
