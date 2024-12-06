set shell := ["cmd.exe", "/c"]

CC := "gcc"
CC_FLAGS := "-Wall -Wextra"
ENTRY := "main.c"
BUILD := "main.exe"
ENTRY2 := "main2.c"
BUILD2 := "main2.exe"

INPUT_FILE := "input.txt"
TEST_FILE := "mock.txt"

build:
  {{CC}} -o {{BUILD}} {{ENTRY}} {{CC_FLAGS}}

run: build
  {{BUILD}} {{INPUT_FILE}}

test: build
  {{BUILD}} {{TEST_FILE}}


build2:
  {{CC}} -o {{BUILD2}} {{ENTRY2}} {{CC_FLAGS}}

run2: build2
  {{BUILD2}} {{INPUT_FILE}}

test2: build2
  {{BUILD2}} {{TEST_FILE}}
