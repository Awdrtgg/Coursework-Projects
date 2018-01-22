data segment
    port    dw 290h
    mes0    db  'Start.',0dh,0ah,'$'
    mes     db  'Exit!$'
data ends

code segment
assume cs:code,ds:data

start:
    mov ax,data
    mov ds,ax

    lea dx,mes0
    mov ah,09h
    int 21h
    
    mov cl,0
    mov dx,port

lp:
    mov al,cl
    out dx,al
    add cl,1
    call key
    jz lp

    lea dx,mes
    mov ah,09h
    int 21h

    mov ah,4ch
    int 21h

key proc near
    push dx
    mov ah,06h
    mov dl,0ffh
    int 21h
    pop dx
    ret
key endp


code ends
end start