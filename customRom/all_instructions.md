

00E0	Clears the screen.

00EE	Returns from a subroutine.

1NNN	Jumps to address NNN.

1210	jmp 0x0210

2NNN	Calls subroutine at NNN.

2300	call 0x0300

3XNN	Skips the next instruction if VX equals NN.

31ee	je v1, 0xee

4XNN	Skips the next instruction if VX doesn't equal NN.

4f01	jne vf, 0x01

5XY0	Skips the next instruction if VX equals VY.

5340	je v3, v4

6XNN	Sets VX to NN.

6622	mov v6, 0x22

7XNN	Adds NN to VX.

7890	add v8, 0x90

8XY0	Sets VX to the value of VY.

8bf0	mov vb, vf

8XY1	Sets VX to VX or VY.

80c1	or v0, vc

8XY2	Sets VX to VX and VY.

8122	and v1, v2

8XY3	Sets VX to VX xor VY.

89b3	xor v9, vb

8XY4	Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.

8904	add v9, v0

8XY5	VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.

8755	sub v7, v5

8X06	Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.[2]

8356	rshift v3

8XY7	Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.

8397	subrev v3, v9

8XYE	Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.[2]

8c0e	lshift vc

9XY0	Skips the next instruction if VX doesn't equal VY.

9ea0	jne ve, va

ANNN	Sets I to the address NNN.

a122	mov I, 0x0122

BNNN	Jumps to the address NNN plus V0.

b004	jmp 0x0004 + v0

CXNN	Sets VX to a random number and NN.

caff	rand va, 0xff

DXYN	Sprites stored in memory at location in index register (I), maximum 8bits wide. Wraps around the screen. If when drawn, clears a pixel, register VF is set to 1 otherwise it is zero. All drawing is XOR drawing (i.e. it toggles the screen pixels)

d345	sprite v3, v4, 0x5

EX9E	Skips the next instruction if the key stored in VX is pressed.

e49e	skip_on_press v4

EXA1	Skips the next instruction if the key stored in VX isn't pressed.

e2a1	skip_on_not_press v2

FX07	Sets VX to the value of the delay timer.

fc07	mov vc, timer_delay

FX0A	A key press is awaited, and then stored in VX.

f30a	wait_for_key v3

FX15	Sets the delay timer to VX.

f215	mov delay_timer, vx

FX18	Sets the sound timer to VX.

fb18	mov sound_timer, vb

FX1E	Adds VX to I.[3]

f21e	add I, v2

FX29	Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.

f329	font_load v3

FX33	Stores the Binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)

f233	stobcd [I], v2

FX55	Stores V0 to VX in memory starting at address I.[4]

f655	sto [I], v6

FX65	Fills V0 to VX with values from memory starting at address I.[4]

f565	load v5, [I]