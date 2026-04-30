# Project Structure
> Mars Rover Grid Navigator — 8086 Assembly | Spring 2026

---

## File Ownership

| File | Owner | Delivers |
|------|-------|----------|
| `sarah_map.inc` | Sarah | `DRAW_GRID` procedure |
| `kareem_input.inc` | Kareem | `GET_COMMANDS` procedure |
| `abdulrhman_logic.inc` | Abdulrhman | `RUN_SEQUENCE` procedure |
| `data.inc` | Everyone | Shared variables — discuss before changing |
| `main.asm` | Everyone | Master file — discuss before changing |

> **Rule: You edit your file only. No exceptions.**

---

## Repository Layout

```
mars-rover-8086/
├── main.asm
├── data.inc
├── sarah_map.inc
├── kareem_input.inc
└── abdulrhman_logic.inc
STRUCTURE.md
Tasks.md
README.md
```

---

## How the Files Link

`main.asm` uses `INCLUDE` to pull every file in at compile time:

```asm
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

    mov ax, 4c00h
    int 21h
ends
end start
```

emu8086 compiles `main.asm` and sees everything as one file.

---

## Shared Variables (`data.inc`)

```asm
grid        db 64 dup('.')      ; 8x8 map
rover_row   db 0                ; current rover row
rover_col   db 0                ; current rover col
fuel        db 20               ; deduct 1 per valid move
cmd_buffer  db 21 dup(0)        ; filled by GET_COMMANDS
cmd_length  db 0                ; number of commands
obstacles   db 0,1, 5,0, 2,0, 2,3, 5,6, 1,7  ; row,col pairs
```

> Do not rename or resize any variable without telling the team.

---

## `.inc` File Format

Each file contains **only its procedure**. Nothing else.

```asm
PROC_NAME PROC
    ; code here
    RET
PROC_NAME ENDP
```

❌ No `data segment`, `code segment`, `stack segment`, `start:`, `org 100h`, or `end start` inside `.inc` files.

---

## How to Run

1. Open **`main.asm`** in emu8086 — never open `.inc` files there
2. All files must be in the **same folder**
3. Hit **Compile** → **Emulate**
4. Edit your `.inc` file in VS Code, then recompile

---

## Git Workflow

One branch (`main`), everyone pushes directly.

**Every session:**
```
1. Fetch origin → Pull        (before you start)
2. Edit your .inc file
3. Commit → Push              (when done)
```

Conflicts will not happen as long as everyone stays in their own file.
