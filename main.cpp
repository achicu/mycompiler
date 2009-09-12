#include <iostream>
#include <errno.h>
#include "ParserArena.h"
#include "Nodes.h"
#include "BytecodeGenerator.h"
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
    inputFile = fopen("../../inputfile.txt", "rb");
    //inputFile = fopen("/www/git/mycompiler/inputfile.txt", "rb");
    if (inputFile)
    {
        RefPtr<ArenaNode> result;
        {
            Arena parserArena;
            if (yyparse() == 0)
            {
                result = parserArena.Result();
                
                /*size_t size = ftell(inputFile);
                char* buffer = new char[size];
                fseek(inputFile, 0, SEEK_SET);
                fread(buffer, size, 1, inputFile);
                int* lines = new int[size];
                int line = 2;
                
                lines[0] = lines[1] = 0;
                
                for (int i=0; i<size; i++)
                {
                    if (buffer[i] == '\n')
                    {
                        lines[line] = i;
                        ++ line;
                        printf("line %d - %d\n", line, i);
                    }
                }
                
                for (int i = 0; i<parserArena.size(); ++i)
                {
                    printf("-> %s\n", parserArena[i]->LocationToString().c_str());
                    int start = lines[parserArena[i]->Line()] + parserArena[i]->Start();
                    int end = lines[parserArena[i]->EndLine()] + parserArena[i]->End();
                    
                    for (int j=start; j<=end; ++j)
                        printf("%c", buffer[j]);
                    printf("\n");
                }
                delete [] buffer;
                delete [] lines;
                */
            }
        }
        if (result.Ptr() != 0)
        {
            printf("%s\n", result->ToString().c_str());
            
            RefPtr<GlobalData> globalData(AdoptRef(new GlobalData()));
            BytecodeGenerator generator(globalData.Ptr(), static_cast<StatementList*> (result.Ptr()));
            generator.Generate();
            
        }
        fclose(inputFile);
    }
    else
    {
        printf("input file not found\n");
    }
    
    return 0;
}
