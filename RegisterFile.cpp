/*
 *  RegisterFile.cpp
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/13/09.
 *
 */

#include "RegisterFile.h"
#include "Interpreter.h"

#define MAX_BUFER_SIZE 50000

RegisterFile::RegisterFile()
{
    m_block = new RegisterValue[MAX_BUFER_SIZE];
    m_end = m_block + MAX_BUFER_SIZE;
}

RegisterFile::~RegisterFile()
{
    delete [] m_block;
}

bool RegisterFile::CanGrow(RegisterValue* to)
{
    assert(to >= m_block);
    return (to < m_end);
}

bool RegisterFile::CanShring(RegisterValue* to)
{
    assert (to < m_end);
    return (to > m_block);
}
