GCC version 4.9 supports version 4.0 of OpenMP[\[1\]](https://software.intel.com/en-us/articles/gcc-49-openmp-code-cannot-be-linked-with-intel-openmp-runtime). This makes any code compiled with OMP before GCC 4.9 binary uncompatable with code compiled under 4.9 or higher. One error cause by this is something similar to `Undefined "GOMP_parallel"`.

To resolve this error, you can either downgrade your compiler to GCC 4.8 or earlier, or recompile all OMP code under GCC 4.9 or later.
