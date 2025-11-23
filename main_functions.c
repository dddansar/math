//------------------------------------------------------------------------------
// Title      : Main functions for the math project
// Project    : A math project
//------------------------------------------------------------------------------
// File       : main_functions.c
// Author     : Danny Sarraf
//------------------------------------------------------------------------------
// Description:
//    performs the main functionality of the math project
//------------------------------------------------------------------------------
#include "./math.h"

//------------------------------------------------------------------------------
// Function: v_main_functions
// Isolates the main functions from the main file and from support functions
// such as v_main_initialization() and v_main_finalization() for benchmarking
//------------------------------------------------------------------------------
void v_main_functions() {
   // printf("Hello World\n");
   struct timespec t_timespec_st;
   clock_gettime(CLOCK_MONOTONIC_RAW, &t_timespec_st);
}

