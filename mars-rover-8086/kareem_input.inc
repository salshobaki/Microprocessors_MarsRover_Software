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
GET_COMMANDS PROC 
    PUSHA
    PUSHF
START_INPUT:
    ;Display the message
    MOV AH, 09h
    LEA DX, PROMPT_MSG
    INT 21h

    ; Read the strings from the user
    MOV AH, 0Ah
    LEA DX, RAW_BUFFER
    INT 21h
    
    ; Print a newline when incorrect or empty input
    MOV AH, 09h
    LEA DX, NEWLINE
    INT 21h
    

    ; Prepare for validation loop
    MOV CL, [RAW_BUFFER + 1]   
    MOV CH, 0            
    CMP CX, 0            
    JE START_INPUT       

    LEA SI, USER_INPUT   
    LEA DI, CMD_BUFFER   

VALIDATE:
    ; Load one character at a time
    MOV AL, [SI]        
         
    ;Checking if the input is not between 'a' - 'z'
    CMP AL, "a"
    JB CHECK_MOVES
    CMP AL, "z"
    JA CHECK_MOVES
    ;if the character between 'a' - 'z', then uppercase
    
    ; Uppercasing
    AND AL, 0DFh         

CHECK_MOVES:
    ; Check valid characters
    CMP AL, "U"
    JE VALID_CHAR
    CMP AL, "D"
    JE VALID_CHAR
    CMP AL, "L"
    JE VALID_CHAR
    CMP AL, "R"
    JE VALID_CHAR

    ; Invalid character operation
    MOV AH, 09h
    LEA DX, ERROR_MSG
    INT 21h
    JMP START_INPUT      

VALID_CHAR:
    MOV [DI], AL         
    INC SI               
    INC DI               
    LOOP VALIDATE  ;loop on the whole string, remember the line 
                   ;before the validation part "MOV CL, [RAW_BUFFER + 1]"
    
    MOV AL, [RAW_BUFFER + 1] 
    MOV CMD_LENGTH, AL    


    MOV BYTE PTR [DI], 0
    POPF
    POPA    
    RET
GET_COMMANDS ENDP
