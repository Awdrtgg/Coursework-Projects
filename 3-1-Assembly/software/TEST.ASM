data    segment
aaa     db     "aaa$"
data    ends

code    segment
        assume cs:code, ds:data
        start:
        mov ax, data
        mov ds, ax

        mov bl, 0abh
        add bl, 09ch
        
        mov al, 80h
        sub al, 0

        mov bl, 0abh
        add bl, 09ch

        mov al, 80h
        sbb al, 0

        mov bl, 0abh
        add bl, 09ch

        mov al, 8ch
        sbb al, 0


        mov ah, 4ch
        int 21h
code    ends
        end start

