; ============================================
; sarah_map.inc — Map & Display
; Owner: Sarah (ID: 20220188)
; ============================================
; HOW TO USE:
;
; In main.asm:
;   CALL DRAW_GRID          ← shows initial map (auto-initializes obstacles on first call)
;
; In RUN_SEQUENCE (Abdulrhman), for each move:
;   ; update rover_row or rover_col here
;   ; decrement fuel here
;   CALL DRAW_GRID          ← handles EVERYTHING: clears old rover, redraws, animates
;   CALL DELAY              ← animation pause
;
; At game end (Abdulrhman):
;   MOV AL, 0               ← 0=success, 1=crash, 2=boundary
;   CALL DISPLAY_STATUS_MSG
; ============================================

; ============================================================
; PROCEDURE: DRAW_GRID
; Purpose:  Master display procedure.
;           - First call: initializes obstacles then draws
;           - Every call: clears old rover position, clears
;             screen, stamps rover at new position, redraws
;             full grid, and displays battery level.
;           Abdulrhman only needs to update rover_row/rover_col
;           and call DRAW_GRID — nothing else is required.
; Inputs:   rover_row, rover_col, fuel, grid (from data.inc)
; Outputs:  Full screen rendered
; ============================================================
DRAW_GRID PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI

    ; --- Check if this is the first call ---
    CMP BYTE PTR grid_initialized, 0
    JNE SKIP_INIT            ; if already initialized, skip

    ; --- First call only: place all 6 obstacles ---
    CALL INIT_GRID
    MOV BYTE PTR grid_initialized, 1  ; set flag

SKIP_INIT:
    ; --- Clear rover's OLD position before moving ---
    ;     (rover_row/rover_col already updated by caller)
    CALL CLEAR_OLD_ROVER

    ; --- Clear the screen ---
    CALL CLEAR_SCREEN

    ; --- Print title ---
    LEA DX, title_msg
    MOV AH, 09h
    INT 21h

    CALL PRINT_NEWLINE

    ; --- Stamp R at rover's NEW (current) position ---
    MOV AL, rover_row
    SHL AL, 1
    SHL AL, 1
    SHL AL, 1                ; AL = rover_row * 8
    ADD AL, rover_col        ; AL = offset
    LEA BX, grid
    MOV AH, 0
    ADD BX, AX
    MOV BYTE PTR [BX], 'R'

    ; --- Print column header ---
    LEA DX, col_header
    MOV AH, 09h
    INT 21h

    ; --- Print all 8 rows ---
    LEA SI, grid
    MOV BYTE PTR row_cnt, 0

DRAW_ROW:
    ; -- Print row number --
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

    ; -- Print 8 cells --
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

    ; --- Print legend ---
    CALL PRINT_NEWLINE
    LEA DX, legend_msg
    MOV AH, 09h
    INT 21h

    ; --- Print battery level ---
    CALL DISPLAY_BATTERY

    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    RET
DRAW_GRID ENDP

; ============================================================
; PROCEDURE: CLEAR_OLD_ROVER  (private — called by DRAW_GRID)
; Purpose:  Resets rover's current cell back to '.'
;           Called automatically inside DRAW_GRID before
;           stamping the rover at its new position.
;           Do NOT call this directly from outside.
; Inputs:   rover_row, rover_col, grid (from data.inc)
; ============================================================
CLEAR_OLD_ROVER PROC
    PUSH AX
    PUSH BX

    MOV AL, rover_row
    SHL AL, 1
    SHL AL, 1
    SHL AL, 1
    ADD AL, rover_col
    LEA BX, grid
    MOV AH, 0
    ADD BX, AX
    MOV BYTE PTR [BX], '.'

    POP BX
    POP AX
    RET
CLEAR_OLD_ROVER ENDP

; ============================================================
; PROCEDURE: DISPLAY_STATUS_MSG
; Purpose:  Prints end-of-game message
; Inputs:   AL = 0 (success), 1 (crash), 2 (boundary)
; ============================================================
DISPLAY_STATUS_MSG PROC
    PUSH AX
    PUSH DX

    CMP AL, 0
    JE STAT_SUCCESS
    CMP AL, 1
    JE STAT_CRASH
    CMP AL, 2
    JE STAT_BOUND
    JMP STAT_DONE

STAT_SUCCESS:
    LEA DX, success_msg
    MOV AH, 09h
    INT 21h
    JMP STAT_DONE

STAT_CRASH:
    MOV DL, 07h              ; beep
    MOV AH, 02h
    INT 21h
    LEA DX, crash_msg
    MOV AH, 09h
    INT 21h
    JMP STAT_DONE

STAT_BOUND:
    LEA DX, bound_msg
    MOV AH, 09h
    INT 21h

STAT_DONE:
    POP DX
    POP AX
    RET
DISPLAY_STATUS_MSG ENDP

; ============================================================
; PROCEDURE: DELAY
; Purpose:  Visible pause between animation frames
; ============================================================
DELAY PROC
    PUSH CX
    MOV CX, 0500h
DELAY_LOOP:
    NOP
    LOOP DELAY_LOOP
    POP CX
    RET
DELAY ENDP

; ============================================================
; PROCEDURE: INIT_GRID  (private — called by DRAW_GRID only)
; Purpose:  Places 6 obstacles on the grid
; ============================================================
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

    POP SI
    POP CX
    POP BX
    POP AX
    RET
INIT_GRID ENDP

; ============================================================
; PROCEDURE: DISPLAY_BATTERY  (private — called by DRAW_GRID)
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
; PROCEDURE: PRINT_NEWLINE  (private helper)
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
; PROCEDURE: CLEAR_SCREEN  (private helper)
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