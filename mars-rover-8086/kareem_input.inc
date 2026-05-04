; ============================================
; kareem_input.inc — Input & Menus
; ============================================

CHOOSE_MODE PROC
    PUSHA
ASK_MODE:
    MOV AH, 09h
    LEA DX, menu_msg1
    INT 21h

    MOV AH, 01h
    INT 21h
    
    CMP AL, '1'
    JE SET_USUAL
    CMP AL, '2'
    JE ASK_LEVEL
    JMP ASK_MODE

SET_USUAL:
    MOV game_mode, 1
    JMP MODE_DONE

ASK_LEVEL:
    MOV game_mode, 2
    MOV AH, 09h
    LEA DX, menu_msg2
    INT 21h

    MOV AH, 01h
    INT 21h

    CMP AL, '1'
    JE SET_EASY
    CMP AL, '2'
    JE SET_MEDIUM
    CMP AL, '3'
    JE SET_HARD
    JMP ASK_LEVEL

SET_EASY:
    MOV target_row, 2
    MOV target_col, 2
    JMP MODE_DONE

SET_MEDIUM:
    MOV target_row, 4
    MOV target_col, 4
    JMP MODE_DONE

SET_HARD:
    MOV target_row, 7
    MOV target_col, 7

MODE_DONE:
    POPA
    RET
CHOOSE_MODE ENDP

GET_COMMANDS PROC 
    PUSHA
    PUSHF
START_INPUT:
    MOV AH, 09h
    LEA DX, PROMPT_MSG
    INT 21h

    MOV AH, 0Ah
    LEA DX, RAW_BUFFER
    INT 21h
    
    MOV AH, 09h
    LEA DX, NEWLINE
    INT 21h
    
    MOV CL, [RAW_BUFFER + 1]   
    MOV CH, 0            
    CMP CX, 0            
    JE START_INPUT       

    LEA SI, USER_INPUT   
    LEA DI, CMD_BUFFER   

VALIDATE:
    MOV AL, [SI]        
    CMP AL, "a"
    JB CHECK_MOVES
    CMP AL, "z"
    JA CHECK_MOVES
    AND AL, 0DFh         

CHECK_MOVES:
    CMP AL, "U"
    JE VALID_CHAR
    CMP AL, "D"
    JE VALID_CHAR
    CMP AL, "L"
    JE VALID_CHAR
    CMP AL, "R"
    JE VALID_CHAR

    MOV AH, 09h
    LEA DX, ERROR_MSG
    INT 21h
    JMP START_INPUT      

VALID_CHAR:
    MOV [DI], AL         
    INC SI               
    INC DI               
    LOOP VALIDATE  
    
    MOV AL, [RAW_BUFFER + 1] 
    MOV CMD_LENGTH, AL    

    MOV BYTE PTR [DI], 0
    POPF
    POPA    
    RET
GET_COMMANDS ENDP
