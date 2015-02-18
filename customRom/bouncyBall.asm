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




option binary

BCD_SCORE_LOC EQU 0xc00

Main:
   LD	   VD, 6
   LD	   VE, 6
   CALL  Reset_Pipe

Main_Loop:
   CLS
   CALL 	Drop_Ball
   CALL	Draw_Ball
   CALL  Draw_Pipe
   CALL  Draw_Score
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


Ball:
	DW	#3C66
	DW	#C381
	DW	#81C3
	DW	#663C

Bottom_Pipe:
	DW	#7E42
   DW #4242
Top_Pipe:
	DW	#4242
   DW #4242
   DW #4242
   DW #427E

