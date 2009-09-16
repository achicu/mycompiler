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
#include <set>
#include <vector>
#include "RefCounted.h"
#include "RefPtr.h"
#include "Nodes.h"
#include "RegisterFile.h"
#include "Collector.h"
#include "Interpreter.h"
#include "OpCodes.h"

class BytecodeGenerator;
class Type;
class MethodEnv;

typedef std::vector<RefPtr<Type> > TypeList;

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
    typedef std::set<Type*> TypeSet;
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
    virtual bool IsCollectorRef() const { return false; }
    virtual bool IsObjectType() const { return false; }
    virtual bool IsVectorRef() const { return false; }
        
    virtual int GetObjectSize() const { return 0; }
    
    virtual Register* EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst);
    virtual Register* EmitUnaryOpBytecode (BytecodeGenerator* generator, UnaryOpcode op, Register* reg1, Register* dst);
    virtual Register* EmitAssignOpBytecode (BytecodeGenerator* generator, AssignOpcode op, Accessor* accessor, ArenaNode* node2, Register* dst);
    
    TypeList* GetTemplateTypes() { return &m_templateTypes;}


    void AddInheritedType(Type* type) { m_inheritance.insert(type); }
    bool InheritsFrom(Type* type) const {
        if (type == this)
            return true;
        
        TypeSet::const_iterator iter = m_inheritance.find(type);
        return iter != m_inheritance.end();
    }
    
private:
    std::string m_name;
    TypeList m_templateTypes;
    TypeSet m_inheritance;
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
    bool CoerceArgsIfNeeded(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* &reg1, Register* &reg2);
};

class IntType: public BuiltinType
{
public:
    IntType()
        : BuiltinType("int")
    {
    }
    
    virtual int GetPriority() const { return 0; }
    
    virtual int GetObjectSize() const { return sizeof(int); }
        
    virtual Register* EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst);
    virtual Register* EmitUnaryOpBytecode (BytecodeGenerator* generator, UnaryOpcode m_op, Register* reg1, Register* dst);
    virtual Register* EmitAssignOpBytecode (BytecodeGenerator* generator, AssignOpcode op, Accessor* accessor, ArenaNode* node2, Register* dst);
    
};

class FloatType: public BuiltinType
{
public:
    FloatType()
        : BuiltinType("number")
    {
    }
    
    virtual int GetPriority() const { return 1; }
    
    virtual int GetObjectSize() const { return sizeof(double); }
    
    virtual Register* EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst);
    virtual Register* EmitUnaryOpBytecode (BytecodeGenerator* generator, UnaryOpcode m_op, Register* reg1, Register* dst);
    virtual Register* EmitAssignOpBytecode (BytecodeGenerator* generator, AssignOpcode op, Accessor* accessor, ArenaNode* node2, Register* dst);

};

class CollectorRefType: public BuiltinType
{
public:
    CollectorRefType(std::string type)
        : BuiltinType(type)
    {
    }
    
    virtual bool IsCollectorRef() const { return true; }
    virtual int GetObjectSize() const { return sizeof(CollectorRef*); }
    
    virtual Register* EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst);
};

class StringType: public CollectorRefType
{
public:
    StringType()
        : CollectorRefType("string")
    {
    }
    
    virtual int GetPriority() const { return 2; }
    
    virtual Register* EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst);

};

class NullType: public CollectorRefType
{
public:
    NullType()
        : CollectorRefType("null")
    {
    }
    
};

class ObjectProperty
{
public:
    ObjectProperty(std::string& name, Type* type, int offset)
        : m_name(name)
        , m_type(type)
        , m_offset(offset)
    {
    }
    
    int GetOffset() const { return m_offset; }

    std::string Name() const { return m_name; }
    Type* GetType() const { return m_type.Ptr(); }
    
private:
    std::string m_name;
    RefPtr<Type> m_type;
    int m_offset;
};

class ObjectPropertyAccessor: public Accessor
{
public:
    ObjectPropertyAccessor(Type* type, int offset, Register* forReg)
        : Accessor(type)
        , m_register(forReg)
        , m_offset(offset)
    {
    }
    
    virtual Register* EmitLoad(BytecodeGenerator* generator, Register* dst);
    virtual Register* EmitSave(BytecodeGenerator* generator, Register* src, Register* dst);

private:
    RefPtr<Register> m_register;
    int m_offset;
};

class ObjectType: public CollectorRefType
{
    typedef std::map<std::string, ObjectProperty> PropertyMap;
public:
    
    ObjectType(std::string name, ObjectType* extendedType)
        : CollectorRefType(name)
        , m_extendedType(extendedType)
        , m_nextOffset(extendedType ? extendedType->m_nextOffset : 0)
    {
    }
    
    int GetNextOffset(int size);
    
    virtual bool IsObjectType() const { return true; }
    
