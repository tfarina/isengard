--- prog list "add" ---
0000 (add.go:3) TEXT    add+0(SB),$0-24
0001 (add.go:3) FUNCDATA $0,gcargs·0+0(SB)
0002 (add.go:3) FUNCDATA $1,gclocals·0+0(SB)
0003 (add.go:3) TYPE    a+0(FP){int},$8
0004 (add.go:3) TYPE    b+8(FP){int},$8
0005 (add.go:3) TYPE    ~anon2+16(FP){int},$8
0006 (add.go:4) MOVQ    a+0(FP),BX
0007 (add.go:4) MOVQ    b+8(FP),BP
0008 (add.go:4) ADDQ    BP,BX
0009 (add.go:4) MOVQ    BX,~anon2+16(FP)
0010 (add.go:4) RET     ,
