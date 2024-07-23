section .data

section .bss
    buffer  resb 12
    newline resb 1
    i resd 1
    j resd 1
    _t1 resd 1
    _t2 resd 1
    _t3 resd 1

section .text
    global _start

_start:
    mov eax, 1
    mov [i], eax
_for1:
    mov eax, [i]
    mov ebx, 10
    cmp eax, ebx
    call _compare_less_equal_subroutine
    mov [_t1], eax
    mov eax, [_t1]
    cmp eax, 0
    jz _for1_end

    mov eax, [i]
    mov [j], eax
_for2:
    mov eax, [j]
    mov ebx, 10
    cmp eax, ebx
    call _compare_less_equal_subroutine
    mov [_t1], eax
    mov eax, [_t1]
    cmp eax, 0
    jz _for2_end

    mov eax, [i]
    add eax, [j]
    mov [_t3], eax
    mov eax, [_t3]
    mov ebx, 10
    cmp eax, ebx
    call _compare_equal_subroutine
    mov [_t2], eax
    mov eax, [_t2]
    cmp eax, 0
    jnz _if1

    jmp _if1_end

_if1:
    mov eax, [i]
    call _print_integer_subroutine

    mov eax, [j]
    call _print_integer_subroutine

_if1_end:
    mov eax, [j]
    add eax, 1
    mov [_t1], eax
    mov eax, [_t1]
    mov [j], eax
    jmp _for2

_for2_end:

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
