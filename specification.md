23-bit instruction format:
x x x x  x x x x  x x x x x x x x  x x x x x x x x   
\_____/  \_/ \_/  \_____________/  \_____________/
   |      |   |          \________________/
  insn   op1 op2         |       |
                        imm     mem
registers:                           
00 - x                        
01 - y
10 - z
11 - w

instructions:
0000 - move reg, reg
0001 - load reg, imm
0010 - copy reg, mem
0011 - save mem, reg 
0100 - iadd reg, reg
0101 - isub reg, reg
0110 - band reg, reg
0111 - bior reg, reg
1000 - bxor reg, reg
1001 - call mem
1010 - exit
1011 - push reg
1100 - pull reg
1101 - brez mem, reg
1110 - XXXX
1111 - XXXX


assembly:

LabelCommand:
@identifier:

defines:
@define identifier = value

macros:
@macro identifier = ( (argument ',')* )

includes:
@include "file.hzs"
