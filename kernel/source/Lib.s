.section .data
scan_code_table:
	.byte 0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0
	.byte 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0
	.byte 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\'
	.byte 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '

.section .text

.global set_cursor
.type set_cursor, @function
set_cursor:
	ret

.global disable_cursor
.type disable_cursor, @function
disable_cursor:
	pushf
	pushl %eax
	pushl %edx
	
	movw $0x3D4, %dx 
	movb $0xA, %al
	outb %al, %dx 
	incw %dx
	mov $0x20, %al
	outb %al, %dx

	popl %edx
	popl %eax
	popf
	ret

.global timestamp
.type timestamp, @function
timestamp:
	pushl %edx
	rdtsc
	popl %edx
	ret

.global shutdown
.type shutdown, @function
shutdown:
	movw $0x5301, %ax
	xorw %bx, %bx
	int $0x15

	movw $0x530e, %ax
	xorw %bx, %bx
	movw $0x0102, %cx
	int $0x15

	movw $0x5307, %ax
	movw $0x0001, %bx
	movw $0x0003, %cx
	int $0x15

.global poll_keyboard
.type poll_keyboard, @function
poll_keyboard:
	# read status register and check for readiness
	inb $0x64, %al
	testb $1, %al
	jnz poll 
	xorb %al, %al
	ret
poll:
	# read scan code
	inb $0x60, %al
	movzx %al, %eax
	cmp 58, %eax
	ja not_ascii
	# convert to ASCII
	movb scan_code_table(, %eax, 1), %al
	ret
not_ascii:
	mov $'?', %al
	ret
