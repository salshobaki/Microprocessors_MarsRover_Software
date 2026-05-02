; ============================================
; abdulrhman_logic.inc — Movement & Logic
; Owner: Abdulrhman
; Delivers: RUN_SEQUENCE procedure
; ============================================

RUN_SEQUENCE PROC
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX
    PUSH SI

    mov si, offset cmd_buffer
    mov cl, cmd_length
    mov ch, 0

LOOP_START:
    ; --- Read next command ---
    mov al, [si]
    inc si

    ; --- Load current position into BL, BH ---
    mov bl, rover_row
    mov bh, rover_col

    ; --- Decode command ---
    cmp al, 'U'
    je DO_UP
    cmp al, 'D'
    je DO_DOWN
    cmp al, 'L'
    je DO_LEFT
    cmp al, 'R'
    je DO_RIGHT
    jmp NEXT_CMD             ; unknown char — skip

DO_UP:
    dec bl
    jmp BOUNDARY_CHECK

DO_DOWN:
    inc bl
    jmp BOUNDARY_CHECK

DO_LEFT:
    dec bh
    jmp BOUNDARY_CHECK

DO_RIGHT:
    inc bh
    jmp BOUNDARY_CHECK

    ; --------------------------------------------------------
    ; BOUNDARY CHECK
    ; BL = new row, BH = new col
    ; Valid range: 0-7 for both
    ; --------------------------------------------------------
BOUNDARY_CHECK:
    cmp bl, 8
    je DO_BOUNDARY_ERROR
    cmp bl, 0FFh             ; -1 as unsigned byte
    je DO_BOUNDARY_ERROR
    cmp bh, 8
    je DO_BOUNDARY_ERROR
    cmp bh, 0FFh
    je DO_BOUNDARY_ERROR
    jmp OBSTACLE_CHECK

DO_BOUNDARY_ERROR:
    mov rover_row, bl        ; save target position for display
    mov rover_col, bh
    mov current_status, 2
    jmp GAME_OVER

    ; --------------------------------------------------------
    ; OBSTACLE CHECK
    ; Loop through all 6 obstacles
    ; --------------------------------------------------------
OBSTACLE_CHECK:
    push si                  ; save command pointer
    push cx                  ; save command counter
    push bx                  ; save new position

    mov si, offset obstacles
    mov cl, obs_count
    mov ch, 0

OBS_LOOP:
    mov al, [si]             ; obstacle row
    cmp bl, al
    jne OBS_NEXT             ; row doesn't match

    mov al, [si+1]           ; obstacle col
    cmp bh, al
    je DO_CRASH_ERROR        ; both match = crash!

OBS_NEXT:
    add si, 2
    loop OBS_LOOP

    ; --- No obstacle found = valid move ---
    pop bx                   ; restore new position
    pop cx                   ; restore command counter
    pop si                   ; restore command pointer

    ; --- Update rover position ---
    mov rover_row, bl
    mov rover_col, bh
    mov current_status, 0

    ; --- Deduct fuel ---
    dec fuel
    cmp fuel, 0
    je FUEL_EMPTY
    dec cx
    jz FUEL_EMPTY


    ; --- Draw this frame ---
    call DRAW_GRID
    call DELAY

    ; --- Next command ---
    cmp cx, 0
    jnz LOOP_START

    ; --- All commands done successfully ---
    mov current_status, 3
    call DRAW_GRID
    jmp RUN_DONE

FUEL_EMPTY:
    mov current_status, 3
    call DRAW_GRID
    jmp RUN_DONE

DO_CRASH_ERROR:
    pop bx                   ; restore new position (crash location)
    pop cx
    pop si
    mov rover_row, bl        ; save crash position for X display
    mov rover_col, bh
    mov current_status, 1
    jmp GAME_OVER

GAME_OVER:
    call DRAW_GRID           ; draws X or empty + error message

RUN_DONE:
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    ret

NEXT_CMD:
    dec cx
    jnz LOOP_START
    ; all commands processed
    mov current_status, 3
    call DRAW_GRID
    jmp RUN_DONE

RUN_SEQUENCE ENDP