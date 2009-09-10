#include <iostream>
#include <errno.h>
#include "ParserArena.h"
#include "Nodes.h"
#include "parser.y.hh"

extern int yyparse (void);

extern "C" void yywrap(void)
{
}

FILE* inputFile;

void readNext(char* buf, int& result, int max_size)
{
    errno=0;
    while ( (result = fread(buf, 1, max_size, inputFile))==0 && ferror(inputFile))
    {
        if( errno != EINTR)
            {
            printf( "input in flex scanner failed" );
            break;
            }
        errno=0;
        clearerr(inputFile);
    }
}

void sendToken(char* buff, int size)
{
    puts(buff);
}
		
int main (int argc, char * const argv[]) {
    Arena parserArena;
    
    inputFile = fopen("/www/c++/lex/inputfile.txt", "rb");
    if (inputFile)
    {
        if (yyparse() == 0 && parserArena.Result() != 0)
        {
            printf("%s\n", parserArena.Result()->ToString().c_str());
        }
        fclose(inputFile);
    }
    else
    {
        printf("input file not found\n");
    }
    
    return 0;
}
