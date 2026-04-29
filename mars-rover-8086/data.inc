; ============================================
; data.inc — Shared Variables
; DO NOT rename or resize without team approval
; ============================================

grid        db 64 dup('.')      ; 8x8 map (row-major, 8 cols per row)
rover_row   db 0                ; current rover row (0-7)
rover_col   db 0                ; current rover col (0-7)
fuel        db 20               ; starts at 20, -1 per valid move
cmd_buffer  db 21 dup(0)        ; filled by GET_COMMANDS (max 20 chars + null)
cmd_length  db 0                ; number of valid commands entered
obs_count   db 6                ; total number of obstacles
obstacles   db 0,1              ; obstacle 1 → row, col
            db 5,0              ; obstacle 2 → row, col
            db 2,0              ; obstacle 3 → row, col
            db 2,3              ; obstacle 4 → row, col
            db 5,6              ; obstacle 5 → row, col
            db 1,7              ; obstacle 6 → row, col
