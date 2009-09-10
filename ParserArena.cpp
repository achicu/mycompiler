/*
 *  ParserArena.cpp
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/10/09.
 *  Copyright 2009 Adobe Systems Romania SRL. All rights reserved.
 *
 */

#include "ParserArena.h"
#include <string>

Arena* Arena::s_active = 0;

ArenaNode::ArenaNode()
    : m_refCount (1)
{ 
    Arena::Active()->RegisterNode(this); 
}

void Arena::RegisterNode(ArenaNode* ptr)
{
    assert(ptr->HasOneRef());
    m_nodes.push_back(RefPtr<ArenaNode>::AdoptRef(ptr));
}

std::string ArenaNode::ToString() const
{
    return "[ArenaNode]";
}
