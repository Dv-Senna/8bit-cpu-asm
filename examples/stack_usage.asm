# the calling convention is as follow :
#    - paramaters are sended through b, c and d
#    - f is the stack pointer
#    - return values are sended through a
#    - you should not assume that any register are preserved, except for the exceptions cited above


jmp main



multiply: # 2 paramaters
	xor a, a
	mov e, 1

	multiplyLoop:
		jz c, multiplyEnd
		add a, b
		sub c, e
		jmp multiplyLoop

	multiplyEnd:
		ret


power: # 2 paramaters
	push b
	mov a, 1

	powerLoop:
		jz c, powerEnd
		xor h, h
		copy l, f
		mov b, 2
		sub l, b
		mread b

		push c
		copy c, a
		call multiply

		pop c
		mov e, 1
		sub c, e
		jmp powerLoop

	powerEnd:
		pop b
		ret


main:
	mov b, 6
	mov c, 5
	call multiply

	push a

	mov b, 2
	mov c, 4
	call power

	pop b
	

end:
	nop