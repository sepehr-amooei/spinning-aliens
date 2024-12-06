#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <string>

#define PI 3.14159265

float alienAngle = 0.0f; // Current angle for the aliens
float radius = 3.0f;   // Radius of the roundabout
bool isBulletActive = false;
float bulletX = 0.0f, bulletY = 0.4f; // Bullet position (starts from gun tip)
float bulletSpeed = 0.1f;             // Speed of the bullet
float bulletAngle = 90.0f;            // Direction of the bullet (default upward)

// Store alien positions
struct alien {
    float x, y;
    bool destroyed; // Indicates if the alien is destroyed
    alien(float x_, float y_) : x(x_), y(y_), destroyed(false) {}
};
std::vector<alien> aliens;

// Store fragments that appear after a alien is destroyed
struct Fragment {
    float x, y, size;
    float speedY;
    Fragment(float x_, float y_, float size_) : x(x_), y(y_), size(size_), speedY(0.05f) {}
};
std::vector<Fragment> fragments;

// Global variables for score and game state
int score = 0;
bool isGameOver = false;
bool isRoundaboutBlack = false;

// Initialize OpenGL settings
void init() {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Background color: blue
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-5.0f, 5.0f, -5.0f, 5.0f); // Set up the 2D view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Draw a alien with windows, trunk, and hood
void drawalien(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    // Main body of the alien
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for the alien body
    glBegin(GL_QUADS);
    glVertex2f(-0.3f, -0.2f);
    glVertex2f(0.3f, -0.2f);
    glVertex2f(0.3f, 0.2f);
    glVertex2f(-0.3f, 0.2f);
    glEnd();

    // Front hood (caput)
    glColor3f(0.8f, 0.0f, 0.0f); // Darker red for the hood
    glBegin(GL_QUADS);
    glVertex2f(0.3f, -0.15f);
    glVertex2f(0.4f, -0.15f);
    glVertex2f(0.4f, 0.15f);
    glVertex2f(0.3f, 0.15f);
    glEnd();

    // Back trunk
    glColor3f(0.8f, 0.0f, 0.0f); // Darker red for the trunk
    glBegin(GL_QUADS);
    glVertex2f(-0.4f, -0.15f);
    glVertex2f(-0.3f, -0.15f);
    glVertex2f(-0.3f, 0.15f);
    glVertex2f(-0.4f, 0.15f);
    glEnd();

    // Windows
    glColor3f(0.5f, 0.5f, 0.9f); // Blueish color for windows
    glBegin(GL_QUADS);
    // Front window
    glVertex2f(-0.2f, 0.0f);
    glVertex2f(-0.1f, 0.0f);
    glVertex2f(-0.1f, 0.15f);
    glVertex2f(-0.2f, 0.15f);
    // Back window
    glVertex2f(0.1f, 0.0f);
    glVertex2f(0.2f, 0.0f);
    glVertex2f(0.2f, 0.15f);
    glVertex2f(0.1f, 0.15f);
    glEnd();

    // Wheels
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for wheels
    glBegin(GL_QUADS);
    // Front wheel
    glVertex2f(-0.2f, -0.25f);
    glVertex2f(-0.1f, -0.25f);
    glVertex2f(-0.1f, -0.2f);
    glVertex2f(-0.2f, -0.2f);
    // Back wheel
    glVertex2f(0.1f, -0.25f);
    glVertex2f(0.2f, -0.25f);
    glVertex2f(0.2f, -0.2f);
    glVertex2f(0.1f, -0.2f);
    glEnd();

    glPopMatrix();
}



// Draw a fragment (square part of a destroyed alien) with a goldish color
void drawFragment(float x, float y, float size) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glColor3f(1.0f, 0.843f, 0.0f); // Goldish color
    glBegin(GL_QUADS);
    glVertex2f(-size / 2, -size / 2);
    glVertex2f(size / 2, -size / 2);
    glVertex2f(size / 2, size / 2);
    glVertex2f(-size / 2, size / 2);
    glEnd();
    glPopMatrix();
}

// Draw the roundabout
void drawRoundabout() {
    if (score >=40) {
        glColor3f(1.0f, 1.0f, 1.0f);
    } else if(score >= 30) {
        glColor3f(0.3f, 0.5f, 1.5f);}
        else if(score >= 20) {
        glColor3f(1.4f, 1.2f, 1.7f);}
    else if(score >= 10) {
        glColor3f(0.0f, 1.0f, 1.0f);}
    else if(score <= -10) {
        glColor3f(0.0f, 0.0f, 0.0f);}
    else {
        glColor3f(0.0f, 1.0f, 0.0f); // Green roundabout otherwise
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f); // Center of the circle
    for (int i = 0; i <= 360; ++i) {
        float theta = i * PI / 180.0f; // Convert degrees to radians
        glVertex2f(radius * cos(theta), radius * sin(theta));
    }
    glEnd();
}

