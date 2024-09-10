section .data
    result db 'Result: %d', 10, 0  ; format string for printf

section .text
    global _start
    extern add_integers
    extern printf

_start:
    ; Push arguments for add_integers onto the stack
    push 10      ; Second argument (10)
    push 5       ; First argument (5)

    ; Call the add_integers function
    call add_integers

    ; Clean up the stack (remove arguments)
    add esp, 8   ; We pushed 2 arguments, each 4 bytes, so 2 * 4 = 8 bytes

    ; The result of add_integers is now in EAX

    ; Set up arguments for printf
    mov ebx, eax ; Move result to EBX (printf expects format string in EDI and value in EBX)
    mov edi, result ; Address of format string in EDI

    ; Call printf
    call printf

    ; Exit the program
    mov eax, 1   ; System call number for exit
    xor ebx, ebx ; Exit code 0
    int 0x80     ; Call kernel

; Function to add two integers
; Arguments:
;   - First argument is at [ebp + 8]
;   - Second argument is at [ebp + 12]
; Return value:
;   - Sum of the two integers in EAX

add_integers:
    ; Function prologue
    push ebp           ; Save old base pointer
    mov ebp, esp       ; Set new base pointer
    ; Function body
    mov eax, [ebp + 8] ; Load first argument (5) into EAX
    mov ebx, [ebp + 12]; Load second argument (10) into EBX
    add eax, ebx       ; Add EBX to EAX
    ; Function epilogue
    pop ebp            ; Restore old base pointer
    ret                ; Return to caller
