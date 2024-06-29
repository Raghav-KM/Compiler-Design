section .data
    variable_name dd 125

section .bss
    buffer resb 12

section .text
    global _start

_start:
    push eax
    mov eax, 123
    call print_integer
    pop eax
    push eax
    mov eax, [variable_name]
    call print_integer
    pop eax

    mov eax, 1
    xor ebx, ebx
    int 0x80
  
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
