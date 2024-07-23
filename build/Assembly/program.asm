section .data

section .bss
    char_buffer resb 1
    newline resb 1
    i resd 1
    _t1 resd 1

section .text
    global _start

_start:
    mov eax, 0
    mov [i], eax
_for1:
    mov eax, [i]
    mov ebx, 9
    cmp eax, ebx
    call _compare_less_equal_subroutine
    mov [_t1], eax
    mov eax, [_t1]
    cmp eax, 0
    jz _for1_end

    mov al, 'i'
    call _print_character_subroutine

    mov al, ' '
    call _print_character_subroutine

    mov eax, [i]
    add eax, 1
    mov [_t1], eax
    mov eax, [_t1]
    mov [i], eax
    jmp _for1

_for1_end:


    call _print_newline_subroutine
    mov eax, 1
    xor ebx, ebx
    int 0x80
  
_print_character_subroutine:
    mov ecx, char_buffer 
    mov [ecx], al
    mov edx, 1 
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


_compare_equal_subroutine:
    je _true
    mov eax, 0 
    ret

_compare_less_subroutine:
    jl _true
    mov eax, 0
    ret

_compare_greater_subroutine:
    jg _true
    mov eax, 0
    ret

_compare_less_equal_subroutine:
    jl _true
    je _true
    mov eax, 0
    ret

_compare_greater_equal_subroutine:
    jg _true
    je _true
    mov eax, 0
    ret

_compare_not_equal_subroutine:
    jne _true
    mov eax, 0
    ret

_true:
    mov eax, 1
    ret
