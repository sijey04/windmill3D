#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <string>

// Rotation angles
float windmillRotation = 0.0f;    // Rotation of the blades
float sceneRotation = 0.0f;       // Rotation of the entire scene

// Camera control
bool autoRotate = true;           // Auto-rotation toggle
float manualRotation = 0.0f;      // Manual rotation angle
float cameraDistance = 5.0f;      // Distance from camera to windmill
float cameraHeight = 2.0f;        // Camera height

// Windmill parameters
const int NUM_BLADES = 4;
const float BLADE_LENGTH = 0.8f;
const float BLADE_WIDTH = 0.2f;
const float TOWER_HEIGHT = 1.5f;
const float TOWER_RADIUS = 0.2f;
const float ROOF_HEIGHT = 0.3f;

// Animation speed
const float ROTATION_SPEED = 1.0f;
const float CAMERA_ROTATION_SPEED = 0.1f;
const float MANUAL_ROTATION_STEP = 5.0f;

// Colors
GLfloat skyBlue[] = {0.53f, 0.81f, 0.92f, 1.0f};
GLfloat grassGreen[] = {0.1f, 0.6f, 0.1f, 1.0f};
GLfloat darkBrown[] = {0.4f, 0.2f, 0.0f, 1.0f};
GLfloat lightBrown[] = {0.8f, 0.5f, 0.2f, 1.0f};
GLfloat bladeWhite[] = {0.9f, 0.9f, 0.9f, 1.0f};
GLfloat roofRed[] = {0.8f, 0.2f, 0.2f, 1.0f};
GLfloat doorColor[] = {0.35f, 0.16f, 0.14f, 1.0f};
GLfloat windowColor[] = {0.9f, 0.9f, 1.0f, 1.0f};
GLfloat buttonColor[] = {0.2f, 0.2f, 0.8f, 1.0f};  // Blue color for button
GLfloat buttonHoverColor[] = {0.4f, 0.4f, 1.0f, 1.0f};  // Lighter blue for hover
GLfloat textColor[] = {1.0f, 1.0f, 1.0f, 1.0f};  // White for text

// Light position
GLfloat lightPos[] = {5.0f, 10.0f, 5.0f, 1.0f};

// UI elements
bool showControls = true;
int lastMouseX = -1;
int lastMouseY = -1;
bool mousePressed = false;
bool buttonHovered = false;
int windowWidth = 800;
int windowHeight = 600;

// Function prototypes
void display();
void reshape(int width, int height);
void timer(int value);
void drawWindmill();
void drawTower();
void drawBlades();
void drawRoof();
void drawDetails();
void drawGround();
void setupLighting();
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void passiveMotion(int x, int y);
void drawUI();
void drawControls();
void drawAutoRotateButton();
void drawString(const char* str, float x, float y);

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("3D Windmill Animation");
    
    // Set background color to sky blue
    glClearColor(skyBlue[0], skyBlue[1], skyBlue[2], skyBlue[3]);
    
    // Enable depth testing and lighting
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
    
    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(25, timer, 0);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);
    
    // Print instructions
    std::cout << "==== 3D Windmill Animation Controls ====" << std::endl;
    std::cout << "Arrow keys: Rotate camera manually" << std::endl;
    std::cout << "A/Z: Zoom in/out" << std::endl;
    std::cout << "W/S: Adjust camera height" << std::endl;
    std::cout << "Space: Toggle auto-rotation" << std::endl;
    std::cout << "R: Reset camera" << std::endl;
    std::cout << "H: Toggle help display" << std::endl;
    std::cout << "ESC: Exit program" << std::endl;
    std::cout << "=======================================" << std::endl;
    
    // Enter the GLUT main loop
    glutMainLoop();
    return 0;
}

void display() {
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // 3D rendering
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Set up lighting
    setupLighting();
    
    // Set the camera position and orientation
    float totalRotation = autoRotate ? sceneRotation : manualRotation;
    float camX = cameraDistance * sin(totalRotation * M_PI / 180.0f);
    float camZ = cameraDistance * cos(totalRotation * M_PI / 180.0f);
    
    gluLookAt(
        camX, cameraHeight, camZ,   // Eye position
        0.0f, 1.0f, 0.0f,          // Look-at position
        0.0f, 1.0f, 0.0f           // Up vector
    );
    
    // Draw the ground
    drawGround();
    
    // Draw the windmill
    drawWindmill();
    
    // Draw UI elements in 2D
    drawUI();
    
    // Swap buffers (double buffering)
    glutSwapBuffers();
}

