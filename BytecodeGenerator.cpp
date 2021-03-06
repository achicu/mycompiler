/*
 *  BytecodeGenerator.cpp
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/11/09.
 *
 */
#include "Platform.h"

#include "BytecodeGenerator.h"
#include "Disassembler.h"
#include "Interpreter.h"
#include "OpCodes.h"

Register* LocalPropertyAccessor::EmitLoad(BytecodeGenerator* generator, Register* dst)
{
    return m_register.Ptr();
}

Register* LocalPropertyAccessor::EmitSave(BytecodeGenerator* generator, Register* src, Register* dst)
{
    if (src != m_register.Ptr())
    {
        generator->EmitBytecode(op_assign);
        generator->EmitRegister(m_register.Ptr());
        generator->EmitRegister(src);
    }
    return m_register.Ptr();
}

Register* ParentPropertyAccessor::EmitLoad(BytecodeGenerator* generator, Register* dst)
{
    if (!dst) dst = generator->NewTempRegister().Ptr();
    
    generator->EmitBytecode(op_load_scope);
    generator->EmitRegister(dst);
    generator->EmitConstantInt(m_scopeNumber);
    generator->EmitConstantInt(m_registerNumber);
    
    dst->SetType(GetType());
    
    return dst;
}

Register* ParentPropertyAccessor::EmitSave(BytecodeGenerator* generator, Register* src, Register* dst)
{
    if (src->GetType() != GetType())
    {
        printf("invalid type\n");
        exit(1);
    }

    generator->EmitBytecode(op_save_scope);
    generator->EmitRegister(src);
    generator->EmitConstantInt(m_scopeNumber);
    generator->EmitConstantInt(m_registerNumber);
    
    return src;
}

bool Scope::HasLocalProperty(std::string& name) const
{
    PropertyMap::const_iterator iter = m_properties.find(name);
    return (iter != m_properties.end());
}

PassRef<Accessor> Scope::GetProperty(std::string& name) const
{
    PropertyMap::const_iterator iter = m_properties.find(name);
    if (iter != m_properties.end())
        return AdoptRef<Accessor>(new LocalPropertyAccessor((*iter).second->GetType(), (*iter).second->GetRegister()));
    
    if (m_parentScope.Ptr())
    {
        // need to get the property and set it into an accessor
        int scopeNumber = 1;
        Property* property = m_parentScope->LookupProperty(name, scopeNumber);
        if (property)
            return AdoptRef<Accessor>(new ParentPropertyAccessor( property->GetType(), scopeNumber, property->GetRegister()->Number() ));
    }

    return 0;
}

Property* Scope::LookupProperty(std::string& name, int& scopeNumber) const
{
    PropertyMap::const_iterator iter = m_properties.find(name);
    if (iter != m_properties.end())
        return (*iter).second.Ptr();
    
    if (m_parentScope.Ptr())
    {
        ++scopeNumber;
        
        return m_parentScope->LookupProperty(name, scopeNumber);
    }
    
    return 0;
}

