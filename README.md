# BoundsCheckMarker

Code should compile given:
    *   Any standard c++14 compliant compiler
    *   A python3 interpreter

Program expects exported llvm IR-file produced by Softbound.
To emit llvm code use -S -emit-llvm flags and for compiling against Softbound
follow the user guide at http://sva.cs.illinois.edu/docs/UsersGuide.html.

Example:

clang -g -fmemsafety -S -emit-llvm -o OUTPUT_FILE INPUT_FILE
