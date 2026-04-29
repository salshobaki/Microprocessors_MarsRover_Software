; ============================================
; kareem_input.inc — Input & Validation
; Owner: Kareem
; Delivers: GET_COMMANDS procedure
; ============================================
; Writes to shared variables:
;   - cmd_buffer  (validated uppercase command string)
;   - cmd_length  (number of commands stored)
; ============================================

GET_COMMANDS PROC
    ; TODO: print input prompt
    ;       "Enter movement sequence (U,D,L,R - Max 20 chars): "
    ; TODO: read up to 20 characters into cmd_buffer
    ; TODO: loop through each character and validate:
    ;       - accept U, D, L, R (upper and lower case)
    ;       - convert lowercase to uppercase
    ;       - reject anything else → re-prompt from start
    ; TODO: store cleaned string in cmd_buffer
    ; TODO: store length in cmd_length
    RET
GET_COMMANDS ENDP
