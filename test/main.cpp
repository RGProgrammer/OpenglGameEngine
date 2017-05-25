#include "Test.h"

using namespace RGP_CORE ;
int main(void){
    Test*   testprogram=new Test();
    if(!testprogram->Init()){
        testprogram->Destroy();
        delete testprogram ;
        testprogram=0;
        printf("error initializing test\n");
        return 0 ;
    }
    testprogram->Start();
    testprogram->Destroy();
    delete testprogram;
    return 0 ;
}
