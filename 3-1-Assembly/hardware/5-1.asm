data segment
    port    dw 298h
    mes0    db  'Start.',0dh,0ah,'$'
    mes     db  'Exit!$'
data ends

code segment
assume cs:code,ds:data

start:
    mov ax,data
    mov ds,ax

    ;��ʾ��ʾ
    lea dx,mes0
    mov ah,09h
    int 21h

llp:
    mov dx,port;����ת��
    out dx,al

    mov cx,0ffh
delay:loop delay;��ʱ

    in al,dx;��������
    mov bl,al
    mov cl,4
    shr al,cl;AL������λ
    call printn;��ʾ����λ
    mov al,bl
    and al,0fh
    call printn;��ʾ����λ
    ;�س�
    mov ah,02h
    mov dl,20h
    int 21h
    mov dl,20h
    int 21h
    
    ;call key
    ;jz llp

    push dx
    mov ah,06h
    mov dl,0ffh
    int 21h
    pop dx

    je llp

    
    ;jmp lp;ѭ����ʾ

    


printn proc near
    push ax
    push dx
    mov dl,al
    cmp dl,9
    jle print
    add dl,7;dl>9,��ʾA-F

    print:
    add dl,30h
    mov ah,02h
    int 21h
    pop dx
    pop ax
    ret
printn endp

;�����˳�
key proc near
    push dx
    mov ah,06h
    mov dl,0ffh
    int 21h
    pop dx
    ret
key endp


exit:
    
    mov ah,4ch
    int 21h

code ends
end start