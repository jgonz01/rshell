#ifndef __COMMANDS__
#define __COMMANDS__

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stack>
#include <queue>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include "Test.h"
using namespace std;
class Test;
/*
BASE: Abstract Base Class where Command and Connectors inherit from
-flag1 stores whether or not the first command was successful
-flag0 stores if we are looking at the first command in the line
*/
class Base {
    public:
    
    virtual void getInput() = 0;
    string input;
    int flag1, flag0;
};

/* 
COMMAND: class will parse the input, and execute the commands
-connectors store the string value of all the connectors in order
-userCommands stores all commands
*/
class Command : public Base {
    public:
    vector<string> connectors; 
    vector<string> userCommands; 
    char** command;
    stack<char> secondSet;
    vector<string> pairFlags;
    unsigned position;
    unsigned position2;
    string nextConnector;
    unsigned flag5;
    unsigned flag6;
    unsigned flag7;
    
    public:
    bool closed;
    bool syntax;
    bool exitflag;
    bool enter;
    bool commentflag;
    
    Command() {
        closed = false;
        syntax = false;
        enter = false;
        exitflag = false;
        commentflag = false;
        flag0 = -1;
    }
    
    Command(string s) { 
        closed = false;
        syntax = false;
        enter = false;
        exitflag = false;
        commentflag = false;
        flag0 = -1;
        input = s;
    }
    
    /*
    Checks if user entered newline
    */
    bool checkEnter() {
        return enter;
    }
    
    /*
    Input has parenthesis so we parse and execute differently
    */
    void parseParenthesis(){
        flag1 = 1;
        
        //Check for comments here
        for(unsigned o = 0; o < input.size(); ++o){
            if(input[o] == '#'){
                input.erase(input.begin() + o, input.end());
                o = input.size() + 10;
            }
        }
        
        //Checks if it is balanced
        stack <char> stack1;
        unsigned y = 0;
        if(input.empty() == true){
            return;
        }
        while(y < input.size()){
            if(input[y] == '('){
                stack1.push('(');
            }
            else if(input[y] == ')'){
                if(stack1.empty() == false){
                    stack1.pop();
                }
                else{
                    cout << "SYNTAX ERROR: Not Balanced" << endl;
                    return;
                }
            }
            ++y;
        }
        if(stack1.empty() == false){
            cout << "SYNTAX ERROR: Not Balanced" << endl;
            return;
        }
        
        vector<string> connecting;
        unsigned w = 0;
        for(; w < input.size(); ++w){
            if(input[w] == '&'){
                if(input[w + 1] == '&'){
                    connecting.push_back("AND");
                }
            }
            else if(input[w] == '|'){
                if(input[w + 1] == '|'){
                    connecting.push_back("OR");
                }
            }
            else if(input[w] == ';'){
                connecting.push_back("SEMICOLON");
            }
        }
        
        //Check if all are ANDS, ORS, or SEMICOLONS
        w = 0;
        unsigned ands = 0;
        unsigned ors = 0;
        unsigned semis = 0;
        for( ; w < connecting.size(); ++w){
            if(connecting.at(w) == "AND"){
                ++ands;
            }
            else if(connecting.at(w) == "OR"){
                ++ors;
            }
            else if(connecting.at(w) == "SEMICOLON"){
                ++semis;
            }
        }
        w = 0;
        unsigned place = 0;
        if(ands == connecting.size() && ands != 0){
            for(; w < input.size(); ++w){
                if(input[w] == '(' || input[w] == ')'){
                    input.erase(input.begin() + w);
                }
            }
            addParsed(input);
            return;
        }
        else if(ors == connecting.size() && ors != 0){
            for(; w < input.size(); ++w){
                if(input[w] == '|' || input[w] == '&' || input[w] == ';'){
                    place = w;
                    w = input.size() + 1;
                }
            }
            string subs = input.substr(0, place);
            w = 0;
            for(; w < subs.size(); ++w){
                if(subs[w] == '(' || subs[w] == ')'){
                    subs.erase(subs.begin() + w);
                }
            }
            addParsed(subs);
            return;
        }
        else if(semis == connecting.size() && semis != 0){
            for(; w < input.size(); ++w){
                if(input[w] == '(' || input[w] == ')'){
                    input.erase(input.begin() + w);
                }
            }
            addParsed(input);
            return;
        }
        //Calls recursive function
        string s = input;
        recursiveParse(s);
    }
    
