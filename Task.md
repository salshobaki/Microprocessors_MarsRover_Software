Person 1 — Map & Display
Owns everything visual.

Define the 8x8 grid array in memory
Place the 6 obstacles using the ID modulo logic
Draw the initial grid to screen (. , O, R)
Redraw the grid after each move (clear + refresh)
Display the legend, battery level, and all status messages ("MISSION SUCCESS!", "CRITICAL FAILURE!", etc.)

Delivers: A procedure DRAW_GRID that takes the current rover position and redraws the full screen, plus the initialized grid data in memory.

Person 2 — Input & Validation
Owns everything the user types.

Display the input prompt
Read up to 20 characters into a buffer
Validate each character (only U, D, L, R — case-insensitive, convert lowercase to upper)
Reject invalid strings and re-prompt
Store the cleaned command string in a shared buffer

Delivers: A procedure GET_COMMANDS that fills a shared buffer with a validated, uppercase command string and a length variable.

Person 3 — Movement & Logic
Owns everything that happens during execution.

Loop through the command buffer character by character
Calculate new row/col from each command
Boundary check — if out of 0–7 range, trigger boundary error
Collision check — if new position matches an obstacle, trigger crash + beep
If valid, update rover position, deduct 1 fuel unit
Call DRAW_GRID after each valid move (Person 1's procedure)
Track and display final fuel remaining

Delivers: A procedure RUN_SEQUENCE that drives the whole execution loop using the buffer from Person 2 and the grid from Person 1.
