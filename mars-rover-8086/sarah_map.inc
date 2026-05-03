; ============================================
; sarah_map.inc — Map & Display
; Owner: Sarah (ID: 20220188)
; ============================================
; PROCEDURES IN THIS FILE:
;   DRAW_GRID          — master display procedure
;                        call after every move
;   DELAY              — animation pause
;
; PRIVATE (called internally by DRAW_GRID only):
;   INIT_GRID
;   CLEAR_SCREEN
;   PRINT_NEWLINE
;   DISPLAY_BATTERY
;
; HOW TO USE:
;   main.asm     → CALL DRAW_GRID  (shows initial map)
;   Abdulrhman   → CALL DRAW_GRID  (after every move)
;
; current_status values (set by Abdulrhman):
;   0 = normal     → redraw map, R at new position
;   1 = crash      → stamp X at rover_row/rover_col
;                    show CRITICAL FAILURE message
;   2 = boundary   → no symbol stamped
;                    show boundary breach message
;   3 = success    → R at rover_row/rover_col
;                    show MISSION SUCCESS message
;
; IMPORTANT: When crash (status=1) or boundary (status=2),
;   Abdulrhman must save the target position into
;   rover_row / rover_col BEFORE calling DRAW_GRID
; ============================================

; ============================================================
; PROCEDURE: DRAW_GRID
; ============================================================
DRAW_GRID PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI

    ; --------------------------------------------------------
    ; STEP 1: First call only — place all 6 obstacles
    ; --------------------------------------------------------
    CMP BYTE PTR grid_initialized, 0
    JNE SKIP_INIT
    CALL INIT_GRID
    MOV BYTE PTR grid_initialized, 1

SKIP_INIT:

    ; --------------------------------------------------------
    ; STEP 2: Clear the PREVIOUS R or X from grid array
    ;         Uses last_drawn_row / last_drawn_col
    ;         saved from the previous DRAW_GRID call
    ; --------------------------------------------------------
    MOV AL, last_drawn_row
    SHL AL, 1
    SHL AL, 1
    SHL AL, 1
    ADD AL, last_drawn_col
    LEA BX, grid
    MOV AH, 0
    ADD BX, AX
    CMP BYTE PTR [BX], 'O'  ; never overwrite an obstacle
    JE SKIP_CLEAR
    MOV BYTE PTR [BX], '.'  ; clear old R or X

SKIP_CLEAR:

    ; --------------------------------------------------------
    ; STEP 3: Stamp correct symbol at rover_row / rover_col
    ; --------------------------------------------------------
    MOV AL, rover_row
    SHL AL, 1
    SHL AL, 1
    SHL AL, 1
    ADD AL, rover_col
    LEA BX, grid
    MOV AH, 0
    ADD BX, AX               ; BX -> grid[rover_row][rover_col]

    MOV CL, current_status

    CMP CL, 2                ; boundary — rover off grid, stamp nothing
    JE SAVE_LAST_DRAWN

    CMP CL, 1                ; crash — stamp X
    JE STAMP_X

    MOV BYTE PTR [BX], 'R'  ; normal or success — stamp R
    JMP SAVE_LAST_DRAWN

STAMP_X:
    MOV BYTE PTR [BX], 'X'

    ; --------------------------------------------------------
    ; STEP 4: Save current position as last drawn
    ;         so next call knows what to clear
    ; --------------------------------------------------------
SAVE_LAST_DRAWN:
    MOV AL, rover_row
    MOV last_drawn_row, AL
    MOV AL, rover_col
    MOV last_drawn_col, AL

    ; --------------------------------------------------------
    ; STEP 5: Clear the screen
    ; --------------------------------------------------------
    CALL CLEAR_SCREEN

    ; --------------------------------------------------------
    ; STEP 6: Print title
    ; --------------------------------------------------------
    LEA DX, title_msg
    MOV AH, 09h
    INT 21h
    CALL PRINT_NEWLINE

    ; --------------------------------------------------------
    ; STEP 7: Print column header
    ; --------------------------------------------------------
    LEA DX, col_header
    MOV AH, 09h
    INT 21h

    ; --------------------------------------------------------
    ; STEP 8: Print all 8 rows of the grid
    ; --------------------------------------------------------
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

    ; --------------------------------------------------------
    ; STEP 9: Print legend
    ; --------------------------------------------------------
    CALL PRINT_NEWLINE
    LEA DX, legend_msg
    MOV AH, 09h
    INT 21h

    ; --------------------------------------------------------
    ; STEP 10: Print battery level
    ; --------------------------------------------------------
    CALL DISPLAY_BATTERY

    ; --------------------------------------------------------
    ; STEP 11: Print end-of-game message if status != 0
    ; --------------------------------------------------------
    MOV CL, current_status

    CMP CL, 1
    JE DRAW_SHOW_CRASH

    CMP CL, 2
    JE DRAW_SHOW_BOUND

    CMP CL, 3
    JE DRAW_SHOW_SUCCESS

    JMP DRAW_DONE            ; status 0 = normal move, no message

    ; --------------------------------------------------------
    ; CRASH MESSAGE:
    ; CRITICAL FAILURE!
    ; Rover crashed into a crater at coordinates (R,C).
    ; Mission Aborted.
    ; Press any key to exit...
    ; --------------------------------------------------------
