; ============================================
; sarah_map.inc — Map & Display
; Owner: Sarah
; Delivers: DRAW_GRID procedure
; ============================================
; Reads from shared variables:
;   - grid        (the 8x8 map array)
;   - rover_row   (current rover row)
;   - rover_col   (current rover col)
;   - obstacles   (obstacle coordinates)
;   - fuel        (for battery display)
; ============================================

DRAW_GRID PROC
    ; TODO: clear the screen
    ; TODO: print title "MARS ROVER NAVIGATION SYSTEM"
    ; TODO: print column headers (0-7)
    ; TODO: loop through grid array and print each cell
    ;       - if cell is rover position → print 'R'
    ;       - if cell is obstacle       → print 'O'
    ;       - otherwise                 → print '.'
    ; TODO: print legend
    ; TODO: print battery level
    RET
DRAW_GRID ENDP
