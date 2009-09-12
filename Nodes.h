/*
 *  Nodes.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/10/09.
 *
 */

#ifndef NODES_H
#define NODES_H

#include "ParserArena.h"
#include <string>
#include <vector>
#include <sstream>

class BytecodeGenerator;

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
class TypeNode;
class ArgumentNode;

typedef ArenaList<RefPtr<ArenaNode> > NodeList;
typedef ArenaList<RefPtr<StatementNode> > StatementList;
typedef ArenaList<RefPtr<IdentifierNode> > IdentifierList;
typedef ArenaList<RefPtr<TypeNode> > TypeNodeList;
typedef ArenaList<RefPtr<ArgumentNode> > ArgumentNodeList;

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
    
    virtual Register* EmitBytecode(BytecodeGenerator* generator, Register* dst);
    
private:
    double m_value;

};

class IntegerValueNode: public ArenaNode
{
public:
    IntegerValueNode(int value);
    
    virtual std::string ToString() const;
    
    virtual Register* EmitBytecode(BytecodeGenerator* generator, Register* dst);

private:
    int m_value;
};

class StringValueNode: public ArenaNode
{
public:
    StringValueNode(char* name);
    
    virtual std::string ToString() const;
    
    virtual Register* EmitBytecode(BytecodeGenerator* generator, Register* dst);
    
private:
    std::string m_value;
};

class IdentifierNode: public ArenaNode
{
public:
    IdentifierNode(char* name);
    
    virtual std::string ToString() const;
    
    std::string& Value() { return m_value; }
    
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
    
    virtual bool IsStructNode() const { return false; }
    virtual bool IsMethodNode() const { return false; }
    virtual bool IsVarStatement() const { return false; }
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
    
    virtual Register* EmitBytecode(BytecodeGenerator* generator, Register* dst);

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

class TypeNode: public ArenaNode
{
public:
    TypeNode(IdentifierNode* typeIdentifier, TypeNodeList* typeNodeList)
        : m_typeIdentifier(typeIdentifier)
        , m_typeNodeList(typeNodeList)
     {
     }
     
     virtual std::string ToString() const;
     
     std::string CompleteTypeName() const;
     
     TypeNodeList* GetTypeNodeList() const { return m_typeNodeList.Ptr(); }
     
private:
    RefPtr<IdentifierNode> m_typeIdentifier;
    RefPtr<TypeNodeList> m_typeNodeList;
};

class ArgumentNode: public ArenaNode
{
public:
    ArgumentNode(TypeNode* typeNode, IdentifierNode* identifier)
        : m_typeNode(typeNode)
        , m_identifier(identifier)
    {
    }
    
    virtual std::string ToString() const;
    
    IdentifierNode* Identifier(){ return m_identifier.Ptr(); }
    TypeNode* Type(){ return m_typeNode.Ptr(); }
    
    
private:
    RefPtr<TypeNode> m_typeNode;
    RefPtr<IdentifierNode> m_identifier;
};

class MethodNode: public StatementNode
{
public:
    MethodNode(TypeNode* typeNode, IdentifierNode* identifier, ArgumentNodeList* argumentsTypeList, StatementList* nodes)
        : m_typeNode(typeNode)
        , m_identifier(identifier)
        , m_argumentsTypeList(argumentsTypeList)
        , m_nodes(nodes)
    {
    }
    
    virtual bool IsMethodNode() const { return true; }
    
    virtual std::string ToString() const;
    
    StatementList* GetStatementList() const { return m_nodes.Ptr(); }
    ArgumentNodeList* GetArgumentNodeList() const { return m_argumentsTypeList.Ptr(); }
    TypeNode* GetReturnType() const { return m_typeNode.Ptr(); }
    
private:
    RefPtr<TypeNode> m_typeNode;
    RefPtr<IdentifierNode> m_identifier;
    RefPtr<ArgumentNodeList> m_argumentsTypeList;
    RefPtr<StatementList> m_nodes;
};

class StructNode: public StatementNode
{
public:
    StructNode(IdentifierNode* identifier, TypeNode* extends, StatementList* nodes)
        : m_identifier(identifier)
        , m_extends(extends)
        , m_nodes(nodes)
    {
    }
    
    virtual bool IsStructNode() const { return true; }
    
    virtual std::string ToString() const;
    
private:
    RefPtr<IdentifierNode> m_identifier;
    RefPtr<TypeNode> m_extends;
    RefPtr<StatementList> m_nodes;
};

class VarStatement: public StatementNode
{
public:
    VarStatement(TypeNode* typeNode, IdentifierNode* nameIdentifier, ArenaNode* initializer)
        : m_typeNode(typeNode)
        , m_nameIdentifier(nameIdentifier)
        , m_initializer(initializer)
    {
    }
    
    virtual bool IsVarStatement() const { return true; }
    
    virtual std::string ToString() const;
    
    TypeNode* GetTypeNode() const { return m_typeNode.Ptr(); }
    IdentifierNode* Identifier() const { return m_nameIdentifier.Ptr(); }
    
private:
    RefPtr<TypeNode> m_typeNode;
    RefPtr<IdentifierNode> m_nameIdentifier;
    RefPtr<ArenaNode> m_initializer;
};

class ExpressionStatement: public StatementNode
{
public:
    ExpressionStatement(ArenaNode* expression)
        : m_expression(expression)
    {
    }
    
    virtual std::string ToString() const;
    
    virtual Register* EmitBytecode(BytecodeGenerator* generator, Register* dst);
    
private:
    RefPtr<ArenaNode> m_expression;
};


#endif // NODES_H
