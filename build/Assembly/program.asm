section .data

section .bss
    buffer resb 12
    a resd 1
    b resd 1
    c resd 1
    t_1 resd 1
    t_2 resd 1

section .text
    global _start

_start:
    mov eax, 1
    mov [a], eax
    mov eax, 2
    mov [b], eax
    mov eax, 3
    mov [c], eax
    mov eax, [a]
    add eax, [b]
    mov [t_2], eax
    mov eax, [t_2]
    add eax, [c]
    mov [t_1], eax
    mov eax, [t_1]
    call print_integer

    mov eax, [a]
    sub eax, [b]
    mov [t_2], eax
    mov eax, [t_2]
    sub eax, [c]
    mov [t_1], eax
    mov eax, [t_1]
    call print_integer

    mov eax, [a]
    imul eax, [b]
    mov [t_2], eax
    mov eax, [t_2]
    imul eax, [c]
    mov [t_1], eax
    mov eax, [t_1]
    call print_integer

    mov eax, [a]
    imul eax, [b]
    mov [t_2], eax
    mov eax, [t_2]
    imul eax, [c]
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

