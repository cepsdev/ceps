# cepsvalidate for Windows

## Requirements

* Visual Studio 2019 (Community Edition and better)
* Windows port of bison 3.x executable accessible through active path settings (change run_bison.bat if you want to deviate from the default settings) 

__Remarks:__
* A bison port for windows can be found here: https://sourceforge.net/projects/winflexbison/ .
* The solution defines a user defined build tool which calls a batch file (__run_bison.bat__) to run bison. 
* __run_bison.bat__  expects the binary __bison.exe__ to be in its search path.
* If you change your global path settings for __bison.exe__ to be found by __run_bison.bat__ you have to restart the Visual Studio IDE (if you intend to build via the VS IDE).    
* Should compile with any MSVC++ compiler supporting C++17,. i.e. other VS versions should work as well. 