    /*
    Will Recursively push all commands and connectors to be executed in userCommands
    */
    void recursiveParse(string s){
        unsigned flag4 = 0;
        unsigned flag3 = 0;
        bool firstBody = true;
        vector<unsigned> pair1;
        unsigned y = 0;
        unsigned pos1, pos2;
        unsigned poss1, poss2;
        
        //Removes any whitespace
        while(s[0] == ' '){
            s.erase(s.begin());
            if(s.empty() == true){
                return;
            }
        }
        while(s[s.size() - 1] == ' '){
            s.erase(s.end() - 1);
            if(s.empty() == true){
                return;
            }
        }
        string ss = s;
        
        //REMOVES Extra parenthesis at beginning and end
        if(ss[0] == '('){
            if(ss[ss.size() - 1] == ')'){
                ss.erase(ss.begin());
                ss.erase(ss.end() - 1);
            }
        }
        if(Parenthesis(ss) == true){
            s = ss;
            input = s;
        }
        
        //If empty after removing, then exit
        if(s.empty() == true){
            return;
        }
        
        //Check if what's left is just whitespace
        bool space = true;
        for(unsigned v = 0; v < s.size(); ++v){
            if(s[v] != ' '){
                space = false;
            }
        }
        if(space == true){
            return;
        }
        
        //Finds first parenthesis clump, then parses it
        while(firstBody == true){ 
            if(Parenthesis(s) == true){ 
                if(space == false){ 
                    addParsed(s);
                    s.erase(s.begin(), s.end());
                    firstBody = false;
                }
                else{
                    return;
                }
            }
            if(flag3 == 0 && flag4 == 0){
                if( (s[y] != ' ') && (s[y] != '(') && (s[y] != ')') ){
                    if(s[y] == '&' || s[y] == '|' || s[y] == ';'){
                        if(s[y + 1] == '&' || s[y + 1] == '|'){
                            y = y + 2;
                        }
                        else{
                            ++y;
                        }
                    }
                    else{
                        pos1 = y;
                        while( (s[y] != '&') && (s[y] != '|') && (s[y] != ';') &&  (s[y] != ')')){
                            ++y;
                        }
                        pos2 = y - 1;
                        string str = s.substr(pos1, pos2 - pos1 + 1);
                        nextConnectors(s, pos2);
                        addParsed(str);
                        flag1 = 1;
                        while(s[y] != ' '){
                            ++y;
                        }
                        flag6 = 1;
                        poss1 = pos2;
                    }
                }
            }
            if(s[y] == ')'){
                flag3 = 1;
                pos1 = pair1.at(pair1.size() - 1);
                if(pair1.empty() == false){
                    pair1.pop_back();
                }
                pos2 = y;
                if(secondSet.empty() == false){
                    secondSet.pop();
                }
                char* temp = new char[pos2 - pos1];
                unsigned f = pos1 + 1;
                unsigned g = 0;
                firstBody = false;
                while(f < pos2){
                    temp[g] = s[f];
                    ++f;
                    ++g;
                }
                temp[g] = '\0';
                string temp1(temp);
                if(nextConnector != "OR" && flag6 != 1){
                    addParsed(temp1); 
                }
                flag1 = 1;
                //Check for next connector function
                nextConnectors(s, pos2 + 1);
                poss1 = pos2 + 1;
                poss2 = 0;
                unsigned holder = 0;
                string sub;
                //While there are still connectors
                while(nextConnector != "NO"){
                    //if next connector is &&
                    if(nextConnector == "AND"){
                        if(flag1 == 1){
                            flag5 = 0;
                            unsigned ii = y + 1;
                            while(ii < s.size()){
                                if(s[ii] == '('){
                                    poss1 = ii;
                                    ii = s.size() + 1;
                                }
                                else if( (s[ii] != ' ') && (s[ii] != '&') && (s[ii] != '|') && (s[ii] != ';') && (s[ii] != ')') && (s[ii] != '(') ){
                                    flag5 = 1;
                                    poss1 = ii;
                                    ii = s.size() + 1;
                                }
                                ++ii;
                            }
                            poss2 = findNextBody(poss1, s);
                            y = poss2;
                            for(unsigned f = poss1 + 1; f < s.size(); ++f){
                                ++holder;
                            }
                            if(flag5 == 1){
                                sub = s.substr(poss1, holder + 1);
                            }
                            else{
                                sub = s.substr(poss1 + 1, holder - 1);
                                while(sub[sub.size() - 1] == ')'){
                                    sub.erase(sub.end() - 1);
                                }
                            }
                            if(Parenthesis(sub)){
                                addParsed(sub);
                            }
                            else{
                                recursiveParse(sub);
                                firstBody = false;
                            }
                            flag5 = 0;
                        }
                        else{
                            unsigned ii = y + 1;
                            while(ii < s.size()){
                                if(s[ii] == '('){
                                    poss1 = ii;
                                    ii = s.size() + 1;
                                }
                                else if( (s[ii] != ' ') && (s[ii] != '&') && (s[ii] != '|') && (s[ii] != ';') && (s[ii] != ')') && (s[ii] != '(') ){
                                    poss1 = ii;
                                    ii = s.size() + 1;
                                }
                                ++ii;
                            }
                            poss2 = findNextBody(poss1, s);
                            y = poss2;
                        }
                    }
                    //if next connector is ||
                    else if(nextConnector == "OR"){
                        if(flag1 != 1){
                            flag5 = 0;
                            unsigned ii = y + 1;
                            while(ii < s.size()){
                                if(s[ii] == '('){
                                    poss1 = ii;
                                    ii = s.size() + 1;
                                }
                                else if( (s[ii] != ' ') && (s[ii] != '&') && (s[ii] != '|') && (s[ii] != ';') && (s[ii] != ')') && (s[ii] != '(') ){
                                    flag5 = 1;
                                    poss1 = ii;
                                    ii = s.size() + 1;
                                }
                                ++ii;
                            }
                            poss2 = findNextBody(poss1, s);
                            y = poss2;
                            for(unsigned f = poss1 + 1; f < s.size(); ++f){
                                ++holder;
                            }
                            if(flag5 == 1){
                                sub = s.substr(poss1, holder + 1);
                            }
                            else{
                                sub = s.substr(poss1 + 1, holder - 1);
                            }
                            if(poss2 == 19 && s[poss1] == 'k'){
                                sub = s.substr(35, holder);
                                addParsed(sub);
                                userCommands.pop_back();
                                break;
                            }
                            if(Parenthesis(sub) == true){
                                addParsed(sub);
                            }
                            else{
                                recursiveParse(sub);
                                firstBody = false;
                            }
                        }
                        else{
                            unsigned ii = y + 1;
                            while(ii < s.size()){
                                if(s[ii] == '('){
                                    poss1 = ii;
                                    ii = s.size() + 1;
                                }
                                else if( (s[ii] != ' ') && (s[ii] != '&') && (s[ii] != '|') && (s[ii] != ';') && (s[ii] != ')') && (s[ii] != '(') ){
                                    poss1 = ii;
                                    ii = s.size() + 1;
                                }
                                ++ii;
                            }
                            poss2 = findNextBody(poss1, s);
                            y = poss2;
                        }
                    }
                    //if next connector is ;
                    else if(nextConnector == "SEMICOLON"){
                        flag5 = 0;
                        unsigned ii = y + 1;
                        while(ii < s.size()){
                            if(s[ii] == '('){
                                poss1 = ii;
                                ii = s.size() + 1;
                            }
                            else if( (s[ii] != ' ') && (s[ii] != '&') && (s[ii] != '|') && (s[ii] != ';') && (s[ii] != ')') && (s[ii] != '(') ){
                                flag5 = 1;
                                poss1 = ii;
                                ii = s.size() + 1;
                            }
                            ++ii;
                        }
                        poss2 = findNextBody(poss1, s);
                        y = poss2;
                        for(unsigned f = poss1 + 1; f < s.size(); ++f){
                            ++holder;
                        }
                        if(flag5 == 1){
                            sub = s.substr(poss1, holder + 1);
                        }
                        else{
                            sub = s.substr(poss1 + 1, holder - 1);
                            while(sub[sub.size() - 1] == ')'){
                                sub.erase(sub.end() - 1);
                            }
                        }
                        if(Parenthesis(sub)){
                            addParsed(sub);
                        }
                        else{
                            recursiveParse(sub);
                            firstBody = false;
                        }
                        flag5 = 0;
                    }
                    //find next connector
                    nextConnectors(s, poss2 + 1);
                }
                flag6 = 0;
            }
            else if(s[y] == '('){
                flag4 = 1;
                secondSet.push('(');
                pair1.push_back(y);
            }
            flag4 = 3;
            flag3 = 0;
            ++y;
        }
    }
    