bool BuiltinType::CoerceArgsIfNeeded(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* &reg1, Register* &reg2)
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
            reg1 = generator->Coerce(reg1, type2);
            return false;
        }
        else
        {
            // coerce the other to me
            reg2 = generator->Coerce(reg2, this);
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

Register* Type::EmitAssignOpBytecode (BytecodeGenerator* generator, AssignOpcode op, Accessor* accessor, ArenaNode* node2, Register* dst)
{
    // arbitrary type operators not supported, yet
    printf("Error: trying to do %s %s\n", AssignOpcodeToString(op), Name().c_str());
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

Register* IntType::EmitAssignOpBytecode (BytecodeGenerator* generator, AssignOpcode op, Accessor* accessor, ArenaNode* node2, Register* dst)
{
    dst->SetType(this);
    
    RefPtr<Register> source (accessor->EmitLoad(generator, 0));
    RefPtr<Register> newValue;
    
    switch(op)
    {
        case assign_op_plusplus_prefix:
            generator->EmitBytecode(op_int_plus_one);
            generator->EmitRegister(source.Ptr());
            dst = source.Ptr();
        break;
        case assign_op_plusplus_sufix:
        {
            // let the user get the old value
            RefPtr<Register> r (generator->NewTempRegister());
            r->SetType(this);
            
            generator->EmitBytecode(op_assign);
            generator->EmitRegister(r.Ptr());
            generator->EmitRegister(source.Ptr());
            dst = r.Ptr();
            
            generator->EmitBytecode(op_int_plus_one);
            generator->EmitRegister(source.Ptr());
        }
        break;
        case assign_op_minusminus_prefix:
            generator->EmitBytecode(op_int_minus_one);
            generator->EmitRegister(source.Ptr());
            dst = source.Ptr();
        break;
        case assign_op_minusminus_sufix:
        {
            // let the user get the old value
            RefPtr<Register> r( generator->NewTempRegister() );
            r->SetType(this);
            
            generator->EmitBytecode(op_assign);
            generator->EmitRegister(r.Ptr());
            generator->EmitRegister(source.Ptr());
            dst = r.Ptr();
            
            generator->EmitBytecode(op_int_minus_one);
            generator->EmitRegister(source.Ptr());
        }
        break;
        default:
            printf(" assign %s operation not supported on ints\n", AssignOpcodeToString(op));
            exit(1);
    }
    
    accessor->EmitSave(generator, source.Ptr(), 0)->SetIgnored();
    
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
        return type2->EmitBinaryOpBytecode(generator, type2, op, reg1, reg2, dst);
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

Register* FloatType::EmitAssignOpBytecode (BytecodeGenerator* generator, AssignOpcode op, Accessor* accessor, ArenaNode* node2, Register* dst)
{
    dst->SetType(this);
    
    RefPtr<Register> source (accessor->EmitLoad(generator, 0));
    RefPtr<Register> newValue;
    
    switch(op)
    {
        case assign_op_plusplus_prefix:
            generator->EmitBytecode(op_float_plus_one);
            generator->EmitRegister(source.Ptr());
            dst = source.Ptr();
        break;
        case assign_op_plusplus_sufix:
        {
            // let the user get the old value
            RefPtr<Register> r (generator->NewTempRegister());
            r->SetType(this);
            
            generator->EmitBytecode(op_assign);
            generator->EmitRegister(r.Ptr());
            generator->EmitRegister(source.Ptr());
            dst = r.Ptr();
            
            generator->EmitBytecode(op_float_plus_one);
            generator->EmitRegister(source.Ptr());
        }
        break;
        case assign_op_minusminus_prefix:
            generator->EmitBytecode(op_float_minus_one);
            generator->EmitRegister(source.Ptr());
            dst = source.Ptr();
        break;
        case assign_op_minusminus_sufix:
        {
            // let the user get the old value
            RefPtr<Register> r( generator->NewTempRegister() );
            r->SetType(this);
            
            generator->EmitBytecode(op_assign);
            generator->EmitRegister(r.Ptr());
            generator->EmitRegister(source.Ptr());
            dst = r.Ptr();
            
            generator->EmitBytecode(op_float_minus_one);
            generator->EmitRegister(source.Ptr());
        }
        break;
        default:
            printf(" assign %s operation not supported on ints\n", AssignOpcodeToString(op));
            exit(1);
    }
    
    accessor->EmitSave(generator, source.Ptr(), 0)->SetIgnored();
    
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
        default:
            printf(" unary %s operation not supported on floats\n", UnaryOpcodeToString(op));
            exit(1);
    }
    
    return dst;
}

Register* CollectorRefType::EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst)
{
    if (type2->IsCollectorRef() && op == binary_op_equal)
    {
        dst->SetType(generator->GetGlobalData()->GetIntType());
        
        generator->EmitBytecode(op_ref_equal);
        generator->EmitRegister(dst);
        generator->EmitRegister(reg1);
        generator->EmitRegister(reg2);

        return dst;
    }
    
    return BuiltinType::EmitBinaryOpBytecode(generator, type2, op, reg1, reg2, dst); 
}

Register* StringType::EmitBinaryOpBytecode(BytecodeGenerator* generator, Type* type2, BinaryOpcode op, Register* reg1, Register* reg2, Register* dst)
{
    if (type2->IsCollectorRef() && type2 != this)
        return CollectorRefType::EmitBinaryOpBytecode(generator, type2, op, reg1, reg2, dst);
    
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
        case binary_op_equal:
            generator->EmitBytecode(op_string_equal);
            dst->SetType(generator->GetGlobalData()->GetIntType());
            break;
        default:
            return CollectorRefType::EmitBinaryOpBytecode(generator, type2, op, reg1, reg2, dst);
    }
    
    generator->EmitRegister(dst);
    generator->EmitRegister(reg1);
    generator->EmitRegister(reg2);
    
    return dst;
}

Register* ObjectPropertyAccessor::EmitLoad(BytecodeGenerator* generator, Register* dst)
{
    if (!dst) dst = generator->NewTempRegister().Ptr();
    
    if (GetType()->IsCollectorRef())
        generator->EmitBytecode(op_load_ref_object_property);
    else if (GetType() == generator->GetGlobalData()->GetIntType())
        generator->EmitBytecode(op_load_int_object_property);
    else if (GetType() == generator->GetGlobalData()->GetFloatType())
        generator->EmitBytecode(op_load_float_object_property);
            
    generator->EmitRegister(dst);
    generator->EmitRegister(m_register.Ptr());
    generator->EmitConstantInt(m_offset);
    
    dst->SetType(GetType());
    
    return dst;
}

