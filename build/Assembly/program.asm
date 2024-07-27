section .data

section .bss
    newline resb 1
    a resd 1

section .text
    global _start

_start:
    mov eax, 1
    mov [a], eax

    call _print_newline_subroutine
    mov eax, 1
    xor ebx, ebx
    int 0x80
  
_print_newline_subroutine:
    mov ecx, newline
    mov byte [ecx], 0xA
    mov edx, 1
    mov eax, 4
    mov ebx, 1
    int 0x80
    ret

