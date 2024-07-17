section .data

section .bss
    buffer resb 12
    variable_name resd 1
    _t1 resd 1

section .text
    global _start

_start:
    mov eax, 10
    mov [variable_name], eax
    mov eax, 11
    add eax, [variable_name]
    mov [_t1], eax
    mov eax, [_t1]
    mov [variable_name], eax
    mov eax, [variable_name]
    call _print_integer_subroutine


    mov eax, 1
    xor ebx, ebx
    int 0x80
  
_print_integer_subroutine:
    push eax
    mov ecx, buffer + 10
    mov byte [ecx], 0xA
    dec ecx

    xor edx, edx
    mov ebx, 10

    cmp eax, 0
    jge ._convert_loop
    neg eax

._convert_loop:
    xor edx, edx
    div ebx
    add dl, '0'
    mov [ecx], dl
    dec ecx
    test eax, eax
    jnz ._convert_loop

    pop eax
    cmp eax, 0
    jge ._print
    mov byte [ecx], '-'
    dec ecx

._print:
    inc ecx
    mov edx, buffer + 11
    sub edx, ecx
    mov eax, 4
    mov ebx, 1
    int 0x80
    ret

