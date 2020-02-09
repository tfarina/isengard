#
# https://efxa.org/2011/03/02/assembly-gnulinux/
#
# This program is written with AT&T syntax.
#

#
# SEE: http://www.linfo.org/system_call_number.html
#
# A system call number is a unique integer (i.e., whole number), from one to
# around 256, that is assigned to each system call in a Unix-like operating
# system.
#
# The system call numbers are listed in the file /usr/include/asm/unistd.h,
# and they can be viewed as:
#
#  cat /usr/include/i386-linux-gnu/asm/unistd_32.h | less
#
.set SYSCALL_EXIT, 1
.set SYSCALL_WRITE, 4
.set STDOUT_FILENO, 1

.section .data

hello_str:
    .asciz "Hello, world!\n" # the string to print
    len = . - hello_str      # length of the string

.section .text
    .global _start

exit:
    movl $SYSCALL_EXIT,      %eax  # system call number (sys_exit).
    movl $0,                 %ebx  # first argument: exit code.
    int $0x80                      # call linux kernel.

_start:
    # write(1, hello_str, 14)
    movl $SYSCALL_WRITE,     %eax  # system call number (sys_write).
    movl $STDOUT_FILENO,     %ebx  # first argument: file handle (stdout).
    movl $hello_str,         %ecx  # second argument: pointer to message to write.
    movl $len,               %edx  # third argument: message length.
    int $0x80                      # call linux kernel.

    call exit
