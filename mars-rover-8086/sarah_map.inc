; ============================================
; sarah_map.inc — Map & Display
; ============================================

DRAW_GRID PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI

    CMP BYTE PTR grid_initialized, 0
    JNE SKIP_INIT
    CALL INIT_GRID
    MOV BYTE PTR grid_initialized, 1

SKIP_INIT:
    MOV AL, last_drawn_row
    SHL AL, 1
    SHL AL, 1
    SHL AL, 1
    ADD AL, last_drawn_col
    LEA BX, grid
    MOV AH, 0
    ADD BX, AX

    CMP BYTE PTR [BX], 'O'  
    JE SKIP_CLEAR
    CMP BYTE PTR [BX], 'T'  
    JE SKIP_CLEAR
    
    MOV BYTE PTR [BX], '.'  

SKIP_CLEAR:
    MOV AL, rover_row
    SHL AL, 1
    SHL AL, 1
    SHL AL, 1
    ADD AL, rover_col
    LEA BX, grid
    MOV AH, 0
    ADD BX, AX               

    MOV CL, current_status
    CMP CL, 2                
    JE SAVE_LAST_DRAWN
    CMP CL, 1                
    JE STAMP_X

    MOV BYTE PTR [BX], 'R'   
    JMP SAVE_LAST_DRAWN    

STAMP_X:
    MOV BYTE PTR [BX], 'X'

SAVE_LAST_DRAWN:
    MOV AL, rover_row
    MOV last_drawn_row, AL
    MOV AL, rover_col
    MOV last_drawn_col, AL

    CALL CLEAR_SCREEN

    LEA DX, title_msg
    MOV AH, 09h
    INT 21h
    CALL PRINT_NEWLINE

    LEA DX, col_header
    MOV AH, 09h
    INT 21h

    LEA SI, grid
    MOV BYTE PTR row_cnt, 0

DRAW_ROW:
    MOV DL, row_cnt
    ADD DL, '0'
    MOV AH, 02h
    INT 21h

    MOV DL, ' '
    MOV AH, 02h
    INT 21h
    MOV DL, ' '
    MOV AH, 02h
    INT 21h

    MOV CL, 8
DRAW_CELL:
    MOV DL, [SI]
    MOV AH, 02h
    INT 21h
    MOV DL, ' '
    MOV AH, 02h
    INT 21h
    INC SI
    DEC CL
    JNZ DRAW_CELL

    CALL PRINT_NEWLINE

    INC BYTE PTR row_cnt
    CMP BYTE PTR row_cnt, 8
    JB DRAW_ROW

    CALL PRINT_NEWLINE
    LEA DX, legend_msg
    MOV AH, 09h
    INT 21h

    CALL DISPLAY_BATTERY

    MOV CL, current_status
    CMP CL, 1
    JE DRAW_SHOW_CRASH
    CMP CL, 2
    JE DRAW_SHOW_BOUND
    CMP CL, 3
    JE DRAW_SHOW_SUCCESS
    CMP CL, 4
    JE DRAW_SHOW_TARGET_WIN
    CMP CL, 5
    JE DRAW_SHOW_TARGET_MISS
    
    JMP DRAW_DONE            

DRAW_SHOW_CRASH:
    MOV DL, 07h              
    MOV AH, 02h
    INT 21h
    LEA DX, crash_line1      
    MOV AH, 09h
    INT 21h
    MOV DL, rover_row        
    ADD DL, '0'
    MOV AH, 02h
    INT 21h
    LEA DX, coord_comma      
    MOV AH, 09h
    INT 21h
    MOV DL, rover_col        
    ADD DL, '0'
    MOV AH, 02h
    INT 21h
    LEA DX, coord_close      
    MOV AH, 09h
    INT 21h
    LEA DX, aborted_msg      
    MOV AH, 09h
    INT 21h
    JMP DRAW_DONE

DRAW_SHOW_BOUND:
    LEA DX, bound_msg
    MOV AH, 09h
    INT 21h
    JMP DRAW_DONE

