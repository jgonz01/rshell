#ifndef __CD__COMMAND__
#define __CD_COMMAND__

#include <iostream>
#include <unistd.h>
#include "Command.h"
#include <string>
#include <vector>
#include <libgen.h>
using namespace std;

class Cd{
    private:
    vector<char *> path;
    unsigned pathSize;
    int successFlag;
    
    public:
    
    /*
    Constructor for "cd" command
    */
    Cd(){ 
        char* temp;
        string hi = "cd";
        temp = const_cast<char *>(hi.c_str());
        path.push_back(temp);
        
        path.at(0) = new char;
        path.at(0)[0] = 'c';
        path.at(0)[1] = 'd';
        path.at(0)[2] = '\0';
        pathSize = 1;
        execute();
    }
    
    /*
    Constructor for "cd -" and "cd <path>"
    */
    Cd(string userPath){ 
        //PUSH BACK "cd" to vector
        char* temp;
        string hi = "cd";
        temp = const_cast<char *>(hi.c_str());
        path.push_back(temp);
        path.at(0) = new char;
        path.at(0)[0] = 'c';
        path.at(0)[1] = 'd';
        path.at(0)[2] = '\0';
    
        //Removes extra whitespace from userPath
        while(userPath[0] == ' '){
            userPath.erase(userPath.begin());
        }
        while(userPath[userPath.size() - 1] == ' '){
            userPath.erase(userPath.end() - 1);
        }
        
        //Check if userPath = '-'
        if(userPath == "-"){
            pathSize = 2;
            string str = "-";
            char* temp2 = const_cast<char *>(str.c_str());
            path.push_back(temp2);
            execute();
            return;
        }
        else{
            //Else <PATH>
            pathSize = 3;
            int y = 0;
            char *userInput = const_cast<char *>(userPath.c_str());   
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
            execute();
        }
    }

    void execute(){
        successFlag = 1;
        //Command entered is "cd"
        if(pathSize == 1) {
            char *cwd = getenv("PWD");
            char *pwd = getenv("HOME");
            //Check if current directory is null
            if(cwd == NULL){
                perror("cwd error");
                successFlag = 0;
            }
            //Check if OLDPWD successfully set to cwd
            int success1 = setenv("OLDPWD", cwd, 1);
            if(success1 == -1){
                perror("setenv error 1");
                successFlag = 0;
            }
            
            //Check if home directory is null
            if(pwd == NULL){
                perror("pwd error");
                successFlag = 0;
            }
            //Update PWD
            int success = setenv("PWD", pwd, 1);
            int success2 = chdir(pwd);
            //Check if chdir was successful
            if(success2 == -1){
                perror("chdir error");
                successFlag = 0;
            }
            //Check if setenv was successful
            if(success == -1){
                perror("setenv error 2");
                successFlag = 0;
            }
            
        }
        //Command entered is "cd -"
        else if(pathSize == 2){
            char *cwd = getenv("PWD");
            char *pwd = getenv("OLDPWD");
            //Check if current directory is null
            if(cwd == NULL){
                perror("cwd error");
                successFlag = 0;
            }
            //Check if OLDPWD successfully set to cwd
            int success1 = setenv("OLDPWD", cwd, 1);
            if(success1 == -1){
                perror("setenv error 1");
                successFlag = 0;
            }
            
            //Check if pwd directory is null
            if(pwd == NULL){
                perror("pwd error");
                successFlag = 0;
            }
            //Update PWD
            int success = setenv("PWD", pwd, 1);
            int success2 = chdir(pwd);
            //Check if chdir was successful
            if(success2 == -1){
                perror("chdir error");
                successFlag = 0;
            }
            //Check if setenv was successful
            if(success == -1){
                perror("setenv error 2");
                successFlag = 0;
            }
        }
        //Command entered is "cd <path>"
        else{
            const char* pwdCurr = getenv("PWD");
            const char* pwdNew = getenv("PWD");
            const char* pwdOrig = getenv("PWD");
            
            for(unsigned i = 1; i < path.size(); ++i){
                pwdOrig = getenv("PWD");
                string pwdNewString = pwdOrig;
                pwdNewString += "/";
                pwdNewString += path.at(i);
                pwdNew = pwdNewString.c_str();
                int success = chdir(pwdNew);

                if(success == 0){
                    //getting current pwd
                    int setEnv = setenv("PWD", pwdNew, 1);
                    if(setEnv == 0){
                        
                    }
                    else{
                        cout << "Failed" << endl;
                        setenv("PWD", pwdCurr, 1);
                        successFlag = 0;
                        break;
                    }
                    
                    //Check if cwd is NULL
                    if(pwdCurr == NULL){
                        perror("pwdCurr error");
                        successFlag = 0;
                    }
                    int temp = setenv("OLDPWD", pwdCurr, 1);
                    int temp2 = chdir(pwdCurr);
                    //Check if pwdCurr exists
                    if(temp2 == -1){
                        perror("error");
                        successFlag = 0;
                    }
                    //Check if OLDPWD successfully set
                    if(temp == -1){
                        perror("error");
                        successFlag = 0;
                    }
                }
                else if(success == -1){
                    cout << "Change directory Failed" << endl;
                    //Set PWD back to original because chdir error
                    setenv("PWD", pwdCurr, 1);
                    successFlag = 0;
                    break;
                }
            }
            
        }
    }
    int flagExecuted(){
        return successFlag;
    }
};


#endif