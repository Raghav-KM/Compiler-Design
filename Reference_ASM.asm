# ---- Print Integer ---- #

# Move the value to print in eax #


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

# Print new_line
_print_newline_subroutine:
    mov ecx, newline
    mov byte [ecx], 0xA
    mov edx, 1
    mov eax, 4
    mov ebx, 1
    int 0x80
    ret



# ---- IF-ELSE ---- #

    mov eax, [comp_exp_res]
    cmp eax, 0
    jz  if_i

    ; else statments 
    jmp if_i_end

if_i:
    ; if statments

if_i_end:



# ---- FOR ---- #

    ;let_statement
for_i:
    ;comp_exp_statement
    mov eax, [comp_exp_res]
    cmp eax, 0
    jz for_i_end

    ;for_statments

    ;assign_statments
    jmp for_i

for_i_end:
    

