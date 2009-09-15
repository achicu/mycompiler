/*
 *  Disassembler.h
 *  lex
 *
 *  Created by Alexandru Chiculita on 9/12/09.
 *
 */

#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <vector>

union Bytecode;
class GlobalData;

void Disassemble(GlobalData* globalData, const std::vector<Bytecode>* buffer);

#endif // DISASSEMBLER_H