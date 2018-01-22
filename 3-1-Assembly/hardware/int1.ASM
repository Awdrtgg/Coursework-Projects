code  segment
	assume cs:code
start:
	; set the 8255
	mov  dx, 28bh
	mov  al, 10111110b
	out  dx, al
	
	; save the old intr
	mov  ax, 3508h
	int  21h
	push es
	push bx
	cli
	push ds
	
	; set the new intr
	mov  dx, offset intr
	mov  ax, seg intr
	mov  ax, 2508h
	int  21h

	
	pop  ds
	in   al, 21h
      push ax
	and  al, 0f7h
	out  21h, al
	sti

again:
	jb   again
	cli
	pop  ax
	out  21h, al
	pop  dx
	pop  ds
	mov  ax, 2508h
	int  21h
	sti
	mov  ax, 4c00h
	int  21h
	end  start
	
intr  proc
	sti
	mov  dx, 28ah
	in   ax, dx

	mov  bx, ax
	mov  ax, 
	int  21h	
	
	jc   endi
	mov  al, ffh
	out  21h, al
	sti

endi: mov  al, 20h
	out  20h, al
	iret
intr  endp