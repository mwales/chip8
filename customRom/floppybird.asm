; VE = Speed of ball
; 0 = +4 pixels per frame
; 1 = +2 pixels per frame
; 2 = +1 pixel per frame
; 3 = 0 pixels per frame
; 4 = -1 pixel per frame
; 5 = -2 pixel per frame
; 6 = -4 pixel per frame

; VD = Y-coord of ball

; VC = X-coord of pipe (0 to 58)
; VB = Y-coord of top of bottom pipe (2-16)
; VA = Score
; V9 = Wing flap state.  1 = wing up, 0 = wing down




option binary

BCD_SCORE_LOC EQU #c00

Main:

   CALL  Load_Title_Screen

   LD    VA, 0
   LD	   VD, 6
   LD	   VE, 6
   LD    V9, 0
   CALL  Reset_Pipe

Main_Loop:
   CLS
   CALL 	Drop_Ball
   CALL  Draw_Score
   CALL  Draw_Pipe

   LD VF, 0
   CALL	Draw_Bird

   ; If there was a collision after draing the ball, floppy bird dead
   SE VF, 0
   JP   Game_Over

   LD	V1, 10
   CALL	Delay

   ADD   VC, -2   ; Move the pipe

   ; If pipe all the way at the end, reset it back to the right
   SNE    VC, 0
   CALL  Reset_Pipe

   JP 	Main_Loop

Drop_Ball:
   ; what is the new ball y-coord
   SNE	VE, 0
   ADD	VD, 4
   SNE	VE, 1
   ADD	VD, 2
   SNE	VE, 2
   ADD	VD, 1
   ; Do nothing for val 3
   SNE	VE, 4
   ADD	VD, -1
   SNE	VE, 5
   ADD	VD, -2
   SNE	VE, 6
   ADD	VD, -4

   ; change the speed of the ball to next state
   SE      VE, 0
   ADD     VE, -1


   LD	V1, 1 ; x-coord
   LD	V2, 1 ; y-coord of top
   LD	V3, 7 ; y-coord of bot

   ;LD      F, VD
   ;DRW	V1, V2, 5

   ;LD	F, VE
   ;DRW	V1, V3, 5

   RET

; Delays for V1/60 seconds
Delay:
	LD	DT, V1
	LD	V2, 5	; 5 if the bounce key
Delay_Wait_Loop:
	LD	V1, DT
	SE	V1, 0
	JP	Delay_Wait_Loop
	SKNP	V2
   LD	VE, 5 ; Cause the ball to bounce next time it is drawn
	RET
	
Draw_Ball:
	LD	I, Ball
	LD	V1, 12
	DRW	V1, VD, 8
	RET

Draw_Bird:
   ; Flap wings if not falling
   SE VE, 0
   ADD V9, 1

   ; If V9 state, goes to 2, wrap back to zero
   SNE V9, 2
   LD V9, 0

   SNE VE, 0
   LD V9, 1

   LD V2, 8
   LD I, Wing_Up_Bird
   SNE V9, 0
   ADD I, V2

   LD V1, 12
   DRW V1, VD, 8
   RET


; Top pipe is 8 pixels high
; Gap between pipes is 14 pixels
; Bottom pipe is 10 pixels high
Draw_Pipe:
   LD V1, VB
   ADD V1, -22  ; Top of the sprite for the top pipe will always be 22 pixels above the top of the botom pipe
   LD I, Top_Pipe
   DRW VC, V1, 8
   LD I, Bottom_Pipe
   DRW VC, VB, 10
   RET

Reset_Pipe:
   LD VC, 58
   RND VB, #0F
   ADD VB, 15

   ADD VA, 1 ; Score a point!
   RET

Draw_Score:
   LD I, BCD_SCORE_LOC
   LD B, VA
   LD V2, [I]
   LD V3, 49 ; x-coord of 1st score digit
   LD V4, 1  ; y-coord of 1st score digit
   LD F, V0
   DRW V3, V4, 5
   ADD V3, 5
   LD F, V1
   DRW V3, V4, 5
   ADD V3, 5
   LD F, V2
   DRW V3, V4, 5
   RET

Load_Title_Screen:
   CLS
   LD V1, 0
   LD V2, 8
   LD V3, 16
   LD V4, 24
   LD V5, 32
   LD V6, 40
   LD V7, 48
   LD V8, 56
   LD V9, 15
   LD VA, 30
   LD VB, 2

   LD I, Title_Screen

   ; Each of the following code blocks draws a column of 8 pixels wide on the screen
   DRW V1, V1, 15
   ADD I, V9
   DRW V1, V9, 15
   ADD I, V9
   DRW V1, VA, 2
   ADD I, VB

   DRW V2, V1, 15
   ADD I, V9
   DRW V2, V9, 15
   ADD I, V9
   DRW V2, VA, 2
   ADD I, VB

   DRW V3, V1, 15
   ADD I, V9
   DRW V3, V9, 15
   ADD I, V9
   DRW V3, VA, 2
   ADD I, VB

   DRW V4, V1, 15
   ADD I, V9
   DRW V4, V9, 15
   ADD I, V9
   DRW V4, VA, 2
   ADD I, VB

   DRW V5, V1, 15
   ADD I, V9
   DRW V5, V9, 15
   ADD I, V9
   DRW V5, VA, 2
   ADD I, VB

   DRW V6, V1, 15
   ADD I, V9
   DRW V6, V9, 15
   ADD I, V9
   DRW V6, VA, 2
   ADD I, VB

   DRW V7, V1, 15
   ADD I, V9
   DRW V7, V9, 15
   ADD I, V9
   DRW V7, VA, 2
   ADD I, VB

   DRW V8, V1, 15
   ADD I, V9
   DRW V8, V9, 15
   ADD I, V9
   DRW V8, VA, 2
   ADD I, VB

   ; Delay 1 second
   LD V1, 60
   CALL Delay

   LD V2, K
   RET

