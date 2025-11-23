#!/bin/bash

# Exit script immediately if a command exits with a non-zero status (for example if compile fails)
set -e

#--------------------------------------------------------------------------------------------------------------
#                                        Variables, Settings and Flags
#--------------------------------------------------------------------------------------------------------------

PROJECT="benchmark"

COMPILE_AND_RUN=1
# PROFILER_VALGRIND=1
# PROFILER_PERF=1
# PROFILER_GPROF=1

# -O0 to not optimize. Most optimizations are completely disabled at -O0
# -O3 to enables the highest level of optimizations for speed, though it can increase code size.
# -Werror: treat warnings as errors!
# -Wall: enables all the warnings about constructions that some users consider questionable,
#     and that are easy to avoid (see man for a list of all the warnings enabled by Wall)
# -fno-exceptions to turn off exceptions.
# -fno-rtti Disable generation of information about every class with virtual functions for use by
#     the C++ run-time type identification
# -pedantic Issue all the warnings demanded by strict ISO C and ISO C++
# -pthread Adds support for multithreading with the pthreads library.
#     Programs using pthreads must include the pthread.h header file in their source code:
#     #include <pthread.h>
#     The most common and recommended way to compile and link pthreads programs with GCC is to use
#     the -pthread option.
#     This option handles both the necessary preprocessor definitions and the linking of the
#     pthreads library.
# -lm is for the math lib (ex: sqrt function)
# -fno-omit-frame-pointer instructs the compiler to generate code that maintains and uses a stack
#     frame pointer for all functions. The primary benefit of fno-omit-frame-pointer for
#     performance analysis is that it greatly simplifies stack unwinding for tools like perf.
#     When frame pointers are present, perf can easily traverse the call stack and accurately
#     reconstruct function call graphs, which is crucial for identifying performance bottlenecks.
#     Without frame pointers, perf may struggle to generate complete or accurate call graphs,
#     often relying on more complex and less reliable methods like DWARF unwinding or Last Branch
#     Records (LBR). NOTE: The trade-off for this improved profiling capability is a potential,
#     albeit usually small, performance overhead.

PRE_FLAGS=
PRE_FLAGS+=" -O3"
PRE_FLAGS+=" -Werror"
PRE_FLAGS+=" -Wall"
PRE_FLAGS+=" -fno-exceptions"
PRE_FLAGS+=" -pedantic"
# PRE_FLAGS+=" -o"

POST_FLAGS=
POST_FLAGS+=" main_functions.c"
# POST_FLAGS+=" -lm"

PIPE_FLAGS=


#--------------------------------------------------------------------------------------------------------------
#                                             Date and Time
#--------------------------------------------------------------------------------------------------------------
echo -n "${PROJECT}.sh: start time: "
date

#--------------------------------------------------------------------------------------------------------------
#                                            Compile and Run
#--------------------------------------------------------------------------------------------------------------
if [[ ${COMPILE_AND_RUN} == 1 ]]; then
   gcc $PRE_FLAGS -o ${PROJECT}_gcc.out ${PROJECT}.c $POST_FLAGS $PIPE_FLAGS
   ./${PROJECT}_gcc.out
fi

#--------------------------------------------------------------------------------------------------------------
#                                               Profilers
#--------------------------------------------------------------------------------------------------------------

# using the valgrind profiler
if [[ ${PROFILER_VALGRIND} == 1 ]]; then

   # will return total instructions executed, cache miss rate, and branch prediction fail rate.
   echo ""
   echo "${PROJECT}.sh: profiler: valgrind cachegrind"
   # Use the Cachegrind tool to profile and log CPU cache operations of program:
   valgrind --tool=cachegrind --branch-sim=yes ./${PROJECT}_gcc.out

   # Use Memcheck to report all possible memory leaks of program in full detail:
   # valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose -s --log-file=valgrind-out ./${PROJECT}_gcc.out
   echo ""
   echo "${PROJECT}.sh: profiler: valgrind leak-check"
   valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose -s ./${PROJECT}_gcc.out

   # Use the Massif tool to profile and log heap memory and stack usage of program:
   echo ""
   echo "${PROJECT}.sh: profiler: valgrind massif"
   valgrind --tool=massif --stacks=yes --verbose ./${PROJECT}_gcc.out
fi

# using the perf profiler
if [[ ${PROFILER_PERF} == 1 ]]; then
   echo ""
   echo "${PROJECT}.sh: profiler: perf gcc compile"

   # added -fno-omit-frame-pointer
   gcc $PRE_FLAGS -fno-omit-frame-pointer -o ${PROJECT}_gcc.out ${PROJECT}.c $POST_FLAGS $PIPE_FLAGS

   echo ""
   echo "${PROJECT}.sh: profiler: perf stat"
   # perf stat -d ./${PROJECT}_gcc.out
   perf stat -e task-clock,context-switches,cpu-migrations,page-faults,cycles,instructions,branches,branch-misses,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses,r1b1,r10e,stalled-cycles-frontend,stalled-cycles-backend,L1-dcache-load-misses,cache-misses ./${PROJECT}_gcc.out

   # NOTE: Run a command and record its profile into perf.data:
   #       shows function overhead, what % of time is spent in each...
   echo ""
   echo "${PROJECT}.sh: profiler: perf record"
   # perf record ./${PROJECT}_gcc.out
   perf record -g ./${PROJECT}_gcc.out

   # Read perf.data (created by perf record) and display the profile:
   # perf report
   # perf report -g
   # perf report -g 'graph,0.5,caller'
   # press + or e to expand! and E/C to expand/collaps all!!
   # press a to annotate function and see it in assembly
fi

# To use gprof, compile your C code with the -pg flag.
if [[ ${PROFILER_GPROF} == 1 ]]; then
   echo ""
   echo "${PROJECT}.sh: profiler: gmon"
   gcc -pg -O3 ${PROJECT}.c $POST_FLAGS -o ${PROJECT}_gcc.out
   ./${PROJECT}_gcc.out # this generates gmon.out
   gprof ${PROJECT}_gcc.out gmon.out
fi

#--------------------------------------------------------------------------------------------------------------
#--------------------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------------------
#                                               Deguggers
#--------------------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------------------
#                                             Date and Time
#--------------------------------------------------------------------------------------------------------------
echo -n "${PROJECT}.sh: end time: "
date