Register* ObjectPropertyAccessor::EmitSave(BytecodeGenerator* generator, Register* src, Register* dst)
{
    if (src->GetType() != GetType())
    {
        printf("invalid type\n");
        exit(1);
    }
    
    if (GetType()->IsCollectorRef())
        generator->EmitBytecode(op_save_ref_object_property);
    else if (GetType() == generator->GetGlobalData()->GetIntType())
        generator->EmitBytecode(op_save_int_object_property);
    else if (GetType() == generator->GetGlobalData()->GetFloatType())
        generator->EmitBytecode(op_save_float_object_property);
        
    generator->EmitRegister(m_register.Ptr());
    generator->EmitRegister(src);
    generator->EmitConstantInt(m_offset);
    
    return src;
}

int ObjectType::ObjectSize()
{
    return m_nextOffset; 
}

void ObjectType::DebugObject(GlobalData* globalData, RefObject* ref)
{
    std::ostringstream o;
    if (!ref)
    {
        o << "null";
    }
    else
    {
        o << "[ " << (intptr_t) ref << "\n";
        
        ObjectType* type = this;
        while (type)
        {
            o << type->Name() << ":\n";
            
            PropertyMap::iterator iter = type->m_properties.begin();
            for(; iter != type->m_properties.end(); ++iter)
            {
                o << "\t";
                ObjectProperty& objectProperty = (*iter).second;
                Type* const propertyType = objectProperty.GetType();
                
                o << '[' << propertyType->Name() << " - " << objectProperty.Name() << "] ";
                
                if (propertyType->IsCollectorRef())
                {
                    CollectorRef* collectorRef = ref->ReadAtOffset<CollectorRef*>(objectProperty.GetOffset());
                    if (propertyType == globalData->GetStringType())
                    {
                        if (collectorRef)
                        {
                            RefString* refString = static_cast<RefString*>(collectorRef);
                            o << refString->Value;
                        }
                    }
                    else
                    {
                        o << (intptr_t)collectorRef;
                    }
                }
                else if (propertyType == globalData->GetIntType())
                {
                    int value = ref->ReadAtOffset<int>(objectProperty.GetOffset());
                    o << value;
                }
                else if (propertyType == globalData->GetFloatType())
                {
                    double value = ref->ReadAtOffset<double>(objectProperty.GetOffset());
                    o << value;
                }
                else
                {
                    assert(false);
                }
                
                o << "\n";
            }
            
            type = type->m_extendedType.Ptr();
        }
    }
    o << "]";
    printf("%s\n", o.str().c_str());
}

void ObjectType::MarkObject(RefObject* ref)
{
    ObjectType* type = this;
    while (type)
    {
        PropertyMap::iterator iter = m_properties.begin();
        for(; iter != m_properties.end(); ++iter)
        {
            ObjectProperty& objectProperty = (*iter).second;
            if (objectProperty.GetType()->IsCollectorRef())
            {
                CollectorRef* collectorRef = ref->ReadAtOffset<CollectorRef*>(objectProperty.GetOffset());
                if (collectorRef && !Heap::IsCellMarked(collectorRef))
                {
                    collectorRef->Mark();
                }
            }
        }

        type = type->m_extendedType.Ptr();
    }
}

int ObjectType::GetNextOffset(int size)
{
    // align the new one to the size of the object
    int returnOffset = m_nextOffset + size - m_nextOffset % size;
    m_nextOffset = returnOffset + size;
    return returnOffset;
}

bool ObjectType::HasProperty(std::string& name)
{
    PropertyMap::const_iterator iter = m_properties.find(name);
    if (iter != m_properties.end())
        return true;
    
    if (m_extendedType.Ptr())
        return m_extendedType->HasProperty(name);
    
    return false;
}

void ObjectType::PutProperty(GlobalData* globalData, std::string& name, Type* type)
{
    if (HasProperty(name))
    {
        printf("property name already added %s\n", name.c_str());
        exit(1);
    }
    
    int offset = GetNextOffset(type->GetObjectSize());
    
    ObjectProperty property( name, type, offset );
    m_properties.insert(make_pair(name, property));
}

