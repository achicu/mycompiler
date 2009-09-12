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

class BytecodeGenerator;
class Type;

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
    
    Type* GetType() const { return m_type.Ptr(); }
    void SetType(Type* type) { m_type = type; }

private:
    int m_number;
    bool m_temporary;
    bool m_ignored;
    RefPtr<Type> m_type;
};

class Type: public RefCounted
{
public:
    Type(std::string name)
        : m_name(name)
    {
    }
    
    std::string Name() const { return m_name; }
    
    void AddTemplateType(Type* type)
    {
        m_templateTypes.push_back(type);
    }
    
    virtual bool IsBuiltin() const { return false; }
    virtual bool IsRefCounted() const { return false; }
    
    virtual Register* EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, char m_op, Register* reg1, Register* reg2, Register* dst);

private:
    std::string m_name;
    std::vector<RefPtr<Type> > m_templateTypes;
};

class BuiltinType: public Type
{
public:
    BuiltinType(std::string name)
        : Type(name)
    {
    }
    
    virtual int GetPriority() const { return 0; }
    
    virtual bool IsBuiltin() const { return true; }

protected:
    bool CoerceArgsIfNeeded(BytecodeGenerator* generator, Type* type2, char op, Register* reg1, Register* reg2);
};

class IntType: public BuiltinType
{
public:
    IntType()
        : BuiltinType("int")
    {
    }
    
    virtual int GetPriority() const { return 0; }
        
    virtual Register* EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, char op, Register* reg1, Register* reg2, Register* dst);
};

class FloatType: public BuiltinType
{
public:
    FloatType()
        : BuiltinType("number")
    {
    }
    
    virtual int GetPriority() const { return 1; }
    
    virtual Register* EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, char op, Register* reg1, Register* reg2, Register* dst);
};

class StringType: public BuiltinType
{
public:
    StringType()
        : BuiltinType("string")
    {
    }
    
    virtual bool IsRefCounted() const { return true; }
    virtual int GetPriority() const { return 2; }
    
    virtual Register* EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, char op, Register* reg1, Register* reg2, Register* dst);
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
    void SetRegister(Register* reg) { m_register = reg; }
    
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
    
    Property* GetProperty(BytecodeGenerator* generator, std::string& name) const;
    
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
    GlobalData();
    
    Type* GetTypeOf(TypeNode* typeNode);
    int GetConstantFloatIndex(double d);
    int GetConstantStringIndex(std::string d);
    
    Type* GetIntType() const { return m_intType.Ptr(); }
    Type* GetFloatType() const { return m_floatType.Ptr(); }
    Type* GetStringType() const { return m_stringType.Ptr(); }
//    Type* GetVectorType() const { return m_vectorType.Ptr(); }

    double GetConstantFloat(int i);
    std::string GetConstantString(int i);
    
private:
    TypeList m_typeList;
    std::vector<double> m_floatConstants;
    std::vector<std::string> m_stringConstants;

    RefPtr<IntType> m_intType;
    RefPtr<FloatType> m_floatType;
    RefPtr<StringType> m_stringType;
//    RefPtr<VectorType> m_vectorType;
    
};

class BytecodeGenerator
{
public:

    BytecodeGenerator(GlobalData* globalData, Scope* parentScope, MethodNode* method);
    BytecodeGenerator(GlobalData* globalData, StatementList* statements);

    PassRef<Register> NewTempRegister();    
    PassRef<Register> NewRegister();    
    
    
    Register* EmitNode(ArenaNode* node, Register* destination);
    Register* EmitNode(ArenaNode* node);
    
    void Generate();

    GlobalData* GetGlobalData() const { return m_globalData.Ptr(); }

    void EmitBytecode(int bytecode);
    void EmitRegister(Register*);
    void EmitConstantFloat(double value);
    void EmitConstantInt(int value);
    void EmitConstantString(std::string value);
    
    Property* GetProperty(std::string& name);
    
    void CoerceInPlace(Register* reg, Type* otherType);
    
    int GetMaxRegisterCount() const { return m_maxRegisterCount; }
    
    void EmitIncRef(Register* reg);
    void EmitDecRef(Register* reg);
    
    int GetLabel();
    void PatchConstantInt(int label, int value);

private:  
    void DeclareArguments(MethodNode* method);
    void DeclareProperty(std::string& name, Type* type);
    
    RefPtr<GlobalData> m_globalData;
    RefPtr<Scope> m_localScope;
    RefPtr<StatementList> m_statements;
    
    std::vector<Bytecode> m_bytes;
    std::vector<RefPtr<Register> > m_registers;
    int m_calleeRegisters;
    int m_maxRegisterCount;
};

#endif // BYTECODEGENERATOR_H