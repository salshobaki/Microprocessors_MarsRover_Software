; ============================================
; Mars Rover Grid Navigator
; Princess Sumaya University for Technology
; 22344 - Microprocessors | Spring 2026
; ============================================

data segment
    INCLUDE data.inc
ends

stack segment
    dw 128 dup(0)
ends

code segment
INCLUDE sarah_map.inc
INCLUDE kareem_input.inc
INCLUDE abdulrhman_logic.inc
start:
    mov ax, data
    mov ds, ax
    mov es, ax

    CALL DRAW_GRID
    CALL GET_COMMANDS
    CALL RUN_SEQUENCE

    ; wait for any key....    
    mov ah, 1
    int 21h
    mov ax, 4c00h
    int 21h

ends
end start
