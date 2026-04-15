# Microprocessors_MarsRover_Software
Authors:
* Sarah Shobaki
* Abdulrhman Atassi
* Kareem Abusharkh

# Grid Obstacles — Coordinate Calculation

## ID 1 — 20210651
**Obstacle 1** 
- Row: `5 mod 8 = 5`
- Col: `1 mod 8 = 1`
- → **(5, 1)** ✓
**Obstacle 2** 
- Row: `0 mod 8 = 0`
- Col: `6 mod 8 = 6`
- → **(0, 6)** ✓
---
## ID 2 — 20220188
**Obstacle 1** 
- Row: `8 mod 8 = 0`
- Col: `8 mod 8 = 0`
- Raw result: **(0, 0)** — ⚠️ collision with Rover start position
- → Shifted to **(0, 1)** ✓
**Obstacle 2** 
- Row: `0 mod 8 = 0`
- Col: `1 mod 8 = 1`
- Raw result: **(0, 1)** — ⚠️ already occupied by Obstacle 1 (shifted above)
- → Shifted to **(0, 2)** ✓
---
## ID 3 — 20230995
**Obstacle 1** 
- Row: `9 mod 8 = 1`
- Col: `5 mod 8 = 5`
- → **(1, 5)** ✓
**Obstacle 2** 
- Row: `0 mod 8 = 0`
- Col: `9 mod 8 = 1`
- Raw result: **(0, 1)** — ⚠️ already occupied (ID 2, Obstacle 1)
- → Shifted to **(0, 3)** ✓
---
