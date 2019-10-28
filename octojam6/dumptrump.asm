# Michael Wales
# https://github.com/mwales/chip8
# Developed using octo (I need to add XO support to my own emulator... one day...)

:const trumpXCoord 15


: main


clear
hires
plane 1

v3 := 75
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

# Scroll dump trump down to line 4 above from bottom
# screen is 64 high, letters are 5 high.  64 - 5 - 4
v5 := 55
loop
  scroll-down 1
	
	v3 := 2
	delay-frac-second
	v5 += -1
	
	if v5 > 0 then again
	
# Draw a line on the screen
v0 := 0
v1 := 60
i := wide-line
sprite v0 v1 1

loop
  v0 += 8
	sprite v0 v1 1
  if v0 < 120 then again


# Draw "Press 5 to" above dump trump
v3 := 78
v4 := 47

draw-P
draw-R
draw-E
draw-S
draw-S
v3 += 2
draw-S
v3 += 2
draw-T
draw-O
	
# Draw the text for the power and angle bar
v3 := 44
v4 := 5

draw-P
draw-O
draw-W
draw-E
draw-R

v3 := 44
v4 := 14

draw-A
draw-N
draw-G
draw-L
draw-E

	
# enter to color mode for glorious graphics!
plane 3

# draw trump at the bottom of the screen 64 - 4 -16
v4 := 44
v3 := trumpXCoord

draw-spin-sprite

#******************************************************************************
# We are going to draw 2 meters, each will be 31 (prime) pixels wide
# Draw first one at 5,8
# Draw second one at 5, 17
#******************************************************************************

v0 := 5
v1 := 8
i := wide-line
sprite v0 v1 1
v0 += 8
sprite v0 v1 1
v0 += 8
sprite v0 v1 1
v0 += 8
sprite v0 v1 1

v0 := 5
v1 := 10
i := wide-line
sprite v0 v1 1
v0 += 8
sprite v0 v1 1
v0 += 8
sprite v0 v1 1
v0 += 8
sprite v0 v1 1


v0 := 5
v1 := 17
sprite v0 v1 1
v0 += 8
sprite v0 v1 1
v0 += 8
sprite v0 v1 1
v0 += 8
sprite v0 v1 1

v0 := 5
v1 := 19
i := wide-line
sprite v0 v1 1
v0 += 8
sprite v0 v1 1
v0 += 8
sprite v0 v1 1
v0 += 8
sprite v0 v1 1

# draw single pixel at start of bars
v0 := 5
v1 := 9
i := single-red-pixel
sprite v0 v1 1

v1 := 18
i := single-red-pixel
sprite v0 v1 1

# draw single pixel(s) at the end of the bars
v0 := 37
v1 := 8
i := single-red-pixel
sprite v0 v1 1
v1 := 9
i := single-red-pixel
sprite v0 v1 1
v1 := 10
i := single-red-pixel
sprite v0 v1 1

v1 := 17
i := single-red-pixel
sprite v0 v1 1
v1 := 18
i := single-red-pixel
sprite v0 v1 1
v1 := 19
i := single-red-pixel
sprite v0 v1 1


#******************************************************************************
# Make the power / angle bars animate until a key is pressed
# Power we will add 6 to (and loop it when it hits 31)
# Angle we will add 5 to (and loop it when it hits 31)
#******************************************************************************
v7 := 6
v8 := 6
v9 := 9
vA := 18
loop
  # draw the pixel
	i := single-pixel
	sprite v7 v9 1
	i := single-pixel
	sprite v8 vA 1
	
	# wait for key press
	vB := 5
	delay := vB
	
	: waiting-for-keypress
	if vB key then jump time-to-dump-trump
	vC := delay
	if vC > 0 then jump waiting-for-keypress
	
	# erase pixel
	i := single-pixel
	sprite v7 v9 1
	i := single-pixel
	sprite v8 vA 1
	
	# move the pixel to next position
	v7 += 6
	if v7 >= 37 then v7 += -31
	
	v8 += 5
	if v8 >= 37 then v8 += -31
	
	again
	