DRAW_SHOW_CRASH:
    MOV DL, 07h              ; beep sound
    MOV AH, 02h
    INT 21h

    LEA DX, crash_line1      ; "CRITICAL FAILURE!\nRover crashed...("
    MOV AH, 09h
    INT 21h

    MOV DL, rover_row        ; print row digit
    ADD DL, '0'
    MOV AH, 02h
    INT 21h

    LEA DX, coord_comma      ; ","
    MOV AH, 09h
    INT 21h

    MOV DL, rover_col        ; print col digit
    ADD DL, '0'
    MOV AH, 02h
    INT 21h

    LEA DX, coord_close      ; ").\n"
    MOV AH, 09h
    INT 21h

    LEA DX, aborted_msg      ; "Mission Aborted.\nPress any key..."
    MOV AH, 09h
    INT 21h

    JMP DRAW_DONE

    ; --------------------------------------------------------
    ; BOUNDARY MESSAGE:
    ; Error: Boundary breach!
    ; Mission Aborted.
    ; Press any key to exit...
    ; --------------------------------------------------------
DRAW_SHOW_BOUND:
    LEA DX, bound_msg
    MOV AH, 09h
    INT 21h

    JMP DRAW_DONE

    ; --------------------------------------------------------
    ; SUCCESS MESSAGE:
    ; MISSION SUCCESS!
    ; Rover reached coordinates: (R,C)
    ; Fuel Consumed: X
    ; Remaining Battery Level: XX Units
    ; Press any key to exit...
    ; --------------------------------------------------------
DRAW_SHOW_SUCCESS:
    LEA DX, success_line1    ; "MISSION SUCCESS!\nRover reached...("
    MOV AH, 09h
    INT 21h

    MOV DL, rover_row        ; print row digit
    ADD DL, '0'
    MOV AH, 02h
    INT 21h

    LEA DX, coord_comma      ; ","
    MOV AH, 09h
    INT 21h

    MOV DL, rover_col        ; print col digit
    ADD DL, '0'
    MOV AH, 02h
    INT 21h

    LEA DX, coord_close      ; ").\n"
    MOV AH, 09h
    INT 21h

    ; --- Print "Fuel Consumed: " ---
    LEA DX, fuel_consumed_msg
    MOV AH, 09h
    INT 21h

    ; --- Calculate fuel consumed = 20 - fuel ---
    MOV AL, initial_fuel
    SUB AL, fuel             ; AL = amount consumed
    MOV AH, 0
    MOV BL, 10
    DIV BL                   ; AL = tens, AH = ones
    MOV BH, AH               ; save ones digit

    ; Skip leading zero on tens digit
    CMP AL, 0
    JE SKIP_TENS
    MOV DL, AL
    ADD DL, '0'
    MOV AH, 02h
    INT 21h
SKIP_TENS:
    MOV DL, BH               ; ones digit
    ADD DL, '0'
    MOV AH, 02h
    INT 21h

    CALL PRINT_NEWLINE

    ; --- Print "Remaining Battery Level: " ---
    LEA DX, remaining_msg
    MOV AH, 09h
    INT 21h

    ; --- Print remaining fuel ---
    MOV AL, fuel
    MOV AH, 0
    MOV BL, 10
    DIV BL
    MOV BH, AH               ; save ones digit

    ; Skip leading zero
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

    ; --- Print " Units\n" ---
    LEA DX, batt_units
    MOV AH, 09h
    INT 21h

    ; --- Print "Press any key to exit..." ---
    LEA DX, press_key_msg
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

; ============================================================
; PROCEDURE: DELAY
; Visible pause between animation frames
; ============================================================
DELAY PROC
    PUSH CX
    MOV CX, 0080h 
DELAY_LOOP:
    NOP
    LOOP DELAY_LOOP
    POP CX
    RET
DELAY ENDP

; ============================================================
; PROCEDURE: INIT_GRID (private)
; Places 6 obstacles into grid array from obstacles table
; ============================================================
INIT_GRID PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH SI

    LEA SI, obstacles
    MOV CL, 6

PLACE_NEXT:
    MOV AL, [SI]             ; row
    SHL AL, 1
    SHL AL, 1
    SHL AL, 1                ; row * 8
    INC SI
    ADD AL, [SI]             ; + col = offset
    LEA BX, grid
    MOV AH, 0
    ADD BX, AX
    MOV BYTE PTR [BX], 'O'
    INC SI
    DEC CL
    JNZ PLACE_NEXT

    POP SI
    POP CX
    POP BX
    POP AX
    RET
INIT_GRID ENDP

; ============================================================
; PROCEDURE: DISPLAY_BATTERY (private)
; Prints "Battery Level: NN Units"
; ============================================================
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
    MOV BH, AH               ; save ones digit

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

; ============================================================
; PROCEDURE: PRINT_NEWLINE (private)
; ============================================================
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

; ============================================================
; PROCEDURE: CLEAR_SCREEN (private)
; ============================================================
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