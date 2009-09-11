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
    : m_refCount (1)
    , m_start(0)
    , m_end(0)
{ 
    Arena::Active()->RegisterNode(this); 
}

void Arena::RegisterNode(ArenaNode* ptr)
{
    assert(ptr->HasOneRef());
    m_nodes.push_back(RefPtr<ArenaNode>::AdoptRef(ptr));
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
