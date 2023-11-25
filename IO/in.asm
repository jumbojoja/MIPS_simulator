        addi $s0, $zero, 1
        addi $s1, $zero, 5
        addi $s2, $zero, 31
        addi $s3, $zero, 32
        add  $a0, $s0, $s0
        sub  $a1, $a0, $s0
        sll  $a2, $s0, 1
        srl  $a3, $s2, 2
        ori  $t0, $s1, 34
        or   $t1, $zero, $s1
        andi $t2, $t1, 37
        and  $t3, $t0, $s1
        nor  $t4, $s0, $s3
        addi $a0, $zero, 1
        addi $a1, $zero, 1
loop:   addi $a1, $a1, 1
        jal  funcA
        bne  $a0, $s1, loop
        jal  funcB
        jal  end
funcA:  addi $a0, $a0, 1
        jr   $ra  
funcB:  sll  $s0, $s0, 1
        beq  $s0, $s3, funcBE
        jal  funcB
funcBE: addi $a1, $a1, 1
        jr   $ra
end:    sub  $s0, $s0, $s0
