#!/bin/bash

#g++ InstDecoder.cpp Disassembler.cpp main.cpp

clang++ Util.cpp InstDecoder.cpp Disassembler.cpp RecursiveDisass.cpp chip8da.cpp -o chip8da

