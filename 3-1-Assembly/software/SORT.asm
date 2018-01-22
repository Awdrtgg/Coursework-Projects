data        segment
buf         db      "zheliyou50gewufuhaozijieshuju,yaodagouzhemeduogezi$"
hint1       db      "The original data:$"
hint2       db      "The sorted data:$"
data        ends

code        segment
            assume cs:code, ds:data
start:      mov     ax, data
            mov     ds, ax

            ; print hint1
            mov     dx, offset hint1
            mov     ah, 09
            int     21h
            mov     dx, 0dh
            mov     ah, 02
            int     21h
            mov     dx, 0ah
            mov     ah, 02
            int     21h

            call    printbuf

            ; adopt bubble sort
            mov     bx, 49 ; the initial step
    lp1:    ; bubble from 1 to bx
            mov     cx, bx
            mov     si, offset buf
    lp2:    mov     dx, [si] ; here the dx reads 2 chars
            cmp     dh, dl
            jge     greater
            xchg    dh, dl
            mov     [si], dx ; save the change
    greater:inc     si
            loop    lp2

            ; if bubbled for 49 times?
            dec     bx
            jnz     lp1

            ; print hint2
            mov     dx, offset hint2
            mov     ah, 09
            int     21h
            mov     dx, 0dh
            mov     ah, 02
            int     21h
            mov     dx, 0ah
            mov     ah, 02
            int     21h

            call    printbuf

            mov     ah, 4ch
            int     21h

printbuf    proc    near
            push    ax
            push    bx
            push    cx
            push    dx
            push    si

            mov     si, offset buf
            mov     bx, 5
            mov     cx, 10
    lp3:    ; print a char
            mov     dx, [si]
            mov     ah, 02
            int     21h

            ; print a blank
            mov     dx, 20h
            mov     ah, 02
            int     21h
            
            ; prepare next char
            inc     si
            loop    lp3
            
            push    dx
            ; print enter
            mov     dx, 0dh
            mov     ah, 02
            int     21h
            mov     dx, 0ah
            mov     ah, 02
            int     21h
            ; prepare next line
            pop     dx
            mov     cx, 10
            dec     bx
            jnz     lp3

            pop     si
            pop     dx
            pop     cx
            pop     bx
            pop     ax
            ret
printbuf    endp

code        ends
            end     start