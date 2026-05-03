# Mars Rover Grid Navigator — 8086 Assembly

> PSUT | 22344 Microprocessors | Spring 2026

An 8×8 grid simulation in 8086 Assembly. The user enters a string of movement commands (`U`/`D`/`L`/`R`); the rover executes them step-by-step with collision and boundary detection.

## Authors

| Name | Student ID | Module |
|------|-----------|--------|
| Sarah Shobaki | 20220188 | `sarah_map.inc` |
| Kareem Abu Sharkh | 20210651 | `kareem_input.inc` |
| Abdulrhman Atassi | 20230995 | `abdulrhman_logic.inc` |

## Rules

- Max 20 commands, case-insensitive.
- Rover starts at (0,0), 20 fuel units.
- 6 obstacles placed from team IDs (see `OBSTACLES.md`).
- Crash, boundary breach, or success ends the run.

## Game Demonstration

[Video Demonstration](https://drive.google.com/file/d/160Fvs_bFg9joC2RulRfXNxxSRZNxV41R/view?usp=sharing)
