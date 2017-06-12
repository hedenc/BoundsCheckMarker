# BoundsCheckMarker

Code should compile given any standard c++14 compliant compiler

Program expects exported llvm IR-file produced by Softbound.
To emit llvm code use -S -emit-llvm flags and for compiling against Softbound
follow the user guide at http://sva.cs.illinois.edu/docs/UsersGuide.html.

Example:

clang -g -fmemsafety -S -emit-llvm -o OUTPUT_FILE INPUT_FILE

Make all should produce example output in Examples/output.txt of the marker run 
on Examples/safe_test.s which is generated from Examples/test.c with Softbound.

Set the path for the Softbound modified clang in the Makefile.

Added a bounds check marker for binary files working currently on MacOSX

It's invoked by passing the binary file.
Code is in the binmarker directory and examples in binmarker/Examples

