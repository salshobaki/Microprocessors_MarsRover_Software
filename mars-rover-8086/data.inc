; ============================================
; data.inc — Shared Variables
; ============================================

grid              db 64 dup('.')
rover_row         db 0
rover_col         db 0
fuel              db 20
cmd_buffer        db 21 dup(0)
cmd_length        db 0
obs_count         db 6
obstacles         db 5,1, 0,6, 0,1, 0,2, 1,5, 0,3

grid_initialized  db 0         
last_drawn_row    db 0         
last_drawn_col    db 0         
row_cnt           db 0         
initial_fuel      db 20        

title_msg         db 'MARS ROVER NAVIGATION SYSTEM', 0Dh, 0Ah, '$'
col_header        db '   0 1 2 3 4 5 6 7', 0Dh, 0Ah, '$'
legend_msg        db 'Legend: [R]=Rover, [O]=Crater, [T]=Target, [.]=Empty', 0Dh, 0Ah, '$'
batt_msg          db 'Battery Level: $'
batt_units        db ' Units', 0Dh, 0Ah, '$'

crash_line1       db 0Dh, 0Ah, 'CRITICAL FAILURE!', 0Dh, 0Ah
                  db 'Rover crashed into a crater at coordinates (', '$'
coord_comma       db ',', '$'
coord_close       db ').', 0Dh, 0Ah, '$'
aborted_msg       db 'Mission Aborted.', 0Dh, 0Ah, 'Press any key to exit...', '$'

bound_msg         db 0Dh, 0Ah, 'Error: Boundary breach!', 0Dh, 0Ah
                  db 'Mission Aborted.', 0Dh, 0Ah, 'Press any key to exit...', '$'

success_line1     db 0Dh, 0Ah, 'MISSION SUCCESS!', 0Dh, 0Ah
                  db 'Rover reached coordinates: (', '$'
fuel_consumed_msg db 'Fuel Consumed: ', '$'
remaining_msg     db 'Remaining Battery Level: ', '$'
press_key_msg     db 'Press any key to exit...', '$'

prompt_msg        db " Enter movement sequence (U, D, L, R Max 20 chars): $"
error_msg         db 0Dh, 0Ah, " Invalid input! Try again.", 0Dh, 0Ah, "$"
newline           db 0Dh, 0Ah, " $"
raw_buffer        db 21 
nthg              db 1       
user_input        db 21 DUP(0) 
current_status    db 0 

; ============================================
; TARGET GAME VARIABLES
; ============================================
game_mode         db 1
target_row        db 0   
target_col        db 0   

menu_msg1         db 0Dh, 0Ah, '*** MARS ROVER OS ***', 0Dh, 0Ah
                  db '1. Usual Mode (Free Roam)', 0Dh, 0Ah
                  db '2. Target Mode (Levels)', 0Dh, 0Ah
                  db 'Select Mode: $'

menu_msg2         db 0Dh, 0Ah, '*** SELECT LEVEL ***', 0Dh, 0Ah
                  db '1. Easy (Optimal: D, D, R, R)', 0Dh, 0Ah
                  db '2. Medium (Optimal: R, D, D, D, R, R, D)', 0Dh, 0Ah
                  db '3. Hard (Optimal: D, D, D, D, D, D, D, R, R, R, R, R, R, R)', 0Dh, 0Ah
                  db 'Select Level: $'

target_win_msg    db 0Dh, 0Ah, '*** TARGET REACHED! YOU WIN! ***', 0Dh, 0Ah, 'Press any key to exit...', '$'
target_miss_msg   db 0Dh, 0Ah, '*** MISSION FAILED: TARGET MISSED! ***', 0Dh, 0Ah, 'Press any key to exit...', '$'