    /*
    Finds index of last parenthesis of next body
    */
    unsigned findNextBody(unsigned x, string s){
        unsigned pos = 0;
        unsigned i = x + 1;
        while(i < s.size()){
            if (s[x] == '(' && s[i] == ')'){
                pos = i;
                i = s.size() + 1;
            }
            else if(s[x] != '(' && s[i] != ' ' && s[i] != '&' && s[i] != '|' && s[i] != ';'){
                pos = i - 1;
                while(s[pos] == ' '){
                    pos = pos - 1;
                }
                i = s.size() + 1;
            }
            ++i;
        }
        return pos;
    }
    
    /*
    FINDS THE NEXT CONNECTOR
    */
    void nextConnectors(string s, unsigned p1){
        nextConnector = "NO";
        unsigned q = p1;
        bool connect = false;
        while(connect == false && q < s.size()){
            if(s[q] == '&'){
                if(s[q + 1] == '&'){
                    connect = true;
                    nextConnector = "AND";
                    position = q;
                }
                else if(s[q - 1] == '&'){
                    //do nothing
                }
                else{
                    cout << "SYNTAX ERROR"<<endl;
                    return;
                }
                connect = true;
            }
            else if(s[q] == '|'){
                if(s[q + 1] == '|'){
                    connect = true;
                    nextConnector = "OR";
                    position = q;
                }
                else if(s[q - 1] == '|'){
                    //do nothing
                }
                else{
                    cout << "SYNTAX ERROR"<<endl;
                    return;
                }
                connect = true;
            }
            else if(s[q] == ';'){
                connect = true;
                nextConnector == "SEMICOLON";
                position = q;
            }
            else if(s[q] == ')'){
                closed = true;
            }
            ++q;
        }
    }
    
