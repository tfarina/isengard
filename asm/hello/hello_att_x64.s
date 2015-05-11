# cs.lmu.edu/~ray/notes/gasexamples/

msg_hello: .ascii "Hello, world!\n"

.text
.global _start

_start:
        mov $1, %rax
        mov $1, %rdi
        mov $msg_hello, %rsi
        mov $14, %rdx
        syscall

        mov $60, %rax
        xor %rdi, %rdi
        syscall
