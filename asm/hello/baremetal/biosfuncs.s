.code16

/* Clear the display. */
clrscr:
        pushw %bp
        movb $0x06, %ah    # BIOS function "Scroll Up Window".
        movb $0x0, %al     # Number of lines to scroll.
        movb $0x7, %bh     # Attribute to fill new lines at bottom.
        movw $0x0, %cx     # CH,CL: row, column upper left corner.
        movw $0x184f, %dx  # DH, DL: row column lower right corner.
        int $0x10          # Call BIOS.

        popw %bp
        retw


/* Set cursor position to 0:0 (top:left). */
curshome:
        movb $0x02, %ah  # BIOS function "SET CURSOR POSITION"
        movb $0x0, %bh   # Page number 0.
        movw $0x0, %dx   # DH=0 row, DL=0 column
        int $0x10        # Call BIOS.
        retw


/* Display 0-terminated string via putc(). */
puts:
        lodsb           # Load next byte from $si buffer into
        cmpb $0x0, %al  # Is %al equal 0?
        je puts1        # Yes? End of string.
        callw putc      # No? Display the current char.
        jmp puts        # Proceed to the next char.
puts1: retw


/* Output char %al via BIOS call int 10h, func 0Eh */
putc:
        movw $0x7, %bx  # BH: page 0, BL: attribute 7
        movb $0xe, %ah  # BIOS function "TELETYPE OUTPUT"
        int $0x10       # Call BIOS.
        retw
