#!/bin/bash

# Exit script immediately if a command exits with a non-zero status (for example if compile fails)
set -e

#-------------------------------------------------------------------------------
#                        Variables, Settings and Flags
#-------------------------------------------------------------------------------

PROJECT="math"
POST_FLAGS=
POST_FLAGS+=" main_functions.c"
# GEN_ASSEMBLY=1

#-------------------------------------------------------------------------------
#                                   Compile
#-------------------------------------------------------------------------------

# -O0 to not optimize. Most optimizations are completely disabled at -O0
# -O3 to enables the highest level of optimizations for speed, though it can increase code size.
# -Werror: treat warnings as errors!
# -Wall: enables all the warnings about constructions that some users consider questionable,
#     and that are easy to avoid (see man for a list of all the warnings enabled by Wall)
# -pedantic Issue all the warnings demanded by strict ISO C and ISO C++
gcc -O3 -pedantic -Werror -Wall -o ${PROJECT}_gcc.out ${PROJECT}.c ${POST_FLAGS}
# gcc -O3 -o ${PROJECT}_gcc.out ${PROJECT}.c ${POST_FLAGS}

#-------------------------------------------------------------------------------
#                              Generate Assembly
#-------------------------------------------------------------------------------

if [[ ${GEN_ASSEMBLY} == 1 ]]; then
   # 1) generate the assembly code from the binary, and replace tabs with space
   # Other useful objdump options include -rwC (to show symbol relocations, disable line-wrapping of long machine code, and demangle C++ names). And if you don't like AT&T syntax for x86, -Mintel.
   objdump -drwC -Mintel -rwC ${PROJECT}_gcc.out > ${PROJECT}_gcc.objdump
   expand -t 3 ${PROJECT}_gcc.objdump > ${PROJECT}_gcc.objdump.tab
   mv ${PROJECT}_gcc.objdump.tab ${PROJECT}_gcc.objdump
   
   # 2) generate the assembly code from the .c file with gcc
   # AT&T Syntax: 
   # gcc -O3 -S -fverbose-asm ${PROJECT}.c ${POST_FLAGS}
   # expand -t 3 ${PROJECT}_gcc.asm > ${PROJECT}_gcc.asm.tab
   # mv ${PROJECT}_gcc.asm.tab ${PROJECT}_gcc.asm
   
   # Intel Syntax:
   gcc -O3 -S -fverbose-asm -masm=intel ${PROJECT}.c ${POST_FLAGS}
   # Use the -Wa,-adhln option to generate assembly with mixed source code, machine code, and assembly instructions all in one:
   # gcc -O3 -g -Wa,-adhln ${PROJECT}.c ${POST_FLAGS} > ${PROJECT}_gcc.asm
   # Detailed annotated + verbose assembly
   # gcc -O3 -g -Wa,-adhln -fverbose-asm ${PROJECT}.c ${POST_FLAGS} > ${PROJECT}_gcc.asm
   expand -t 3 ${PROJECT}_gcc.asm > ${PROJECT}_gcc.asm.tab
   mv ${PROJECT}_gcc.asm.tab ${PROJECT}_gcc.asm
   
   # The -save-temps option can be used to generate all intermediate files created during the compilation process, including the preprocessed file (.i), the assembly file (.s), and the object file (.o).
   # gcc -O3 -save-temps ${PROJECT}.c ${POST_FLAGS}
fi

#-------------------------------------------------------------------------------
#                                     Run Code
#-------------------------------------------------------------------------------

# NOTE: |& tee ${PROJECT}.out.txt will cause the command line not to print anything until done.
#       to fix this, force line buffering instead of full buffering by adding: stdbuf -oL !!!
# $@ is used to pass the arguments list from ${PROJECT}.sh to ${PROJECT}_gcc.out
stdbuf -oL ./${PROJECT}_gcc.out $@  |& tee ${PROJECT}.out.txt

