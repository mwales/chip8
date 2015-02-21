OPTION SCHIP11
option binary

HIGH
CLS

; v1 - v6 with abcdef
ld v1, #08
ld v2, #0b
ld v3, #0c
ld v4, #0d
ld v5, #0e
ld v6, #09

redraw:

ld v7, 34
ld v8, 27


; Testing drawing over bottom edge
;ld v8, 59




; draw a character
ld hf, v1
drw v7, v8, 10

; move x over 10 pixels
add v7, 10

; draw b character
ld hf, v2
drw v7, v8, 10

; move x over 10 pixels
add v7, 10

; draw c character
ld hf, v3
drw v7, v8, 10

; move x over 10 pixels
add v7, 10

; draw d character
ld hf, v4
drw v7, v8, 10

; move x over 10 pixels
add v7, 10

; draw e character
ld hf, v5
drw v7, v8, 10

; move x over 10 pixels
add v7, 10

; draw f character
ld hf, v6
drw v7, v8, 10

Scroll_Start:
ld v9, k

sne v9, 2
scd 4

sne v9, 4
scl

sne v9, 6
scr

jp Scroll_Start

exit
