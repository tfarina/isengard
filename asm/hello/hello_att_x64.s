# cs.lmu.edu/~ray/notes/gasexamples/

message:
        .ascii "Hello, world!\n"

.text
.global _start

_start:
        mov $1, %rax
        mov $1, %rdi
        mov $message, %rsi
        mov $14, %rdx
        syscall

        mov $60, %rax
        xor %rdi, %rdi
        syscall
