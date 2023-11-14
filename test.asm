mov a, 5
mov b, 3

# copy original data to avoid modifying b
copy d, b

# while d != 0
loop:
    jmpz d, end

    # c += a
    add c, a

    # d--
    mov e, 1
    sub d, e
    jmp loop

end:
    nop
# this basically makes c = a + a + a
# c == 15