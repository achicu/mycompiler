/*
 *  BytecodeGenerator.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/11/09.
 *
 */

#ifndef BYTECODEGENERATOR_H
#define BYTECODEGENERATOR_H

#include <string>
#include <map>
#include <vector>
#include "RefCounted.h"
#include "RefPtr.h"
#include "Nodes.h"

class Register : public RefCounted
{
public:
    Register(int number) 
        : m_number(number)
        , m_ignored(false)
    {
    }
   
    int Number() const { return m_number; }
    
    void SetIgnored() { m_ignored = true; }
    bool IsIgnored() const { return m_ignored; }

private:
    int m_number;
    bool m_temporary;
    bool m_ignored;
};

enum BuiltinTypes
{
    NUMBER,
    STRING,
    CODE
};

class Type: public RefCounted
{
public:
    Type(std::string& name)
        : m_name(name)
    {
    }
    
    std::string Name() const { return m_name; }
    
    void AddTemplateType(Type* type)
    {
        m_templateTypes.push_back(type);
    }

private:
    bool isBuiltin;
    std::string m_name;
    std::vector<RefPtr<Type> > m_templateTypes;
};

class Property: public RefCounted
{
public:
    Property(std::string& name, Type* type)
        : m_name(name)
        , m_type(type)
    {
    }
    
    Register* GetRegister() const { return m_register.Ptr(); }
    void SetRegister(PassRef<Register> reg) { m_register = reg; }
    
    std::string Name() const { return m_name; }
private:
    std::string m_name;
    RefPtr<Type> m_type;
    RefPtr<Register> m_register;
    
};

class Scope: public RefCounted
{
    typedef std::map<std::string, RefPtr<Property> > PropertyMap;
public:
    Scope(Scope* parentScope)
        : m_parentScope(parentScope)
    {
    }
    
    Property* GetProperty(std::string& name) const
    {
        PropertyMap::const_iterator iter = m_properties.find(name);
        if (iter != m_properties.end())
            return (*iter).second.Ptr();
        
        if (m_parentScope.Ptr())
        {
            return m_parentScope->GetProperty(name);
        }

        return 0;
    }
    
    Property* PutProperty(std::string& name, Type* type)
    {
        RefPtr<Property> property(AdoptRef(new Property(name, type)));
        m_properties.insert(make_pair(name, property));
        return property.Ptr();
    }
    
private:
    RefPtr<Scope> m_parentScope;
    PropertyMap m_properties;
};

union Bytecode
{
    int Code;
    int RegisterNumber;
    int ConstantFloatIndex;
    int ConstantInt;
    int ConstantStringIndex;
};

class GlobalData: public RefCounted
{
    typedef std::map<std::string, RefPtr<Type> > TypeList;
public:

    Type* GetTypeOf(TypeNode* typeNode)
    {
        std::string completeName = typeNode->CompleteTypeName();
        TypeList::const_iterator iter = m_typeList.find( completeName );
        if (iter != m_typeList.end())
        {
            return (*iter).second.Ptr();
        }
        
        PassRef<Type> type (AdoptRef( new Type(completeName) ));
        
        TypeNodeList* typeNodeList = typeNode->GetTypeNodeList();
        if (typeNodeList)
        {
            for (int i=0; i<typeNodeList->size(); ++i)
                type->AddTemplateType( GetTypeOf (typeNodeList->at(i).Ptr()) );
        }
        
        m_typeList[completeName] = type;
        
        return type.Ptr();
    }
    
    int GetConstantFloatIndex(double d)
    {
        for (int i=0; i<m_floatConstants.size(); ++i)
            if (m_floatConstants[i] == d)
                return i;

        m_floatConstants.push_back(d);
        return m_floatConstants.size() - 1;
    }
    
    int GetConstantStringIndex(std::string d)
    {
        for (int i=0; i<m_stringConstants.size(); ++i)
            if (m_stringConstants[i] == d)
                return i;

        m_stringConstants.push_back(d);
        return m_stringConstants.size() - 1;
    }
    
private:
    TypeList m_typeList;
    std::vector<double> m_floatConstants;
    std::vector<std::string> m_stringConstants;
    
};

class BytecodeGenerator
{
public:

    BytecodeGenerator(GlobalData* globalData, Scope* parentScope, MethodNode* method);
    BytecodeGenerator(GlobalData* globalData, StatementList* statements);

    PassRef<Register> NewTempRegister();    
    PassRef<Register> NewRegister();    
    
    void DeclareArguments(MethodNode* method);
    
    Register* EmitNode(ArenaNode* node, Register* destination);
    Register* EmitNode(ArenaNode* node);
    
    void Generate();

    GlobalData* GetGlobalData() const { return m_globalData.Ptr(); }

    void EmitBytecode(int bytecode);
    void EmitRegister(Register*);
    void EmitConstantFloat(double value);
    void EmitConstantInt(int value);
    void EmitConstantString(std::string value);

private:    
    void DeclareProperty(std::string& name, Type* type);
    
    RefPtr<GlobalData> m_globalData;
    RefPtr<Scope> m_localScope;
    RefPtr<StatementList> m_statements;
    
    std::vector<Bytecode> m_bytes;
    std::vector<RefPtr<Register> > m_registers;
    int m_calleeRegisters;
};

#endif // BYTECODEGENERATOR_H