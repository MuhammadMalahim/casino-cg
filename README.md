# Casino Graphics Project

This project is a 3D interactive scene built using OpenGL, GLM, GLEW, and SDL2, designed around a virtual casino table with cards, poker chips, and animated components.

## Camera Setup

- Initially placed at (0, 30, 25)
- Looks at (0, 0, 0)
- Up vector: (0, 1, 0)

## Geometry Components

### Table
- Rendered as a textured quad with size 30 × 21
- Texture: green_fabric.jpg
- Position: 0.1 units below the origin

### Cards
- Five cards on the table initialized at positions: x = -4, -2, 0, 2, 4 (z = 0)
- Each card is 1.6 × 2.48 in size
- Backside uses card_back.png
- State (position and type) stored in a std::vector

### Hand
- Model: hand.obj, Texture: hand.png
- Position: (-3, 2, 12)
- Scaled 3× and rotatable using a GUI slider (-45° to +45°)

### Chips
- 10 chips stacked in a tower at (-10, 0, 5)
- 5 chips rotate and roll around the tower in a circle of radius 4
- Chip count adjustable using ImGui::SliderInt for both stack and rotating chips (1–20)
- Texture-based coloring via shader:
  - Red, Green, Blue, Yellow, Magenta, Cyan via RGB component manipulation

## Lighting

### Point Light (Swinging)
- White Phong-shaded point light
- Position path:  
  p(t) = [ -15 + 30t, 10 - 20t + 20t², 0 ]
- t ∈ [0,1], oscillates every 2 seconds
- Quadratic attenuation: 0.001
- Two GUI checkboxes:
  - Toggle light motion (pause/resume without jumping)
  - Toggle lighting effects (texture-only mode)

## Card Texturing

- Cards are from a single atlas: cards.png
- The u-coordinate in the fragment shader is computed based on card type (0–4)
- Allows for individual card face control

## Interaction Features

### Card Placement (Ctrl + Click)
- Adds a new card on the table at the nearest grid point (x divisible by 2, z divisible by 3)
- Works only within table bounds
- Card type set via number keys 0–4

### Custom Card Back
- Two colors c1 and c2 can be chosen via ImGui
- On button click:
  - Gradient applied to back of cards between c1 (top) and c2 (bottom)
  - Black-suited cards (spades, clubs): white background replaced by c1
  - Red-suited cards (hearts, diamonds): white background replaced by c2
- Texture updated live on GPU

## Technologies Used

- OpenGL (Core Profile)
- GLM for math
- GLEW for extension loading
- SDL2 for context and input
- ImGui for GUI controls

## Build Requirements

- C++17
- SDL2
- GLEW
- GLM
- ImGui

## Notes

This project is based on the official End-Term Exam Assignment and the readme is refined using ChatGPT.
