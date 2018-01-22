data segment
    port    dw 290h
    mes0    db  'Start.',0dh,0ah,'$'
    mes     db  'Exit!$'
    ;正弦波数据表
    sin     db  80h,96h,0aeh,0c5h,0d8h,0e9h,0f5h,0fdh
            db  0ffh,0fdh,0f5h,0e9h,0d8h,0c5h,0aeh,96h
            db  80h,66h,4eh,38h,25h,15h,09h,04h
            db  00h,04h,09h,15h,25h,38h,4eh,66h
data ends

code segment
assume cs:code,ds:data

start:
    mov ax,data
    mov ds,ax

    lea dx,mes0
    mov ah,09h
    int 21h

lp:
    lea si,sin
    mov bh,32h
llp:
    mov al,[si]
    mov dx,port
    out dx,al

    ;按键退出
    mov ah,06h
    mov dl,0ffh
    int 21h
    jne exit
    mov cx,1
    delay:loop delay;延时
    inc si
    dec bh
    jne llp
    jmp lp

exit:
    lea dx,mes
    mov ah,09h
    int 21h
    mov ah,4ch
    int 21h

code ends
end start