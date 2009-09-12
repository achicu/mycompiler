/*
 *  BytecodeGenerator.cpp
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/11/09.
 *
 */

#include "BytecodeGenerator.h"
#include "Disassembler.h"
#include "Interpreter.h"
#include "OpCodes.h"

Property* Scope::GetProperty(BytecodeGenerator* generator, std::string& name) const
{
    PropertyMap::const_iterator iter = m_properties.find(name);
    if (iter != m_properties.end())
        return (*iter).second.Ptr();
    
    if (generator && m_parentScope.Ptr())
    {
        return m_parentScope->GetProperty(generator, name);
    }

    return 0;
}

bool BuiltinType::CoerceArgsIfNeeded(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2)
{
    if (type2 != this)
    {
        // have to convert me or the other to my type
        if (!type2->IsBuiltin())
        {
            printf("Error: trying to do %s %s %s\n", Name().c_str(), BinaryOpcodeToString(op), type2->Name().c_str());
            exit(1);
        }
        
        if (static_cast<BuiltinType*>(type2)->GetPriority() > GetPriority())
        {
            // coerce me to the other
            generator->CoerceInPlace(reg1, type2);
            return false;
        }
        else
        {
            // coerce the other to me
            generator->CoerceInPlace(reg2, this);
        }
    }
    
    return true;
}

Register* Type::EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst)
{
    // arbitrary type operators not supported, yet
    printf("Error: trying to do %s %s %s\n", Name().c_str(), BinaryOpcodeToString(op), type2->Name().c_str());
    exit(1);
}

Register* Type::EmitUnaryOpBytecode(BytecodeGenerator* generator, UnaryOpcode op, Register* reg1, Register* dst)
{
    // arbitrary type operators not supported, yet
    printf("Error: trying to do %s %s\n", UnaryOpcodeToString(op), Name().c_str());
    exit(1);
}


Register* IntType::EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst)
{
    if (!CoerceArgsIfNeeded(generator, type2, op, reg1, reg2))
    {
        // reverse if cannot do it
        return type2->EmitBinaryOpBytecode(generator, type2, op, reg1, reg2, dst);
    }
    
    assert(reg1->GetType() == reg2->GetType());
    dst->SetType(this);
    
    switch(op)
    {
        case binary_op_plus:
            generator->EmitBytecode(op_int_plus);
        break;
        case binary_op_multiply:
            generator->EmitBytecode(op_int_multiply);
        break;
        case binary_op_minus:
            generator->EmitBytecode(op_int_minus);
        break;
        case binary_op_divide:
            generator->EmitBytecode(op_int_divide);
        break;
        case binary_op_less:
            generator->EmitBytecode(op_int_less);
        break;
        case binary_op_less_or_equal:
            generator->EmitBytecode(op_int_less_or_equal);
        break;
        case binary_op_more:
            generator->EmitBytecode(op_int_more);
        break;
        case binary_op_more_or_equal:
            generator->EmitBytecode(op_int_more_or_equal);
        break;
        case binary_op_equal:
            generator->EmitBytecode(op_int_equals);
        break;
        default:
            printf("%s operation not supported on ints\n", BinaryOpcodeToString(op));
            exit(1);
    }
    
    generator->EmitRegister(dst);
    generator->EmitRegister(reg1);
    generator->EmitRegister(reg2);
    
    return dst;
}

