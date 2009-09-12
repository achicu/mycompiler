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
class MethodEnv;

class Register : public RefCounted
{
public:
    Register(int number) 
        : m_number(number)
        , m_ignored(false)
        , m_type(0)
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
    
    virtual Register* EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst);
    virtual Register* EmitUnaryOpBytecode (BytecodeGenerator* generator, UnaryOpcode op, Register* reg1, Register* dst);
    virtual Register* EmitAssignOpBytecode (BytecodeGenerator* generator, AssignOpcode op, Accessor* accessor, Register* dst);

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
    bool CoerceArgsIfNeeded(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2);
};

class IntType: public BuiltinType
{
public:
    IntType()
        : BuiltinType("int")
    {
    }
    
    virtual int GetPriority() const { return 0; }
        
    virtual Register* EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst);
    virtual Register* EmitUnaryOpBytecode (BytecodeGenerator* generator, UnaryOpcode m_op, Register* reg1, Register* dst);
    virtual Register* EmitAssignOpBytecode (BytecodeGenerator* generator, AssignOpcode op, Accessor* accessor, Register* dst);
    
};

class FloatType: public BuiltinType
{
public:
    FloatType()
        : BuiltinType("number")
    {
    }
    
    virtual int GetPriority() const { return 1; }
    
    virtual Register* EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst);
    virtual Register* EmitUnaryOpBytecode (BytecodeGenerator* generator, UnaryOpcode m_op, Register* reg1, Register* dst);
    virtual Register* EmitAssignOpBytecode (BytecodeGenerator* generator, AssignOpcode op, Accessor* accessor, Register* dst);

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
    
    virtual Register* EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst);

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
    Type* GetType() const { return m_type.Ptr(); }
    
private:
    std::string m_name;
    RefPtr<Type> m_type;
    RefPtr<Register> m_register;
    
};

class Accessor: public RefCounted
{
public:
    Accessor(Type* type)
        : m_type(type)
    {
    }
    
    virtual Register* EmitLoad(BytecodeGenerator* generator, Register* dst) = 0;
    virtual Register* EmitSave(BytecodeGenerator* generator, Register* src, Register* dst) = 0;
    
    Type* GetType() const { return m_type.Ptr(); }

private:
    RefPtr<Type> m_type;
};

class LocalPropertyAccessor: public Accessor
{
public:
    LocalPropertyAccessor(Type* type, Register* reg)
        : Accessor(type)
        , m_register(reg)
    {
    }
    
    virtual Register* EmitLoad(BytecodeGenerator* generator, Register* dst);
    virtual Register* EmitSave(BytecodeGenerator* generator, Register* src, Register* dst);

private:
    RefPtr<Register> m_register;
};

class ParentPropertyAccessor: public Accessor
{
public:
    ParentPropertyAccessor(Type* type, int scopeNumber, int registerNumber)
        : Accessor(type)
        , m_scopeNumber(scopeNumber)
        , m_registerNumber(registerNumber)
    {
    }

    virtual Register* EmitLoad(BytecodeGenerator* generator, Register* dst);
    virtual Register* EmitSave(BytecodeGenerator* generator, Register* src, Register* dst);

private:
    int m_scopeNumber;
    int m_registerNumber;
};

class Scope: public RefCounted
{
    typedef std::map<std::string, RefPtr<Property> > PropertyMap;
public:
    Scope(Scope* parentScope)
        : m_parentScope(parentScope)
    {
    }
    
    PassRef<Accessor> GetProperty(std::string& name) const;
    bool HasLocalProperty(std::string& name) const;
    
    Property* PutProperty(std::string& name, Type* type)
    {
        RefPtr<Property> property(AdoptRef(new Property(name, type)));
        m_properties.insert(make_pair(name, property));
        return property.Ptr();
    }
    
private:
    Property* LookupProperty(std::string& name, int& scopeNumber) const;

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
    typedef std::map<std::string, RefPtr<MethodEnv> > MethodList;
public:
    GlobalData();
    
    Type* GetTypeOf(TypeNode* typeNode);
    MethodEnv* GetMethod(std::string name);
    
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
    MethodList m_methodList;
    
    std::vector<double> m_floatConstants;
    std::vector<std::string> m_stringConstants;

    RefPtr<IntType> m_intType;
    RefPtr<FloatType> m_floatType;
    RefPtr<StringType> m_stringType;
//    RefPtr<VectorType> m_vectorType;
    
};

class MethodEnv: public RefCounted
{
public:
    MethodEnv()
        : m_compiled(false)
        , m_registerCount(0)
    {
    }
    
    void Compiled(GlobalData* globalData, int registerCount, std::vector<Bytecode>& bytes)
    {
        m_compiled = true;
        m_globalData = globalData;
        m_registerCount = registerCount;
        m_bytes = bytes;
    }
    
    void Run();

private:
    bool m_compiled;
    RefPtr<GlobalData> m_globalData;
    int m_registerCount;
    std::vector<Bytecode> m_bytes;
};

class BytecodeGenerator
{
public:

    BytecodeGenerator(GlobalData* globalData, Scope* parentScope, MethodNode* method);
    BytecodeGenerator(GlobalData* globalData, StatementList* statements);

    void CleanupRegisters();

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
    
    PassRef<Accessor> GetProperty(std::string& name);
    
    void CoerceInPlace(Register* reg, Type* otherType);
    
    int GetMaxRegisterCount() const { return m_maxRegisterCount; }
    
    void EmitIncRef(Register* reg);
    void EmitDecRef(Register* reg);
    
    int GetLabel();
    void PatchConstantInt(int label, int value);
    
    PassRef<MethodEnv> GetMethodEnv();

private:  
    void DeclareArguments(MethodNode* method);
    void DeclareProperty(std::string& name, Type* type);
    
    RefPtr<GlobalData> m_globalData;
    RefPtr<Scope> m_localScope;
    RefPtr<StatementList> m_statements;
    RefPtr<MethodEnv> m_methodEnv;
    
    std::vector<Bytecode> m_bytes;
    std::vector<RefPtr<Register> > m_registers;
    int m_calleeRegisters;
    int m_maxRegisterCount;
};

#endif // BYTECODEGENERATOR_H