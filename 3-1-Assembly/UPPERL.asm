data        segment
string      db      "aba1bDW#r!da*bF$"
len_str     dw      $-string
data        ends

code        segment
            assume cs:code, ds:data
start:      mov     ax, data
            mov     ds, ax

            mov     cx, len_str
            xor     si, si

    lp1:    mov     al, [si]
            cmp     al, 61h     ; if greater than or equal to 'a'
            jnge    next
            cmp     al, 7ah     ; if less then or equal to 'z'
            jnle    next
            sub     al, 20h    
            mov     [si], al
    next:   inc     si
            loop    lp1

            lea     dx, string
            mov     ah, 09h
            int     21h

            mov     ah, 4ch
            int     21h

code        ends
            end     start