PassRef<Accessor> ObjectType::GetPropertyAccessor(std::string& name, Register* forReg)
{
    PropertyMap::const_iterator iter = m_properties.find(name);
    if (iter != m_properties.end())
        return (AdoptRef<Accessor>(new ObjectPropertyAccessor( (*iter).second.GetType(), (*iter).second.GetOffset(), forReg )));

    if (m_extendedType.Ptr())
        return m_extendedType->GetPropertyAccessor(name, forReg);
    
    return false;
}

Register* VectorAccessor::EmitLoad(BytecodeGenerator* generator, Register* dst)
{
    if (!dst) dst = generator->NewTempRegister().Ptr();
    
    if (GetType()->IsCollectorRef())
        generator->EmitBytecode(op_load_ref_vector_property);
    else if (GetType() == generator->GetGlobalData()->GetIntType())
        generator->EmitBytecode(op_load_int_vector_property);
    else if (GetType() == generator->GetGlobalData()->GetFloatType())
        generator->EmitBytecode(op_load_float_vector_property);
    
    generator->EmitRegister(dst);
    generator->EmitRegister(m_register.Ptr());
    generator->EmitRegister(m_offsetRegister.Ptr());
    
    dst->SetType(GetType());
    
    return dst;
}

Register* VectorAccessor::EmitSave(BytecodeGenerator* generator, Register* src, Register* dst)
{
    if (src->GetType() != GetType())
    {
        printf("invalid type\n");
        exit(1);
    }
    
    if (GetType()->IsCollectorRef())
        generator->EmitBytecode(op_save_ref_vector_property);
    else if (GetType() == generator->GetGlobalData()->GetIntType())
        generator->EmitBytecode(op_save_int_vector_property);
    else if (GetType() == generator->GetGlobalData()->GetFloatType())
        generator->EmitBytecode(op_save_float_vector_property);
        
    generator->EmitRegister(m_register.Ptr());
    generator->EmitRegister(src);
    generator->EmitRegister(m_offsetRegister.Ptr());
    
    return src;
}

PassRef<Accessor> VectorType::GetRegisterAccessor(Register* offsetRegister, Register* forReg)
{
    return AdoptRef<Accessor>(new VectorAccessor(GetElementType(), offsetRegister, forReg));
}

void VectorType::MarkObject(RefVector* ref)
{
    assert(ref);
    
    if (GetElementType()->IsCollectorRef())
    {
        CollectorRef** iter = reinterpret_cast<CollectorRef**>(ref->GetBuffer());
        CollectorRef** last = iter + ref->GetSize();
        while (iter < last)
        {
            CollectorRef* const collectorRef = *iter;
            if (collectorRef && !Heap::IsCellMarked(collectorRef))
            {
                collectorRef->Mark();
            }
            ++ iter;
        }
    }
}

void VectorType::DebugObject(GlobalData* globalData, RefVector* ref)
{
    std::ostringstream o;
    if (!ref)
    {
        o << "null";
    }
    else
    {
        o << "[ " << (intptr_t) ref << "\n" << Name() << ":\n";
    
        Type* elementType = GetElementType();
        if (elementType->IsCollectorRef())
        {
            CollectorRef** iter = reinterpret_cast<CollectorRef**>(ref->GetBuffer());
            CollectorRef** last = iter + ref->GetSize();
            int index = 0;
            while (iter < last)
            {
                CollectorRef* const collectorRef = *iter;
                o << "\t[" << index++ << "] ";

                if (elementType == globalData->GetStringType())
                {
                    if (collectorRef)
                    {
                        RefString* refString = static_cast<RefString*>(collectorRef);
                        o << refString->Value;
                    }
                }
                else
                {
                    o << (intptr_t) collectorRef;
                }
                
                o << "\n";
                ++ iter;
            }
        }
        else if (elementType == globalData->GetIntType())
        {
            int* iter = reinterpret_cast<int*>(ref->GetBuffer());
            int* last = iter + ref->GetSize();
            int index = 0;
            while (iter < last)
            {
                o << "\t[" << index++ << "] " << *iter << "\n";
                ++ iter;
            }
        }
        else if (elementType == globalData->GetFloatType())
        {
            double* iter = reinterpret_cast<double*>(ref->GetBuffer());
            double* last = iter + ref->GetSize();
            int index = 0;
            while (iter < last)
            {
                o << "\t[" << index++ << "] " << *iter << "\n";
                ++ iter;
            }
        }
        else
        {
            assert(false);
        }
        
        o << "]";
        printf("%s\n", o.str().c_str());
    }
}

Type* VectorType::GetElementType()
{
    if (m_elementType.Ptr())
        return m_elementType.Ptr();
    
    TypeList* templateTypes = GetTemplateTypes();
    
    if (templateTypes->size() != 1)
    {
        printf("only 1 template argument is accepted by vector type\n");
        exit(1);
    }
    
    Type* objectType = templateTypes->at(0).Ptr();
    
    if (!objectType)
    {
        printf("invalid vector template type\n");
        exit(1);
    }
    
    m_elementType = objectType;
    
    return objectType;
}