: time-to-dump-trump
v7 += -5    # power
v8 += -5    # angle

v0 := 0
v1 := 0
draw-height-on-screen

#******************************************************************************
# Kick trump animation
#******************************************************************************

punch-animation


#******************************************************************************
# Kick calculations and what not (v7 and v8 only things we know at this point)
#******************************************************************************
vc >>= v8
vc >>= vc  # height lookup table pos (starting pos)
vb := 7
vb -= vc   # acceleration lookup table pos (starting pos)
va := v7
#va >>= va
va >>= va  # scrolls per sec
vd := 0    # score


# erase trump before going into loop (since he hasn't scrolled yet)
v3 := trumpXCoord
v4 := 44
erase-spin-sprite 

: flying-loop-start

i := height-add-lookup-table
i += vb
load v0
v2 := v0

vc := 0
loop
	# erase the current height
	i := current-height
	load v1
	
	draw-height-on-screen

	# Figure out current height
	i := current-height
	load v1
	
	# If he is on screen, erase him
	if v0 != 0 then jump trump-not-on-screen-erase
	if v1 > 44 then jump trump-not-on-screen-erase    # don't erase him if he is above the screen
	if v1 == 0 then jump trump-not-on-screen-erase    # don't erase him if he is on ground
	
  # map the height to x,y coordinates on screen
	v3 := trumpXCoord
	v4 := 44
	v4 -= v1
	erase-spin-sprite  # clobbers the v0 and v1
	
	i := current-height
	load v1	
	
	: trump-not-on-screen-erase
	
	# update his height
	
	if vb <= 7 then big-addition
	if vb > 7 then big-subtraction

	scroll-left

	i := current-height
	save v1
	
	draw-height-on-screen
	
	i := current-height
	load v1
	
	# If he is on screen, draw him
	if v0 != 0 then jump trump-not-on-screen-draw
	if v1 > 44 then jump trump-not-on-screen-draw
	
	v3 := trumpXCoord
	v4 := 44
	v4 -= v1
	draw-spin-sprite  # clobbers the v0 and v1
	
		
	: trump-not-on-screen-draw
	
	
	vc += 1
	vd += 1
	
	v0 := 1
	v0 &= vd
	if v0 == 0 then draw-new-terrain
	
	# scroll-left
	
	v3 := 5
	# v3 -= va
	delay-frac-second
	
	if vc < va then again



vb += 1

i := current-height
load v1

if v0 != 0 then jump flying-loop-start
if v1 != 0 then jump flying-loop-start





#******************************************************************************
# Display score
#******************************************************************************

plane 1

v3 := 74
v4 := 12

draw-S
draw-C
draw-O
draw-R
draw-E

i := score
bcd vd

v3 += 9
i := score
load v2
i := hex v0
sprite v3 v4 5

v3 += 5
i := hex v1
sprite v3 v4 5

v3 += 5
i := hex v2
sprite v3 v4 5

plane 3


: endprogram

v0 := key

#reset globals
i := spinState
v0 := 7
save v0

v0 := 0
v1 := 0
v2 := 0

i := current-distance
save v1

i := score
save v2

jump main




#******************************************************************************
#******************************************************************************
#******************************************************************************
#           Functions
#******************************************************************************
#******************************************************************************
#******************************************************************************

: boxing-glove-sprite  # 11 rows
0x1C 0x34 0x67 0x49 0xC3 0xC1 0xC3 0xC1
0x43 0x41 0x3E 0x1C 0x34 0x67 0x49 0x43
0x41 0x43 0x41 0x43 0x41 0x3E