void setupLighting() {
    // Set up light properties
    GLfloat ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};  // Increased ambient light
    GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    // Update light position with scene rotation to keep it consistent
    GLfloat rotatedLightPos[4];
    float angleRad = sceneRotation * M_PI / 180.0f;
    rotatedLightPos[0] = lightPos[0] * cos(angleRad) - lightPos[2] * sin(angleRad);
    rotatedLightPos[1] = lightPos[1];
    rotatedLightPos[2] = lightPos[0] * sin(angleRad) + lightPos[2] * cos(angleRad);
    rotatedLightPos[3] = lightPos[3];
    
    glLightfv(GL_LIGHT0, GL_POSITION, rotatedLightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void reshape(int width, int height) {
    // Store current window dimensions
    windowWidth = width;
    windowHeight = height;
    
    // Set the viewport to cover the entire window
    glViewport(0, 0, width, height);
    
    // Set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Set perspective projection
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    
    // Return to the modelview matrix
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
    // Update rotation angles
    windmillRotation += ROTATION_SPEED;
    if (windmillRotation > 360.0f) {
        windmillRotation -= 360.0f;
    }
    
    // Slowly rotate the scene if auto-rotation is enabled
    if (autoRotate) {
        sceneRotation += CAMERA_ROTATION_SPEED;
        if (sceneRotation > 360.0f) {
            sceneRotation -= 360.0f;
        }
    }
    
    // Request a redisplay
    glutPostRedisplay();
    
    // Call this function again after 25 milliseconds
    glutTimerFunc(25, timer, 0);
}

// Handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:  // ESC key
            exit(0);
            break;
        case ' ':  // Space bar
            autoRotate = !autoRotate;
            if (autoRotate) {
                sceneRotation = manualRotation;  // Sync rotations when toggling
            } else {
                manualRotation = sceneRotation;
            }
            break;
        case 'h':
        case 'H':
            showControls = !showControls;
            break;
        case 'r':
        case 'R':
            // Reset camera position
            cameraDistance = 5.0f;
            cameraHeight = 2.0f;
            if (!autoRotate) {
                manualRotation = 0.0f;
            }
            break;
        case 'a':
        case 'A':
            // Zoom in
            cameraDistance = std::max(2.0f, cameraDistance - 0.2f);
            break;
        case 'z':
        case 'Z':
            // Zoom out
            cameraDistance = std::min(10.0f, cameraDistance + 0.2f);
            break;
        case 'w':
        case 'W':
            // Move camera up
            cameraHeight = std::min(5.0f, cameraHeight + 0.2f);
            break;
        case 's':
        case 'S':
            // Move camera down
            cameraHeight = std::max(0.5f, cameraHeight - 0.2f);
            break;
    }
    glutPostRedisplay();
}

// Handle special keys (arrows)
void specialKeys(int key, int x, int y) {
    if (!autoRotate) {  // Only use manual control when auto-rotate is off
        switch (key) {
            case GLUT_KEY_LEFT:
                manualRotation += MANUAL_ROTATION_STEP;
                if (manualRotation > 360.0f) manualRotation -= 360.0f;
                break;
            case GLUT_KEY_RIGHT:
                manualRotation -= MANUAL_ROTATION_STEP;
                if (manualRotation < 0.0f) manualRotation += 360.0f;
                break;
            case GLUT_KEY_UP:
                cameraHeight = std::min(5.0f, cameraHeight + 0.2f);
                break;
            case GLUT_KEY_DOWN:
                cameraHeight = std::max(0.5f, cameraHeight - 0.2f);
                break;
        }
        glutPostRedisplay();
    }
}

// Handle mouse clicks
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mousePressed = true;
            lastMouseX = x;
            lastMouseY = y;
            
            // Check if clicked on auto-rotate button
            if (x >= 10 && x <= 210 && y >= 10 && y <= 40) {
                autoRotate = !autoRotate;
                if (autoRotate) {
                    sceneRotation = manualRotation;
                } else {
                    manualRotation = sceneRotation;
                }
            }
        } else {
            mousePressed = false;
        }
    }
}

// Handle mouse motion with button pressed
void motion(int x, int y) {
    if (mousePressed && !autoRotate) {
        // Rotate camera based on mouse movement
        float deltaX = x - lastMouseX;
        manualRotation -= deltaX * 0.5f;
        
        // Wrap around 360 degrees
        if (manualRotation < 0.0f) manualRotation += 360.0f;
        if (manualRotation > 360.0f) manualRotation -= 360.0f;
        
        lastMouseX = x;
        lastMouseY = y;
        
        glutPostRedisplay();
    }
}