int VectorType::GetElementSize()
{
    if (m_elementSize != 0)
        return m_elementSize;
    
    m_elementSize = GetElementType()->GetObjectSize();
    
    return m_elementSize;
}

Register* CodeType::EmitAssignOpBytecode (BytecodeGenerator* generator, AssignOpcode op, Accessor* accessor, ArenaNode* node2, Register* dst)
{
    dst->SetType(this);
    
    RefPtr<Register> source (accessor->EmitLoad(generator, 0));
    RefPtr<Register> newValue;
    
    switch(op)
    {
        case assign_op_plus_equal:
        {
            RefPtr<Register> tempString (generator->NewTempRegister());
            tempString = node2->EmitBytecode(generator, tempString.Ptr());
            
            generator->EmitBytecode(op_code_plus_string);
            generator->EmitRegister(source.Ptr());
            generator->EmitRegister(tempString.Ptr());
            
            dst = source.Ptr();
        }
        break;

        default:
            printf(" assign %s operation not supported on strings\n", AssignOpcodeToString(op));
            exit(1);
    }
    
    accessor->EmitSave(generator, source.Ptr(), 0);
    
    return dst;
}

GlobalData::GlobalData()
    : m_intType(AdoptRef(new IntType()))
    , m_floatType(AdoptRef(new FloatType()))
    , m_stringType(AdoptRef(new StringType()))
    , m_nullType(AdoptRef(new NullType()))
    , m_codeType(AdoptRef(new CodeType()))
    , m_nextMethodId(0)
{
    m_typeList[m_intType->Name()] = m_intType.Ptr();
    m_typeList[m_floatType->Name()] = m_floatType.Ptr();
    m_typeList[m_stringType->Name()] = m_stringType.Ptr();
    m_typeList[m_codeType->Name()] = m_codeType.Ptr();
    
    m_stringType->AddInheritedType(m_nullType.Ptr());
    m_codeType->AddInheritedType(m_nullType.Ptr());
    
    m_heap.AdoptRef(new Heap(this));
}

GlobalData::~GlobalData()
{
}

Type* GlobalData::GetDefinedType(std::string completeName)
{
    TypeList::const_iterator iter = m_typeList.find( completeName );
    if (iter != m_typeList.end())
    {
        return (*iter).second.Ptr();
    }
    
    return 0;
}

MethodEnv* GlobalData::GetDefinedMethod(std::string completeName)
{
    MethodList::const_iterator iter = m_methodList.find( completeName );
    if (iter != m_methodList.end())
    {
        return (*iter).second.Ptr();
    }
    
    return 0;
}

Type* GlobalData::GetTypeOf(TypeNode* typeNode)
{
    std::string completeName = typeNode->CompleteTypeName();
    TypeList::const_iterator iter = m_typeList.find( completeName );
    if (iter != m_typeList.end())
    {
        return (*iter).second.Ptr();
    }
    
    if (typeNode->GetName() != "vector")
    {
        printf("invalid typename \"%s\"\n", typeNode->GetName().c_str());
        exit(1);
    }
    
    RefPtr<Type> type (AdoptRef<Type>( new VectorType(completeName) ));
    
    type->AddInheritedType(m_nullType.Ptr());
    
    TypeNodeList* typeNodeList = typeNode->GetTypeNodeList();
    if (typeNodeList)
    {
        for (unsigned i=0; i<typeNodeList->size(); ++i)
            type->AddTemplateType( GetTypeOf (typeNodeList->at(i).Ptr()) );
    }
    
    m_typeList[completeName] = type;
    
    return type.Ptr();
}

