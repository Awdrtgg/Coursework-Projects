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

    ;显示提示
    lea dx,mes0
    mov ah,09h
    int 21h

llp:
    mov dx,port;启动转换
    out dx,al

    mov cx,0ffh
delay:loop delay;延时

    in al,dx;输入数据
    mov bl,al
    mov cl,4
    shr al,cl;AL右移四位
    call printn;显示高四位
    mov al,bl
    and al,0fh
    call printn;显示低四位
    ;回车
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

    
    ;jmp lp;循环显示

    


printn proc near
    push ax
    push dx
    mov dl,al
    cmp dl,9
    jle print
    add dl,7;dl>9,显示A-F

    print:
    add dl,30h
    mov ah,02h
    int 21h
    pop dx
    pop ax
    ret
printn endp

;按键退出
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