// Handle mouse motion without button pressed
void passiveMotion(int x, int y) {
    // Check if mouse is over the auto-rotate button
    buttonHovered = (x >= 10 && x <= 210 && y >= 10 && y <= 40);
    glutPostRedisplay();
}

void drawWindmill() {
    // Draw the tower
    drawTower();
    
    // Draw the roof
    drawRoof();
    
    // Draw additional details (door, windows)
    drawDetails();
    
    // Draw the blades
    drawBlades();
}

// Draw UI elements
void drawUI() {
    // Switch to 2D orthographic projection for UI
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, windowHeight, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Disable lighting for UI elements
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    // Draw auto-rotate button
    drawAutoRotateButton();
    
    // Draw controls if enabled
    if (showControls) {
        drawControls();
    }
    
    // Re-enable lighting and depth test
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    // Restore matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// Draw the auto-rotate button
void drawAutoRotateButton() {
    // Set button color (blue, or lighter blue if hovered)
    if (buttonHovered) {
        glColor3f(buttonHoverColor[0], buttonHoverColor[1], buttonHoverColor[2]);
    } else {
        glColor3f(buttonColor[0], buttonColor[1], buttonColor[2]);
    }
    
    // Draw button background
    glBegin(GL_QUADS);
    glVertex2f(10, 10);
    glVertex2f(210, 10);
    glVertex2f(210, 40);
    glVertex2f(10, 40);
    glEnd();
    
    // Draw button text
    glColor3f(textColor[0], textColor[1], textColor[2]);
    std::string buttonText = autoRotate ? "Auto-Rotate: ON" : "Auto-Rotate: OFF";
    drawString(buttonText.c_str(), 30, 28);
}

// Draw control instructions
void drawControls() {
    int yPos = 70;
    int lineHeight = 20;
    
    glColor3f(textColor[0], textColor[1], textColor[2]);
    
    drawString("=== CONTROLS ===", 20, yPos);
    yPos += lineHeight;
    
    drawString("Arrow keys: Rotate camera manually", 20, yPos);
    yPos += lineHeight;
    
    drawString("A/Z: Zoom in/out", 20, yPos);
    yPos += lineHeight;
    
    drawString("W/S: Adjust camera height", 20, yPos);
    yPos += lineHeight;
    
    drawString("Space: Toggle auto-rotation", 20, yPos);
    yPos += lineHeight;
    
    drawString("R: Reset camera", 20, yPos);
    yPos += lineHeight;
    
    drawString("H: Toggle help display", 20, yPos);
    yPos += lineHeight;
    
    drawString("ESC: Exit program", 20, yPos);
}

// Draw a text string
void drawString(const char* str, float x, float y) {
    glRasterPos2f(x, y);
    for (const char* c = str; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }
}

void drawTower() {
    // Direct coloring for the tower (wooden texture)
    glColor3f(lightBrown[0], lightBrown[1], lightBrown[2]);
    
    // Set material properties
    GLfloat specular[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat shininess = 10.0f;
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    // Draw a cylinder for the tower
    GLUquadricObj* tower = gluNewQuadric();
    gluQuadricDrawStyle(tower, GLU_FILL);
    gluQuadricNormals(tower, GLU_SMOOTH);
    
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(tower, TOWER_RADIUS, TOWER_RADIUS * 0.8f, TOWER_HEIGHT, 20, 20);
    
    // Add a base to the tower - darker brown
    glColor3f(darkBrown[0], darkBrown[1], darkBrown[2]);
    gluDisk(tower, 0.0, TOWER_RADIUS * 1.2, 20, 1);
    glPopMatrix();
    
    gluDeleteQuadric(tower);
}

void drawRoof() {
    // Direct coloring for the roof (red color)
    glColor3f(roofRed[0], roofRed[1], roofRed[2]);
    
    // Set material properties
    GLfloat specular[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat shininess = 25.0f;
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    // Position at the top of the tower
    glPushMatrix();
    glTranslatef(0.0f, TOWER_HEIGHT, 0.0f);
    
    // Draw a cone for the roof
    GLUquadricObj* roof = gluNewQuadric();
    gluQuadricDrawStyle(roof, GLU_FILL);
    gluQuadricNormals(roof, GLU_SMOOTH);
    
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(roof, TOWER_RADIUS * 0.9f, 0.0f, ROOF_HEIGHT, 20, 20);
    
    gluDeleteQuadric(roof);
    glPopMatrix();
}

void drawDetails() {
    // Draw door - deep brown
    glColor3f(doorColor[0], doorColor[1], doorColor[2]);
    
    // Set material properties
    GLfloat specular[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat shininess = 5.0f;
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    glPushMatrix();
    glTranslatef(0.0f, 0.25f, TOWER_RADIUS * 0.99f);
    glScalef(TOWER_RADIUS * 0.5f, 0.5f, 0.01f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Draw windows (two small ones) - blue-white
    glColor3f(windowColor[0], windowColor[1], windowColor[2]);
    
    // Set material properties
    GLfloat windowSpecular[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat windowShininess = 80.0f;
    glMaterialfv(GL_FRONT, GL_SPECULAR, windowSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, windowShininess);
    
    // Window 1
    glPushMatrix();
    glTranslatef(TOWER_RADIUS * 0.7f, TOWER_HEIGHT * 0.6f, TOWER_RADIUS * 0.7f);
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.15f, 0.15f, 0.01f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Window 2
    glPushMatrix();
    glTranslatef(-TOWER_RADIUS * 0.7f, TOWER_HEIGHT * 0.6f, TOWER_RADIUS * 0.7f);
    glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.15f, 0.15f, 0.01f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawBlades() {
    // Position at the top of the tower
    glPushMatrix();
    glTranslatef(0.0f, TOWER_HEIGHT, 0.0f);
    
    // Small hub connecting the blades
    glColor3f(darkBrown[0], darkBrown[1], darkBrown[2]);
    
    GLUquadricObj* hub = gluNewQuadric();
    gluQuadricDrawStyle(hub, GLU_FILL);
    gluQuadricNormals(hub, GLU_SMOOTH);
    gluSphere(hub, TOWER_RADIUS * 0.4f, 20, 20);
    gluDeleteQuadric(hub);
    
    // Apply blade rotation
    glRotatef(windmillRotation, 0.0f, 0.0f, 1.0f);
    
    // Draw blades
    for (int i = 0; i < NUM_BLADES; i++) {
        glPushMatrix();
        glRotatef(i * (360.0f / NUM_BLADES), 0.0f, 0.0f, 1.0f);
        
        // White blade
        glColor3f(bladeWhite[0], bladeWhite[1], bladeWhite[2]);
        
        // Set material properties
        GLfloat specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
        GLfloat shininess = 50.0f;
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        
        glBegin(GL_TRIANGLES);
        glNormal3f(0.0f, 0.1f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(BLADE_LENGTH, BLADE_WIDTH/2, 0.0f);
        glVertex3f(BLADE_LENGTH, -BLADE_WIDTH/2, 0.0f);
        glEnd();
        
        // Add a crossbeam to each blade for more detail - brown
        glColor3f(darkBrown[0], darkBrown[1], darkBrown[2]);
        
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(BLADE_LENGTH * 0.2f, BLADE_WIDTH * 0.15f, 0.01f);
        glVertex3f(BLADE_LENGTH * 0.7f, BLADE_WIDTH * 0.15f, 0.01f);
        glVertex3f(BLADE_LENGTH * 0.7f, -BLADE_WIDTH * 0.15f, 0.01f);
        glVertex3f(BLADE_LENGTH * 0.2f, -BLADE_WIDTH * 0.15f, 0.01f);
        glEnd();
        
        glPopMatrix();
    }
    
    glPopMatrix();
}

void drawGround() {
    // Direct coloring for the ground (grass-like)
    glColor3f(grassGreen[0], grassGreen[1], grassGreen[2]);
    
    // Set material properties
    GLfloat specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat shininess = 0.0f;
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    // Draw a large square for the ground
    glPushMatrix();
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);  // Normal pointing up
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, -10.0f);
    glEnd();
    
    // Add some simple path around the windmill - tan color
    glColor3f(0.9f, 0.8f, 0.7f);
    
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    // Path
    glVertex3f(-0.2f, 0.01f, 0.0f);
    glVertex3f(0.2f, 0.01f, 0.0f);
    glVertex3f(0.2f, 0.01f, 5.0f);
    glVertex3f(-0.2f, 0.01f, 5.0f);
    glEnd();
    
    glPopMatrix();
}
