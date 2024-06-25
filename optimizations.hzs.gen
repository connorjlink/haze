; with current optimizations
main:
	call $test
	pull r0
	save &$0000, r0
	copy r0, #$01
	save &$0001, r0
	load r0, &$0000
	load r1, &$0001
	iadd r0, r1
	push r0
	exit
test:
	copy r0, #$03
	push r0
	exit


; with target optimizations
main:
	call $test
	pull r0
	copy r1, #$01
	iadd r0, r1
	push r0
	exit
test:
	copy r0, #$03
	push r0
	exit


; with VERY distant future optimizations (inlining)
main:
	copy r0, #$03
	copy r1, #$01
	iadd r0, r1
	push r0
	exit
