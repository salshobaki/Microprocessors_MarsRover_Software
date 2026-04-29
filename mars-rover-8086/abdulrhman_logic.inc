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

    ; --- MAIN LOOP ---
    ; TODO: read next character from [SI], increment SI

    ; TODO: calculate new position based on command:
    ;       U → rover_row - 1
    ;       D → rover_row + 1
    ;       L → rover_col - 1
    ;       R → rover_col + 1

    ; TODO: BOUNDARY CHECK
    ;       if new row < 0 or > 7 → print "Error: Boundary Breach!" → halt

    ; TODO: COLLISION CHECK
    ;       loop through obstacles array
    ;       if new row,col matches any obstacle → beep + print "CRITICAL FAILURE!" → halt

    ; TODO: VALID MOVE
    ;       update rover_row / rover_col
    ;       decrement fuel
    ;       CALL DRAW_GRID
    ;       delay loop (so user can see movement)

    ; TODO: repeat loop until CX = 0

    ; --- SEQUENCE COMPLETE ---
    ; TODO: print "Sequence Complete. Fuel Remaining: X"

    RET
RUN_SEQUENCE ENDP
