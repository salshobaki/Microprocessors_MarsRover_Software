; ============================================
; abdulrhman_logic.inc — Movement & Logic
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
    mov al, [si]
    inc si

    mov bl, rover_row
    mov bh, rover_col

    cmp al, 'U'
    je DO_UP
    cmp al, 'D'
    je DO_DOWN
    cmp al, 'L'
    je DO_LEFT
    cmp al, 'R'
    je DO_RIGHT
    jmp NEXT_CMD             

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

BOUNDARY_CHECK:
    cmp bl, 8
    je DO_BOUNDARY_ERROR
    cmp bl, 0FFh             
    je DO_BOUNDARY_ERROR
    cmp bh, 8
    je DO_BOUNDARY_ERROR
    cmp bh, 0FFh
    je DO_BOUNDARY_ERROR
    jmp OBSTACLE_CHECK

DO_BOUNDARY_ERROR:
    mov rover_row, bl        
    mov rover_col, bh
    mov current_status, 2
    jmp GAME_OVER

OBSTACLE_CHECK:
    push si                  
    push cx                  
    push bx                  

    mov si, offset obstacles
    mov cl, obs_count
    mov ch, 0

OBS_LOOP:
    mov al, [si]             
    cmp bl, al
    jne OBS_NEXT             

    mov al, [si+1]           
    cmp bh, al
    je DO_CRASH_ERROR        

OBS_NEXT:
    add si, 2
    loop OBS_LOOP

    pop bx                   
    pop cx                   
    pop si                   

    mov rover_row, bl
    mov rover_col, bh
    mov current_status, 0

    CMP game_mode, 2
    JNE SKIP_TARGET_CHECK
    
    MOV AL, target_row
    CMP bl, AL
    JNE SKIP_TARGET_CHECK
    
    MOV AL, target_col
    CMP bh, AL
    JNE SKIP_TARGET_CHECK

    MOV current_status, 4    
    CALL DRAW_GRID
    JMP RUN_DONE

SKIP_TARGET_CHECK:
    dec fuel
    cmp fuel, 0
    je FUEL_EMPTY
    dec cx
    jz FUEL_EMPTY

    call DRAW_GRID
    call DELAY

    cmp cx, 0
    jnz LOOP_START

    CMP game_mode, 2
    JE DO_TARGET_MISS        
    mov current_status, 3    
    call DRAW_GRID
    jmp RUN_DONE

DO_CRASH_ERROR:
    pop bx                   
    pop cx
    pop si
    mov rover_row, bl        
    mov rover_col, bh
    mov current_status, 1
    jmp GAME_OVER

GAME_OVER:
    call DRAW_GRID           
    jmp RUN_DONE

FUEL_EMPTY:
    CMP game_mode, 2
    JE DO_TARGET_MISS        
    mov current_status, 3    
    call DRAW_GRID
    jmp RUN_DONE

NEXT_CMD:
    dec cx
    jnz LOOP_START
    CMP game_mode, 2
    JE DO_TARGET_MISS        
    mov current_status, 3    
    call DRAW_GRID
    jmp RUN_DONE

DO_TARGET_MISS:
    mov current_status, 5    
    call DRAW_GRID
    jmp RUN_DONE

RUN_DONE:
    POP SI
    POP DX
    POP CX
    POP BX
    POP AX
    ret
RUN_SEQUENCE ENDP
