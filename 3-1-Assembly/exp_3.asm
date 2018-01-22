data        segment
port_8254   equ     280h
port_8251   equ     2b8h
esc         equ     27h
            hint    db  'Please input number one: $'
data        ends

code        segment
            assume cs: code, ds: data
    start:  
            mov     ax, data 
            mov     ds, ax

            ; init 8254
            mov     dx, port_8254 + 3
            mov     al, 00110110b
            out     dx, al
            mov     dx, port_8254
            mov     al, 52
            out     dx, al
            mov     al, ah
            out     dx, al

            ; init 8251
            mov     dx, port_8251 + 3
            xor     al, al
            out     dx, al
            mov     al, 40h
            out     dx, al
            nop
            mov     al, 11001110b
            out     dx, al
            mov     al, 00100111b
            out     dx, al

            ; print the hint
            mov     dx, offset hint
            mov     ah, 09
            int     21h

            ; read the state byte from 8251
    l1:     mov     dx, port_8251 + 1
            in      al, dx
            and     al, 01b ; if TxRDY == 1?
            jz      l1

            ; read char from the keyboard
            mov     ah, 01
            int     21h
            cmp     al, esc
            jz      over

            ; add 1 to char 
            inc     al
            ; send char by 8251 data port
            mov     dx, port_8251
            out     dx, al

            ; read the state byte from 8251
    l2:     mov     dx, port_8251 + 1
            in      al, dx
            and     al, 10b 
            jz      l2

            ; read from 8251
            mov     dx, port_8251
            in      al, dx
            mov     dl, al
            mov     ah, 02
            int     21h
            jmp     l1

    over:   mov     ah, 4ch
            int     21h
code        ends
end         start