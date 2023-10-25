.global main
.syntax unified

test:
    .thumb

    PUSH            {R4-R7,LR}
    MOV             R4, R0
    MOV             R5, R1
    LDRB            R6, [R4]
    LDRB            R0, [R4,#1]
    CMP             R0, #2
    BNE             loc_backdoor
    LDRB            R6, [R4,#2]


loc_backdoor:
    LDR             R0, =0x20003000
    CMP             R4, R0
    BNE             loc_5CA0
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
    .byte     0xBF
    .byte     0xF3
    .byte     0x60
    .byte     0x8F

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

loc_5CA0:
    LDR             R0, =0x6EA5
    CMP             R4, R0
    BNE             loc_5CA8
    MOVS            R6, #0x35

loc_5CA8:
    LDR             R0, =0x6E66
    CMP             R4, R0
    BNE             loc_5CB0
    MOVS            R6, #0x3F

loc_5CB0:
    LDR             R0, =0x6EDA
    CMP             R4, R0
    BNE             loc_5CB8
    MOVS            R6, #0x83

loc_5CB8:
    CMP             R6, R5
    BLE             loc_5CBE
    MOV             R6, R5

loc_5CBE:
    LDR             R1, =0x40060518
    LDR             R0, [R1]

    MOVS            R2, #0x80
    BICS            R0, R2
    ADDS            R0, #0x80

    STR             R0, [R1]

    MOV             R2, R6
    MOV             R1, R4
    MOVS            R0, #1
    BL              sub_5894
    POP             {R4-R7,PC}
