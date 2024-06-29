section .data
    msg db '2', 0xA  ; Message to write, followed by newline (0xA)

section .bss

section .text
    global _start    ; Entry point for the program

_start:
    ; Write the message to stdout
    mov eax, 4       ; syscall number for sys_write
    mov ebx, 1       ; file descriptor 1 (stdout)
    mov ecx, msg     ; pointer to the message
    mov edx, 2       ; number of bytes to write (including newline)
    int 0x80         ; make kernel call

    ; Exit the program
    mov eax, 1       ; syscall number for sys_exit
    xor ebx, ebx     ; exit code 0
    int 0x80         ; make kernel call
