/*
 *  Interpreter.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/12/09.
 *
 */
 
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>
#include <string>
#include "Collector.h"

union Bytecode;
class GlobalData;

class CollectorRef;
class ObjectType;
class VectorType;

class RefString: public CollectorRef
{
public:
    RefString(std::string value)
        : Value (value)
    {
    }
    
    std::string Value;
};

class RefObject: public CollectorRef
{
public:
    RefObject(ObjectType* type);
    virtual ~RefObject();
    
    virtual void Mark();
    
    template <typename T>
    T ReadAtOffset(int offset)
    {
        return *(reinterpret_cast<T*>(&m_buffer[offset]));
    }
    
    template <typename T>
    void WriteAtOffset(int offset, T value)
    {
        *(reinterpret_cast<T*>(&m_buffer[offset])) = value;
    }
    
private:
    ObjectType* m_type;
    char* m_buffer;
};

class RefVector: public CollectorRef
{
public:
    RefVector(VectorType* type, int size);
    virtual ~RefVector();
    
    virtual void Mark();
    
    template <typename T>
    T ReadAtOffset(int offset)
    {
        CheckSpace(offset);
        return *(reinterpret_cast<T*>(&m_buffer[offset]));
    }
    
    template <typename T>
    void WriteAtOffset(int offset, T value)
    {
        CheckSpace(offset);
        *(reinterpret_cast<T*>(&m_buffer[offset])) = value;
    }
    
    char* GetBuffer() const { return m_buffer; }
    int GetSize() const { return m_size; }
    
private:

    void CheckSpace(int offset);
    
    VectorType* m_type;
    char* m_buffer;
    int m_bufferSize;
    int m_size;
};

union RegisterValue
{
    int asInt;
    double asFloat;
    CollectorRef* asReference;
};

void Interpret(GlobalData* globalData, RegisterValue* registers, std::vector<Bytecode>* buffer);

 
#endif // INTERPRETER_H

