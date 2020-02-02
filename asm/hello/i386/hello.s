#
# https://efxa.org/2011/03/02/assembly-gnulinux/
#
# This program is written with AT&T syntax.
#

.set WRITE_SYSCALL, 4
.set EXIT_SYSCALL, 1
.set STDOUT_FD, 1

.section .data

hello_str:
    .asciz "Hello, world!\n" # the string to print
    len = . - hello_str      # length of the string

.section .text
    .global main

main:
    # write(1, hello_str, 14)
    movl $WRITE_SYSCALL,     %eax  # system call number (sys_write).
    movl $STDOUT_FD,         %ebx  # first argument: file handle (stdout).
    movl $hello_str,         %ecx  # second argument: pointer to message to write.
    movl $len,               %edx  # third argument: message length.
    int $0x80                      # call linux kernel.

    # and exit.

    movl $EXIT_SYSCALL,      %eax  # system call number (sys_exit).
    movl $0,                 %ebx  # first argument: exit code.
    int $0x80                      # call linux kernel.
