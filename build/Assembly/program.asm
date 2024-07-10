section .data

section .bss
    buffer resb 12
    t_1 resd 1
    t_2 resd 1
    t_3 resd 1
    t_4 resd 1

section .text
    global _start

_start:
    mov eax, 4
    imul eax, 10
    mov [t_3], eax
    mov eax, 1
    add eax, 13
    mov [t_4], eax
    mov eax, [t_4]
    sub eax, [t_3]
    mov [t_2], eax
    mov eax, [t_2]
    add eax, 20
    mov [t_1], eax
    mov eax, [t_1]
    call print_integer


    mov eax, 1
    xor ebx, ebx
    int 0x80
  
print_integer:
    push eax
    mov ecx, buffer + 10
    mov byte [ecx], 0xA
    dec ecx

    xor edx, edx
    mov ebx, 10

    cmp eax, 0
    jge .convert_loop
    neg eax

.convert_loop:
    xor edx, edx
    div ebx
    add dl, '0'
    mov [ecx], dl
    dec ecx
    test eax, eax
    jnz .convert_loop

    pop eax
    cmp eax, 0
    jge .print
    mov byte [ecx], '-'
    dec ecx

.print:
    inc ecx
    mov edx, buffer + 11
    sub edx, ecx
    mov eax, 4
    mov ebx, 1
    int 0x80
    ret

