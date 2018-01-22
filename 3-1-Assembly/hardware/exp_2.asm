port_io     equ     0d400h - 0280h
port_8255a  equ     port_io + 0288h
port_8255b  equ     port_io + 028bh
port_8255c  equ     port_io + 28ah

code        segment
            assume cs: code
    start:  
            mov     dx, port_8255b
            mov     al, 8bh
            out     dx, al

    inout:  mov     dx, port_8255c
            in      al, dx
            mov     dx, port_8255a
            out     dx, al
            mov     dl, 0ffh
            mov     ah, 06h
            int     21h
            jz      inout
            mov     ah, 4ch
            int     21h
code        ends
end         start