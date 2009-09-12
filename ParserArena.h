/*
 *  ParserArena.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/10/09.
 *
 */

#ifndef PARSER_ARENA_H
#define PARSER_ARENA_H

#include <vector>
#include <assert.h>
#include "RefCounted.h"
#include "RefPtr.h"

class BytecodeGenerator;
class Register;
class Accessor;

class ArenaNode: public RefCounted
{
public:
    ArenaNode();
    
    std::string LocationToString() const;
    virtual std::string ToString() const;

    void SetLocation(int line, int endLine, int start, int end)
    {
        m_line = line;
        m_endLine = endLine;
        m_start = start;
        m_end = end;
    }
    
    int Line() const { return m_line; }
    int EndLine() const { return m_endLine; }
    int Start() const { return m_start; }
    int End() const { return m_end; }

    virtual Register* EmitBytecode(BytecodeGenerator* generator, Register* dst);
    virtual PassRef<Accessor> GetAccessor(BytecodeGenerator* generator);

private:
    int m_line;
    int m_endLine;
    int m_start;
    int m_end;
};

class Arena: public std::vector< RefPtr<ArenaNode> >
{
public:
    Arena() { m_oldActive = s_active; s_active = this; }
    ~Arena() { s_active = m_oldActive; }
    
    void RegisterNode(ArenaNode* node);
    
    static Arena* Active() { return s_active; }
    
    void SetResult(ArenaNode* result) { m_result = result; }
    ArenaNode* Result() const { return m_result.Ptr(); }
    
private:
    static Arena* s_active;
    Arena* m_oldActive;
    RefPtr<ArenaNode> m_result;
};

#endif // PARSER_ARENA_H