: boxing-glove-erase-sprite # 11 rows
# this sprite doesn't draw/erase the arm
0x1C 0x34 0x67 0x49 0x43 0x41 0x43 0x41
0x43 0x41 0x3E 0x1C 0x34 0x67 0x49 0x43
0x41 0x43 0x41 0x43 0x41 0x3E


: punch-animation
v1 := 0
v2 := 40
i := boxing-glove-sprite
sprite v1 v2 11

v3 := 5
delay-frac-second

loop
  i := boxing-glove-erase-sprite
	sprite v1 v2 11
	v1 += 1
	i := boxing-glove-sprite
	sprite v1 v2 11
	
	
	v3 := 1
	delay-frac-second
	
	if v1 < 11 then again
	
return





: spinState
0x07

: erase-spin-sprite
# r3 = x, r4 = y, clobbers r2, r1
i := spinState
load v0

# draw new sprite
i := spinner-frame-0
v1 := 64      # size of each sprite to skip

if v0 == 0 then jump erase-frame-found

loop
  i += v1
	v0 += -1
	if v0 > 0 then
again

: erase-frame-found
sprite v3 v4 0
return



: draw-spin-sprite
# r3 = x, r4 = y, clobbers v0, v1
i := spinState
load v0

v0 += 1
if v0 > 7 then v0 := 0

i := spinState
save v0

# draw new sprite
i := spinner-frame-0
v1 := 64      # size of each sprite to skip

if v0 == 0 then jump draw-frame-found

loop
  i += v1
	v0 += -1
	if v0 > 0 then
again

: draw-frame-found
sprite v3 v4 0
return




: wide-line
0xff 0xff

: single-red-pixel #uses the byte from single-pixel. what optimization!
0x80
: single-pixel
0x80 0x00



# I made a program on my github to help rotate these sprites

: spinner-frame-0
0x03 0x80 0x0a 0x20 0x0f 0xf0 0x08 0x10  0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x07 0xe0 0x07 0xe0 0x07 0xe0 0x07 0xe0  0x07 0xe0 0x07 0xe0 0x06 0x60 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x05 0xa0  0x0f 0xf0 0x0e 0x70 0x05 0xa0 0x07 0xe0
0x21 0x84 0x79 0x9e 0x23 0xc4 0x03 0xc0  0x01 0x80 0x00 0x00 0x00 0x00 0x0e 0x70

: spinner-frame-1
0x64 0x00 0xcc 0x00 0x98 0x00 0xe0 0x00  0x40 0x00 0x40 0x00 0x00 0x40 0x01 0xe0
0x01 0xf0 0x03 0xf8 0x01 0xfc 0x00 0xfe  0x00 0x70 0x00 0x30 0x00 0x10 0x00 0x00
0x00 0x00 0x00 0x00 0x07 0x00 0x1b 0x8c  0x3f 0x8c 0x2c 0x90 0x39 0xa0 0x3b 0x00
0x1e 0x80 0x00 0x60 0x02 0x61 0x04 0x11  0x18 0x03 0x18 0x00 0x00 0x08 0x00 0x38

: spinner-frame-2
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00  0x30 0x00 0x60 0xfe 0x20 0xfe 0xa0 0xfc
0xa0 0xfc 0xe0 0xfe 0x20 0xfe 0x70 0x00  0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x40 0x00 0xe0 0x00 0x40  0x0c 0x41 0x1f 0x01 0x0d 0x31 0x1b 0xf8
0x1b 0xf8 0x0d 0x31 0x1f 0x01 0x0c 0x41  0x00 0x40 0x00 0xe0 0x00 0x40 0x00 0x00

: spinner-frame-3
0x00 0x00 0x00 0x10 0x00 0x30 0x00 0x70  0x00 0xfe 0x01 0xfc 0x03 0xf8 0x01 0xf0
0x01 0xe0 0x00 0x40 0xc0 0x00 0x60 0x00  0x20 0x00 0x90 0x00 0xdc 0x00 0x70 0x00
0x00 0x38 0x00 0x08 0x18 0x00 0x18 0x03  0x04 0x11 0x02 0x61 0x00 0x60 0x1e 0x80
0x3b 0x00 0x39 0xa0 0x2c 0x90 0x1f 0x8c  0x1b 0x8c 0x0f 0x00 0x00 0x00 0x00 0x00

