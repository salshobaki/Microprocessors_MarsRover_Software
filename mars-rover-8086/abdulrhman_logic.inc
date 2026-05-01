; ============================================
; abdulrhman_logic.inc — Movement & Logic
; Owner: Abdulrhman
; Delivers: RUN_SEQUENCE procedure
; ============================================
; Reads from shared variables:
;   - cmd_buffer  (command string from GET_COMMANDS)
;   - cmd_length  (number of commands)
;   - obstacles   (to check collisions)
;   - obs_count   (number of obstacles)
; Writes to shared variables:
;   - rover_row   (updated after each valid move)
;   - rover_col   (updated after each valid move)
;   - fuel        (deduct 1 per valid move)
; Calls:
;   - DRAW_GRID   (after every valid move)
; ============================================

RUN_SEQUENCE PROC
    ; TODO: set SI = offset cmd_buffer
    ; TODO: set CX = cmd_length (loop counter)
    mov si, offset cmd_buffer   ; point SI to the command string
    mov cl, cmd_length           ; load length into CL
    mov ch, 0                  ; clear CH so CX = length

    ; --- MAIN LOOP ---
    LOOP_START:
    ; TODO: read next character from [SI], increment SI
    mov al, [si]               ; read one character
    inc si                     ; move to next character

    ; print the character to screen (just to verify)
    push ax
    mov ah, 02h
    mov dl, al
    int 21h

    ; newline
    mov dl, 0Dh   ; carriage return
    int 21h

    mov dl, 0Ah   ; line feed
    int 21h
    pop ax
    ; TODO: calculate new position based on command:
    mov bl, rover_row
    mov bh, rover_col

    ; print (bl, bh)
    push ax
        mov ah, 02h
        mov dl, '('
        int 21h

        mov al, bl
        call print_small

        mov dl, ','
        int 21h

        mov dl, ' '
        int 21h

        mov al, bh
        call print_small

        mov dl, ')'
        int 21h
    pop ax
    ; print END


    cmp al, 'U'
    je DO_UP

    cmp al, 'D'
    je DO_DOWN

    cmp al, 'L'
    je DO_LEFT

    cmp al, 'R'
    je DO_RIGHT

    ; U → rover_row - 1
    DO_UP:
    dec bl
    jmp BOUNDARY_VALIDATION
    
    ; D → rover_row + 1
    DO_DOWN:
    inc bl
    jmp BOUNDARY_VALIDATION

    ; L → rover_col - 1
    DO_LEFT:
    dec bh
    jmp BOUNDARY_VALIDATION
    
    ; R → rover_col + 1
    DO_RIGHT:
    inc bh
    jmp BOUNDARY_VALIDATION
    
    
    BOUNDARY_VALIDATION:
    ; TODO: BOUNDARY CHECK
    ;       if new row < 0 or > 7 → print "Error: Boundary Breach!" → halt
    cmp bl, 8
    je BOUNDARY_ERROR
    cmp bl, -1
    je BOUNDARY_ERROR
    cmp bh, 8
    je BOUNDARY_ERROR
    cmp bh, -1
    je BOUNDARY_ERROR

    jmp OBS_VALIDATION


    BOUNDARY_ERROR:
        ; mov ah, 09h
        ; lea dx, boundary_msg
        ; int 21h
        mov current_status, 2
        jmp EXIT_SEQUENCE

    ; TODO: COLLISION CHECK
    ;       loop through obstacles array
    ;       if new row,col matches any obstacle → beep + print "CRITICAL FAILURE!" → halt
    OBS_VALIDATION:
        push si
        push cx 
        mov si, offset obstacles
        mov cl, obs_count

    OBS_LOOP:
        cmp bl, [si]
        jne OBS_NEXT
        cmp bh, [si+1]
        je OBS_ERROR

    OBS_NEXT:
        add si, 2
        loop OBS_LOOP
    
    pop cx 
    pop si
    call OBS_CORRECT
    OBS_ERROR:
        pop cx 
        pop si
        ; mov ah, 09h
        ; lea dx, collision_msg
        ; int 21h
        mov current_status, 1
        jmp EXIT_SEQUENCE

    OBS_CORRECT:
    ; TODO: VALID MOVE
    ;       update rover_row / rover_col
    mov rover_row, bl
    mov rover_col, bh
    ;       decrement fuel
    dec fuel
    cmp fuel, 0
    mov current_status, 3
    je EXIT_SEQUENCE
    ;       CALL DRAW_GRID
    CALL DRAW_GRID
    ;       delay loop (so user can see movement)

    ; TODO: repeat loop until CX = 0
    loop LOOP_START            ; CX--, repeat until CX = 0
    ; --- SEQUENCE COMPLETE ---
    ; TODO: print "Sequence Complete. Fuel Remaining: X"
    ; mov ah, 09h
    ; lea dx, complete_msg
    ; int 21h
    ; lea dx, fuel
    ; int 21h
    jmp EXIT_SEQUENCE

    EXIT_SEQUENCE:
    CALL DRAW_GRID
    RET
RUN_SEQUENCE ENDP


print_small proc
    cmp al, -1
    jne not_minus_one

    ; print "-1"
    mov ah, 02h
    mov dl, '-'
    int 21h

    mov dl, '1'
    int 21h
    ret

not_minus_one:
    add al, '0'      ; convert 0–8 to ASCII
    mov dl, al
    mov ah, 02h
    int 21h
    ret
print_small endp