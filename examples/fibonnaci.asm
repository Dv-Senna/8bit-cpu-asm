# this program output the nth fibonacci's serie's element

mov a, 1
mov b, 1
mov c, 1
mov f, 10 # the index of the element you want, starting from 0

jz f, end
mov d, 1
sub f, d
jz f, end

loop:
	copy b, a
	copy a, c
	adds a, b, c
	sub f, d
	jz f, end
	jmp loop


end:
	nop