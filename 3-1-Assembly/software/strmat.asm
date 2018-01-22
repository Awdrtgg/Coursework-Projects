data        segment
string      db      "ababdwrdabcf"
pos         dw      0
found       dw      00h
hint1       db      "Please type the string"
hint2       db      "Please type the substring"
answer1     db      "Match failed"
answer2     db      "Successfully match, pos = "
data        ends

extra       segment
substr      db       "abc"
extra       ends

stack       segment

stack       ends

code        segment
            assume cs:code, ds:data, es:extra
start:      
            mov     ax, data
            mov     ds, ax

;            lea     dx, hint1       ; print hint1
;            mov     ah, 09h
;            int     21h
;
;            lea     dx, string      ; get string
;            mov     ah, 0Ah
;            int     21h
;
;            mov     ah, 02h         ; print enter
;            mov     dl, 0Ah
;            int     21h
;
;           lea     dx, hint2       ; print hint2
;            mov     ah, 0Ah
;            int     21h
;
;            lea     dx, substr      ; get substr
;            mov     ah, 0Ah
;            int     21h
;            
;            mov     ah, 02h         ; print enter
;            mov     dl, 0Ah
;            int     21h


;            mov     bx, substr[1]+2 ; the length of substr
;            mov     cx, string[1]   ; the length of str
            mov     bx, 3
            mov     cx, 12
            lea     si, 0           ; 0A make the string start from 2nd db
            lea     di, 0
            or      flags, 0400h    ; make sure that DF is set to 1
            xor     ax, ax

    ; ax = pos, bx balabala, cx = len(string), dx = len(substr)
    next:   cmpsb   
            jz      right                                
            mov     di, 0           ; substr starts from the beginning
            mov     ax, 0
            jmp     fail
    right:  cmp     di, bx          ; if ths substr is all matched
            jz      success
            cmp     ax, 0           ; if a is 0 (failed to match last time)
            jnz     aispos          ; a is not 0, don't need record this si
            mov     ax, si          ; a is 0, so this is the pos
    aispos:     
    fail:   loop    next

            mov     [found], 0      
            lea     dx, answer1     ; print failing information
            mov     ah, 09h
            int     21h
            jmp     e

    success:mov     [pos], ax
            lea     dx, answer2     ; print success
            mov     ah, 09h
            int     21h

    e:      mov     ax, 4C00H
            int     21h
code        ends
            end     start