void GlobalData::DefineObjectType(StructNode* structNode)
{
    std::string completeName = structNode->GetIdentifier()->Value();
    TypeList::const_iterator iter = m_typeList.find( completeName );
    if (iter != m_typeList.end())
    {
        printf("%s struct already defined\n", completeName.c_str());
        exit(1);
    }
    
    TypeNode* extendedTypeNode = structNode->GetExtendedType();
    Type* extendedType = extendedTypeNode ? GetTypeOf(extendedTypeNode) : 0;
    if (extendedType && !extendedType->IsObjectType())
    {
        printf("%s can only extend other structures\n", completeName.c_str());
        exit(1);
    }
    
    ObjectType* extendedObjectType = extendedType ? static_cast<ObjectType*>(extendedType) : 0;
    
    RefPtr<ObjectType> newType (AdoptRef(new ObjectType (completeName, extendedObjectType)));
    m_typeList[completeName] = newType.Ptr();
    
    newType->AddInheritedType(m_nullType.Ptr());
    
    while (extendedObjectType)
    {
        newType->AddInheritedType(extendedObjectType);
        extendedObjectType = extendedObjectType->GetExtendedObjectType();
    }
        
    StatementList* statementsList = structNode->GetDeclarations();
    if (statementsList)
    {
        for (unsigned i=0; i<statementsList->size(); ++i)
        {
            StatementNode* statement = statementsList->at(i).Ptr();
            if (!statement)
                continue; // pass over empty lines
                
            if (statement->IsVarStatement())
            {
                VarStatement* varStatement = static_cast<VarStatement*>(statement);
                newType->PutProperty(this, varStatement->Identifier()->Value(), GetTypeOf(varStatement->GetTypeNode()));
            }
        }
    }
}

MethodEnv* GlobalData::GetMethod(std::string name, MethodNode* methodNode)
{
    MethodList::const_iterator iter = m_methodList.find( name );
    if (iter != m_methodList.end())
    {
        return (*iter).second.Ptr();
    }
    
    RefPtr<MethodEnv> method (AdoptRef( new MethodEnv(this, name) ));
    if (methodNode)
    {
        method->PrependArgumentsFromMethodNode(methodNode);
    }
    
    m_methodList[name] = method;
    
    return method.Ptr();
}

unsigned GlobalData::GetConstantFloatIndex(double d)
{
    for (unsigned i=0; i<m_floatConstants.size(); ++i)
        if (m_floatConstants[i] == d)
            return i;

    m_floatConstants.push_back(d);
    return m_floatConstants.size() - 1;
}

unsigned GlobalData::GetConstantStringIndex(std::string d)
{
    for (unsigned i=0; i<m_stringConstants.size(); ++i)
        if (m_stringConstants[i] == d)
            return i;

    m_stringConstants.push_back(d);
    return m_stringConstants.size() - 1;
}

double GlobalData::GetConstantFloat(unsigned i)
{
    assert(i < m_floatConstants.size());
    return m_floatConstants.at(i);
}

std::string GlobalData::GetConstantString(unsigned i)
{
    assert(i < m_stringConstants.size());
    return m_stringConstants.at(i);
}

BytecodeGenerator::BytecodeGenerator(GlobalData* globalData, Scope* parentScope, MethodNode* method)
    : m_globalData(globalData)
    , m_statements(method->GetStatementList())
    , m_maxRegisterCount(0)
    , m_breakOrContinueHelper(0)
{
    assert(method);
    
    m_methodEnv = globalData->GetMethod(method->Identifier()->Value(), method);
    
    m_localScope.AdoptRef(new Scope(parentScope));
    
    m_methodEnv->SetLocalScope(m_localScope.Ptr());
    
    DeclareArguments(method);
    m_calleeRegisters = m_registers.size();
    
    // declare variables;
    StatementList* statements = m_statements.Ptr();
    if (statements)
    {
        for (unsigned i=0; i<statements->size(); ++i)
        {
            StatementNode* statement = statements->at(i).Ptr();
            if (!statement)
                continue;
            
            // methods cannot contain methods or structs
            assert (!statement->IsMethodNode());
            assert (!statement->IsStructNode());
            
            if (statement->IsVarStatement())
            {
                VarStatement* varStatement = static_cast<VarStatement*>(statement);
                DeclareProperty(varStatement->Identifier()->Value(), m_globalData->GetTypeOf(varStatement->GetTypeNode()));
            }
        }
    }
}

// used to generate methods
BytecodeGenerator::BytecodeGenerator(MethodEnv* methodEnv, StatementList* statements, Scope* scope)
    : m_globalData(methodEnv->GetGlobalData())
    , m_statements(statements)
    , m_maxRegisterCount(0)
    , m_breakOrContinueHelper(0)
{
    m_methodEnv = methodEnv;

    if (scope)
    {
        m_localScope = scope;
        for (int i=scope->Count(); i > 0; --i)
        {
            NewRegister();
        }
    }
    else
    {
        m_localScope.AdoptRef(new Scope(0));
    }
    
    m_calleeRegisters = 0;
    
    m_methodEnv->SetLocalScope(m_localScope.Ptr());

    // declare variables;
    if (statements)
    {
        for (unsigned i=0; i<statements->size(); ++i)
        {
            StatementNode* statement = statements->at(i).Ptr();
            if (!statement)
                continue;
            
            // methods cannot contain methods or structs
            assert (!statement->IsMethodNode());
            assert (!statement->IsStructNode());
            
            if (statement->IsVarStatement())
            {
                VarStatement* varStatement = static_cast<VarStatement*>(statement);
                DeclareProperty(varStatement->Identifier()->Value(), m_globalData->GetTypeOf(varStatement->GetTypeNode()));
            }
        }
    }
}

