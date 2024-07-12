section .data

section .bss
    buffer resb 12
    conditionA resd 1
    varA resd 1

section .text
    global _start

_start:
    mov eax, 0
    mov [conditionA], eax
    mov eax, 2
    mov [varA], eax
    mov eax, [conditionA]
    cmp eax, 0
    jnz if_1

    jmp if_1_end

if_1:
    mov eax, 1
    call print_integer

if_1_end:
    mov eax, [varA]
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

