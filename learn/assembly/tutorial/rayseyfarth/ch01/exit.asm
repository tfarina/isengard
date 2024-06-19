#   Program: exit
#
#   Executes the exit system call
#
#   No input
#
#   Output: only the exit status ($? in the shell)
#   echo $?
#

.text

.global  _start

_start:
    mov  $1, %eax       # 1 is the exit syscall number
    mov  $5, %ebx       # the status value to return
    int  $0x80          # execute a system call
