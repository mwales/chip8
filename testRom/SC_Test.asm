; Setting used by the chipper assembler
option schip11
option binary
align off

; Recursive Disassembly
End of file
jp begin_program

; Tronix copyright notice embedded string
db #20	;GRAPHIC =   #     
db #54	;GRAPHIC =  # # #  	ASCII(T)
db #72	;GRAPHIC =  ###  # 	ASCII(r)
db #6f	;GRAPHIC =  ## ####	ASCII(o)
db #6e	;GRAPHIC =  ## ### 	ASCII(n)
db #69	;GRAPHIC =  ## #  #	ASCII(i)
db #78	;GRAPHIC =  ####   	ASCII(x)
db #20	;GRAPHIC =   #     
db #28	;GRAPHIC =   # #   	ASCII(()
db #63	;GRAPHIC =  ##   ##	ASCII(c)
db #29	;GRAPHIC =   # #  #	ASCII())
db #20	;GRAPHIC =   #     
db #32	;GRAPHIC =   ##  # 	ASCII(2)
db #30	;GRAPHIC =   ##    	ASCII(0)
db #31	;GRAPHIC =   ##   #	ASCII(1)
db #30	;GRAPHIC =   ##    	ASCII(0)

begin_program:   ; == START OF CODE BLOCK ==
cls
se vf, #00
jp display_error_ini
se ve, #00
jp display_error_ini
se vd, #00
jp display_error_ini
se vc, #00
jp display_error_ini
se vb, #00
jp display_error_ini
se va, #00
jp display_error_ini
se v9, #00
jp display_error_ini
se v8, #00
jp display_error_ini
se v7, #00
jp display_error_ini
se v6, #00
jp display_error_ini
se v5, #00
jp display_error_ini
se v4, #00
jp display_error_ini
se v3, #00
jp display_error_ini
se v2, #00
jp display_error_ini
se v1, #00
jp display_error_ini
se v0, #00
jp display_error_ini
ld v0, #00
ld v1, #01
ld v2, #02
ld v3, #03
ld v4, #04
ld v5, #05
ld v6, #06
ld v7, #07
ld v8, #08
ld v9, #09
ld va, #0a
ld vb, #0b
ld vc, #0c
ld vd, #0d
ld ve, #0e
ld vf, #0f
ld I, #0478
ld vf, [I]
se vf, #00
jp loc_03f8
se ve, #00
jp loc_03f8
se vd, #00
jp loc_03f8
se vc, #00
jp loc_03f8
se vb, #00
jp loc_03f8
se va, #00
jp loc_03f8
se v9, #00
jp loc_03f8
se v8, #00
jp loc_03f8
se v7, #00
jp loc_03f8
se v6, #00
jp loc_03f8
se v5, #00
jp loc_03f8
se v4, #00
jp loc_03f8
se v3, #00
jp loc_03f8
se v2, #00
jp loc_03f8
se v1, #00
jp loc_03f8
se v0, #00
jp loc_03f8
ld v0, #00
ld f, v0
ld v0, [I]
sne v0, #00
jp loc_0402
ld I, #0452
ld ve, #7b
ld b, ve
ld v2, [I]
se v0, #01
jp loc_03c6
se v1, #02
jp loc_03c6
se v2, #03
jp loc_03c6
ld ve, #02
ld vf, #00
ld v0, #fe
ld v1, #01
add v0, v1
se vf, #00
jp loc_040c
ld ve, #03
se v0, #ff
jp loc_040c
ld ve, #04
add v0, v1
se vf, #01
jp loc_040c
ld ve, #05
se v0, #00
jp loc_040c
ld v0, #01
ld ve, #06
ld vf, #00
sub v0, v1
se vf, #01
jp loc_040c
ld ve, #07
se v0, #00
jp loc_040c
ld ve, #08
sub v0, v1
se vf, #00
jp loc_040c
ld ve, #09
se v0, #ff
jp loc_040c
ld v0, #01
ld ve, #0a
ld vf, #00
subn v0, v1
se vf, #01
jp loc_040c
ld ve, #0b
se v0, #00
jp loc_040c
ld ve, #0c
ld v0, #01
ld v1, #00
subn v0, v1
se vf, #00
jp loc_040c
ld ve, #0d
se v0, #ff
jp loc_040c
ld v0, #ff
ld ve, #0e
ld vf, #00
shr v0
se vf, #01
jp loc_040c
ld ve, #0f
se v0, #7f
jp loc_040c
ld v0, #40
ld ve, #10
shr v0
se vf, #00
jp loc_040c
ld ve, #11
se v0, #20
jp loc_040c
ld ve, #12
ld vf, #01
shl v0
se vf, #00
jp loc_040c
ld ve, #13
se v0, #40
jp loc_040c
ld v0, #fa
ld ve, #14
shl v0
se vf, #01
jp loc_040c
ld ve, #15
se v0, #f4
jp loc_040c
ld v1, #7b
ld ve, #16
xor v0, v1
se v0, #8f
jp loc_040c
ld I, #0488
ld v7, [I]
ld r, v7 ; superchip-8 HP flag instruction
ld I, #0478
ld v7, [I]
ld v7, r ; superchip-8 flag instruction
ld ve, #17
se v7, #07
jp loc_040c
se v6, #06
jp loc_040c
se v5, #05
jp loc_040c
se v4, #04
jp loc_040c
se v3, #03
jp loc_040c
se v2, #02
jp loc_040c
se v1, #01
jp loc_040c
se v0, #00
jp loc_040c
ld ve, #18
ld I, #0ffe
ld v0, #02
ld vf, #00
add I, v0
se vf, #01
jp loc_040c
jp loc_0490
loc_03c6:   ; == START OF CODE BLOCK ==
call draw_error_on_screen
add v0, #0a
ld v2, #0b
ld f, v2
drw v0, v1, #5
add v0, #05
ld v2, #0c
ld f, v2
drw v0, v1, #5
add v2, #01
ld f, v2
add v0, #05
drw v0, v1, #5
jp loop_forever_do_nothing

