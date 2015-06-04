.include "biosfuncs.s"

        /*
         * These can't be changed, because they are set by the firmware (BIOS).
         */
        .set LOAD, 0x7c00   # BIO loads and jumps here
        .set MAGIC, 0xaa55  # Must be at the end of the 512-by
        .set BLOCKSIZE, 512

.text
.code16
.globl start

start:
        /*
         * The processor starts in real mode and executes the first instruction
         * at address %0xFFFF:FFF0. System designers usually map BIOS at this
         * adddress, so the CPU starts running
         */

        /*
         * Initialize the segment descriptors %ds, %es, and %ss to 0x0.
         * %cs:%ip is already set by the BIOS to 0x0:%LOAD.
         */
        xorw %ax, %ax
        movw %ax, %es
        movw %ax, %ds
        movw %ax, %ss

        /*
         * Initialize the stack.
         *
         * Since the stack on x86 grows towards *lower* addresses,
         * we anchor it at $LOAD. Note that we don't collide with
         * the code because the stack will always remain below
         * (i.e. less than) $LOAD and grows downwards from there.
         */
        movw $LOAD, %sp

        /*
         * This is the "main" program:
         *
         * Clear the screen, move the cursor to the top:left,
         * and display a friendly greetings.
         */
        callw clrscr
        callw curshome
        callw greeting

        /*
         * That is all, folks!
         *
         * We could run a tight loop here, but it is better to halt the
         * processor. When running on bare metal, a halted processor
         * consumes less power (especially useful if ran on battery).
         * When running under an emulator, the emulator doesn't consume
         * further CPU cycles.
         */
        hlt

/* Display a little message. */
greeting:
        /*
         * greeting display the string located at label msg, using the
         * convenience function puts() defined below.
         * We pass the *address* of that string (thus $msg instead of msg) in
         * the %si register.
         */
        movw $msg, %si
        callw puts
        retw

msg: .asciz "Hello, World!\r\n"  # Must be 0-terminated.

        /*
         * The boot block MUST end with a MAGIC sequence.
         *
         * The BIOS checks this, and would refuse to boot unless
         * MAGIC is there. The last two bytes of the BLOCKSIZE
         * long block must contain the magic sequence 0x55, 0xaa.
         * We move the assembler pointer .org there, and emit the
         * word MAGIC. Note that MAGIC is set to 0xaa55, and not
         * 0x55aa, because the IA-32 platform is little endian.
         */
        .org BLOCKSIZE - 2
        .word MAGIC
