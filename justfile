set shell := ["cmd.exe", "/c"]

CC := "gcc"
CC_FLAGS := "-Wall -Wextra"
ENTRY := "main.c"
BUILD := "main.exe"

INPUT_FILE := "input.txt"
TEST_FILE := "mock.txt"

build:
  {{CC}} -o {{BUILD}} {{ENTRY}} {{CC_FLAGS}}

run: build
  {{BUILD}} {{INPUT_FILE}}

test: build
  {{BUILD}} {{TEST_FILE}}
