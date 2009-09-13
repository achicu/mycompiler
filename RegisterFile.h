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
    bool CanShring(RegisterValue* to);
    
    RegisterValue* GetBlock() const { return m_block; }
    
private:
    RegisterValue* m_block;
    RegisterValue* m_end;
};

#endif // REGISTER_FILE_H