Register* IntType::EmitUnaryOpBytecode(BytecodeGenerator* generator, UnaryOpcode op, Register* reg1, Register* dst)
{
    dst->SetType(this);
    
    switch(op)
    {
        case unary_op_not:
            generator->EmitBytecode(op_int_not);
            generator->EmitRegister(dst);
            generator->EmitRegister(reg1);
    
        break;
        case unary_op_plusplus_prefix:
            generator->EmitBytecode(op_int_plus_one);
            generator->EmitRegister(reg1);
            dst = reg1;
        break;
        case unary_op_plusplus_sufix:
        {
            // let the user get the old value
            RefPtr<Register> r (generator->NewTempRegister());
            r->SetType(this);
            
            generator->EmitBytecode(op_assign);
            generator->EmitRegister(r.Ptr());
            generator->EmitRegister(reg1);
            dst = r.Ptr();
            
            generator->EmitBytecode(op_int_plus_one);
            generator->EmitRegister(reg1);
        }
        break;
        case unary_op_minusminus_prefix:
            generator->EmitBytecode(op_int_minus_one);
            generator->EmitRegister(reg1);
            dst = reg1;
        break;
        case unary_op_minusminus_sufix:
        {
            // let the user get the old value
            RefPtr<Register> r( generator->NewTempRegister() );
            r->SetType(this);
            
            generator->EmitBytecode(op_assign);
            generator->EmitRegister(r.Ptr());
            generator->EmitRegister(reg1);
            dst = r.Ptr();
            
            generator->EmitBytecode(op_int_minus_one);
            generator->EmitRegister(reg1);
        }
        break;
        default:
            printf(" unary %s operation not supported on ints\n", UnaryOpcodeToString(op));
            exit(1);
    }
    
    return dst;
}


Register* FloatType::EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst)
{
    if (!CoerceArgsIfNeeded(generator, type2, op, reg1, reg2))
    {
        // reverse if cannot do it
        return type2->EmitBinaryOpBytecode(generator, this, op, reg1, reg2, dst);
    }
    
    assert(reg1->GetType() == reg2->GetType());
    dst->SetType(this);
    
    switch(op)
    {
        case binary_op_plus:
            generator->EmitBytecode(op_float_plus);
        break;
        case binary_op_multiply:
            generator->EmitBytecode(op_float_multiply);
        break;
        case binary_op_minus:
            generator->EmitBytecode(op_float_minus);
        break;
        case binary_op_divide:
            generator->EmitBytecode(op_float_divide);
        break;
        case binary_op_less:
            generator->EmitBytecode(op_float_less);
            dst->SetType(generator->GetGlobalData()->GetIntType());
        break;
        case binary_op_less_or_equal:
            generator->EmitBytecode(op_float_less_or_equal);
            dst->SetType(generator->GetGlobalData()->GetIntType());
        break;
        case binary_op_more:
            generator->EmitBytecode(op_float_more);
            dst->SetType(generator->GetGlobalData()->GetIntType());
        break;
        case binary_op_more_or_equal:
            generator->EmitBytecode(op_float_more_or_equal);
            dst->SetType(generator->GetGlobalData()->GetIntType());
        break;
        case binary_op_equal:
            generator->EmitBytecode(op_float_equals);
            dst->SetType(generator->GetGlobalData()->GetIntType());
        break;
        default:
            printf("%s operation not supported on floats\n", BinaryOpcodeToString(op));
            exit(1);
    }
    
    generator->EmitRegister(dst);
    generator->EmitRegister(reg1);
    generator->EmitRegister(reg2);
    
    return dst;
}

Register* FloatType::EmitUnaryOpBytecode(BytecodeGenerator* generator, UnaryOpcode op, Register* reg1, Register* dst)
{
    dst->SetType(this);
    
    switch(op)
    {
        case unary_op_not:
            generator->EmitBytecode(op_float_not);
            generator->EmitRegister(dst);
            generator->EmitRegister(reg1);
    
        break;
        case unary_op_plusplus_prefix:
            generator->EmitBytecode(op_float_plus_one);
            generator->EmitRegister(reg1);
            dst = reg1;
        break;
        case unary_op_plusplus_sufix:
        {
            // let the user get the old value
            RefPtr<Register> r (generator->NewTempRegister());
            r->SetType(this);
            
            generator->EmitBytecode(op_assign);
            generator->EmitRegister(r.Ptr());
            generator->EmitRegister(reg1);
            dst = r.Ptr();
            
            generator->EmitBytecode(op_float_plus_one);
            generator->EmitRegister(reg1);
        }
        break;
        case unary_op_minusminus_prefix:
            generator->EmitBytecode(op_float_minus_one);
            generator->EmitRegister(reg1);
            dst = reg1;
        break;
        case unary_op_minusminus_sufix:
        {
            // let the user get the old value
            RefPtr<Register> r( generator->NewTempRegister() );
            r->SetType(this);
            
            generator->EmitBytecode(op_assign);
            generator->EmitRegister(r.Ptr());
            generator->EmitRegister(reg1);
            dst = r.Ptr();
            
            generator->EmitBytecode(op_float_minus_one);
            generator->EmitRegister(reg1);
        }
        break;
        default:
            printf(" unary %s operation not supported on floats\n", UnaryOpcodeToString(op));
            exit(1);
    }
    
    return dst;
}

