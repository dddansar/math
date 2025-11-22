#!/bin/bash

# Exit script immediately if a command exits with a non-zero status (for example if compile fails)
set -e

# NOTE: -Werror: treat warnings as errors!
#       -Wall: enables all the warnings about constructions that some users consider questionable, and that are easy to avoid
#              (see man for a list of all the warnings enabled by Wall)
gcc -pedantic -Werror -W -Wall -o math_gcc.out math.c
# gcc -W -o math_gcc.out math.c

# NOTE: |& tee math.out.txt will cause the command line not to print anything until done.
#       to fix this, force line buffering instead of full buffering by adding: stdbuf -oL !!!
# $@ is used to pass the arguments list from math.sh to math_gcc.out
stdbuf -oL ./math_gcc.out $@  |& tee math.out.txt