: spinner-frame-4
0x00 0x00 0x06 0x60 0x07 0xe0 0x07 0xe0  0x07 0xe0 0x07 0xe0 0x07 0xe0 0x07 0xe0
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00  0x08 0x10 0x0f 0xf0 0x04 0x50 0x01 0xc0
0x0e 0x70 0x00 0x00 0x00 0x00 0x01 0x80  0x03 0xc0 0x23 0xc4 0x79 0x9e 0x21 0x84
0x07 0xe0 0x05 0xa0 0x0e 0x70 0x0f 0xf0  0x05 0xa0 0x00 0x00 0x00 0x00 0x00 0x00

: spinner-frame-5
0x00 0x00 0x08 0x00 0x0c 0x00 0x0e 0x00  0x7f 0x00 0x3f 0x80 0x1f 0xc0 0x0f 0x80
0x07 0x80 0x02 0x00 0x00 0x02 0x00 0x02  0x00 0x07 0x00 0x19 0x00 0x33 0x00 0x26
0x1c 0x00 0x10 0x00 0x00 0x18 0xc0 0x18  0x88 0x20 0x86 0x40 0x06 0x00 0x01 0x78
0x00 0xdc 0x05 0x9c 0x09 0x34 0x31 0xfc  0x31 0xd8 0x00 0xe0 0x00 0x00 0x00 0x00

: spinner-frame-6
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00  0x00 0x0e 0x7f 0x04 0x7f 0x07 0x3f 0x05
0x3f 0x05 0x7f 0x04 0x7f 0x06 0x00 0x0c  0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x02 0x00 0x07 0x00 0x02 0x00  0x82 0x30 0x80 0xf8 0x8c 0xb0 0x1f 0xd8
0x1f 0xd8 0x8c 0xb0 0x80 0xf8 0x82 0x30  0x02 0x00 0x07 0x00 0x02 0x00 0x00 0x00

: spinner-frame-7
0x00 0x0e 0x00 0x3b 0x00 0x09 0x00 0x04  0x00 0x06 0x00 0x03 0x02 0x00 0x07 0x80
0x0f 0x80 0x1f 0xc0 0x3f 0x80 0x7f 0x00  0x0e 0x00 0x0c 0x00 0x08 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0xf0 0x31 0xd8  0x31 0xf8 0x09 0x34 0x05 0x9c 0x00 0xdc
0x01 0x78 0x06 0x00 0x86 0x40 0x88 0x20  0xc0 0x18 0x00 0x18 0x10 0x00 0x1c 0x00

: height-add-lookup-table
21 16 11 7 4 2 1 0
: height-sub-lookup-table
1 2 4 7 11 16 21

: current-height
0x00 0x00

: current-distance
0x00 0x00

: score
0x00 0x00 0x00

# I'm basically storing the height as base-100 number.  Each byte could represent 255-values,
# but in my program, each bytes is 0-99.  I can easily print out base-100 numbers
: height-bcd-upperconv
0 
: height-bcd-thousands
0 
: height-bcd-hundreds
0

: height-bcd-lowerconv
0 
: height-bcd-tens
0
: height-bcd-ones
0

: draw-height-on-screen
# height in v0 v1
i := height-bcd-upperconv
bcd v0
v0 := 3
i += v0
bcd v1

plane 1

v3 := 69
v4 := 5
draw-H
draw-E
draw-I
draw-G
draw-H
draw-T

v3 += 4

i := height-bcd-thousands
load v0
i := hex v0
sprite v3 v4 5
v3 += 5

i := height-bcd-hundreds
load v0
i := hex v0
sprite v3 v4 5
v3 += 5

