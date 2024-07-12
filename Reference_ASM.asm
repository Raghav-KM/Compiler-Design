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


# ---- IF Condition ---- #


# if add_exp stmt_list
# move the result of add_exp to eax reg
# compare eax to 0
# if yes -> jump to if_i label
# if no 


/*
    mov eax, [add_exp_res]
    cmp eax, 0
    jz  if_i

    ###### 
    else statments 

    jmp if_i_end
    ######

if_i:
    #####
    if statments
    #####

if_i_end:



*/