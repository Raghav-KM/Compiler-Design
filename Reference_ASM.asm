# ---- Print Integer ---- #

# Move the value to print in eax #

print_integer:
    push eax
    mov ecx, buffer + 10
    mov byte [ecx], 0xA
    dec ecx   

.convert_loop:
    xor edx, edx     
    mov ebx, 10
    div ebx
    add dl, '0'
    mov [ecx], dl
    dec ecx
    test eax, eax
    jnz .convert_loop
    inc ecx
    mov edx, buffer + 11
    sub edx, ecx
    mov eax, 4          
    mov ebx, 1           
    int 0x80             
    pop eax              
    ret

