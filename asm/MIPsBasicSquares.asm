add $t0, $zero, $zero
addi $a0, $zero, 200
L1: add $t1, $zero, $zero
add $t2, $zero, $zero
L2: add $t2, $t2, $t0
addi $t1, $t1, 1
slt $at, $t1, $t0
bne $at, $zero, L2
sw $t2, ($sp)
addi $t0, $t0, 1
addi $sp, $sp, 4
slt $at, $t0, $a0
bne $at, $zero, L1
beq $t0, $a0, L1