    void PutProperty(GlobalData* globalData, std::string& name, Type* type);
    bool HasProperty(std::string& name);
    PassRef<Accessor> GetPropertyAccessor(std::string& name, Register* forReg);
    
    int ObjectSize();
    void MarkObject(RefObject* ref);
    
    void DebugObject(GlobalData* globalData, RefObject* ref);
    
    ObjectType* GetExtendedObjectType() const { return m_extendedType.Ptr(); }

private:
    RefPtr<ObjectType> m_extendedType;
    PropertyMap m_properties;
    int m_nextOffset;
};

class CodeType: public CollectorRefType
{
public:
    
    CodeType()
        : CollectorRefType("code")
    {
    }
    
    virtual Register* EmitAssignOpBytecode (BytecodeGenerator* generator, AssignOpcode op, Accessor* accessor, ArenaNode* node2, Register* dst);
};

class VectorAccessor: public Accessor
{
public:
    VectorAccessor(Type* type, Register* offsetRegister, Register* forReg)
        : Accessor(type)
        , m_register(forReg)
        , m_offsetRegister(offsetRegister)
    {
    }
    
    virtual Register* EmitLoad(BytecodeGenerator* generator, Register* dst);
    virtual Register* EmitSave(BytecodeGenerator* generator, Register* src, Register* dst);

private:
    RefPtr<Register> m_register;
    RefPtr<Register> m_offsetRegister;
};

class VectorType: public CollectorRefType
{
public:
    VectorType(std::string name)
        : CollectorRefType(name)
        , m_elementSize(0)
    {
    }
    
    virtual bool IsVectorRef() const { return true; }
    
    PassRef<Accessor> GetRegisterAccessor(Register* offsetRegister, Register* forReg);
    
    void MarkObject(RefVector* ref);
    
    void DebugObject(GlobalData* globalData, RefVector* ref);
    
    int GetElementSize();
    Type* GetElementType();

private:
    RefPtr<Type> m_elementType;
    int m_elementSize;
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
    
    int Count() const { return m_properties.size(); }
    
private:
    Property* LookupProperty(std::string& name, int& scopeNumber) const;

    RefPtr<Scope> m_parentScope;
    PropertyMap m_properties;
};

union Bytecode
{
    OpCode Code;
    unsigned RegisterNumber;
    unsigned ConstantFloatIndex;
    unsigned ConstantInt;
    unsigned ConstantStringIndex;
};

class GlobalData: public RefCounted
{
    typedef std::map<std::string, RefPtr<Type> > TypeList;
    typedef std::map<std::string, RefPtr<MethodEnv> > MethodList;
public:
    GlobalData();
    virtual ~GlobalData();
    
    Type* GetDefinedType(std::string completeName);
    MethodEnv* GetDefinedMethod(std::string completeName);
    Type* GetTypeOf(TypeNode* typeNode);
    MethodEnv* GetMethod(std::string name, MethodNode* methodNode = 0);
    
    unsigned GetConstantFloatIndex(double d);
    unsigned GetConstantStringIndex(std::string d);
    
    Type* GetIntType() const { return m_intType.Ptr(); }
    Type* GetFloatType() const { return m_floatType.Ptr(); }
    Type* GetStringType() const { return m_stringType.Ptr(); }
    Type* GetNullType() const { return m_nullType.Ptr(); }
    Type* GetCodeType() const { return m_codeType.Ptr(); }

    double GetConstantFloat(unsigned i);
    std::string GetConstantString(unsigned i);
    
    RegisterFile* GetRegisterFile() { return &m_registerFile; }
    
    void DefineObjectType(StructNode* structNode);
    
    int GetNextMethodId() { return ++m_nextMethodId; }
    
private:
    TypeList m_typeList;
    MethodList m_methodList;
    
    std::vector<double> m_floatConstants;
    std::vector<std::string> m_stringConstants;

    RefPtr<IntType> m_intType;
    RefPtr<FloatType> m_floatType;
    RefPtr<StringType> m_stringType;
    RefPtr<NullType> m_nullType;
    RefPtr<CodeType> m_codeType;

    RegisterFile m_registerFile;
    RefPtr<Heap> m_heap;
    int m_nextMethodId;
    
};

class MethodEnv: public RefCounted
{
public:
    MethodEnv(GlobalData* globalData, std::string name)
        : m_name (name)
        , m_globalData(globalData)
        , m_refCode(0)
        , m_registerCount(0)
        , m_compiled(false)
    {
    }
    
    void Compiled(int registerCount, std::vector<Bytecode>& bytes)
    {
        m_compiled = true;
        m_registerCount = registerCount;
        m_bytes = bytes;
    }
    
    void Run(RegisterValue* startingRegister);
    
    Type* GetReturnType() const { return m_returnType.Ptr(); }
    const TypeList* GetArgumentsType() const { return &m_argumentsType; }
    
    void PrependArgumentsFromMethodNode(MethodNode* method);
    
