
# Instruction Set - Von Neumann CPU Simulator

| **Mnemonic** | **Opcode (Binary)** | **Description** |
|------------- |-------------------- |---------------- |
| `hlt`        | `00000`             | HALT: Stops the processor. No register is modified. Used at end of program. |
| `nop`        | `00001`             | NO OPERATION: Increments PC. No other changes. |
| `ldr rX, rY` | `00010`             | LOAD VIA REGISTER: `rX = rY` |
| `str rX, rY` | `00011`             | STORE VIA REGISTER: `rY = rX` |
| `add rX, rY` | `00100`             | ADD REGISTER: `rX = rX + rY` |
| `sub rX, rY` | `00101`             | SUBTRACT REGISTER: `rX = rX - rY` |
| `mul rX, rY` | `00110`             | MULTIPLY REGISTER: `rX = rX * rY` |
| `div rX, rY` | `00111`             | DIVIDE REGISTER: `rX = rX / rY` |
| `cmp rX, rY` | `01000`             | COMPARE REGISTER: Sets flags E, L, G based on comparison between `rX` and `rY`. |
| `movr rX, rY`| `01001`             | MOVE REGISTER: `rX = rY` |
| `and rX, rY` | `01010`             | LOGICAL AND: `rX = rX & rY` |
| `or rX, rY`  | `01011`             | LOGICAL OR: `rX = rX | rY` |
| `xor rX, rY` | `01100`             | LOGICAL XOR: `rX = rX ^ rY` |
| `not rX`     | `01101`             | LOGICAL NOT: `rX = !rX` |
| `je Z`       | `01110`             | JUMP IF EQUAL: If `E == 1`, sets `PC = Z`. |
| `jne Z`      | `01111`             | JUMP IF NOT EQUAL: If `E == 0`, sets `PC = Z`. |
| `jl Z`       | `10000`             | JUMP IF LOWER: If `L == 1`, sets `PC = Z`. |
| `jle Z`      | `10001`             | JUMP IF LOWER OR EQUAL: If `E == 1` or `L == 1`, sets `PC = Z`. |
| `jg Z`       | `10010`             | JUMP IF GREATER: If `G == 1`, sets `PC = Z`. |
| `jge Z`      | `10011`             | JUMP IF GREATER OR EQUAL: If `E == 1` or `G == 1`, sets `PC = Z`. |
| `jmp Z`      | `10100`             | UNCONDITIONAL JUMP: Sets `PC = Z`. |
| `ld rX, Z`   | `10101`             | LOAD FROM MEMORY: Loads 16-bit word from memory address `Z` into `rX`. |
| `st rX, Z`   | `10110`             | STORE TO MEMORY: Stores 16-bit value from `rX` to memory address `Z`. |
| `movi rX, IMM` | `10111`           | MOVE IMMEDIATE: `rX = IMM`. |
| `addi rX, IMM` | `11000`          | ADD IMMEDIATE: `rX = rX + IMM`. |
| `subi rX, IMM` | `11001`          | SUBTRACT IMMEDIATE: `rX = rX - IMM`. |
| `muli rX, IMM` | `11010`          | MULTIPLY IMMEDIATE: `rX = rX * IMM`. |
| `divi rX, IMM` | `11011`          | DIVIDE IMMEDIATE: `rX = rX / IMM`. |
| `lsh rX, IMM`  | `11100`          | LEFT SHIFT: Shifts `rX` left by `IMM` bits. |
| `rsh rX, IMM`  | `11101`          | RIGHT SHIFT: Shifts `rX` right by `IMM` bits. |