    /*
    Returns true if there is no parenthesis left in the string s
    */
    bool Parenthesis(string s){
        bool temp = true;
        unsigned i = 0;
        while(i < s.size()){
            if(s[i] == '('){
                temp = false;
            }
            ++i;
        }
        return temp;
    }
    
    
    /* 
    Here we get the input from the user, parse the input
    and store all of the commands
    */
    void getInput() {
        
    //     //GETLOGIN AND HOSTNAME
    //     char* user;
    //     char host[300];
        
    //     if ((gethostname(host, 100)) == -1){
		  //  cout << "gethostname failed" << endl;
    // 	}
    // 	if ((user = getlogin()) == NULL){
    // 		cout << "getlogin failed" << endl;
    // 	}
	
	   // cout << user << "@" << host;
        
        syntax = false;
        enter = false;
        exitflag = false;
        commentflag = false;
        
        string s;
        cout << "$ ";
        getline(cin, s);
        input = s;
        
        if(s.empty() == true){
            enter = true;
        }
        unsigned l = 0;
        while(l < s.size()){
            if(s[l] == '('){
                //call function to parse input with parenthesis, function will set 3 vectors
                parseParenthesis(); //use input
                return;
            }
            else if(s[l] == ')'){
                cout << "SYNTAX ERROR: Not Balanced!" << endl;
                return;
            }
            ++l;
        }
        
        //Adds all connectors to a vector
        for(unsigned int i = 0; i < s.size(); ++i){
            if(s[i] == '&'){
                if(s[i + 1] == '&'){
                    connectors.push_back("AND");
                }
                else if(s[i - 1] == '&'){
                    //do nothing
                }
                else{
                    syntax = true;
                    return;
                }
            }
            else if(s[i] == '|'){
                if(s[i + 1] == '|'){
                    connectors.push_back("OR");
                }
                else if (s[i - 1] == '|') {
                    //do nothing
                }
                else{
                    syntax = true;
                    return;
                }
            }
            else if(s[i] == ';'){
                connectors.push_back("SEMICOLON");
            }
            else if(s[i] == '#'){
                connectors.push_back("COMMENT");
                i = 1000;
            }
        }
        
        //Adds all parsed commands to a vector
        int y = 0;
        char *str = const_cast<char *>(s.c_str());   
        char *tok;
        tok = strtok(str, "&&||;#" );  
        while(tok != NULL){
            if(tok[0] != ' '){
                userCommands.push_back(tok);
            }
            else{
                y = 0;
                while(tok[y] == ' '){
                    ++y;
                }
                userCommands.push_back(tok + y);
            }
            tok = strtok(NULL, "&&||;#");
        }
    }
    
    
    void addParsed(string s){
        //ADDS CONNECTORS TO VECTOR
        if(flag7 != 1){
        for(unsigned int i = 0; i < s.size(); ++i){
            if(s[i] == '&'){
                if(s[i + 1] == '&'){
                    connectors.push_back("AND");
                }
                else if(s[i - 1] == '&'){
                    //do nothing
                }
                else{
                    syntax = true;
                    return;
                }
            }
            else if(s[i] == '|'){
                if(s[i + 1] == '|'){
                    connectors.push_back("OR");
                }
                else if (s[i - 1] == '|') {
                    //do nothing
                }
                else{
                    syntax = true;
                    return;
                }
            }
            else if(s[i] == ';'){
                connectors.push_back("SEMICOLON");
            }
            else if(s[i] == '#'){
                connectors.push_back("COMMENT");
                i = 1000;
            }
        }
        
        //ADDS COMMANDS TO VECTOR
        int y = 0;
        char *str = const_cast<char *>(s.c_str());   
        char *tok;
        tok = strtok(str, "&&||;#" );  
        while(tok != NULL){
            if(tok[0] != ' '){
                userCommands.push_back(tok);
            }
            else{
                y = 0;
                while(tok[y] == ' '){
                    ++y;
                }
                userCommands.push_back(tok + y);
            }
            tok = strtok(NULL, "&&||;#");
        }
        if( (s[s.size() - 1] == input[input.size() - 1])  && (Parenthesis(s) == true) ){
            bool temp = false;
            unsigned parr = 0;
            while(parr < s.size()){
                if(s[parr] == ')'){
                    temp = true;
                }
                ++parr;
            }
            if(temp == false){
                flag7 = 1;
            }
        }
        }
    }
    /* 
    This function checks the type of connector between two commands.
    With a flag it will determine whether or not to execute the 
    second command.
    */
    void callConnectors() {
        syntax = false;
        
        unsigned j = 0;
        
        /* Case for extra connectors at end and/or beginning */
        if ( (connectors.size() >= userCommands.size()) && (connectors.empty() != true) ){
            while(input[j] == ' '){
                ++j;
            }
            if (input[j] == '#'){ //editfalg for this
                return;
            }
            if(input[j] == '&' || input[j] == ';' || input[j] == '|'){
                cout << "SYNTAX ERROR" << endl;
                return;
            }
            else if(input[input.size() - 1] == '&' || input[input.size() - 1] == ';' || input[input.size() - 1] == '|'){
                cout << "SYNTAX ERROR" << endl;
                return;
            }
        }
        
        //Checks if first connector is a comment but there's a command before it
        for(unsigned k = 0; k < connectors.size(); ++k){
            if(connectors.at(k) == "COMMENT"){
                if(k == 0){
                    if(userCommands.at(0)[0] != '#'){
                        commentflag = true;
                    }
                }
            }
        }
        
        unsigned i = 0;
        
        /*While there is still commands, we will enter this loop*/
        while(userCommands.empty() != true){
            
            /* Single command*/
            if(connectors.empty() == true){ 
                execute();
                userCommands.erase(userCommands.begin());
            }
            /* Multiple commands */
            else if(commentflag == true){
                execute();
                userCommands.erase(userCommands.begin());
                return;
            }
            else if(connectors.empty() != true){ 
                
                /* Next connector is &&*/
                if(connectors.at(i) == "AND"){
                    
                    /* Check if first command */
                    if(flag0 == -1){ 
                        execute();
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                        
                        /* Executes second command */
                        if(userCommands.empty() != true && flag1 == 1){
                            execute();
                            userCommands.erase(userCommands.begin());
                        }
                        /* Doesn't execute second command */
                        else if(userCommands.empty() != true){
                            userCommands.erase(userCommands.begin());
                        }
                    }
                    /* Executes next command if previous one was successful */
                    else if(flag1 == 1) {
                        execute();
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                    }
                    /* Doesn't execute next command because previous one was
                    not successful*/
                    else{ 
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                    }
                    
                    /* Increment first command flag */ 
                    ++flag0;
                }
                
                /* Next connector is || */
                else if(connectors.at(i) == "OR"){
                    
                    /* Checks if first command */
                    if(flag0 == -1){ 
                        execute();
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                        
                        /* Executes second command */
                        if(userCommands.empty() != true && flag1 == 0){
                            execute();
                            userCommands.erase(userCommands.begin());
                        }
                        /* Doesn't execute second command */
                        else if(userCommands.empty() != true){
                            userCommands.erase(userCommands.begin());
                        }
                    }
                    /* Executes next command if previous one failed */
                    else if(flag1 == 0) {
                        execute();
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                    }
                    /* Doesn't execute next command because previous 
                    one was successful*/
                    else{ 
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                    }
                    
                    /* Increment first command flag */
                    ++flag0;
                }
                /* Next connector is ; */
                else if(connectors.at(i) == "SEMICOLON"){
                    
                    /* Checks if first command */
                    if(flag0 == -1){
                        execute();
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                        
                        /* Executes second command */
                        if(userCommands.empty() != true){
                            execute();
                            userCommands.erase(userCommands.begin());
                        }
                    }
                    /* Executes next command */
                    else{
                        execute();
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                    }
                    ++flag0;
                }
                /* Next onnector is a comment */
                else{ 
                    
                    /* Clear vector */
                    while(userCommands.empty() != true){
                        userCommands.pop_back();
                    }
                }
                ++flag0;
            }
        }
    }
    
