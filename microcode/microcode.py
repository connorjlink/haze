X_REG = 0b00
Y_REG = 0b01
Z_REG = 0b10
W_REG = 0b11

MOVE_REG_REG = 0b0000
LOAD_REG_MEM = 0b0001
COPY_REG_IMM = 0b0010
SAVE_MEM_REG = 0b0011
IADD_REG_REG = 0b0100
ISUB_REG_REG = 0b0101
BAND_REG_REG = 0b0110
BIOR_REG_REG = 0b0111
BXOR_REG_REG = 0b1000
BNOT_REG_XXX = 0b1001
CALL_MEM_XXX = 0b1010
EXIT_XXX_XXX = 0b1011
PUSH_REG_XXX = 0b1100
PULL_REG_XXX = 0b1101
BREZ_MEM_REG = 0b1110
RSVD_XXX_XXX = 0b1111

CTRL_RQJMP = 0x0001  # request jump
CTRL_DCDOE = 0x0002  # decode output enable immediate
CTRL_RFWEN = 0x0004  # register file write enable
CTRL_ALUOE = 0x0008  # alu output enable
CTRL_RAMIE = 0x0010  # ram input enable
CTRL_RAMOE = 0x0020  # ram output enable
CTRL_ISRST = 0x0040  # instruction step counter reset
CTRL_EXECD = 0x0080  # execute disable (#UD, halt insn, ...)
CTRL_RFRD1 = 0b0100  # register file read 1 to data bus
CTRL_RFRD2 = 0x0200  # register file read 2 to data bus
CTRL_ALUIE = 0x0400  # alu input enable for computation persistence
CTRL_DCDAO = 0x0800  # decode output enable address


microcode = [None] * (0b1111 + 1)

microcode[MOVE_REG_REG] = [CTRL_RFRD2 | CTRL_RFWEN,
                           CTRL_ISRST]

microcode[LOAD_REG_MEM] = [CTRL_RAMOE | CTRL_RFWEN,
                           CTRL_ISRST]

microcode[COPY_REG_IMM] = [CTRL_DCDOE | CTRL_RFWEN,
                           CTRL_ISRST]

microcode[SAVE_MEM_REG] = [CTRL_RFRD2 | CTRL_DCDAO | CTRL_RAMIE,
                           CTRL_ISRST]

microcode[IADD_REG_REG] = [CTRL_RFRD2 | CTRL_ALUIE,
                           CTRL_ALUOE | CTRL_RFWEN,
                           CTRL_ISRST]

microcode[ISUB_REG_REG] = [CTRL_RFRD2 | CTRL_ALUIE,
                           CTRL_ALUOE | CTRL_RFWEN,
                           CTRL_ISRST]

microcode[BAND_REG_REG] = [CTRL_RFRD2 | CTRL_ALUIE,
                           CTRL_ALUOE | CTRL_RFWEN,
                           CTRL_ISRST]

microcode[BIOR_REG_REG] = [CTRL_RFRD2 | CTRL_ALUIE,
                           CTRL_ALUOE | CTRL_RFWEN,
                           CTRL_ISRST]

microcode[BXOR_REG_REG] = [CTRL_RFRD2 | CTRL_ALUIE,
                           CTRL_ALUOE | CTRL_RFWEN,
                           CTRL_ISRST]

microcode[BNOT_REG_XXX] = [CTRL_ALUIE,
                           CTRL_ALUOE | CTRL_RFWEN,
                           CTRL_ISRST]

microcode[CALL_MEM_XXX] = [CTRL_PCHOE | CTRL_STKAO | CTRL_RAMIE,
                           CTRL_INSTK,
                           CTRL_PCLOE | CTRL_STKAO | CTRL_RAMIE,
                           CTRL_INSTK,
                           CTRL_DCDAO | CTRL_DOJMP,
                           CTRL_ISRST]

microcode[EXIT_XXX_XXX] = [CTRL_DESTK,
                           CTRL_PCCLI | CTRL_STKAO | CTRL_RAMOE,
                           CTRL_DESTK,
                           CTRL_PCCHI | CTRL_STKAO | CTRL_RAMOE,
                           CTRL_PCCAO | CTRL_DOJMP,
                           CTRL_ISRST]

microcode[PUSH_REG_XXX] = [CTRL_STKAO | CTRL_RAMOE | CTRL_RFWEN,
                           CTRL_INSTK,
                           CTRL_ISRST]

microcode[PULL_REG_XXX] = [CTRL_DESTK,
                           CTRL_RFRD1 | CTRL_STKAO | CTRL_RAMIE,
                           CTRL_ISRT]

microcode[BREZ_MEM_REG] = [CTRL_DCDAO | CTRL_RQJMP,
                           CTRL_ISRST]

microcode[RSVD_XXX_XXX] = [CTRL_EXECD]
