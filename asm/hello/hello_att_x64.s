# cs.lmu.edu/~ray/notes/gasexamples/

.set SYS_EXIT, 60

msg_hello: .ascii "Hello, world!\n"

.text
.global _start

_start:
	# write(1, msg_hello, 14)
        mov $1, %rax
        mov $1, %rdi
        mov $msg_hello, %rsi
        mov $14, %rdx
        syscall

        mov $SYS_EXIT, %rax
        xor %rdi, %rdi
        syscall
