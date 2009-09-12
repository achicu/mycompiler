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

Arena* Arena::s_active = 0;

ArenaNode::ArenaNode()
    : m_line(0)
    , m_endLine(0)
    , m_start(0)
    , m_end(0)
{ 
    Arena::Active()->RegisterNode(this); 
}

Register* ArenaNode::EmitBytecode(BytecodeGenerator* generator, Register* dst)
{
    // should not be here
    //assert(false);
    return 0;
}

void Arena::RegisterNode(ArenaNode* ptr)
{
    assert(ptr->HasOneRef());
    push_back(AdoptRef(ptr));
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
