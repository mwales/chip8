; This ROM will display a 0 in the upper-left corner, a 63 in the top-right
; corner, and a 31 in the bottom left corner

option binary

Main:

   CLS

   ; Make the 0 point (0)
   LD V0, 0
   LD F, V0
   LD V2, 1
   LD V3, 1
   DRW V2, V3, 5

   ; Make the right point (63)
   LD V0, 6
   LD F, V0
   LD V2, 54
   DRW V2, V3, 5
   
   LD V0, 3
   LD F, V0
   LD V2, 59
   DRW V2, V3, 5

   ; Make the bottom point (31)
   ; Sprite '3' already loaded
   LD V2, 1
   LD V3, 26
   DRW V2, V3, 5

   LD V0, 1
   LD F, v0
   LD V2, 5
   DRW V2, V3, 5





