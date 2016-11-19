#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "Command.h"
#include <string.h> 
#include <unistd.h>
#include "Test.h"
using namespace std;

int main(){
    
    Command* test = new Command();
    
    //LOOP TO CHECK FOR EXIT
    while(test->exitflag != true){
        
        test = new Command();
        test->getInput();
        
        if(test->checkEnter() != true){
            test->callConnectors();
        }
    }
    
    return 0;
}
