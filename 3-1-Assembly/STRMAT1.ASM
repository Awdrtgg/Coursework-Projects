data        segment
string      db      "ababdwrdabf"
len_str     dw      $-string
pos         dw      0
found       dw      00h
answer1     db      "Match failed$"
answer2     db      "Successfully matched$"
data        ends

extra       segment
substr      db       "abc"
len_subs    dw       $-substr
extra       ends

stack       segment

stack       ends

code        segment
            assume cs:code, ds:data, es:extra
start:      
            mov     ax, data
            mov     ds, ax
            mov     ax, extra
            mov     es, ax

            mov     bx, len_subs
            mov     cx, len_str
            lea     si, substr
            lea     di, string
            xor     ax, ax

    ; ax = pos, bx = len(substr), cx = len(string)
    next:   cmpsb   
            jz      right                                
            lea     di, substr      ; substr starts from the beginning
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
            lea     dx, answer2
            mov     ah, 09h
            int     21h

    e:      mov     ax, 4C00H
            int     21h
code        ends
            end     start
