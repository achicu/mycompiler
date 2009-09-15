#include <fstream>
#include "ParserArena.h"
#include "Nodes.h"
#include "BytecodeGenerator.h"
		
int main (int argc, char * const argv[])
{
    RefPtr<GlobalData> globalData(AdoptRef(new GlobalData()));
    assert(globalData->HasOneRef());
        
    RefPtr<ArenaNode> result;
    
    std::ifstream inputFile("../../inputfile.txt", std::ifstream::in);
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