    GlobalData* GetGlobalData() const { return m_globalData; }

    std::string GetName() const { return m_name; }
    
    std::string GetSourceCode() const { return  m_next.Ptr() ? m_sourceCode + m_next->GetSourceCode() : m_sourceCode ; }
    void SetSourceCode(std::string sourceCode) { m_sourceCode = sourceCode; }
    
    void SetLocalScope(Scope* scope) { m_localScope = scope; }
    Scope* GetLocalScope() { return m_localScope.Ptr(); }
    
    MethodEnv* GetNext() const { return m_next.Ptr(); }
    MethodEnv* GetLast() { return m_next.Ptr() != 0 ? m_next->GetLast() : this; }
    void SetNext(MethodEnv* next) { m_next = next; }
    
    CollectorRef* GetRefCode() const { return m_refCode; }
    void SetRefCode(CollectorRef* refCode) { m_refCode = refCode; }
    
    void Debug() const;

private:
    std::string m_name;
    std::string m_sourceCode;
    std::vector<Bytecode> m_bytes;
    GlobalData* m_globalData; // globaldata keeps a reference to this object (circular references)
    RefPtr<Type> m_returnType;
    RefPtr<Scope> m_localScope;
    RefPtr<MethodEnv> m_next;
    CollectorRef* m_refCode;
    TypeList m_argumentsType;
    int m_registerCount;
    bool m_compiled;
};

class BreakOrContinueHelper;

class BytecodeGenerator
{
    friend class BreakOrContinueHelper;
public:

    BytecodeGenerator(GlobalData* globalData, Scope* parentScope, MethodNode* method);
    BytecodeGenerator(GlobalData* globalData, StatementList* statements, std::string methodName = "$main");
    BytecodeGenerator(MethodEnv* methodEnv, StatementList* statements, Scope* siblingScope);

    void CleanupRegisters();

    PassRef<Register> NewTempRegister();    
    PassRef<Register> NewRegister();    
    
    
    Register* EmitNode(ArenaNode* node, Register* destination);
    Register* EmitNode(ArenaNode* node);
    
    MethodEnv* Generate();
    void FinishMethod();

    GlobalData* GetGlobalData() const { return m_globalData.Ptr(); }

    void EmitBytecode(OpCode bytecode);
    void EmitRegister(Register*);
    void EmitConstantFloat(double value);
    void EmitConstantInt(int value);
    void EmitConstantString(std::string value);
    
    PassRef<Accessor> GetProperty(std::string& name, bool onlyLocal = false);
    
    Register* Coerce(Register* reg, Type* otherType);
    
    int GetMaxRegisterCount() const { return m_maxRegisterCount; }
    
    unsigned GetLabel();
    void PatchConstantInt(unsigned label, int value);
    
    void EmitBreak();
    void EmitContinue();
    
private:  
    void DeclareArguments(MethodNode* method);
    void DeclareProperty(std::string& name, Type* type, bool isArgument = false);
    
    RefPtr<GlobalData> m_globalData;
    RefPtr<Scope> m_localScope;
    RefPtr<StatementList> m_statements;
    RefPtr<MethodEnv> m_methodEnv;
    
    std::vector<Bytecode> m_bytes;
    std::vector<RefPtr<Register> > m_registers;
    unsigned m_calleeRegisters;
    unsigned m_maxRegisterCount;
    
    BreakOrContinueHelper* m_breakOrContinueHelper;
};

class BreakOrContinueHelper
{
public:
    BreakOrContinueHelper(BytecodeGenerator* generator)
        : m_generator(generator)
    {
        m_savedBreakOrContinueHelper = generator->m_breakOrContinueHelper;
        generator->m_breakOrContinueHelper = this;
    }
    
    ~BreakOrContinueHelper()
    {
        Patch();
        m_generator->m_breakOrContinueHelper = m_savedBreakOrContinueHelper;
    }
    
    void SetContinueLabel(int label) { m_continueLabel = label; }
    void SetBreakLabel(int label) { m_breakLabel = label; }
    
    void PushPatchContinueLabel(int label)
    {
        m_continuePatches.push_back(label);
    }
    
    void PushPatchBreakLabel(int label)
    {
        m_breakPatches.push_back(label);
    }

private:
    void Patch()
    {
        for (unsigned i=0; i<m_continuePatches.size(); i++)
            m_generator->PatchConstantInt(m_continuePatches.at(i), m_continueLabel);
        
        for (unsigned i=0; i<m_breakPatches.size(); i++)
            m_generator->PatchConstantInt(m_breakPatches.at(i), m_breakLabel);
    }
    
    std::vector<int> m_continuePatches;
    std::vector<int> m_breakPatches;
    
    BytecodeGenerator* m_generator;
    
    BreakOrContinueHelper* m_savedBreakOrContinueHelper;
    int m_continueLabel;
    int m_breakLabel;
};

#endif // BYTECODEGENERATOR_H