Register* StringType::EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst)
{
    if (!CoerceArgsIfNeeded(generator, type2, op, reg1, reg2))
    {
        // reverse if cannot do it
        return type2->EmitBinaryOpBytecode(generator, this, op, reg1, reg2, dst);
    }
    
    assert(reg1->GetType() == reg2->GetType());
    dst->SetType(this);
    
    switch(op)
    {
        case binary_op_plus:
            generator->EmitBytecode(op_string_plus);
        break;
        default:
            printf("%s operation not supported on strings\n", BinaryOpcodeToString(op));
            exit(1);
    }
    
    generator->EmitRegister(dst);
    generator->EmitRegister(reg1);
    generator->EmitRegister(reg2);
    
    return dst;
}

GlobalData::GlobalData()
    : m_intType(AdoptRef(new IntType()))
    , m_floatType(AdoptRef(new FloatType()))
    , m_stringType(AdoptRef(new StringType()))
//    , m_vectorType(AdoptRef(new VectorType()))
{
    m_typeList[m_intType->Name()] = m_intType.Ptr();
    m_typeList[m_floatType->Name()] = m_floatType.Ptr();
    m_typeList[m_stringType->Name()] = m_stringType.Ptr();
//    m_typeList["vector"] = m_vectorType;
}

Type* GlobalData::GetTypeOf(TypeNode* typeNode)
{
    std::string completeName = typeNode->CompleteTypeName();
    TypeList::const_iterator iter = m_typeList.find( completeName );
    if (iter != m_typeList.end())
    {
        return (*iter).second.Ptr();
    }
    
    RefPtr<Type> type (AdoptRef( new Type(completeName) ));
    
    TypeNodeList* typeNodeList = typeNode->GetTypeNodeList();
    if (typeNodeList)
    {
        for (int i=0; i<typeNodeList->size(); ++i)
            type->AddTemplateType( GetTypeOf (typeNodeList->at(i).Ptr()) );
    }
    
    m_typeList[completeName] = type;
    
    return type.Ptr();
}

int GlobalData::GetConstantFloatIndex(double d)
{
    for (int i=0; i<m_floatConstants.size(); ++i)
        if (m_floatConstants[i] == d)
            return i;

    m_floatConstants.push_back(d);
    return m_floatConstants.size() - 1;
}

int GlobalData::GetConstantStringIndex(std::string d)
{
    for (int i=0; i<m_stringConstants.size(); ++i)
        if (m_stringConstants[i] == d)
            return i;

    m_stringConstants.push_back(d);
    return m_stringConstants.size() - 1;
}

double GlobalData::GetConstantFloat(int i)
{
    assert(i < m_floatConstants.size());
    return m_floatConstants.at(i);
}

std::string GlobalData::GetConstantString(int i)
{
    assert(i < m_stringConstants.size());
    return m_stringConstants.at(i);
}

