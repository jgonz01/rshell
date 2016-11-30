### rshell

--------------

#UCR CS100 Assignment
Fall 2016

---------------------

Description of Command Shell 

- Basic and limited terminal

- Executes user's command input 

+ Does not read in or execute comments following # symbol

- Accepts Command Chaining 'echo name && mkdir test ; ls'
  + '&&': Executes previous then the latter command if previous is successful
  + '||': Executes following command only if the previous fails
  + ';' : Executes following command no matter what

+ Displays error messages if failed execution at any point

+ Added test command implementation, runs with keyword "test" or the bracket symbols as well
  + flag "-e" to check if the file or directory exists (default flag)
  + flag "-f" to check if the file or directory exists and is a regular file
  + flag "-d" to check if the file or directory exists and is a directory
  + example: "test -e Test.h" or "[ Command.h ]
  + Command chaining is compatible
    - "test -e main.cpp && echo taco"

+ Use of precedence using parentheses
  - Separate sets can be used "(echo hello && echo name) || (echo world && echo llama)"
  - Nested parentheses can be used "(echo hey && (echo this || echo yes))"

- Exits the terminal using the command 'exit'

+ Added cd command implementation
  + may use cd, cd -, or cd <path>

# Known Bugs
- The command following || is not executed if the first command is not recognize 

- Include quotation marks when echoing

- No variations of the word exit can be used 'Exit' or 'EXIT'

- No built in bash commands work

- Commands are still executed if user enters single connectors '&' '|'

