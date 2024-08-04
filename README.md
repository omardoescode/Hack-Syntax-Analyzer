# Jack Syntax Analyzer

This repository contains the Hack computer syntax analyzer, which is used by the Hack compiler to generate VM code. This project is part of Project 10 in the Nand2Tetris course.

# Structure

- `HackMap`: Where all information about the Jack language such as keywords, data about tokens, non terminal rules are stored.
- `Tokenizer`: Breaks down input into tokens.
- `CompilationEngine`: Processes tokens and generates corresponding VM code.
- `OutputEngine`: An abstract class for any output engine, that is, the output of the analyzing process
- `XMLOutputEngine`: A subclass of the `OutputEngine` that translates code into XML.
- `JackSyntaxAnalyzer`: A wrapper for the whole program.

# Usage

1. Clone the repository
2. Compilation

```bash
cd Hack-Syntax-Analyzer
mkdir build
cmake -S . -B ./build
cmake --build ./build --target analyzer
```

3. Usage

```cpp
./build/main <file_or_directory_path>
```

This will generate corresponding XML files in the same path that show the syntax of the code. An example test is found in `tests` directory

# Extend this program

Output engine in this class is an abstract class that is extendable to generate more output engines, and this output engine is injected as an argument to the analyzer

```cpp
#include <OutputEngine.h>
class YAMLOutputEngine: public OutputEngine {
    // code goes here
}

/////////////////////////////////////
```
