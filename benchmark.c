//------------------------------------------------------------------------------
// Title      : Benchmarking code for the math project
// Project    : A math project
//------------------------------------------------------------------------------
// File       : benchmark.c
// Author     : Danny Sarraf
//------------------------------------------------------------------------------
// Description:
//    Perform benchmarking on selected functions of the math prover.
//------------------------------------------------------------------------------
// Usage:
//    To compile: gcc -O3 -o benchmark_gcc.out benchmark.c main_functions.c
//    To run: ./benchmark_gcc.out
//
// Examples:
//    ./benchmark_gcc.out
//------------------------------------------------------------------------------
#include "./benchmark.h"
#include "./math.h"

//------------------------------------------------------------------------------
// Function: main
//
//------------------------------------------------------------------------------
int main(void) {

   v_benchmarking(&v_main_functions);
   return 0;
}


//------------------------------------------------------------------------------
// Function: v_benchmarking
//
//------------------------------------------------------------------------------
void v_benchmarking(void (*p_v_func)(void)) {

   unsigned int ui_seed;

   //------------------------------------------------
   // STEP 0: setup initial seed
   //------------------------------------------------
   printf("--------------------------------------------------------------------------------\n");
   printf("benchmark.c: Performing %d (%gM) Benchmark Tests on selected functions\n", (int) REPEAT_BENCH, REPEAT_BENCH/1e6);
   struct timespec t_timespec_start_time;
   clock_gettime(CLOCK_REALTIME, &t_timespec_start_time);
   ui_seed = (t_timespec_start_time).tv_nsec;
   srand(ui_seed);
   printf("benchmark.c: Seed = %u\n", ui_seed);
   printf("--------------------------------------------------------------------------------\n");

   //------------------------------------------------
   // STEP 1: setup any pre-benchmark code and randomization
   //------------------------------------------------
   // add benchmark specific code here if needed

   //------------------------------------------------
   // STEP 2: track benchmark start time
   //------------------------------------------------
   clock_gettime(CLOCK_MONOTONIC_RAW, &t_timespec_start_time);

   //------------------------------------------------
   // STEP 3: run the benchmark function REPEAT_BENCH number of times
   //------------------------------------------------
   // DO NOT add extra logic that can interfere with the benchmark test!!
   for (int repeat_test_num=0; repeat_test_num<REPEAT_BENCH; repeat_test_num++) {
      p_v_func();
   }

   //------------------------------------------------
   // STEP 4: Track benchmark end time in the same order as their start time
   //------------------------------------------------
   struct timespec t_timespec_end_time;
   clock_gettime(CLOCK_MONOTONIC_RAW, &t_timespec_end_time);

   //------------------------------------------------
   // STEP 5: print benchmark results
   //------------------------------------------------

   // 1) print total elapsed time
   double d_elapsed_time_sec;
   double d_clock_count;
   d_elapsed_time_sec = (double) (t_timespec_end_time.tv_sec  - t_timespec_start_time.tv_sec ) + \
                        (double) (t_timespec_end_time.tv_nsec - t_timespec_start_time.tv_nsec) / 1e9;
   d_clock_count = (d_elapsed_time_sec * CLK_FREQ);
   printf("benchmark.c: total elapsed time = %.9f seconds\n", d_elapsed_time_sec);

   // 2) print total clock count
   printf("benchmark.c: total clock count  = %.9f\n", d_clock_count);

   // 3) print average time per single function call
   printf("benchmark.c: time / call        = %.15f seconds ( %.6f ns)\n", d_elapsed_time_sec / REPEAT_BENCH, (d_elapsed_time_sec / REPEAT_BENCH) * 1e9);

   // 4) print average clock count per single function call
   printf("benchmark.c: clocks / call      = %.15f\n", d_clock_count / REPEAT_BENCH);

   printf("\n");

   //------------------------------------------------
   // STEP 6: handle any post-benchmark data
   //------------------------------------------------
   // add benchmark specific code here if needed
}

//------------------------------------------------------------------------------
// Function: v_wrapper_function
// Can be used if function under test takes in inputs
//------------------------------------------------------------------------------
void v_wrapper_function(void) {
   // i_benchmark_test(in1, in2);
}

