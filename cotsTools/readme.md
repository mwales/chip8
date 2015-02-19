
# About COTS tools

These are tools that are developed by someone other than myself.  Just included here for convenience.

# CHIPPER

I found the CHIPPER Chip-8 assembler in a download file that David Winter hosted on his website.  David Winter 
indicated that his download was freeware.  I didn't see any indication of a license on the source posted by Christian 
Egeberg.

Anyways, this tool is not developed by me, just putting it here for convenience.

# How I used CHIPPER

Before I learned of this tool, I was hand creating my Chip-8 test roms using a hex editor only.  But I found this
and decided to use it for my own custom game, floppy bird.

## Compiling

    gcc chipper.c -o chipper

## Assembling ROM

    ./chipper floppybird.rom floppybird.asm

# References

- David Winter's Chip-8 Webpage:  http://www.pong-story.com/chip8/


