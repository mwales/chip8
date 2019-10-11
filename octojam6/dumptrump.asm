












: main

clear

v3 := 0
v4 := 0

draw-0
draw-1
draw-2
draw-3
draw-4
draw-5
draw-6
draw-7
draw-8
draw-9

v3 := 0
v4 += 6

draw-A
draw-B
draw-C
draw-D
draw-E
draw-F
draw-G
draw-H
draw-I
draw-J

v3 := 0
v4 += 6

draw-K
draw-L
draw-M
draw-N
draw-O
draw-P
draw-Q
draw-R
draw-S
draw-T

v3 := 0
v4 += 6

draw-U
draw-V
draw-W
draw-X
draw-Y
draw-Z

v3 := 2
delay-seconds


clear
hires

v3 := 41
v4 := 0

draw-D
draw-U
draw-M
draw-P

v3 += 2

draw-T
draw-R
draw-U
draw-M
draw-P

v6 := 10
v7 := 0
plane 3
i := trump-sprite
sprite v6 v7 15


v3 := 2
delay-seconds


: scroll-down-title-screen
v2 := 0
loop
v3 := 6
delay-frac-second
scroll-down 1
v2 += 1
while v2 <= 45
again

v2 := 0
loop
v3 := 6
delay-frac-second
scroll-up 1
v2 += 1
while v2 <= 45
again

scroll-down-title-screen







: endprogram
jump endprogram












: trump-sprite
0x38 0xA2 0xFF 0x81 0x00 0x00 0x00 0x00 0x7E 0x7E 
0x7E 0x7E 0x7E 0x7E 0x66 0x00 0x00 0x00 0x5A 0x7E 
0x66 0x5A 0x7E 0x99 0x99 0x3C 0x3C 0x18 0x00 0x00







: delay-seconds
# v3 is number of seconds to delay
# clobbers v4
loop
if v3 == 0 then return
delay-one-second
v3 += -1
again

: delay-one-second
# clobbers v4
# Chip-8 delay timer runs at 60 Hz
v4 := 60
delay := v4

loop
	v4 := delay

if v4 != 0 then again

return

: delay-frac-second
# v3 is how long to delay
delay := v3
loop
v3 := delay
if v3 == 0 then return
again

: draw-letter
# I is the address of the null terminated string
# v3 = x
# v4 = y
# v5 = ascii code
# clobbers i 

if v5 <= 0x39 begin
	if v5 >= 0x30 begin
		# ASCII digit
		v5 += -0x30
		i := hex v5
		sprite v3 v4 5
		v3 += 5
	end

end

return

: draw-O
: draw-0
v5 := 0
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-1
v5 := 1
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-2
v5 := 2
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-3
v5 := 3
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-4
v5 := 4
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-5
v5 := 5
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-6
v5 := 6
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-7
v5 := 7
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-8
v5 := 8
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-9
v5 := 9
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-A
v5 := 0xa
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-B
v5 := 0xb
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-C
v5 := 0xc
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-D
v5 := 0xd
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-E
v5 := 0xe
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-F
v5 := 0xf
i := hex v5
sprite v3 v4 5
v3 += 5
return

: draw-G
i := letter-G
sprite v3 v4 5
v3 += 5
return

: draw-H
i := letter-H
sprite v3 v4 5
v3 += 5
return

: draw-I
i := letter-I
sprite v3 v4 5
v3 += 4
return

: draw-J
i := letter-J
sprite v3 v4 5
v3 += 5
return

: draw-K
i := letter-K
sprite v3 v4 5
v3 += 5
return

: draw-L
i := letter-L
sprite v3 v4 5
v3 += 5
return

: draw-M
i := letter-M
sprite v3 v4 5
v3 += 6
return

: draw-N
i := letter-N
sprite v3 v4 5
v3 += 5
return

: draw-P
i := letter-P
sprite v3 v4 5
v3 += 5
return

: draw-Q
i := letter-Q
sprite v3 v4 5
v3 += 5
return

: draw-R
i := letter-R
sprite v3 v4 5
v3 += 5
return

: draw-S
i := letter-S
sprite v3 v4 5
v3 += 5
return

: draw-T
i := letter-T
sprite v3 v4 5
v3 += 6
return

: draw-U
i := letter-U
sprite v3 v4 5
v3 += 5
return

: draw-V
i := letter-V
sprite v3 v4 5
v3 += 6
return

: draw-W
i := letter-W
sprite v3 v4 5
v3 += 6
return

: draw-X
i := letter-X
sprite v3 v4 5
v3 += 5
return

: draw-Y
i := letter-Y
sprite v3 v4 5
v3 += 5
return

: draw-Z
i := letter-Z
sprite v3 v4 5
v3 += 5
return










: letter-G
0xF0 0x80 0xB0 0x90 0xF0
: letter-H
0x90 0x90 0xF0 0x90 0x90
: letter-I      # 3 wide
0xE0 0x40 0x40 0x40 0xE0 
: letter-J
0x70 0x20 0x20 0xA0 0xE0
: letter-K
0x90 0xA0 0xC0 0xA0 0x90
: letter-L
0x80 0x80 0x80 0x80 0xF0
: letter-M      # 5 wide!!
0xD8 0xA8 0xA8 0xA8 0xA8
: letter-N
0x90 0xD0 0xF0 0xB0 0x90
# skipping O, just use 0
: letter-P
0xE0 0x90 0xE0 0x80 0x80
: letter-Q
0x40 0xA0 0xA0 0xA0 0x50
: letter-R
0xE0 0x90 0xE0 0xB0 0x90
: letter-S
0xF0 0x80 0xF0 0x10 0xF0
: letter-T   #  5-wide!!
0xF8 0x20 0x20 0x20 0x20
: letter-U
0x90 0x90 0x90 0x90 0x60
: letter-V   # 5 wide
0x88 0x88 0x88 0x50 0x20
: letter-W   # 5 wide
0x88 0xA8 0xA8 0xA8 0xF8
: letter-X
0x90 0xB0 0x60 0xD0 0x90
: letter-Y
0x90 0x90 0xF0 0x60 0x60
: letter-Z
0xF0 0x30 0x60 0xC0 0xF0


