#
# https://efxa.org/2011/03/02/assembly-gnulinux/
#
# This program is written with AT&T syntax.
#--------------------------------------------------------------------------#


#---------------------------- CONSTANTS -----------------------------------#
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
.set SYSCALL_EXIT, 1  # int status
.set SYSCALL_WRITE, 4 # int fd, const void *buf, size_t count
.set STDOUT_FILENO, 1

#------------------------------ DATA --------------------------------------#
.section .data

hello_str:
    .asciz "Hello, world!\n" # the string to print
    len = . - hello_str      # length of the string

#---------------------------- MAIN CODE -----------------------------------#
.section .text

.global _start

.type _start, @function
_start:
    # write(1, hello_str, 14)
    movl $len,               %edx  # third argument: message length.
    movl $hello_str,         %ecx  # second argument: pointer to message to write.
    movl $STDOUT_FILENO,     %ebx  # first argument: file handle (stdout).
    movl $SYSCALL_WRITE,     %eax  # system call number (sys_write).
    int $0x80                      # call linux kernel.

    call L_exit

L_exit:
    movl $0,                 %ebx  # first argument: exit code.
    movl $SYSCALL_EXIT,      %eax  # system call number (sys_exit).
    int $0x80                      # call linux kernel.