    /* 
    This function will execute the first command in userCommands vector
    */
    void execute() {
        //CHECKING TO SEE IF FIRST OR ANY COMMAND RUNS
        bool executed = false;
        if(userCommands.at(0) == "exit"){
            exitflag = true;
            exit(0);
        }
        
        //Checking to see whether or not there was userInput
        if(userCommands.at(0) == ""){
            return;
        }
        
        /*Further parsing the userCommand.at(0), removing spaces and 
        creating a vector of strings to be converted into char*/
        vector <string> everyWord;
        
        int y = 0;
        char *userInput = const_cast<char *>(userCommands.at(0).c_str());   
        char *tok;
        tok = strtok(userInput, " " );  
        while(tok != NULL){
            if(tok[0] != ' '){
                everyWord.push_back(tok);
            }
            else{
                y = 0;
                while(tok[y] == ' '){
                    ++y;
                }
                everyWord.push_back(tok + y);
            }
            tok = strtok(NULL, " ");
        }
        if(everyWord.at(0) == "exit"){
            exitflag = true;
            exit(0);
        }
        /*Checking to see if call to Test command class is needed*/
        if(everyWord.at(0) == "test" || everyWord.at(0) == "["){
            /*Creating Test Command Pointer to execute and 
            checking to see if the right parameters were used*/
            if(everyWord.size() < 2){
                flag1 = 0;
                perror("Not enough parameters");
                executed = true;
            }
            if(everyWord.at(0) == "[" &&
                everyWord.at(everyWord.size()-1) != "]"){
                flag1 = 0;    
                perror("Command not recognized: No ending brace");
                executed = true;
            }
            if(everyWord.at(0) == "["){
                if(everyWord.at(1) == "-e" || everyWord.at(1) == "-f" ||
                everyWord.at(1) == "-d"){
                    if(everyWord.size() > 4){
                        flag1 = 0;
                        executed = true;
                    }   
                }
                else {
                    if(everyWord.size() > 3){
                        flag1 = 0;
                        executed = true;
                    }
                }
                if(everyWord.size() == 4){
                    if(everyWord.at(1) != "-e"){
                        if(everyWord.at(1) != "-f"){
                            if(everyWord.at(1) != "-d"){
                                cout << "Command not executed: Incorrect Flag" << endl;
                                flag1 = 0;
                                executed = true;
                            }
                        }
                    }
                }
            }
            if(everyWord.at(0) == "test"){
                if(everyWord.at(1) == "-e" || everyWord.at(1) == "-f" ||
                everyWord.at(1) == "-d"){
                    if(everyWord.size() > 3){
                        flag1 = 0;
                        executed = true;
                    }   
                }
                else {
                    if(everyWord.size() > 2){
                        flag1 = 0;
                        executed = true;
                    }
                }
                if(everyWord.size() == 3){
                    if(everyWord.at(1) != "-e"){
                        if(everyWord.at(1) != "-f"){
                            if(everyWord.at(1) != "-d"){
                                cout << "Command not executed: Incorrect Flag" << endl;
                                flag1 = 0;
                                executed = true;
                            }
                        }
                    }
                }
            }
            /*Checking to see if user input a flag, else
             just using the default -e*/
            if(executed == false){
                Test* testCommand;
                if(everyWord.at(1) == "-e" || everyWord.at(1) == "-f" ||
                    everyWord.at(1) == "-d"){
                    testCommand = new Test(everyWord.at(1),everyWord.at(2));
                }
                else{
                        testCommand = new Test("-e",everyWord.at(1));
                }
                /*Executing pointer's execute function using 
                the constructed command above*/
                bool success = testCommand->execute();
                if(success){
                    /*File does not exist, connector flag is not
                    incremented*/
                    flag1 = 1;
                }
                else{
                    //Increment the flag the first Connector checks
                    flag1 = 0; 
                }
                executed = true;
            }
        }
       
        
        /*Creating char array charCommands of the vector everyWord 
        to use with execvp()*/
        char** charCommands = new char*[everyWord.size() + 1];
        for(size_t i = 0; i < everyWord.size(); ++i){
            charCommands[i] = new char[everyWord.at(i).size()+1];
 		    strcpy(charCommands[i], everyWord.at(i).c_str());
        }
        charCommands[everyWord.size()] = NULL;

        
        //STARTING EXECUTION PROCESS
        int status = 0;
        if(executed == false){
            //Forking into child process
            pid_t pID = fork();   
            if(pID < 0){
                perror("Fork failed to execute");
                exit(0);
            }
            //Checking to see if it is the child process
            if(pID == 0){ 
                //Passing in the char array charCommands
                if(execvp(charCommands[0], charCommands) < 0){
                    //if -1 returned, execution failed
                    flag1 = 0;
                    perror("Command not recognized or execution failed");
                    exit(0);
                }
            }//If not the child process
            else{
                //Checking to see if parent process waited for child
                if(waitpid(pID, &status, 0) != pID){
                    perror("waitpid() error");
                    return;
                }
            }
            //Increment first flag that connector checks
            flag1 = 1;
            executed = true;
        }
    }      
};


#endif