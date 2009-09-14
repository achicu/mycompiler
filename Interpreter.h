/*
 *  Interpreter.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/12/09.
 *
 */
 
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>

union Bytecode;
class GlobalData;

class CollectorRef;

union RegisterValue
{
    int asInt;
    double asFloat;
    CollectorRef* asReference;
};

void Interpret(GlobalData* globalData, RegisterValue* registers, std::vector<Bytecode>* buffer);

 
#endif // INTERPRETER_H

