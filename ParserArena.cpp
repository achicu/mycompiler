/*
 *  ParserArena.cpp
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/10/09.
 *
 */

#include "ParserArena.h"
#include <string>
#include <sstream>
#include "BytecodeGenerator.h"

extern int yyparse(Arena* arena);

ArenaNode::ArenaNode()
    : m_line(0)
    , m_endLine(0)
    , m_start(0)
    , m_end(0)
{    
}

Register* ArenaNode::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    // should not be here
    //assert(false);
    return 0;
}

PassRef<Accessor> ArenaNode::GetAccessor(BytecodeGenerator* generator)
{
    return 0;
}

ArenaNode* Arena::RegisterNode(ArenaNode* ptr)
{
    assert(ptr->HasOneRef());
    push_back(AdoptRef(ptr));
    return ptr;
}

std::string ArenaNode::LocationToString() const
{
    std::ostringstream o;
    o << '[' << m_line << " to " << m_endLine << " [" << m_start << ':' << m_end << ']';
    return o.str();
}

std::string ArenaNode::ToString() const
{
    return "[ArenaNode]";
}

Arena::Arena(std::istream* inputStream)
    : m_inputStream (inputStream)
{
}

int Arena::Read(char* buffer, int max_size)
{
    int len = m_inputStream->readsome(buffer, max_size);
    if (len)
    {
        m_sourceCode.write(buffer, len);
    }
    return len;
}

ArenaNode* Arena::Parse()
{
    InitScanner();
    yyparse(this);
    DestroyScanner();
    return m_result.Ptr();
}
