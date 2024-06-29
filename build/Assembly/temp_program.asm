section .data
    newline db 0xA       ; Newline character
    variable_name dd 123456
section .bss
    buffer resb 12       ; Buffer to hold the converted integer string (10 digits max + newline + null terminator)

section .text
    global _start

_start:
    ; Example usage of print_integer subroutine
    mov eax, [variable_name]  ; Example integer to print
    call print_integer
    mov eax, 999
    call print_integer

    ; Exit the program
    mov eax, 1           ; syscall number for sys_exit
    xor ebx, ebx         ; exit code 0
    int 0x80             ; make kernel call

print_integer:
    push eax             ; Save eax value (integer to print)
    mov ecx, buffer + 10 ; Point ECX to the end of the number part of the buffer

    ; Add newline character at the end of the buffer
    mov byte [ecx], 0xA  ; newline character
    dec ecx              ; Move back to position for last digit

.convert_loop:
    xor edx, edx         ; Clear EDX for division
    mov ebx, 10          ; Divisor
    div ebx              ; Divide EAX by 10, quotient in EAX, remainder in EDX
    add dl, '0'          ; Convert remainder to ASCII
    mov [ecx], dl        ; Store ASCII character in buffer
    dec ecx              ; Move buffer pointer back
    test eax, eax        ; Test if quotient is zero
    jnz .convert_loop    ; If not, loop again

    inc ecx              ; Move ECX to the start of the string

    ; Calculate length of the string (including newline)
    mov edx, buffer + 11
    sub edx, ecx

    ; Write the integer string to stdout
    mov eax, 4           ; syscall number for sys_write
    mov ebx, 1           ; file descriptor 1 (stdout)
    int 0x80             ; make kernel call

    pop eax              ; Restore eax value
    ret