Game_Over:
   LD V1, 120
   LD ST, V1

   LD V1, #0D
   LD V2, #0E
   LD V3, #0A
   LD V4, #0D
   LD V5, 32
   LD V6, 16

   ; Draw D
   LD F, V1
   DRW V5, V6, 5
   ADD V5, 5

   ; Draw E
   LD F, V2
   DRW V5, V6, 5
   ADD V5, 5

   ; Draw A
   LD F, V3
   DRW V5, V6, 5
   ADD V5, 5

   ; Draw D
   LD F, V4
   DRW V5, V6, 5
   ADD V5, 5

   ; Wait 3 seconds
   LD V1, 180
   CALL Delay

   ; Then reset on key press
   LD V2, K
   JP Main

Ball:
	DW	#3C66
	DW	#C381
	DW	#81C3
	DW	#663C

Wing_Up_Bird:
   DW #3FE9
   DW #8B8F
   DW #F82C
   DW #2420

Wing_Down_Bird:
   DW #0F09
   DW #0BFF
   DW #869C
   DW #F420

Bottom_Pipe:
	DW	#7E42
   DW #7E42
   DW #4242

Top_Pipe:
	DW	#4242
   DW #4242
   DW #427E
   DW #427E

Title_Screen:
   ; 1st column of graphics
   DW $0000000001111111
   DW $0100000101011111
   DW $0101000001011100
   DW $0100010001011100
   DW $0101000001010000
   DW $0101000001110000
   DW $0000000000000000
   DW $0000000000000000

   DW $0000000000000000
   DW $0000000000000000
   DW $0111111001000001
   DW $0101110101010101
   DW $0101110101000010
   DW $0101110101010101
   DW $0101110101000001
   DW $0111111000000000

   ; 2nd column of graphics
   DW $0000000001110000
   DW $0101000001010000
   DW $0101000001010000
   DW $0101000001010000
   DW $0101000001011110
   DW $0100001001111110
   DW $0000000000000000
   DW $0000000000000000

   DW $0000000000000000
   DW $0000000000000000
   DW $0111111101000001
   DW $0111011100010100
   DW $0001010000010100
   DW $0001010000010100
   DW $0111011101000001
   DW $0111111100000000

   ; 3rd column of graphics
   DW $0000000011111111
   DW $1000000010000000
   DW $1000000010001110
   DW $1000101010001110
   DW $1000000010001110
   DW $1000111011111111
   DW $0000000000000000
   DW $0000000000000000

   DW $0000000000000000
   DW $0000000000000000
   DW $0111111101000000
   DW $0101110001010100
   DW $0101110101000001
   DW $0101100101010100
   DW $0101001001010010
   DW $0111001100000000

   ; 4th column of graphics
   DW $0000000011101111
   DW $0010100001101011
   DW $0100101001101011
   DW $0010100010101011
   DW $0010101000101010
   DW $0010101011101110
   DW $0000000000000000
   DW $0000000000000000

   DW $0000000000000000
   DW $0000000000000000
   DW $0011111010100001
   DW $1010110010101010
   DW $1010101000101010
   DW $0010101010101010
   DW $1010110010100001
   DW $1011111000000000

   ; 5th column of graphics
   DW $0000000011101111
   DW $0010100010101011
   DW $1010101010101011
   DW $0010100011101011
   DW $0000101000001010
   DW $0000101000001110
   DW $0000000000000000
   DW $0000000000000000

   DW $0000000000000000
   DW $0000000000000000
   DW $0000000000000000
   DW $1000000010000000
   DW $1000000010000000
   DW $1000000010000000
   DW $1000000000000000
   DW $0000000000000000

   ; 6th column of graphics
   DW $0000000011101110
   DW $0010101010101010
   DW $1010101010101011
   DW $0010100011101110
   DW $0000001000000010
   DW $0000001000000011
   DW $0000000000000000
   DW $0000111000001010

   DW $0000111000001000
   DW $0000100000000000
   DW $0000010100001111
   DW $0000010100001111
   DW $0000010100000000
   DW $0000001100000010
   DW $0000001100000010
   DW $0000001000000000

   ; 7th column of graphics
   DW $0000000011100000
   DW $1010000010100000
   DW $1010000010100000
   DW $0010000011100000
   DW $1000000010000000
   DW $1000000010000000
   DW $0000000000000000
   DW $1100111010101000

   DW $1100110010101000
   DW $1010111000000000
   DW $0011100010100000
   DW $0011100010001000
   DW $0011100000000000
   DW $1010001100100010
   DW $0010001000100010
   DW $0011101100000000

   ; 8th column of graphics
   DW $0000000000000000
   DW $0000000000000000
   DW $0000000000000000
   DW $0000000000000000
   DW $0000000000000000
   DW $0000000000000000
   DW $0000000000000000
   DW $1110111010001000

   DW $1110111000100010
   DW $1110111000000000
   DW $1110111001001010
   DW $0100101001001010
   DW $0100111000000000
   DW $1011100010101000
   DW $1011100010100000
   DW $1010000000000000









