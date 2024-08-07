section .data

section .bss
    i_0 resd 1
    i_1 resd 1
    buffer  resb 12
    newline resb 1

section .text
    global _start

_start:
    mov eax, 1
    mov [i_0], eax
    mov eax, 1
    cmp eax, 1
    jne _if1

    mov eax, [i_0]
    call _print_integer_subroutine
    mov eax, 2
    mov [i_1], eax
    mov eax, [i_1]
    call _print_integer_subroutine
    jmp _if1_end

_if1:
_if1_end:

    call _print_newline_subroutine
    mov eax, 1
    xor ebx, ebx
    int 0x80
  
_print_integer_subroutine:
    push eax
    mov ecx, buffer + 10

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
    mov edx, buffer + 12
    sub edx, ecx
    mov eax, 4
    mov ebx, 1
    int 0x80
    ret


_print_newline_subroutine:
    mov ecx, newline
    mov byte [ecx], 0xA
    mov edx, 1
    mov eax, 4
    mov ebx, 1
    int 0x80
    ret


