#!/bin/bash

#g++ InstDecoder.cpp Disassembler.cpp main.cpp

clang++ InstDecoder.cpp Disassembler.cpp RecursiveDisass.cpp chip8da.cpp -o chip8da

