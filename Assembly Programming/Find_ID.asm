.data
    myId: .asciiz "My Id is: "
    inputMsg: .asciiz "The input is: "
    subMsg: .asciiz "\nThe subtraction is: "
    prompt: .asciiz "\nEnter a positive integer: "
    wrongOut: .asciiz "\nNegative result exception!"
    id: .word 11223344


.text

    li $v0, 4
    la $a0, myId
    syscall

    li $v0, 1
    lw $a0, id
    move $t1, $a0
    syscall

    li $v0, 4
    la $a0, prompt
    syscall

    li $v0, 5
    syscall

    move $t0, $v0

    li $v0, 4
    la $a0, inputMsg
    syscall

    li $v0, 1
    move  $a0, $t0
    syscall

    sub $t2, $t1, $t0

    slt $t4, $t2, $zero
    beq $t4, $zero, L1
    bne $t4, $zero, L2

    L1:
       li $v0, 4
       la $a0, subMsg
       syscall

       li $v0, 1
       move $a0, $t2
       syscall
       j ex

    L2:
       li $v0, 4
       la $a0, wrongOut
       syscall
       j ex

    ex:
       li $v0, 10
       syscall
