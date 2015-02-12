#!/bin/bash

#g++ InstDecoder.cpp Disassembler.cpp main.cpp

clang++ InstDecoder.cpp Disassembler.cpp RecursiveDisass.cpp main.cpp -o chip8da

