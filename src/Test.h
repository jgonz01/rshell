#ifndef __TEST__
#define __TEST__
#include "Command.h"
using namespace std;

/*
TEST: Test Command class called from the Command class to execute 
the command "test" or use of the brackets
*/
class Test{
    public:
        Test(string otherFlag, string prevCommand){
            /*Flag is already passed in, but another parser
            is used to parse brackets from the path*/
            this->flag = otherFlag;
            int y = 0;
            char *userInput = const_cast<char *>(prevCommand.c_str());   
            char *tok;
            tok = strtok(userInput, "/" );  
            while(tok != NULL){
                if(tok[0] != ' '){
                    path.push_back(tok);
                }
                else{
                    y = 0;
                    while(tok[y] == '/'){
                        ++y;
                    }
                    path.push_back(tok + y);
                }
                tok = strtok(NULL, "/");
            }
            if(path.size() > 1){
                /*Filename is the last one in the paht
                set that one equal to the userCommand*/
                userCommand = path.at(path.size() - 1);
            }
            else{
                /*No path was input, prevCommand is filename*/
                userCommand = prevCommand;
            }
        }
    //protected:
        string flag;
        string userCommand;
        vector<string> path;
        bool execute(){
            struct stat buffer;
            int status;
            bool isRightType = false;
            
            string fileName = userCommand;
            /*Checking to see if the file exists
            looking at flags first*/
            if(flag == "-e"){
                status = stat(fileName.c_str(), &buffer);
                if(status == 0){
                    cout << "(TRUE)" << endl;
                    return true;
                }
                else{
                    cout << "(FALSE)" << endl;
                    return false;
                }
            }
            /*Typechecking the file using the different parameters*/
            else if(flag == "-f"){
                status = stat(fileName.c_str(), &buffer);
                isRightType = S_ISREG(buffer.st_mode);
            }
            else if(flag == "-d"){
                status = stat(fileName.c_str(), &buffer);
                isRightType = S_ISDIR(buffer.st_mode);
            }
            /*Outputting the correct value and returning the boolean
            value of the execution*/
            if(isRightType){
                cout << "(TRUE)" << endl;
                return true;
            }
            else{
                cout << "(FALSE)" << endl;
                return false;
            }   
        
        }
};

#endif
