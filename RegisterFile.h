/*
 *  RegisterFile.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/13/09.
 *
 */

#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

union RegisterValue;

class RegisterFile
{
public:
    RegisterFile();
    ~RegisterFile();
    
    bool CanGrow(RegisterValue* to);
    bool CanShrink(RegisterValue* to);
    
    RegisterValue* GetBlock() const { return m_block; }
    
    RegisterValue* GetLastUsed() const { return m_lastUsed; }
    
private:
    RegisterValue* m_block;
    RegisterValue* m_end;
    RegisterValue* m_lastUsed;
};

#endif // REGISTER_FILE_H
