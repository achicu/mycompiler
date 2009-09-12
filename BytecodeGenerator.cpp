/*
 *  BytecodeGenerator.cpp
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/11/09.
 *
 */

#include "BytecodeGenerator.h"
#include "Disassembler.h"

BytecodeGenerator::BytecodeGenerator(GlobalData* globalData, Scope* parentScope, MethodNode* method)
    : m_globalData(globalData)
    , m_statements(method->GetStatementList())
{
    assert(method);
    
    m_localScope.AdoptRef(new Scope(parentScope));
    
    DeclareArguments(method);
    m_calleeRegisters = m_registers.size();
    
    // declare variables;
    StatementList* statements = m_statements.Ptr();
    if (statements)
    {
        for (int i=0; i<statements->size(); ++i)
        {
            StatementNode* statement = statements->at(i).Ptr();
            
            // methods cannot contain methods or structs
            assert (!statement->IsMethodNode());
            assert (!statement->IsStructNode());
            
            if (statement->IsVarStatement())
            {
                VarStatement* varStatement = static_cast<VarStatement*>(statement);
                printf( " var : %s\n", varStatement->ToString().c_str());
            }
        }
    }
}

BytecodeGenerator::BytecodeGenerator(GlobalData* globalData, StatementList* statements)
    : m_globalData(globalData)
    , m_statements(statements)
{
    m_localScope.AdoptRef(new Scope(0));
    m_calleeRegisters = 0;
    
    // declare methods
    // declare structs
    // declare variables
    
    for (int i=0; i<statements->size(); ++i)
    {
        StatementNode* statement = statements->at(i).Ptr();
        if (statement->IsMethodNode())
        {
            MethodNode* methodNode = static_cast<MethodNode*>(statement);
            (void) methodNode;
            // printf( " method : %s\n", methodNode->ToString().c_str());
        }
        else if (statement->IsStructNode())
        {
            StructNode* structNode = static_cast<StructNode*>(statement);
            (void) structNode;
            // printf( " struct : %s\n", structNode->ToString().c_str());
        }
        else if (statement->IsVarStatement())
        {
            VarStatement* varStatement = static_cast<VarStatement*>(statement);
            DeclareProperty(varStatement->Identifier()->Value(), m_globalData->GetTypeOf(varStatement->GetTypeNode()));
        }
    }
}

PassRef<Register> BytecodeGenerator::NewTempRegister()
{
    PassRef<Register> r = NewRegister();
    r->SetIgnored();
    return r;
}

PassRef<Register> BytecodeGenerator::NewRegister()
{
    while( m_registers.size() > m_calleeRegisters && m_registers.back()->HasOneRef() && m_registers.back()->IsIgnored())
        m_registers.pop_back();

    RefPtr<Register> reg(AdoptRef(new Register(m_registers.size())));
    m_registers.push_back( reg );
    
    return reg.ReleaseRef();
}

void BytecodeGenerator::DeclareArguments(MethodNode* method)
{
    TypeNode* returnType = method->GetReturnType();
    if (returnType)
    {
        static std::string returnValue("$ReturnValue");
        DeclareProperty( returnValue, m_globalData->GetTypeOf(returnType) );
    }

    ArgumentNodeList* arguments = method->GetArgumentNodeList();
    if (arguments)
    {
        for (int i=0; i<arguments->size(); ++i)
        {
            ArgumentNode* argument = arguments->at(i).Ptr();
            DeclareProperty( argument->Identifier()->Value(), m_globalData->GetTypeOf(argument->Type()) );
        }
    }
}

void BytecodeGenerator::DeclareProperty(std::string& name, Type* type)
{
    if (m_localScope->GetProperty(name))
    {
        printf("property name redclartion %s\n", name.c_str());
        return;
    }
    
    Property* property = m_localScope->PutProperty(name, type);
    property->SetRegister(NewRegister());
}

Register* BytecodeGenerator::EmitNode(ArenaNode* node, Register* dst)
{
    return node->EmitBytecode(this, dst);
}

Register* BytecodeGenerator::EmitNode(ArenaNode* node)
{
    Register* result = EmitNode(node, 0);
    if (result)
    {
        result->SetIgnored();
    }
    return result;
}

void BytecodeGenerator::Generate()
{
    StatementList* statements = m_statements.Ptr();
    if (statements)
    {
        for (int i=0; i<statements->size(); ++i)
        {
            StatementNode* statement = statements->at(i).Ptr();
            EmitNode(statement);
        }
    }
    
    Disassemble(&m_bytes);
}

void BytecodeGenerator::EmitBytecode(int bytecode)
{
    printf("Emitting bytecode %d\n", bytecode);
    Bytecode b;
    b.Code = bytecode;
    m_bytes.push_back(b);
}

void BytecodeGenerator::EmitRegister(Register* r)
{
    assert(r);
    Bytecode b;
    b.RegisterNumber = r->Number();
    m_bytes.push_back(b);
}

void BytecodeGenerator::EmitConstantFloat(double value)
{
    Bytecode b;
    b.ConstantFloatIndex = m_globalData->GetConstantFloatIndex(value);
    m_bytes.push_back(b);
}

void BytecodeGenerator::EmitConstantInt(int value)
{
    Bytecode b;
    b.ConstantInt =  value;
    m_bytes.push_back(b);
}

void BytecodeGenerator::EmitConstantString(std::string value)
{
    Bytecode b;
    b.ConstantStringIndex = m_globalData->GetConstantStringIndex(value);
    m_bytes.push_back(b);
}

