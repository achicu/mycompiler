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
#include <iostream>
#include <sstream>
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
    Arena(std::istream* inputStream);
    
    ArenaNode* RegisterNode(ArenaNode* node);
    
    ArenaNode* Parse();
    
    void* GetScanner() const { return m_scanner; }
    
    char* GetLexerText() const;
    void SetResult(ArenaNode* result) { m_result = result; }
    
    std::string GetSourceCode() const { return m_sourceCode.str(); }

    int Read(char* buffer, int max_size);


private:
    
    RefPtr<ArenaNode> m_result;
    
    void InitScanner();
    void DestroyScanner();
    
    void* m_scanner;
    std::istream* m_inputStream;
    std::ostringstream m_sourceCode;
};

#endif // PARSER_ARENA_H
