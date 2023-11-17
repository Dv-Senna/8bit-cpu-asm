# RSASM (Really Specific Assembly)

<p align="center">
	<img src="https://img.shields.io/badge/version-1.0.0-informational" />
	<img src="https://img.shields.io/badge/Windows-success-success" />
	<img src="https://img.shields.io/badge/Linux-not tested-important" />
	<img src="https://img.shields.io/badge/MacOS-success-success" />
	<img src="https://img.shields.io/badge/Tests-passed-success" />
</p>

An open-source assembler for a really specific custom 8-bit cpu.


# Installation
 - Build the project using [premake](https://premake.github.io/docs/Using-Premake)
 - Install `bin/release/assembler(.exe)` in your path or wherever you want
 - Happy coding !

# CLI
The CLI can be used with one of this two ways :
	- `assembler (informational_flags)`
	- `assembler source_file_path (flags)`

For the second variant, you **must** place the source_file_path first.


# CPU architecture
The CPU is composed of 6 8-bits general purpose registers (`a` - `f`), an `hl` 16-bits register (`h` is the high part, `l` the low part) that is used to point to the 64kib RAM.

The stack proposed by RSASM is purely virtual. Thus, a choice had to be made concerning the stack pointer. The register `f` is choosen to be the stack pointer. Any stack-related operation will modify `e`. `CALL` will modify `a` too and `RET` `d`.

Function's paramaters are passed, in order, to `b`, `c` and `d`. If more paramaters are necessary, they are passed by the stack with the fourth paramater first pushed and the last paramater lastly pushed (on top of the stack). The return value of functions (if any) is passed through `a`.

Functions can and will modify registers. It is the responsability of the caller to save register's state to the RAM or stack (if needed) before calling a function.


# Assembly language
Here lay the documentation of RSASM instructions :

|        opcode        |     src argument     |     dst argument     |        effect        |
|----------------------|----------------------|----------------------|----------------------|
| `NOP` |  |  | set the current instruction to 0 |
| `ADD` | `lreg`, `rreg` |  | `lreg = lreg + rreg` |
| `SUB` | `lreg`, `rreg` |  | `lreg = lreg - rreg` |
| `LTEQ` | `lreg`, `rreg` |  | `lreg = lreg <= rreg` |
| `GTEQ` | `lreg`, `rreg` |  | `lreg = lreg >= rreg` |
| `LT` | `lreg`, `rreg` |  | `lreg = lreg < rreg` |
| `GT` | `lreg`, `rreg` |  | `lreg = lreg > rreg` |
| `EQ` | `lreg`, `rreg` |  | `lreg = lreg == rreg` |
| `AND` | `lreg`, `rreg` |  | `lreg = lreg & rreg` |
| `OR` | `lreg`, `rreg` |  | `lreg = lreg \| rreg` | 
| `XOR` | `lreg`, `rreg` |  | `lreg = lreg ^ rreg` |
| `NOT` | `reg` |  | `reg = ~reg` |
| `ADDS` | `lreg`, `rreg` | `dreg` | `dreg = lreg + rreg` |
| `SUBS` | `lreg`, `rreg` | `dreg` | `dreg = lreg - rreg` |
| `LTEQS` | `lreg`, `rreg` | `dreg` | `dreg = lreg <= rreg` |
| `GTEQS` | `lreg`, `rreg` | `dreg` | `dreg = lreg >= rreg` |
| `LTS` | `lreg`, `rreg` | `dreg` | `dreg = lreg < rreg` |
| `GTS` | `lreg`, `rreg` | `dreg` | `dreg = lreg > rreg` |
| `EQS` | `lreg`, `rreg` | `dreg` | `dreg = lreg == rreg` |
| `ANDS` | `lreg`, `rreg` | `dreg` | `dreg = lreg & rreg` |
| `ORS` | `lreg`, `rreg` | `dreg` | `dreg = lreg \| rreg` | 
| `XORS` | `lreg`, `rreg` | `dreg` | `dreg = lreg ^ rreg` |
| `NOTS` | `reg` | `dreg` | `dreg = ~reg` |
| `LDX` | `reg` |  | `reg = value of next byte` |
| `RI` | `sreg` | `dreg` | `dreg = IO[sreg]` DOES NOTHING RIGHT NOW |
| `CARRY` |  | `dreg` | `dreg = carry bit` |
| `WI` | `lreg`, `rreg` |  | `IO[lreg] = rreg` DOES NOTHING RIGHT NOW |
| `COPY` | `sreg` | `dreg` | `dreg = sreg` |
| `JHL` |  |  | `pc = hl` |
| `JIHL` | `sreg` |  | `if sreg == 0xff then pc = hl` |
| `JZHL` | `sreg` |  | `if sreg == 0 then pc = hl` |
| `JCHL` |  |  | `if carry bit == 1 then pc = hl` |
| `MWRITE` | `sreg` |  | `RAM[hl] = sreg` |
| `MREAD` |  | `dreg` | `dreg = RAM[hl]` |
| `BYTE` | `number` |  | set the current instruction to `number` |
| `MOV` | `number` | `dreg` | `dreg = number` |
| `PUSH` | `sreg` |  | `stack.push(sreg)` |
| `POP` |  | `dreg` | `dreg = stack.pop()` |
| `CALL` | `tag` |  | `push pc; jmp tag` |
| `RET` |  |  | `pop pc` |
| `JMP` | `tag` |  | `pc = addr(tag)` |
| `JI` | `tag`, `sreg` |  | `if sreg == 0xff then pc = addr(tag)` |
| `JZ` | `tag`, `sreg` |  | `if sreg == 0 then pc = addr(tag)` |
| `JC` | `tag` |  | `if carry bit == 1 then pc = addr(tag)` |