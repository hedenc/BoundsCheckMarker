A runtime bounds checks marker for Softbound+CETS

It marks the lines which Softbound+CETS inserts runtime bounds checks.

Compile: make marker

Invocation: ./marker BINARY_FILE

The runtime bounds checker depends on line-info in the binaries, and currently can't track inlined instances of the runtime checks. So to compile binaries using Softbound+CETS that this tool supports it has to be compile without link-time-optimization and with debug line info.

I.e: clang -fsoftboundcets -L/SOFTBOUND_LIBRARY_DIR -lm -lrt -g SOURCE_FILE -o BINARY_FILE

For the Softbound+CETS project and credits to the authors of this cool tool:
https://www.cs.rutgers.edu/~santosh.nagarakatte/softbound/

