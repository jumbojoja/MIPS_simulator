        addi $s0, $zero, 1
        addi $s1, $zero, 2
        addi $s2, $zero, 3
        addi $s3, $zero, 4
        addi $t8, $zero, 64
        add $t0, $zero, $zero
        add $t1, $zero, $zero
loop:   andi $t1, $t1, 255
        add $t1, $t1, $t2
        or $t1, $t1, $t2
        ori $t1, $t1, 256
        sub $t1, $t1, $t2
        sub $s2, $s2, $s0
        nor $t1, $t1, $t2
        and $t1, $t1, $t2
        jal func
        sll $t3, $t2, 2
        srl $t3, $t2, 1
        bne $t0, $s2, loop
        and $s3, $s3, $zero
        beq $t0, $s3, end
        addi $t0, $t0, 1
func:   addi $t4, $zero, 1
        sub $t4, $t4, $t5
        sll $t4, $t4, 10
        jr $ra
end:    addi $t5, $zero, 1
        sll $t5, $t5, 10