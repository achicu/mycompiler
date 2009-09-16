#include "Platform.h"
#include <iostream>
#include <fstream>
#include "ParserArena.h"
#include "Nodes.h"
#include "BytecodeGenerator.h"
        
int main (int argc, char * const argv[])
{
    RefPtr<GlobalData> globalData(AdoptRef(new GlobalData()));
    assert(globalData->HasOneRef());
        
    RefPtr<ArenaNode> result;
    
    if (argc!=2)
    {
        printf("usage: mycompiler.exe filename\n");
        exit(1);
    }

    std::ifstream inputFile(argv[1], std::ios::in | std::ios::binary);
    if (!inputFile.is_open())

    {

        printf("file not found\n");

        exit(1);

    }


    Arena parser (&inputFile);
    result = parser.Parse();
    
    if (result.Ptr() != 0)
    {
        BytecodeGenerator generator(globalData.Ptr(), static_cast<StatementList*> (result.Ptr()));
        RefPtr<MethodEnv> methodEnv = generator.Generate();
        methodEnv->SetSourceCode(parser.GetSourceCode());
        methodEnv->Run( globalData->GetRegisterFile()->GetBlock() );
    }
    
    return 0;
}