BytecodeGenerator::BytecodeGenerator(GlobalData* globalData, StatementList* statements, std::string methodName)
    : m_globalData(globalData)
    , m_statements(statements)
    , m_maxRegisterCount(0)
    , m_breakOrContinueHelper(0)
{
    m_methodEnv = globalData->GetMethod(methodName);
    
    m_localScope.AdoptRef(new Scope(0));
    m_methodEnv->SetLocalScope(m_localScope.Ptr());
    
    m_calleeRegisters = 0;
    
    // declare methods
    // declare structs
    // declare variables
    
    for (unsigned i=0; i<statements->size(); ++i)
    {
        StatementNode* statement = statements->at(i).Ptr();
        if (statement == 0)
            continue;
        
        if (statement->IsStructNode())
        {
            StructNode* structNode = static_cast<StructNode*>(statement);
            m_globalData->DefineObjectType(structNode);
        }
        else if (statement->IsVarStatement())
        {
            VarStatement* varStatement = static_cast<VarStatement*>(statement);
            DeclareProperty(varStatement->Identifier()->Value(), m_globalData->GetTypeOf(varStatement->GetTypeNode()));
        }
    }

    for (unsigned i=0; i<statements->size(); ++i)
    {
        StatementNode* statement = statements->at(i).Ptr();
        if (statement == 0)
            continue;
        
        if (statement->IsMethodNode())
        {
            MethodNode* methodNode = static_cast<MethodNode*>(statement);
            BytecodeGenerator generator(globalData, m_localScope.Ptr(), methodNode);
            generator.Generate();
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
        DeclareProperty( returnValue, m_globalData->GetTypeOf(returnType));
    }

    ArgumentNodeList* arguments = method->GetArgumentNodeList();
    if (arguments)
    {
        for (unsigned i=0; i<arguments->size(); ++i)
        {
            ArgumentNode* argument = arguments->at(i).Ptr();
            DeclareProperty( argument->Identifier()->Value(), m_globalData->GetTypeOf(argument->Type()), true  );
        }
    }
}

PassRef<Accessor> BytecodeGenerator::GetProperty(std::string& name, bool onlyLocal)
{
    if (onlyLocal && !m_localScope->HasLocalProperty(name))
        return 0;
    
    return m_localScope->GetProperty(name);
}

void BytecodeGenerator::DeclareProperty(std::string& name, Type* type, bool isArgument)
{
    if (m_localScope->HasLocalProperty(name))
    {
        printf("property name redclartion %s\n", name.c_str());
        exit(1);
    }
    
    Property* property = m_localScope->PutProperty(name, type);
    RefPtr<Register> reg = NewRegister();
    reg->SetType(type);
    property->SetRegister(reg.Ptr());
    
    if (type->IsCollectorRef() && !isArgument)
    {
        EmitBytecode(op_init_ref);
        EmitRegister(reg.Ptr());
    }
    
    // printf("Variable %s has register %d\n", name.c_str(), property->GetRegister()->Number());
}

Register* BytecodeGenerator::EmitNode(ArenaNode* node, Register* dst)
{
    dst = node->EmitBytecode(this, dst);
    
    // enable this for debugging
    //EmitBytecode(op_debug_line);
    //EmitConstantInt(node->Line());
    
    return dst;
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

MethodEnv* BytecodeGenerator::Generate()
{
    StatementList* statements = m_statements.Ptr();
    if (statements)
    {
        for (unsigned i=0; i<statements->size(); ++i)
        {
            StatementNode* statement = statements->at(i).Ptr();
            if (statement == 0)
                continue;
            
            EmitNode(statement);
        }
    }
    
    FinishMethod();
    
    return m_methodEnv.Ptr();
}

void BytecodeGenerator::FinishMethod()
{
    m_methodEnv->Compiled(m_maxRegisterCount, m_bytes);
}

void BytecodeGenerator::EmitBytecode(OpCode bytecode)
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

Register* BytecodeGenerator::Coerce(Register* reg, Type* otherType)
{
    Register* dst = reg->IsIgnored() ? reg : NewTempRegister().Ptr();
    
    bool converted = false;
    Type* type = reg->GetType();
    assert(type != otherType);
    
    if (type->InheritsFrom(otherType))
    {
        // just convert the thing
        if (dst != reg)
        {
            EmitBytecode(op_assign);
            EmitRegister(dst);
            EmitRegister(reg);
        }
        dst->SetType(otherType);
        return dst;
    }
    
    if (otherType->IsCollectorRef() && type == m_globalData->GetNullType())
    {
        // upcasting null to the other type
        dst->SetType(otherType);
        return dst;
    }
    
    if (otherType->InheritsFrom(type))
    {
        // just dynamic upcasting
        EmitBytecode(op_dynamic_cast);
        EmitRegister(dst);
        EmitRegister(reg);
        EmitConstantString(otherType->Name());
        
        dst->SetType(otherType);
        return dst;
    }
    
    if (m_globalData->GetIntType() == type)
    {
        if (m_globalData->GetFloatType() == otherType)
        {
            EmitBytecode(op_coerce_int_float);
            converted = true;
        }
        else if (m_globalData->GetStringType() == otherType)
        {
            EmitBytecode(op_coerce_int_string);
            converted = true;
        }
    }
    else if (m_globalData->GetFloatType() == type)
    {
        if (m_globalData->GetIntType() == otherType)
        {
            EmitBytecode(op_coerce_float_int);
            converted = true;
        }
        else if (m_globalData->GetStringType() == otherType)
        {
            EmitBytecode(op_coerce_float_string);
            converted = true;
        }
    }
    else if (m_globalData->GetStringType() == type)
    {
        if (m_globalData->GetIntType() == otherType)
        {
            EmitBytecode(op_coerce_string_int);
            converted = true;
        }
        else if (m_globalData->GetFloatType() == otherType)
        {
            EmitBytecode(op_coerce_string_float);
            converted = true;
        }
        else if (m_globalData->GetCodeType() == otherType)
        {
            EmitBytecode(op_coerce_string_code);
            converted = true;
        }
    }
    else if (m_globalData->GetCodeType() == type)
    {
        if (m_globalData->GetStringType() == otherType)
        {
            EmitBytecode(op_coerce_code_string);
            converted = true;
        }
    }
    
    if (converted)
    {
        EmitRegister(dst);
        EmitRegister(reg);
        dst->SetType(otherType);
    }
    else
    {
        printf("Error: unable to coerce %s to %s\n", type->Name().c_str(), otherType->Name().c_str());
        exit(1);
    }
    
    return dst;
}

unsigned BytecodeGenerator::GetLabel()
{
    return m_bytes.size();
}

void BytecodeGenerator::PatchConstantInt(unsigned label, int value)
{
    assert(label < m_bytes.size());
    m_bytes.at(label).ConstantInt = value;
}

void BytecodeGenerator::EmitBreak()
{
    CleanupRegisters();
    
    EmitBytecode(op_jmp);
    assert(m_breakOrContinueHelper);
    m_breakOrContinueHelper->PushPatchBreakLabel(GetLabel());
    EmitConstantInt(0);
}

void BytecodeGenerator::EmitContinue()
{
    CleanupRegisters();
    
    EmitBytecode(op_jmp);
    assert(m_breakOrContinueHelper);
    m_breakOrContinueHelper->PushPatchContinueLabel(GetLabel());
    EmitConstantInt(0);
}

void MethodEnv::Run(RegisterValue* startingRegister)
{
    if (!m_compiled)
    {
        printf("the method is not compiled \n");
        exit(1);
    }
        
    RegisterFile* registerFile = m_globalData->GetRegisterFile();
    RegisterValue* lastUsedRegister = registerFile->GetLastUsed();
    
    if (!registerFile->CanGrow(startingRegister + m_registerCount))
    {
        printf("if I enter this functon I will get a stack overflow\n");
        exit(1);
    }
    
    Interpret(m_globalData, startingRegister, &m_bytes);
    
    if (!registerFile->CanShrink(lastUsedRegister))
    {
        printf("can't shrink???\n");
        exit(1);
    }
    
    if (m_next.Ptr())
    {
        m_next->Run(startingRegister);
    }
}

void MethodEnv::Debug() const
{
    printf("Method: %s\n", m_name.c_str());
    Disassemble(m_globalData, &m_bytes);
    if (m_next.Ptr())
    {
        m_next->Debug();
    }
}

void MethodEnv::PrependArgumentsFromMethodNode(MethodNode* method)
{
    TypeNode* returnTypeNode = method->GetReturnType();
    if (returnTypeNode)
        m_returnType = m_globalData->GetTypeOf(returnTypeNode);
    
    ArgumentNodeList* argumentNodeList = method->GetArgumentNodeList();
    if (argumentNodeList)
    {
        for (unsigned i=0; i<argumentNodeList->size(); ++i)
        {
            m_argumentsType.push_back(m_globalData->GetTypeOf(argumentNodeList->at(i)->Type()));
        }
    }
}