i := height-bcd-tens
load v0
i := hex v0
sprite v3 v4 5
v3 += 5

i := height-bcd-ones
load v0
i := hex v0
sprite v3 v4 5

plane 3

return



: dumpster-fire-sprite      # 13 rows
0x10 0x00 0x58 0x0E 0x28 0x20 0x31 0x7B
0x7D 0xF9 0x8A 0x8C 0xF8 0x10 0x10 0x5C
0x7E 0x7E 0x7E 0xFE 0xFC 0xF8 0x00 0x00
0x00 0x00

: tree-sprite # 14 rows
0x00 0x10 0x00 0x4A 0x00 0x17 0xC0 0xE2
0x24 0x24 0x24 0xE7 0x24 0x66 0x3C 0x7E
0xFF 0xFF 0xFF 0x78 0x7E 0x1E 0x00 0x00
0x00 0x00 0x00 0x00

: wall-sprite # 13 rows
0x7E 0x4A 0x4A 0x7E 0x52 0x52 0x7E 0x4A
0x4A 0x7E 0xD3 0x52 0x7E 0x7E 0x42 0x42
0x42 0x42 0x42 0x42 0x42 0x42 0x42 0x42
0x42 0x7E

: flag-sprite   # 12 rows
0x1E 0x1E 0x1E 0x1E 0x1E 0x40 0x40 0x00
0x00 0x40 0xFF 0x00 0x66 0x66 0x66 0x66
0x66 0x40 0x40 0x40 0x40 0x40 0x40 0x40 

: road-sign-sprite # 12 rows
0x1C 0x3E 0x7F 0x7F 0x7F 0x3E 0x1C 0x08
0x00 0x00 0x00 0xFF 0x1C 0x22 0x6B 0x7B
0x73 0x36 0x1C 0x00 0x08 0x08 0x08 0x00

: pooh-pile-sprite #9-rows
0x00 0x00 0x00 0x00 0x08 0x18 0x3C 0xFF
0x7E 0xA0 0x40 0x02 0x04 0x0A 0x18 0x3C
0x3E 0x7E

: draw-new-terrain
v1 := random 0x0f
if v1 == 0 then jump draw-wall
if v1 == 1 then jump draw-dumpster-fire
if v1 == 2 then jump draw-tree
if v1 == 3 then jump draw-flag
if v1 == 4 then jump draw-street-sign
if v1 == 5 then jump draw-poop
jump draw-blank-terrain





: draw-blank-terrain
plane 1
v0 := 120
v1 := 60
i := wide-line
sprite v0 v1 1
plane 3
return

: draw-dumpster-fire
v0 := 120
v1 := 50
i := dumpster-fire-sprite
sprite v0 v1 13
return

: draw-tree
v0 := 120
v1 := 49
i := tree-sprite
sprite v0 v1 14
return

: draw-wall
v0 := 120
v1 := 50
i := wall-sprite
sprite v0 v1 13
return

: draw-flag
v0 := 120
v1 := 50
i := flag-sprite
sprite v0 v1 12
return

: draw-street-sign
v0 := 120
v1 := 49
i := road-sign-sprite
sprite v0 v1 12
return

: draw-poop
v0 := 120
v1 := 53
i := pooh-pile-sprite
sprite v0 v1 9
return


#******************************************************************************
# 16-bit addition (base 100)
#******************************************************************************
# v0, v1 = 16-bit number, and result
# v2 = 256-bit number 

: big-addition
v1 += v2
if v1 < 100 then return
v1 += -100
v0 += 1
return

: big-subtraction
# v0, v1 = 16-bit number, and result
# v2 = 256-bit number 
v1 -= v2
if vf == 1 then return
v1 += 100
v0 += -1
return


#******************************************************************************
# Timer helpers
#******************************************************************************


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






#******************************************************************************
# Drawing letters
#******************************************************************************


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





