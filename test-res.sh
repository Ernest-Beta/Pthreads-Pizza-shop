#!/bin/bash


SOURCE_FILE="p3220239-p3220133-pizza.c"
HEADER_FILE="p3220239-p3220133-pizza.h"
OUTPUT_FILE="pizzaria"
NUM_CUSTOMERS=100
SEED=10


gcc -o $OUTPUT_FILE $SOURCE_FILE -pthread


if [ $? -eq 0 ]; then
  echo "Compilation successful."
else
  echo "Compilation failed."
  exit 1
fi


./$OUTPUT_FILE $NUM_CUSTOMERS $SEED
