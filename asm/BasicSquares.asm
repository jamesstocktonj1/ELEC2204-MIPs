add $t0, $zero, $zero		# clear outer for loop
addi $a0, $zero, 200		# max square value
# outer for loop
L1: add $t1, $zero, $zero		# clear inner for loop
add $t2, $zero, $zero		# clear temparary square value
# inner for loop
L2: add $t2, $t2, $t0			# add initial
addi $t1, $t1, 1		# increment inner for loop
slt $at, $t1, $t0		# set if inner for loop is less than outer
bne $at, $zero, L2		# branch inner for loop
sw $t2, ($sp)			# save square to memory
addi $t0, $t0, 1		# increment outer for loop
addi $sp, $sp, 4		# increment memory location
slt $at, $t0, $a0		# set if outer for loop is less than max square ($a0)
bne $at, $zero, L1		# branch outer for loop
beq $t0, $a0, L1		# branch if equal to 200
