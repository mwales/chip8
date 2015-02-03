# Chip-8 Project Notes

Note to audience:  This will be an exploration process (not reading a guide on how its recommended to do something, but just blast forward on my own path of discovery, which could end in spectacular, but educational, failure.)  I realize that this is not the best attitude for a workplace, but my participation in the challenge is also to have fun and learn.

# Tools

Haroopad for notes / documentation

C and C\+\+ for the underlying code.  I usually Qt-ify my personal projects because I like Qt, but I fear my C/C++ core skills may have rusted a bit in the process

Qt for graphical interface

# Disassembler

I want to take a few approaches with this.

I want to start making a disassembler that just ingests the whole ROM and interprets every 2 bytes as op-code information.  I realize this is wrong because I've noticed some of the ROMs have binary data in the ROM that isn't executable code.  Also wondering if code will ever jump to an odd-address (addr % 2 == 1), because that would produce garbage during disassemble.

I then want to expand on that to develop a recursive disassembler (I've read that is how IDA Pro and radare are).  This seems like it would be straightforward for hardcoded jump instruction like 1NNN and 2NNN, but will be problematic for BNNN which uses V0 as an offset for the jump.

If I'm fealing really crafty and over-achieving:

* Let the user add addresses to the chunks of binary that will be interpreted as instructions (so the user can analyze a BNNN instruction and manually provide the destination)
* Create a table of all the locations jumped from and too, then comment the disassembly with that info

## Op-Code Patterns

Exact match op-codes

* 00E0: clear screen
* 00EE: return from subroutine

Jump Instructions (provides 12-bit address)

* 1NNN: jump to address
* 2NNN: call address

Address Instructions (provides 12-bit address)

* ANNN: I = NNN
* BNNN: Jump to NNN + V0

Operates on register with 8-bit constant

* 3XNN: Skip next instruction if VX == NN
* 4XNN: Skip next instruction if VX != NN
* 6XNN: VX = NN
* 7XNN: VX += NN
* CXNN: VX = rand() & NN

Operations involving 2 registers
* 5XY0: Skip next instruction if VX == VY
* 9XY0: Skip next instruction if VX != VY
* 8XY0: VX = VY
* 8XY1: VX = VX | VY
* 8XY2: VX = VX & VY
* 8XY3: VX = VX xor VY
* 8XY4: VX += VY (VF set to 1 on carry)
* 8XY5: VX = VX - VY (VF set to 0 on borrow)
* 8XY7: VX = VY - VX (VF set to 0 on borrow)

Operations involving only 1 general purpose register:

* 8X06: VX = VX >> 1 (VF set to bit shifted out) - Discrepency between wikipedia and other sites on this opcode
* 8X0E: VX = VX << 1 (VF set to bit shifted out) - Discrepency between wikipedia and other sites on this opcode
* EX9E: Skip next instruction if key stored in VX is pressed
* EXA1: Skip next instruction if key stored in VX is not pressed
* FX0A: Wait for key press, store in VX
* FX07: VX = Delay Timer value
* FX15: Delay Timer = VX
* FX18: Sound Timer = VX
* FX1E: I += VX
* FX29: Sets I to location of character sprite for VX
* FX33: Sets *I - *I+2 to BCD value of VX
* FX55: Stores V0-VX in memory at address I.  I += N + 1
* FX65: Fills V0-VX with memory from address I.  I += N + 1

Draw Sprite Command (2 registers, + 4 bit constant)

* DXYN: Draw sprite on screen at coordinate (VX,VY).  Sprite Height N.  Sprite contents at I.

## Registers

* V0: General purpose / jump offset 
* V1-VE: General purpose
* VF: General purpose / Carry flag
* I: Index Register (16 bits)
* IP: Instruction Pointer (0 - 0xFFF)
* Stack:  16 registers deep, 16-bits
* Delay Timer
* Sound Timer



# Reference Sites:

* Chip 8 instruction set (including superchip instructions):  http://www.multigesture.net/wp-content/uploads/mirror/goldroad/chip8_instruction_set.shtml

* http://www.emulator101.com/