// Draw the gun at the center
void drawGun() {
    glColor3f(0.5f, 0.5f, 0.5f); // Gray gun
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.2f);
    glVertex2f(0.2f, -0.2f);
    glVertex2f(0.2f, 0.4f); // Extended barrel pointing upward
    glVertex2f(-0.2f, 0.4f);
    glEnd();
}

// Draw the bullet if active
void drawBullet() {
    if (isBulletActive) {
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow bullet
        glPushMatrix();
        glTranslatef(bulletX, bulletY, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-0.1f, -0.1f);
        glVertex2f(0.1f, -0.1f);
        glVertex2f(0.1f, 0.1f);
        glVertex2f(-0.1f, 0.1f);
        glEnd();
        glPopMatrix();
    }
}

// Function to display the score at the bottom of the screen
void displayScore() {
    glColor3f(1.0f, 1.0f, 1.0f); // White color for the score text
    glRasterPos2f(-4.5f, -4.5f);  // Position the score text at the bottom-left corner

    // Render the score as text
    std::string scoreText = "Score: " + std::to_string(score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);  // Render each character
    }
}

// Function to display "Game Over" text in the center
// Function to display "Game Over" or "You Won"
void displayGameOver() {
    // Draw a black rectangle as the background for the text
    glColor3f(0.0f, 0.0f, 0.0f); // Black background
    glBegin(GL_QUADS);
    glVertex2f(-2.5f, -0.5f); // Rectangle covering the text area
    glVertex2f(2.5f, -0.5f);
    glVertex2f(2.5f, 0.5f);
    glVertex2f(-2.5f, 0.5f);
    glEnd();

    // Check the score and render the appropriate message
    if (score >= 50) {
        glColor3f(0.0f, 1.0f, 0.0f); // Green text for "You Won"
        glRasterPos2f(-1.0f, 0.0f);  // Center the text
        std::string youWonText = "YOU WON!";
        for (char c : youWonText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);  // Render each character
        }
    } else {
        glColor3f(1.0f, 0.0f, 0.0f); // Red text for "Game Over"
        glRasterPos2f(-1.0f, 0.0f);  // Center the text
        std::string gameOverText = "GAME OVER";
        for (char c : gameOverText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);  // Render each character
        }
    }
}


// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (isGameOver) {
        displayGameOver(); // Show "Game Over" if the game is over
    } else {
        drawRoundabout(); // Draw the roundabout
        drawGun();        // Draw the gun

        // Draw the aliens
        for (const auto &alien : aliens) {
            if (!alien.destroyed) {
                drawalien(alien.x, alien.y);
            }
        }

        // Draw fragments if they exist
        for (const auto &fragment : fragments) {
            drawFragment(fragment.x, fragment.y, fragment.size);
        }

        drawBullet();  // Draw the bullet
        displayScore(); // Display the score
    }

    glutSwapBuffers();
}
float speedMultiplierfunc(int currentScore) {
    if (currentScore < 0) {
        return 1.0f; 
    } else if (currentScore < 10) {
        return 1.25f; 
    } else if (currentScore < 20) {
        return 1.5f; 
    } else if (currentScore < 30) {
        return 1.75f;
    } else {
        return 2.0f; 
    }
}

// Timer callback for updating animation
void timer(int value) {
    if (isGameOver) return;

    // Adjust alien speed based on the score
    float speedMultiplier = speedMultiplierfunc(score); // Double the speed if score > 25
    alienAngle += 1.0f * speedMultiplier; // Increment angle
    if (alienAngle >= 360.0f)
        alienAngle -= 360.0f;

    // Update alien positions
    aliens.clear();
    for (int i = 0; i < 4; ++i) {
        float angle = alienAngle + i * 90.0f; // Offset each alien by 90 degrees
        float x = radius * cos(angle * PI / 180.0f);
        float y = radius * sin(angle * PI / 180.0f);
        aliens.emplace_back(x, y);
    }

    // Update bullet position
    if (isBulletActive) {
        bulletX += bulletSpeed * cos(bulletAngle * PI / 180.0f);
        bulletY += bulletSpeed * sin(bulletAngle * PI / 180.0f);

        bool hitalien = false;
        for (auto &alien : aliens) {
            if (!alien.destroyed && fabs(bulletX - alien.x) < 0.4f && fabs(bulletY - alien.y) < 0.4f) {
                alien.destroyed = true; // Destroy the alien
                score += 4;          // Increase score
                isBulletActive = false;
                hitalien = true;
                break;
            }
        }

        if (!hitalien && (fabs(bulletX) > 5.0f || fabs(bulletY) > 5.0f)) {
            score -= 5;
            isBulletActive = false;
        }
    }

    if (score == 40) isRoundaboutBlack = true; // Make roundabout black at score 40
    if (score <= -20 || score >= 50) isGameOver = true;

    for (auto &fragment : fragments) fragment.y -= fragment.speedY;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// Keyboard input for shooting
void keyboard(unsigned char key, int x, int y) {
    if (key == ' ' && !isBulletActive && !isGameOver) {
        bulletX = 0.0f;
        bulletY = 0.4f; // Start bullet at gun tip
        isBulletActive = true;
    }
}

// Main function
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Spinning aliens");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
