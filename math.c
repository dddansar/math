//------------------------------------------------------------------------------
// Title      : Main code for the math project
// Project    : A math project
//------------------------------------------------------------------------------
// File       : math.c
// Author     : Danny Sarraf
//------------------------------------------------------------------------------
// Description:
//    Process argument list if any and initialize the randomization seed
//    perform the main functionality of the project
//       TODO:
//    Wrap up the project with any final processing or messaging
//------------------------------------------------------------------------------
// Usage:
//    To compile: gcc -O3 -o math_gcc.out math.c main_functions.c
//    To run: ./math_gcc.out [options]
//
// Options:
//    -s=   Set the randomization seed, otherwise it's based on start time.
//    -h    Prints the help message. -help, --h, and --help also work.
//
// Examples:
//    ./math_gcc.out
//    ./math_gcc.out -s=123456789
//    ./math_gcc.out -s=$RANDOM
//    ./math_gcc.out -s=$RANDOM$RANDOM
//------------------------------------------------------------------------------
#include "./math.h"

//------------------------------------------------------------------------------
// Function: main
//
//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {

   v_main_initialization(argc, argv);

   v_main_functions();

   v_main_finalization();
   return 0;
}

//------------------------------------------------------------------------------
// Function: v_main_initialization
// Process argument list if any and initialize the randomization seed
//------------------------------------------------------------------------------
void v_main_initialization(int argc, char *argv[]) {

   int i_seed_count = 0;
   struct timespec t_timespec_start_time;
   uint32_t ui32_seed;
   char *p_c_argv_substring;
   char *p_c_time_string;

   clock_gettime(CLOCK_REALTIME, &t_timespec_start_time);

   p_c_time_string = ctime(&t_timespec_start_time.tv_sec);
   printf("===========================================================================\n");
   printf("Start time: %s", p_c_time_string);

   // print detected command
   printf("---------------------------------------------------------------------------\n");
   printf("Detected the following input command:\n  ");
   for (int i=0; i<argc; i++) {
      printf(" %s", argv[i]);
   }
   printf("\n");
   printf("Number of arguments detected argc = %d\n", argc);

   // process all argument options
   // printf("---------------------------------------------------------------------------\n");
   for (int i=0; i<argc; i++) {
      printf("argv[%d] = %s\n", i, argv[i]);
      if (i == 0) {
         continue;
      }

      // -s=
      // update the seed from the terminal if -s is detected
      if ((p_c_argv_substring = strstr(argv[i], "-s=")) != NULL && p_c_argv_substring == argv[i] \
            && *(p_c_argv_substring = p_c_argv_substring + ARG_OPTION_SIZE) != '\0') {

         // check if digit
         for (int j = 0; p_c_argv_substring[j] != '\0'; j++) {
            if (isdigit(p_c_argv_substring[j]) == 0) {
               printf("\n   ERROR: %s is NOT a digit!!!\n\n", p_c_argv_substring);
               exit(1); // indicates an error
            }
         }

         // check if number is greater than 4294967295 = 2^32 - 1, max val of uint32_t for ui32_seed
         if (strlen(p_c_argv_substring) > strlen(MAX_SEED_VALUE_STR) || (strlen(p_c_argv_substring) == strlen(MAX_SEED_VALUE_STR) && strcmp(p_c_argv_substring, MAX_SEED_VALUE_STR) > 0)) {
            printf("\n   ERROR: %s is too long, max seed value is %s = 2^%u - 1 !!!\n\n", p_c_argv_substring, MAX_SEED_VALUE_STR, MAX_SEED_BITS);
            exit(1); // indicates an error
         }

         printf("   detected -s option in argv[%d] = %s\n", i, p_c_argv_substring);
         ui32_seed = atoi(p_c_argv_substring);
         i_seed_count++;
         continue;
      }

      // -h
      // print help message
      if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "--h") == 0 || strcmp(argv[i], "--help") == 0) {
         printf("---------------------------------------------------------------------------\n");
         printf("Usage:\n");
         printf("   To compile: gcc -O3 -o math_gcc.out math.c main_functions.c\n");
         printf("   To run: ./math_gcc.out [options]\n");
         printf("\n");
         printf("Options:\n");
         printf("   -s=   Set the randomization seed manually, otherwise randomization is based on start time.\n");
         printf("   -h    Prints the help message. -help, --h, and --help also work. \n");
         printf("\n");
         printf("Examples:\n");
         printf("   ./math_gcc.out\n");
         printf("   ./math_gcc.out -s=123456789\n");
         printf("   ./math_gcc.out -s=$RANDOM\n");
         printf("   ./math_gcc.out -s=$RANDOM$RANDOM\n");
         printf("---------------------------------------------------------------------------\n");
         exit(0);
      }

      // Print warning that the option was not valid
      printf("\n   ERROR: invalid argument %s detected!!!\n\n", argv[i]);
      exit(1); // indicates an error
   }
   printf("---------------------------------------------------------------------------\n");

   if (i_seed_count == 0) {
      // Use the start time as a seed for reproducible randomness
      ui32_seed = (t_timespec_start_time).tv_nsec;
      printf("Generating random seed based on start time\n");
   } else if(i_seed_count == 1) {
      printf("Using manual seed from inputted arguments\n");
   } else {
      printf("\nERROR: Multiple seed values inputted!!!\n\n");
      exit(1); // indicates an error
   }

   // Seed the random number generator
   srand(ui32_seed);

   // Print the seed number
   printf("Seed = %u\n", ui32_seed);
   printf("---------------------------------------------------------------------------\n");
   printf("\n");
}



//------------------------------------------------------------------------------
// Function: v_main_finalization
// Wrap up the project with any final processing or messaging
//------------------------------------------------------------------------------
void v_main_finalization() {

   char *p_c_time_string;
   struct timespec t_timespec_end_time;
   clock_gettime(CLOCK_REALTIME, &t_timespec_end_time);

   // Print the seed number again
   printf("\n");
   printf("---------------------------------------------------------------------------\n");
   printf("Debug rand value 1 = %d\n",  rand());
   printf("Debug rand value 2 = %d\n",  rand());

   // print end time and compare to start time
   p_c_time_string = ctime(&t_timespec_end_time.tv_sec);
   printf("---------------------------------------------------------------------------\n");
   printf("End time: %s", p_c_time_string);
   printf("===========================================================================\n");
}

