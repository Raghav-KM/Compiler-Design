# ---- Print Integer ---- #

# Move the value to print in eax #
print_integer:
    push eax
    mov ecx, buffer + 10
    mov byte [ecx], 0xA
    dec ecx   

.convert_loop:
    xor edx, edx     
    mov ebx, 10
    div ebx
    add dl, '0'
    mov [ecx], dl
    dec ecx
    test eax, eax
    jnz .convert_loop
    inc ecx
    mov edx, buffer + 11
    sub edx, ecx
    mov eax, 4          
    mov ebx, 1           
    int 0x80             
    pop eax              
    ret


# ---- IF-ELSE ---- #


# if add_exp stmt_list
# move the result of add_exp to eax reg
# compare eax to 0
# if yes -> jump to if_i label
# if no 

    mov eax, [comp_exp_res]
    cmp eax, 0
    jz  if_i

    ; else statments 
    jmp if_i_end

if_i:
    ; if statments

if_i_end:



# ---- FOR ---- #

# let_stmt
# check condition
# label : for_i_start
## condition false 
#### jmp for_i_end
## statments 
## update statments
## jmp for_i_start 
## label : for_i_end

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
    