display_error_ini:   ; == START OF CODE BLOCK ==
call draw_error_on_screen
add v0, #0a
ld I, sprite_letter_I
drw v0, v1, #5
add v0, #06
ld I, #0469
drw v0, v1, #5
add v0, #06
ld I, sprite_letter_I
drw v0, v1, #5
jp loop_forever_do_nothing
loc_03f8:   ; == START OF CODE BLOCK ==
call draw_error_on_screen
add v0, #0a
ld I, sprite_letter_O
drw v0, v1, #5
jp loop_forever_do_nothing
loc_0402:   ; == START OF CODE BLOCK ==
call draw_error_on_screen
add v0, #0a
ld I, sprite_letter_1
drw v0, v1, #5
jp loop_forever_do_nothing
loc_040c:   ; == START OF CODE BLOCK ==
call draw_error_on_screen
call loc_0432
jp loop_forever_do_nothing

; draws ERROR on top of screen
draw_error_on_screen:   ; == START OF CODE BLOCK ==
ld v0, #00
ld v1, #00
ld I, sprite_letter_E
drw v0, v1, #5
add v0, #05
ld I, #046e
drw v0, v1, #5
add v0, #06
drw v0, v1, #5
ld I, sprite_letter_O
add v0, #06
drw v0, v1, #5
ld I, #046e
add v0, #05
drw v0, v1, #5
ret
loc_0432:   ; == START OF CODE BLOCK ==
ld v4, v0
add v4, #0a
ld v5, v1
ld I, #0452
ld b, ve
ld v2, [I]
ld f, v0
drw v4, v5, #5
add v4, #06
ld f, v1
drw v4, v5, #5
add v4, #06
ld f, v2
drw v4, v5, #5
ret

; end of program
loop_forever_do_nothing:   ; == START OF CODE BLOCK ==
jp loop_forever_do_nothing

db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         

; letter 1
sprite_letter_1:
db #10	;GRAPHIC =    #    
db #30	;GRAPHIC =   ##    	ASCII(0)
db #10	;GRAPHIC =    #    
db #10	;GRAPHIC =    #    
db #10	;GRAPHIC =    #    

; letter O
sprite_letter_O:
db #f0	;GRAPHIC = ####    
db #90	;GRAPHIC = #  #    
db #90	;GRAPHIC = #  #    
db #90	;GRAPHIC = #  #    
db #f0	;GRAPHIC = ####    

; letter E
sprite_letter_E:
db #f0	;GRAPHIC = ####    
db #80	;GRAPHIC = #       
db #f0	;GRAPHIC = ####    
db #80	;GRAPHIC = #       
db #f0	;GRAPHIC = ####    

; letter I
sprite_letter_I:
db #f8	;GRAPHIC = #####   
db #20	;GRAPHIC =   #     
db #20	;GRAPHIC =   #     
db #20	;GRAPHIC =   #     
db #f8	;GRAPHIC = #####   

; letter N
sprite_letter_N:
db #88	;GRAPHIC = #   #   
db #c8	;GRAPHIC = ##  #   
db #a8	;GRAPHIC = # # #   
db #98	;GRAPHIC = #  ##   
db #88	;GRAPHIC = #   #   

; letter R
sprite_letter_R:
db #e0	;GRAPHIC = ###     
db #90	;GRAPHIC = #  #    
db #e0	;GRAPHIC = ###     
db #90	;GRAPHIC = #  #    
db #88	;GRAPHIC = #   #   

; letter K
sprite_letter_K:
db #90	;GRAPHIC = #  #    
db #a0	;GRAPHIC = # #     
db #c0	;GRAPHIC = ##      
db #a0	;GRAPHIC = # #     
db #90	;GRAPHIC = #  #    

db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #00	;GRAPHIC =         
db #01	;GRAPHIC =        #
db #02	;GRAPHIC =       # 
db #03	;GRAPHIC =       ##
db #04	;GRAPHIC =      #  
db #05	;GRAPHIC =      # #
db #06	;GRAPHIC =      ## 
db #07	;GRAPHIC =      ###
loc_0490:   ; == START OF CODE BLOCK ==
ld v0, #00
ld v1, #00
ld f, v0
drw v0, v1, #5
add v0, #05
ld I, #0473
drw v0, v1, #5
jp loop_forever_do_nothing
