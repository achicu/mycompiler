/*
 *  RegisterFile.cpp
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/13/09.
 *
 */
#include "Platform.h"

#include "RegisterFile.h"
#include "Interpreter.h"

#define MAX_BUFER_SIZE 50000

RegisterFile::RegisterFile()
{
    m_block = new RegisterValue[MAX_BUFER_SIZE];
    m_lastUsed = m_block;
    m_end = m_block + MAX_BUFER_SIZE;
}

RegisterFile::~RegisterFile()
{
    delete [] m_block;
}

bool RegisterFile::CanGrow(RegisterValue* to)
{
    assert(to >= m_block);
    if (to < m_end)
    {
        m_lastUsed = to;
        return true;
    }
    
    return false;
}

bool RegisterFile::CanShrink(RegisterValue* to)
{
    assert (to < m_end);
    if (to >= m_block)
    {
        m_lastUsed = to;
        return true;
    }
    
    return false;
}