DRAW_SHOW_SUCCESS:
    LEA DX, success_line1    
    MOV AH, 09h
    INT 21h
    MOV DL, rover_row        
    ADD DL, '0'
    MOV AH, 02h
    INT 21h
    LEA DX, coord_comma      
    MOV AH, 09h
    INT 21h
    MOV DL, rover_col        
    ADD DL, '0'
    MOV AH, 02h
    INT 21h
    LEA DX, coord_close      
    MOV AH, 09h
    INT 21h
    LEA DX, fuel_consumed_msg
    MOV AH, 09h
    INT 21h
    MOV AL, initial_fuel
    SUB AL, fuel             
    MOV AH, 0
    MOV BL, 10
    DIV BL                   
    MOV BH, AH               
    CMP AL, 0
    JE SKIP_TENS
    MOV DL, AL
    ADD DL, '0'
    MOV AH, 02h
    INT 21h
SKIP_TENS:
    MOV DL, BH               
    ADD DL, '0'
    MOV AH, 02h
    INT 21h
    CALL PRINT_NEWLINE
    LEA DX, remaining_msg
    MOV AH, 09h
    INT 21h
    MOV AL, fuel
    MOV AH, 0
    MOV BL, 10
    DIV BL
    MOV BH, AH               
    CMP AL, 0
    JE SKIP_BATT_TENS
    MOV DL, AL
    ADD DL, '0'
    MOV AH, 02h
    INT 21h
SKIP_BATT_TENS:
    MOV DL, BH
    ADD DL, '0'
    MOV AH, 02h
    INT 21h
    LEA DX, batt_units
    MOV AH, 09h
    INT 21h
    LEA DX, press_key_msg
    MOV AH, 09h
    INT 21h
    JMP DRAW_DONE

DRAW_SHOW_TARGET_WIN:
    LEA DX, target_win_msg
    MOV AH, 09h
    INT 21h
    JMP DRAW_DONE

DRAW_SHOW_TARGET_MISS:
    LEA DX, target_miss_msg
    MOV AH, 09h
    INT 21h

DRAW_DONE:
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
DRAW_GRID ENDP

DELAY PROC
    PUSH CX
    MOV CX, 0080h 
DELAY_LOOP:
    NOP
    LOOP DELAY_LOOP
    POP CX
    RET
DELAY ENDP

INIT_GRID PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH SI

    LEA SI, obstacles
    MOV CL, 6

PLACE_NEXT:
    MOV AL, [SI]             
    SHL AL, 1
    SHL AL, 1
    SHL AL, 1                
    INC SI
    ADD AL, [SI]             
    LEA BX, grid
    MOV AH, 0
    ADD BX, AX
    MOV BYTE PTR [BX], 'O'
    INC SI
    DEC CL
    JNZ PLACE_NEXT

    CMP game_mode, 2
    JNE SKIP_TARGET_DRAW
    
    MOV AL, target_row
    SHL AL, 1
    SHL AL, 1
    SHL AL, 1
    ADD AL, target_col
    LEA BX, grid
    MOV AH, 0
    ADD BX, AX
    MOV BYTE PTR [BX], 'T'

SKIP_TARGET_DRAW:
    POP SI
    POP CX
    POP BX
    POP AX
    RET    
INIT_GRID ENDP

DISPLAY_BATTERY PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX

    LEA DX, batt_msg
    MOV AH, 09h
    INT 21h

    MOV AL, fuel
    MOV AH, 0
    MOV BL, 10
    DIV BL
    MOV BH, AH               

    MOV DL, AL
    ADD DL, '0'
    MOV AH, 02h
    INT 21h

    MOV DL, BH
    ADD DL, '0'
    MOV AH, 02h
    INT 21h

    LEA DX, batt_units
    MOV AH, 09h
    INT 21h

    POP DX
    POP CX
    POP BX
    POP AX
    RET
DISPLAY_BATTERY ENDP

PRINT_NEWLINE PROC
    PUSH AX
    PUSH CX
    PUSH DX
    MOV DL, 0Dh
    MOV AH, 02h
    INT 21h
    MOV DL, 0Ah
    MOV AH, 02h
    INT 21h
    POP DX
    POP CX
    POP AX
    RET
PRINT_NEWLINE ENDP

CLEAR_SCREEN PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX

    MOV AH, 06h
    MOV AL, 0
    MOV BH, 07h
    MOV CX, 0000h
    MOV DX, 184Fh
    INT 10h

    MOV AH, 02h
    MOV BH, 0
    MOV DH, 0
    MOV DL, 0
    INT 10h

    POP DX
    POP CX
    POP BX
    POP AX
    RET
CLEAR_SCREEN ENDP
