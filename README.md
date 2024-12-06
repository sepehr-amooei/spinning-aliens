#2D Spinning Game

A simple 2D game built with OpenGL, where players control a central gun to shoot bullets at rotating aliens. The game combines basic graphics programming techniques with interactive gameplay.
## Features
-Rotating Aliens: Aliens move in a circular path around the central gun.
-Central Gun: Shoots bullets upward toward the aliens.
-Dynamic Scoring: Gain or lose points based on alien hits and bullet misses.
-Game Over Logic: The game ends with a "win" at 50 points or a "loss" at -20 points.
-Visual Feedback: The roundabout changes colors dynamically based on the score.
---
Table of Contents

1. Installation
2. How to Run
3. Controls
4. Dependencies
5. Code Structure
6. Suggestions for Improvement

## Installation

1.  **Clone the Repository**:

  git clone https://github.com/username/2DSpinningGame.git
  cd 2DSpinningGame

2. **Install OpenGL**:

    On Linux:

      sudo apt-get install freeglut3 freeglut3-dev

    On Windows: Download and configure FreeGLUT with your compiler.
    On Mac:

      brew install freeglut

3. **Compile the Code**:
Use a C++ compiler like g++:
    g++ -o spinning_game spinning_game.cpp -lGL -lGLU -lglut

## How to Run

After compiling, run the game:

./spinning_game

## Controls

  - **Spacebar**: Shoot bullets toward the aliens.
  - **Game Over**: Ends with a message displaying "You Won" or "Game Over" based on your score.

## Dependencies

This project requires:

- OpenGL: For rendering graphics.
- GLUT: For handling window management and user inputs.

## Code Structure
### Key Functions:

- init(): Sets up the OpenGL environment with initial parameters.
- drawGun(): Renders the central gun.
- drawAliens(): Draws rotating aliens along a circular path.
- display(): Handles the main rendering loop.
- timer(int): Updates animations and alien speeds based on the current score.
- keyboard(unsigned char key, int x, int y): Handles user inputs for shooting bullets.

### Main Flow:

1. Initialize OpenGL settings and game elements.
2. Start the rendering loop with alien animations.
3. Detect collisions between bullets and aliens.
4. Adjust scores and trigger win/loss conditions.

## Author

Developed by **[Sepehr Amooeinejad]**. Feel free to contribute or reach out for suggestions!

