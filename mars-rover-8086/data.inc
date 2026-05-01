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
obstacles   db 5,1              ; ID1 Obs1
            db 0,6              ; ID1 Obs2
            db 0,1              ; ID2 Obs1 (shifted from 0,0)
            db 0,2              ; ID2 Obs2 (shifted from 0,1)
            db 1,5              ; ID3 Obs1
            db 0,3              ; ID3 Obs2 (shifted from 0,1)

; ============================================
; Sarah's display variables (sarah_map.inc)
; ============================================
row_cnt     db 0
title_msg   db 'MARS ROVER NAVIGATION SYSTEM', 0Dh, 0Ah, '$'
col_header  db '   0 1 2 3 4 5 6 7', 0Dh, 0Ah, '$'
legend_msg  db 'Legend: [R] = Rover, [O] = Crater/obstacle, [.] = Empty Space', 0Dh, 0Ah, '$'
batt_msg    db 'Battery Level: $'
batt_units  db ' Units', 0Dh, 0Ah, '$'
success_msg db 0Dh, 0Ah, 'MISSION SUCCESS!', 0Dh, 0Ah, '$'
crash_msg   db 0Dh, 0Ah, 'CRITICAL FAILURE!', 0Dh, 0Ah
            db 'Rover crashed into a crater.', 0Dh, 0Ah, '$'
bound_msg   db 0Dh, 0Ah, 'Error: Boundary breach!', 0Dh, 0Ah
            db 'Mission Aborted.', 0Dh, 0Ah, '$'
grid_initialized db 0

; ============================================
; Kareem's input-handelling variables (kareem_input.inc)
; ============================================
prompt_msg  db " Enter movement sequence (U, D, L, R Max 20 chars): $"
error_msg   db 0Dh, 0Ah, " Invalid input! Try again.", 0Dh, 0Ah, "$"
newline     db 0Dh, 0Ah, " $"
raw_buffer  db 21 
nthg        db 1       
user_input  db 21 DUP(0) 

; ============================================
; Abdulrhman's logic variables (abdulrhman_logic.inc)
; ============================================
; boundary_msg    db "Error: Boundary Breach!", 13, 10, "$"
; collision_msg   db "CRITICAL FAILURE: Rover Crashed!", 13, 10, "$"
; complete_msg    db "Sequence Complete. Fuel Remaining: $"
current_status db 0 ; 0 = normal state, 1 = crash occurred, 2 = boundary violation, 3 = mission successful (fuel depleted)