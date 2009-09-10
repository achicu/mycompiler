/*
 *  ParserArena.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/10/09.
 *  Copyright 2009 Adobe Systems Romania SRL. All rights reserved.
 *
 */

#ifndef PARSER_ARENA_H
#define PARSER_ARENA_H

#include <vector>
#include <assert.h>
#include "RefPtr.h"

class ArenaNode
{
public:
    ArenaNode();
    virtual ~ArenaNode() {
        assert(m_refCount == 0);
    }
    
    void Ref()
    {
        ++m_refCount;
    }
    
    int Deref()
    {
        if (--m_refCount != 0)
        {
            return m_refCount;
        }
        
        delete this;
        return 0;
    }
    
    virtual std::string ToString() const;
    
#ifndef NDEBUG
    bool HasOneRef() const { return m_refCount == 1; }
#endif

private:
    int m_refCount;
};

class Arena
{
public:
    Arena() { m_oldActive = s_active; s_active = this; }
    ~Arena() { s_active = m_oldActive; }
    
    void RegisterNode(ArenaNode* node);
    
    static Arena* Active() { return s_active; }
    
    void SetResult(ArenaNode* result) { m_result = result; }
    ArenaNode* Result() const { return m_result.Ptr(); }
    
private:
    std::vector< RefPtr<ArenaNode> > m_nodes;
    static Arena* s_active;
    Arena* m_oldActive;
    RefPtr<ArenaNode> m_result;
};

#endif // PARSER_ARENA_H