BytecodeGenerator::BytecodeGenerator(GlobalData* globalData, Scope* parentScope, MethodNode* method)
    : m_globalData(globalData)
    , m_statements(method->GetStatementList())
    , m_maxRegisterCount(0)
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
    , m_maxRegisterCount(0)
{
    m_localScope.AdoptRef(new Scope(0));
    m_calleeRegisters = 0;
    
    // declare methods
    // declare structs
    // declare variables
    
    for (int i=0; i<statements->size(); ++i)
    {
        StatementNode* statement = statements->at(i).Ptr();
        if (statement == 0)
            continue;
        
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

void BytecodeGenerator::CleanupRegisters()
{
    while( m_registers.size() > m_calleeRegisters && m_registers.back()->HasOneRef() && m_registers.back()->IsIgnored())
    {
        Register* reg = m_registers.back().Ptr();
        if (reg->GetType() && reg->GetType()->IsRefCounted())
            EmitDecRef(reg);
        
        m_registers.pop_back();
    }
}

PassRef<Register> BytecodeGenerator::NewRegister()
{
    CleanupRegisters();

    RefPtr<Register> reg(AdoptRef(new Register(m_registers.size())));
    m_registers.push_back( reg );
    
    m_maxRegisterCount = (m_maxRegisterCount > m_registers.size()) ? m_maxRegisterCount : m_registers.size();
    
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

Property* BytecodeGenerator::GetProperty(std::string& name)
{
    return m_localScope->GetProperty(this, name);
}

void BytecodeGenerator::DeclareProperty(std::string& name, Type* type)
{
    if (m_localScope->GetProperty(0, name))
    {
        printf("property name redclartion %s\n", name.c_str());
        exit(1);
    }
    
    Property* property = m_localScope->PutProperty(name, type);
    RefPtr<Register> reg = NewRegister();
    reg->SetType(type);
    property->SetRegister(reg.Ptr());
    
    if (type->IsRefCounted())
    {
        EmitBytecode(op_init_ref);
        EmitRegister(reg.Ptr());
    }
    
    printf("Variable %s has register %d\n", name.c_str(), property->GetRegister()->Number());
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
            if (statement == 0)
                continue;
            
            EmitNode(statement);
        }
    }
    
    for( int i=0; i<m_registers.size(); ++i )
    {
        Register* reg = m_registers.at(i).Ptr();
        if (reg->GetType() && reg->GetType()->IsRefCounted())
            EmitDecRef(reg);
    }

    
    Disassemble(m_globalData.Ptr(), &m_bytes);
    Interpret(m_globalData.Ptr(), m_maxRegisterCount, &m_bytes);
}

void BytecodeGenerator::EmitBytecode(int bytecode)
{
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

void BytecodeGenerator::CoerceInPlace(Register* reg, Type* otherType)
{
    bool converted = false;
    Type* type = reg->GetType();
    assert(type != otherType);
    
    if (m_globalData->GetIntType() == type)
    {
        if (m_globalData->GetFloatType() == otherType)
        {
            EmitBytecode(op_coerce_int_float);
            EmitRegister(reg);
            converted = true;
        }
        else if (m_globalData->GetStringType() == otherType)
        {
            EmitBytecode(op_coerce_int_string);
            EmitRegister(reg);
            converted = true;
        }
    }
    else if (m_globalData->GetFloatType() == type)
    {
        if (m_globalData->GetIntType() == otherType)
        {
            EmitBytecode(op_coerce_float_int);
            EmitRegister(reg);
            converted = true;
        }
        else if (m_globalData->GetStringType() == otherType)
        {
            EmitBytecode(op_coerce_float_string);
            EmitRegister(reg);
            converted = true;
        }
    }
    else if (m_globalData->GetStringType() == type)
    {
        if (m_globalData->GetIntType() == otherType)
        {
            EmitBytecode(op_coerce_string_int);
            EmitRegister(reg);
            converted = true;
        }
        else if (m_globalData->GetFloatType() == otherType)
        {
            EmitBytecode(op_coerce_string_float);
            EmitRegister(reg);
            converted = true;
        }
    }
    
    if (converted)
    {
        reg->SetType(otherType);
    }
    else
    {
        printf("Error: unable to coerce %s to %s\n", type->Name().c_str(), otherType->Name().c_str());
        exit(1);
    }
}

void BytecodeGenerator::EmitIncRef(Register* reg)
{
    assert(reg->GetType()->IsRefCounted());
    EmitBytecode(op_inc_ref);
    EmitRegister(reg);
}

void BytecodeGenerator::EmitDecRef(Register* reg)
{
    assert(reg->GetType()->IsRefCounted());
    EmitBytecode(op_dec_ref);
    EmitRegister(reg);
}

int BytecodeGenerator::GetLabel()
{
    return m_bytes.size();
}

void BytecodeGenerator::PatchConstantInt(int label, int value)
{
    assert(label < m_bytes.size());
    m_bytes.at(label).ConstantInt = value;
}
