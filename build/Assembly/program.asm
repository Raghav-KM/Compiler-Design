section .data

section .text
    global _start

_start:
    mov eax, 0
    mov [i_3], eax
_for1:
    mov eax, [i_3]
    mov ebx, 5
    cmp eax, ebx
    mov eax, 0
    jge _cmp1_end
    mov eax, 1
_cmp1_end:
    mov [_t1], eax
    mov eax, [_t1]
    cmp eax, 0
    jz _for1_end

    call funcA
    mov eax, [i_3]
    mov ebx, 4
    cmp eax, ebx
    mov eax, 0
    jge _cmp2_end
    mov eax, 1
_cmp2_end:
    mov [_t2], eax
    mov eax, [_t2]
    cmp eax, 1
    jne _if1

    mov al, 10
    call _print_character_subroutine

    jmp _if1_end

_if1:
_if1_end:
    mov eax, [i_3]
    add eax, 1
    mov [_t1], eax
    mov eax, [_t1]
    mov [i_3], eax
    jmp _for1

_for1_end:


    call _print_newline_subroutine
    mov eax, 1
    xor ebx, ebx
    int 0x80
  
funcA:
    mov eax, 97
    mov [i_2], eax
_for2:
    mov eax, [i_2]
    mov ebx, 122
    cmp eax, ebx
    mov eax, 0
    jg _cmp3_end
    mov eax, 1
_cmp3_end:
    mov [_t1], eax
    mov eax, [_t1]
    cmp eax, 0
    jz _for2_end

    mov al, [i_2]
    call _print_character_subroutine

    mov al, 32
    call _print_character_subroutine

    mov eax, [i_2]
    add eax, 1
    mov [_t1], eax
    mov eax, [_t1]
    mov [i_2], eax
    jmp _for2

_for2_end:

    ret

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

section .bss
    char_buffer resb 1
    newline resb 1
    i_3 resd 1
    i_2 resd 1
    _t1 resd 1
    _t2 resd 1

