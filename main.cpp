// Urban city simulation updated with a foreground Supershop, Taxi, Police Station, traffic lights, landmark building, fuel stations, park, playground, parked cars, pedestrian walks, a small school, road fences, a shopping mall, car parking area, flying birds, and a small hospital.
// Build with MinGW FreeGLUT: g++ city_simulation.cpp -o city_simulation.exe -I"C:\Program Files\CodeBlocks\MinGW\x86_64-w64-mingw32\include" -L"C:\Program Files\CodeBlocks\MinGW\x86_64-w64-mingw32\lib" -lfreeglut -lopengl32 -lglu32 -lgdi32

#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

float car1Pos = -28.0f;
float car2Pos = 28.0f;
float car3Pos = -14.0f;
float car4Pos = 0.0f;
float car5Pos = 42.0f;
float busPos = -32.0f;
// wheel rotation angles (degrees)
float car1WheelRot = 0.0f;
float car2WheelRot = 0.0f;
float car3WheelRot = 0.0f;
float car4WheelRot = 0.0f;
float car5WheelRot = 0.0f;
float busWheelRot = 0.0f;
float trafficLightTimer = 0.0f;
// taxi
float taxiPos = 12.0f;
float taxiWheelRot = 0.0f;
float leafSway = 0.0f;
const float PI = 3.14159265f;

// Bird animation factor
float birdWingTimer = 0.0f;
float birdFlyAngle = 0.0f;

// Pedestrian Walk Position Vector Trackers
float ped1Pos = -22.0f;
float ped2Pos = 18.0f;

// Camera state
float cameraAngleX = 20.0f;
float cameraAngleY = -30.0f;
float cameraDistance = 40.0f;
float cameraPosX = 0.0f;
float cameraPosY = 10.0f;
float cameraPosZ = 30.0f;
int lastMouseX = 0;
int lastMouseY = 0;
bool mouseDragging = false;
bool isDay = true;

void setColor(float r, float g, float b) {
    glColor3f(r, g, b);
}

void drawBox(float x, float y, float z, float width, float height, float depth) {
    float x2 = x + width;
    float y2 = y + height;
    float z2 = z + depth;

    glBegin(GL_QUADS);
    // Front
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x, y, z2); glVertex3f(x2, y, z2); glVertex3f(x2, y2, z2); glVertex3f(x, y2, z2);
    // Back
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x2, y, z); glVertex3f(x, y, z); glVertex3f(x, y2, z); glVertex3f(x2, y2, z);
    // Left
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(x, y, z); glVertex3f(x, y, z2); glVertex3f(x, y2, z2); glVertex3f(x, y2, z);
    // Right
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x2, y, z2); glVertex3f(x2, y, z); glVertex3f(x2, y2, z); glVertex3f(x2, y2, z2);
    // Top
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x, y2, z2); glVertex3f(x2, y2, z2); glVertex3f(x2, y2, z); glVertex3f(x, y2, z);
    // Bottom
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(x, y, z); glVertex3f(x2, y, z); glVertex3f(x2, y, z2); glVertex3f(x, y, z2);
    glEnd();
}

void drawCylinder(float baseRad, float topRad, float height, int slices) {
    GLUquadric* obj = gluNewQuadric();
    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricNormals(obj, GLU_SMOOTH);
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(obj, baseRad, topRad, height, slices, 1);
    glPopMatrix();
    gluDeleteQuadric(obj);
}

void drawWindowPanel(float x, float y, float z, float width, float height) {
    if(!isDay) setColor(0.98f, 0.95f, 0.45f);
    else setColor(0.55f, 0.78f, 0.92f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x, y, z); glVertex3f(x + width, y, z);
    glVertex3f(x + width, y + height, z); glVertex3f(x, y + height, z);
    glEnd();
}

void drawWindowPanelSide(float x, float y, float z, float width, float height) {
    if(!isDay) setColor(0.98f, 0.95f, 0.45f);
    else setColor(0.55f, 0.78f, 0.92f);
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x, y, z); glVertex3f(x, y, z + width);
    glVertex3f(x, y + height, z + width); glVertex3f(x, y + height, z);
    glEnd();
}

void drawText3D(float x, float y, float z, const char* text, float scale, float r, float g, float b) {
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(scale, scale, scale);
    glColor3f(r, g, b);
    for (const char* p = text; *p; ++p) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    }
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void drawRoadFences() {
    setColor(0.7f, 0.7f, 0.7f);
    for (float x = -45.0f; x <= -5.0f; x += 1.5f) {
        drawBox(x, 0.16f, -5.2f, 0.1f, 0.6f, 0.1f);
        drawBox(x, 0.6f, -5.2f, 1.5f, 0.05f, 0.05f);
    }
    for (float x = 5.0f; x <= 45.0f; x += 1.5f) {
        drawBox(x, 0.16f, -5.2f, 0.1f, 0.6f, 0.1f);
        drawBox(x, 0.6f, -5.2f, 1.5f, 0.05f, 0.05f);
    }
    for (float x = -45.0f; x <= -5.0f; x += 1.5f) {
        if (x >= -20.0f && x <= -10.0f) continue;
        drawBox(x, 0.16f, 5.1f, 0.1f, 0.6f, 0.1f);
        drawBox(x, 0.6f, 5.1f, 1.5f, 0.05f, 0.05f);
    }
    for (float x = 5.0f; x <= 45.0f; x += 1.5f) {
        if (x >= 12.0f && x <= 22.0f) continue;
        drawBox(x, 0.16f, 5.1f, 0.1f, 0.6f, 0.1f);
        drawBox(x, 0.6f, 5.1f, 1.5f, 0.05f, 0.05f);
    }
    for (float z = -45.0f; z <= -5.0f; z += 1.5f) {
        drawBox(-5.2f, 0.16f, z, 0.1f, 0.6f, 0.1f);
        drawBox(-5.2f, 0.6f, z, 0.05f, 0.05f, 1.5f);
    }
    for (float z = 5.0f; z <= 45.0f; z += 1.5f) {
        drawBox(-5.2f, 0.16f, z, 0.1f, 0.6f, 0.1f);
        drawBox(-5.2f, 0.6f, z, 0.05f, 0.05f, 1.5f);
    }
    for (float z = -45.0f; z <= -5.0f; z += 1.5f) {
        drawBox(5.1f, 0.16f, z, 0.1f, 0.6f, 0.1f);
        drawBox(5.1f, 0.6f, z, 0.05f, 0.05f, 1.5f);
    }
    for (float z = 5.0f; z <= 45.0f; z += 1.5f) {
        if (z >= 8.0f && z <= 20.0f) continue;
        drawBox(5.1f, 0.16f, z, 0.1f, 0.6f, 0.1f);
        drawBox(5.1f, 0.6f, z, 0.05f, 0.05f, 1.5f);
    }
}

void drawShoppingMall(float x, float z) {
    setColor(0.2f, 0.25f, 0.3f);
    drawBox(x, 0.0f, z, 16.0f, 0.4f, 14.0f);
    setColor(0.1f, 0.4f, 0.6f);
    drawBox(x + 0.5f, 0.4f, z + 0.5f, 15.0f, 9.0f, 13.0f);
    setColor(0.85f, 0.85f, 0.85f);
    for (float px = x + 0.5f; px <= x + 15.5f; px += 3.75f) {
        drawBox(px - 0.1f, 0.4f, z + 13.51f, 0.3f, 9.0f, 0.1f);
    }
    drawBox(x + 0.5f, 3.4f, z + 13.51f, 15.0f, 0.4f, 0.1f);
    drawBox(x + 0.5f, 6.4f, z + 13.51f, 15.0f, 0.4f, 0.1f);
    drawBox(x + 0.5f, 9.4f, z + 0.5f, 15.0f, 0.5f, 13.0f);
    setColor(0.9f, 0.1f, 0.2f);
    drawBox(x + 6.0f, 0.4f, z + 13.55f, 4.0f, 3.0f, 0.15f);
    setColor(0.95f, 0.95f, 0.95f);
    drawBox(x + 6.5f, 0.4f, z + 13.6f, 3.0f, 2.6f, 0.05f);
    drawText3D(x + 1.8f, 10.3f, z + 13.6f, "CITY CENTRAL MALL", 0.007f, 0.95f, 0.95f, 0.1f);
}

void drawSupershop(float x, float z) {
    setColor(0.22f, 0.24f, 0.26f);
    drawBox(x - 0.5f, 0.0f, z - 0.5f, 15.0f, 0.2f, 13.0f);
    setColor(0.88f, 0.22f, 0.15f);
    drawBox(x, 0.2f, z, 14.0f, 6.8f, 12.0f);
    setColor(0.95f, 0.95f, 0.95f);
    drawBox(x - 0.2f, 7.0f, z - 0.2f, 14.4f, 0.4f, 12.4f);
    setColor(0.15f, 0.15f, 0.18f);
    drawBox(x + 4.5f, 0.2f, z + 12.01f, 5.0f, 3.2f, 0.2f);
    if(!isDay) setColor(0.95f, 0.95f, 0.5f); else setColor(0.6f, 0.85f, 0.95f);
    drawBox(x + 5.0f, 0.2f, z + 12.12f, 1.8f, 2.8f, 0.02f);
    drawBox(x + 7.2f, 0.2f, z + 12.12f, 1.8f, 2.8f, 0.02f);
    setColor(0.9f, 0.75f, 0.1f);
    drawBox(x + 4.0f, 3.4f, z + 11.5f, 6.0f, 0.2f, 1.2f);
    drawWindowPanel(x + 0.8f, 0.8f, z + 12.01f, 3.0f, 2.2f);
    drawWindowPanel(x + 10.2f, 0.8f, z + 12.01f, 3.0f, 2.2f);
    for (float wx = x + 1.0f; wx <= x + 12.0f; wx += 2.8f) {
        drawWindowPanel(wx, 4.2f, z + 12.01f, 1.8f, 1.8f);
    }
    for (float wz = z + 2.0f; wz <= z + 9.0f; wz += 3.0f) {
        drawWindowPanelSide(x - 0.01f, 1.5f, wz, 1.8f, 2.0f);
        drawWindowPanelSide(x - 0.01f, 4.5f, wz, 1.8f, 1.8f);
    }
    setColor(0.6f, 0.62f, 0.65f);
    drawBox(x + 10.5f, 0.22f, z + 12.3f, 1.5f, 0.9f, 1.0f);
    setColor(0.1f, 0.1f, 0.1f);
    drawBox(x + 10.6f, 0.3f, z + 13.31f, 1.3f, 0.1f, 0.05f);
    drawText3D(x + 3.4f, 7.6f, z + 12.2f, "SUPERSHOP", 0.009f, 0.95f, 0.9f, 0.05f);
}

void drawCar(float x, float z, float r, float g, float b, float wheelRot, bool facesPositiveX = true) {
    float dir = facesPositiveX ? 1.0f : -1.0f;
    setColor(r, g, b);
    drawBox(x - 2.0f, 0.3f, z - 1.0f, 4.0f, 0.8f, 2.0f);
    setColor(r + 0.1f, g + 0.1f, b + 0.1f);
    drawBox(x - 1.5f, 1.1f, z - 0.8f, 3.0f, 0.7f, 1.6f);

    drawWindowPanel(x - 1.2f, 1.2f, z + 0.81f, 0.9f, 0.4f);
    drawWindowPanel(x + 0.1f, 1.2f, z + 0.81f, 0.9f, 0.4f);
    drawWindowPanel(x - 1.2f, 1.2f, z - 0.81f, 0.9f, 0.4f);
    drawWindowPanel(x + 0.1f, 1.2f, z - 0.81f, 0.9f, 0.4f);

    setColor(0.6f, 0.8f, 0.95f);
    drawBox(x + dir * 1.42f, 1.15f, z - 0.6f, 0.05f, 0.5f, 1.2f);

    setColor(0.06f, 0.06f, 0.06f);
    glPushMatrix(); glTranslatef(x - 1.4f, 0.35f, z - 1.1f); glRotatef(wheelRot, 0.0f, 0.0f, 1.0f); glutSolidTorus(0.08f, 0.32f, 12, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(x + 1.6f, 0.35f, z - 1.1f); glRotatef(wheelRot, 0.0f, 0.0f, 1.0f); glutSolidTorus(0.08f, 0.32f, 12, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(x - 1.4f, 0.35f, z + 0.9f); glRotatef(wheelRot, 0.0f, 0.0f, 1.0f); glutSolidTorus(0.08f, 0.32f, 12, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(x + 1.6f, 0.35f, z + 0.9f); glRotatef(wheelRot, 0.0f, 0.0f, 1.0f); glutSolidTorus(0.08f, 0.32f, 12, 12); glPopMatrix();

    setColor(1.0f, 1.0f, 0.8f);
    drawBox(x + dir * 1.9f, 0.4f, z - 0.7f, 0.1f, 0.2f, 0.4f);
    drawBox(x + dir * 1.9f, 0.4f, z + 0.3f, 0.1f, 0.2f, 0.4f);
}

// Custom function to draw static Police Car
void drawPoliceCar(float x, float z, float angle) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    // Main Body (Blue/White Police theme)
    setColor(0.05f, 0.1f, 0.4f);
    drawBox(-1.8f, 0.3f, -0.9f, 3.6f, 0.7f, 1.8f);
    setColor(0.95f, 0.95f, 0.95f);
    drawBox(-1.2f, 1.0f, -0.7f, 2.4f, 0.6f, 1.4f);

    // Windows
    drawWindowPanel(-0.9f, 1.05f, 0.71f, 0.7f, 0.4f);
    drawWindowPanel(0.1f, 1.05f, 0.71f, 0.7f, 0.4f);

    // Siren Light on Top
    setColor(1.0f, 0.0f, 0.0f);
    drawBox(-0.2f, 1.6f, -0.2f, 0.2f, 0.15f, 0.4f);
    setColor(0.0f, 0.0f, 1.0f);
    drawBox(0.0f, 1.6f, -0.2f, 0.2f, 0.15f, 0.4f);

    // Wheels
    setColor(0.05f, 0.05f, 0.05f);
    glPushMatrix(); glTranslatef(-1.1f, 0.35f, -0.95f); glutSolidTorus(0.06f, 0.26f, 10, 10); glPopMatrix();
    glPushMatrix(); glTranslatef(1.1f, 0.35f, -0.95f); glutSolidTorus(0.06f, 0.26f, 10, 10); glPopMatrix();
    glPushMatrix(); glTranslatef(-1.1f, 0.35f, 0.95f); glutSolidTorus(0.06f, 0.26f, 10, 10); glPopMatrix();
    glPushMatrix(); glTranslatef(1.1f, 0.35f, 0.95f); glutSolidTorus(0.06f, 0.26f, 10, 10); glPopMatrix();

    glPopMatrix();
}

// New Police Station Function (Placed at the requested red mark area)
void drawPoliceStation(float x, float z) {
    // Ground Lot Base
    setColor(0.3f, 0.32f, 0.35f);
    drawBox(x - 0.5f, 0.0f, z - 0.5f, 13.0f, 0.15f, 12.0f);

    // Main Police Building (Professional Dark Blue and Grey theme)
    setColor(0.1f, 0.15f, 0.3f);
    drawBox(x, 0.15f, z, 12.0f, 5.5f, 10.0f);

    // Front Concrete Pillar Decors
    setColor(0.7f, 0.72f, 0.75f);
    drawBox(x + 0.5f, 0.15f, z + 10.01f, 0.8f, 5.5f, 0.2f);
    drawBox(x + 10.7f, 0.15f, z + 10.01f, 0.8f, 5.5f, 0.2f);

    // Roof Border Accent
    setColor(0.8f, 0.1f, 0.15f); // Red Alert Line
    drawBox(x - 0.2f, 5.65f, z - 0.2f, 12.4f, 0.3f, 10.4f);

    // Main Entrance Glass Door
    setColor(0.5f, 0.7f, 0.85f);
    drawBox(x + 4.5f, 0.15f, z + 10.02f, 3.0f, 2.8f, 0.05f);
    setColor(0.1f, 0.1f, 0.1f); // Frame
    drawBox(x + 5.95f, 0.15f, z + 10.03f, 0.1f, 2.8f, 0.06f);

    // Windows On Front Face
    drawWindowPanel(x + 1.8f, 1.2f, z + 10.01f, 1.8f, 1.6f);
    drawWindowPanel(x + 8.4f, 1.2f, z + 10.01f, 1.8f, 1.6f);
    drawWindowPanel(x + 2.2f, 3.8f, z + 10.01f, 2.0f, 1.2f);
    drawWindowPanel(x + 7.8f, 3.8f, z + 10.01f, 2.0f, 1.2f);

    // Roof Police Siren/Beacon Light
    setColor(1.0f, 0.0f, 0.0f);
    drawBox(x + 5.5f, 5.95f, z + 5.0f, 0.4f, 0.5f, 0.4f);
    setColor(0.0f, 0.0f, 1.0f);
    drawBox(x + 6.1f, 5.95f, z + 5.0f, 0.4f, 0.5f, 0.4f);

    // Signboard Text
    drawText3D(x + 2.8f, 6.2f, z + 10.1f, "POLICE STATION", 0.0075f, 0.95f, 0.95f, 0.95f);

    // Stationary Guarded Police Car in Front Yard
    drawPoliceCar(x + 3.2f, z + 11.2f, 180.0f);
}

void drawBus(float x, float z, bool facesPositiveX = true) {
    float dir = facesPositiveX ? 1.0f : -1.0f;
    setColor(0.7f, 0.1f, 0.1f);
    drawBox(x - 4.0f, 0.3f, z - 1.0f, 8.0f, 1.6f, 2.2f);
    setColor(0.95f, 0.95f, 0.95f);
    drawBox(x - 3.2f, 1.4f, z - 0.7f, 6.4f, 0.9f, 1.6f);

    for(float wx = x - 2.8f; wx <= x + 2.4f; wx += 1.3f) {
        drawWindowPanel(wx, 1.5f, z + 0.91f, 0.8f, 0.6f);
        drawWindowPanel(wx, 1.5f, z - 0.91f, 0.8f, 0.6f);
    }

    setColor(0.05f, 0.05f, 0.05f);
    glPushMatrix(); glTranslatef(x - 2.6f, 0.35f, z - 0.9f); glRotatef(busWheelRot, 0.0f, 0.0f, 1.0f); glutSolidTorus(0.10f, 0.45f, 12, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(x + 2.2f, 0.35f, z - 0.9f); glRotatef(busWheelRot, 0.0f, 0.0f, 1.0f); glutSolidTorus(0.10f, 0.45f, 12, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(x - 2.6f, 0.35f, z + 1.0f); glRotatef(busWheelRot, 0.0f, 0.0f, 1.0f); glutSolidTorus(0.10f, 0.45f, 12, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(x + 2.2f, 0.35f, z + 1.0f); glRotatef(busWheelRot, 0.0f, 0.0f, 1.0f); glutSolidTorus(0.10f, 0.45f, 12, 12); glPopMatrix();

    setColor(1.0f, 1.0f, 0.8f);
    drawBox(x + dir * 4.05f, 0.5f, z - 0.7f, 0.12f, 0.25f, 0.4f);
    drawBox(x + dir * 4.05f, 0.5f, z + 0.4f, 0.12f, 0.25f, 0.4f);
}

void drawTaxi(float x, float z, float wheelRot, bool facesPositiveX = true) {
    setColor(0.95f, 0.85f, 0.1f);
    drawBox(x - 1.6f, 0.3f, z - 0.9f, 3.2f, 0.7f, 1.6f);
    setColor(0.95f, 0.85f, 0.1f);
    drawBox(x - 1.1f, 0.9f, z - 0.7f, 2.2f, 0.5f, 1.2f);

    drawWindowPanel(x - 0.9f, 1.0f, z + 0.51f, 0.6f, 0.35f);
    drawWindowPanel(x + 0.1f, 1.0f, z + 0.51f, 0.6f, 0.35f);
    drawWindowPanel(x - 0.9f, 1.0f, z - 0.51f, 0.6f, 0.35f);
    drawWindowPanel(x + 0.1f, 1.0f, z - 0.51f, 0.6f, 0.35f);

    setColor(0.95f, 0.95f, 0.95f);
    drawBox(x - 0.25f, 1.45f, z - 0.2f, 0.5f, 0.12f, 0.5f);

    setColor(0.06f, 0.06f, 0.06f);
    glPushMatrix(); glTranslatef(x - 1.2f, 0.35f, z - 0.95f); glRotatef(wheelRot, 0.0f, 0.0f, 1.0f); glutSolidTorus(0.06f, 0.24f, 10, 10); glPopMatrix();
    glPushMatrix(); glTranslatef(x + 1.1f, 0.35f, z - 0.95f); glRotatef(wheelRot, 0.0f, 0.0f, 1.0f); glutSolidTorus(0.06f, 0.24f, 10, 10); glPopMatrix();
    glPushMatrix(); glTranslatef(x - 1.2f, 0.35f, z + 0.75f); glRotatef(wheelRot, 0.0f, 0.0f, 1.0f); glutSolidTorus(0.06f, 0.24f, 10, 10); glPopMatrix();
    glPushMatrix(); glTranslatef(x + 1.1f, 0.35f, z + 0.75f); glRotatef(wheelRot, 0.0f, 0.0f, 1.0f); glutSolidTorus(0.06f, 0.24f, 10, 10); glPopMatrix();
}

void drawFuelStation(float x, float z) {
    setColor(0.12f, 0.18f, 0.28f);
    drawBox(x - 6.5f, 3.2f, z - 2.0f, 13.0f, 0.6f, 8.0f);
    setColor(0.95f, 0.1f, 0.08f);
    drawBox(x - 6.6f, 3.55f, z + 5.8f, 13.2f, 1.0f, 0.22f);
    drawText3D(x - 5.45f, 3.88f, z + 6.1f, "FUEL STATION", 0.0075f, 1.0f, 1.0f, 1.0f);

    setColor(0.72f, 0.72f, 0.72f);
    drawBox(x - 4.8f, 0.0f, z - 1.5f, 0.5f, 3.2f, 0.5f);
    drawBox(x + 4.3f, 0.0f, z - 1.5f, 0.5f, 3.2f, 0.5f);
    drawBox(x - 4.8f, 0.0f, z + 4.5f, 0.5f, 3.2f, 0.5f);
    drawBox(x + 4.3f, 0.0f, z + 4.5f, 0.5f, 3.2f, 0.5f);

    setColor(0.9f, 0.12f, 0.12f);
    drawBox(x - 2.0f, 0.05f, z + 1.0f, 0.6f, 1.1f, 0.7f);
    drawBox(x + 1.5f, 0.05f, z + 1.0f, 0.6f, 1.1f, 0.7f);

    float storeX = x - 4.0f;
    float storeZ = z - 11.0f;
    setColor(0.85f, 0.88f, 0.92f);
    drawBox(storeX, 0.0f, storeZ, 8.5f, 3.2f, 5.0f);

    setColor(0.2f, 0.55f, 0.75f);
    drawWindowPanel(storeX + 0.8f, 0.8f, storeZ + 5.01f, 2.2f, 1.6f);
    drawWindowPanel(storeX + 5.5f, 0.8f, storeZ + 5.01f, 2.2f, 1.6f);
    setColor(0.1f, 0.1f, 0.12f);
    drawBox(storeX + 3.7f, 0.0f, storeZ + 5.01f, 1.3f, 2.3f, 0.02f);

    drawCar(x - 3.5f, z + 2.5f, 0.1f, 0.6f, 0.2f, 0.0f, true);
    drawCar(x + 3.0f, z + 2.5f, 0.8f, 0.4f, 0.1f, 0.0f, true);

    setColor(0.2f, 0.2f, 0.22f);
    drawBox(x - 5.0f, 0.015f, z + 6.0f, 10.0f, 0.01f, 15.0f);
}

void drawPedestrian(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.05f, z);
    setColor(0.12f, 0.15f, 0.45f);
    drawBox(-0.12f, 0.0f, -0.1f, 0.08f, 0.6f, 0.2f);
    drawBox(0.04f, 0.0f, -0.1f, 0.08f, 0.6f, 0.2f);
    setColor(0.92f, 0.35f, 0.15f);
    drawBox(-0.2f, 0.6f, -0.15f, 0.4f, 0.8f, 0.3f);
    setColor(0.95f, 0.78f, 0.62f);
    glTranslatef(0.0f, 1.5f, 0.0f);
    glutSolidSphere(0.16f, 8, 8);
    glPopMatrix();
}

void drawKidsPlayground(float x, float z, float width, float depth) {
    setColor(0.25f, 0.65f, 0.25f);
    drawBox(x, 0.02f, z, width, 0.08f, depth);

    setColor(0.8f, 0.5f, 0.2f);
    for (float fx = x; fx <= x + width; fx += 2.0f) {
        drawBox(fx, 0.1f, z, 0.1f, 0.8f, 0.1f);
        drawBox(fx, 0.1f, z + depth - 0.1f, 0.1f, 0.8f, 0.1f);
    }

    glPushMatrix();
    glTranslatef(x + 3.0f, 0.1f, z + 4.0f);
    setColor(0.7f, 0.7f, 0.7f);
    drawBox(0.0f, 0.0f, 0.0f, 0.1f, 1.8f, 0.8f);
    setColor(1.0f, 0.3f, 0.3f);
    drawBox(0.0f, 1.8f, 0.0f, 0.8f, 0.1f, 0.8f);
    setColor(0.2f, 0.4f, 0.9f);
    glPushMatrix();
    glTranslatef(0.8f, 1.8f, 0.1f);
    glRotatef(30.0f, 0.0f, 0.0f, -1.0f);
    drawBox(0.0f, -0.1f, 0.0f, 2.5f, 0.1f, 0.6f);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x + 8.0f, 0.1f, z + 5.0f);
    setColor(0.2f, 0.2f, 0.2f);
    drawBox(0.0f, 0.0f, 0.0f, 0.1f, 2.2f, 0.1f);
    drawBox(3.0f, 0.0f, 0.0f, 0.1f, 2.2f, 0.1f);
    drawBox(0.0f, 2.2f, 0.0f, 3.1f, 0.1f, 0.1f);
    setColor(0.8f, 0.8f, 0.8f);
    drawBox(0.8f, 0.4f, 0.05f, 0.03f, 1.8f, 0.03f);
    drawBox(2.2f, 0.4f, 0.05f, 0.03f, 1.8f, 0.03f);
    setColor(0.9f, 0.1f, 0.1f);
    drawBox(0.7f, 0.4f, -0.1f, 1.6f, 0.08f, 0.4f);
    glPopMatrix();
}

void drawCityPark(float x, float z, float width, float depth) {
    setColor(0.22f, 0.52f, 0.2f);
    drawBox(x, 0.02f, z, width, 0.08f, depth);

    setColor(0.28f, 0.16f, 0.08f);
    for(float fx = x; fx <= x + width; fx += 1.8f) {
        drawBox(fx, 0.1f, z, 0.08f, 0.7f, 0.08f);
        drawBox(fx, 0.1f, z + depth - 0.1f, 0.08f, 0.7f, 0.08f);
    }

    setColor(0.48f, 0.28f, 0.12f);
    drawBox(x + width * 0.25f, 0.22f, z + depth * 0.4f, 2.0f, 0.25f, 0.7f);
    drawBox(x + width * 0.65f, 0.22f, z + depth * 0.55f, 2.0f, 0.25f, 0.7f);

    setColor(0.12f, 0.42f, 0.12f);
    glPushMatrix(); glTranslatef(x + width * 0.15f, 0.1f, z + depth * 0.25f); drawCylinder(0.14f, 0.14f, 1.4f, 8); glTranslatef(0.0f, 1.4f, 0.0f); glutSolidSphere(0.85f, 8, 8); glPopMatrix();
    glPushMatrix(); glTranslatef(x + width * 0.82f, 0.1f, z + depth * 0.72f); drawCylinder(0.14f, 0.14f, 1.4f, 8); glTranslatef(0.0f, 1.4f, 0.0f); glutSolidSphere(1.0f, 8, 8); glPopMatrix();
}

void drawBuilding(float x, float z, float width, float depth, float height, float r, float g, float b) {
    setColor(r, g, b);
    drawBox(x, 0.0f, z, width, height, depth);
    for (float h = 2.0f; h < height - 2.0f; h += 3.0f) {
        for (float w = 1.0f; w < width - 1.0f; w += 2.0f) {
            drawWindowPanel(x + w, h, z + depth + 0.01f, 1.0f, 1.5f);
        }
    }
}

void drawHospital(float x, float z) {
    setColor(0.9f, 0.92f, 0.95f);
    drawBox(x, 0.0f, z, 16.0f, 6.5f, 9.0f);
    setColor(0.1f, 0.45f, 0.35f);
    drawBox(x, 6.5f, z, 16.0f, 0.4f, 9.0f);
    setColor(0.2f, 0.2f, 0.25f);
    drawBox(x + 6.5f, 0.0f, z + 9.02f, 3.0f, 3.2f, 0.05f);
    setColor(0.6f, 0.8f, 0.95f);
    drawWindowPanel(x + 6.8f, 1.0f, z + 9.09f, 1.0f, 1.8f);
    drawWindowPanel(x + 8.2f, 1.0f, z + 9.09f, 1.0f, 1.8f);

    for (float wx = 1.0f; wx < 15.0f; wx += 2.6f) {
        if (wx > 5.0f && wx < 9.0f) continue;
        drawWindowPanel(x + wx, 1.5f, z + 9.01f, 1.6f, 1.8f);
        drawWindowPanel(x + wx, 4.2f, z + 9.01f, 1.6f, 1.5f);
    }
    setColor(0.85f, 0.1f, 0.1f);
    drawBox(x + 7.5f, 4.2f, z + 9.05f, 1.0f, 2.0f, 0.1f);
    drawBox(x + 7.0f, 4.7f, z + 9.05f, 2.0f, 1.0f, 0.1f);
    drawText3D(x + 4.2f, 7.2f, z + 9.01f, "HOSPITAL", 0.0075f, 0.85f, 0.1f, 0.1f);
}

void drawCarParking(float x, float z, float width, float depth) {
    setColor(0.25f, 0.25f, 0.27f);
    drawBox(x, 0.02f, z, width, 0.05f, depth);

    setColor(0.95f, 0.95f, 0.95f);
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (float pz = z + 2.0f; pz <= z + depth - 2.0f; pz += 4.0f) {
        glVertex3f(x + 0.5f, 0.08f, pz); glVertex3f(x + width - 0.5f, 0.08f, pz);
    }
    glEnd();
    glEnable(GL_LIGHTING);

    setColor(0.9f, 0.8f, 0.1f);
    drawBox(x, 0.08f, z, width, 0.1f, 0.3f);
    drawBox(x, 0.08f, z + depth - 0.3f, width, 0.1f, 0.3f);

    drawCar(x + 3.0f, z + 3.5f, 0.9f, 0.1f, 0.1f, 0.0f, false);
    drawCar(x + 8.0f, z + 7.5f, 0.1f, 0.4f, 0.8f, 0.0f, false);
    drawCar(x + 4.0f, z + 11.5f, 0.8f, 0.8f, 0.8f, 0.0f, true);
}

void drawBirds() {
    glDisable(GL_LIGHTING);
    setColor(0.08f, 0.08f, 0.1f);

    float birdOffsetsX[3] = {-5.0f, 0.0f, 4.0f};
    float birdOffsetsZ[3] = {-20.0f, -24.0f, -18.0f};
    float birdHeights[3] = {15.0f, 16.5f, 14.8f};

    float wingSway = sin(birdWingTimer) * 25.0f;

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glRotatef(birdFlyAngle, 0.0f, 1.0f, 0.0f);
        glTranslatef(birdOffsetsX[i], birdHeights[i], birdOffsetsZ[i]);

        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 0.4f);
        glVertex3f(-0.15f, 0.0f, -0.4f);
        glVertex3f(0.15f, 0.0f, -0.4f);
        glEnd();

        glPushMatrix();
        glRotatef(wingSway, 0.0f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 0.1f);
        glVertex3f(-0.8f, 0.1f, 0.0f);
        glVertex3f(0.0f, 0.0f, -0.2f);
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glRotatef(-wingSway, 0.0f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 0.1f);
        glVertex3f(0.8f, 0.1f, 0.0f);
        glVertex3f(0.0f, 0.0f, -0.2f);
        glEnd();
        glPopMatrix();

        glPopMatrix();
    }
    glEnable(GL_LIGHTING);
}

void drawTrafficLight(float x, float z) {
    setColor(0.08f, 0.08f, 0.08f);
    drawBox(x, 0.0f, z, 0.28f, 4.2f, 0.28f);
    setColor(0.12f, 0.12f, 0.12f);
    drawBox(x + 0.3f, 3.6f, z - 0.15f, 7.2f, 0.18f, 0.18f);

    float hx = x + 6.5f;
    float hy = 3.1f;
    float hz = z - 0.25f;
    setColor(0.08f, 0.08f, 0.08f);
    drawBox(hx - 0.25f, hy + 0.15f, hz - 0.15f, 0.6f, 0.9f, 0.5f);

    glDisable(GL_LIGHTING);
    if (trafficLightTimer >= 3.0f) setColor(1.0f, 0.2f, 0.2f); else setColor(0.12f, 0.12f, 0.12f);
    drawBox(hx - 0.18f, hy + 0.6f, hz, 0.18f, 0.18f, 0.18f);
    if (trafficLightTimer >= 2.5f && trafficLightTimer < 3.0f) setColor(1.0f, 0.75f, 0.12f); else setColor(0.12f, 0.12f, 0.12f);
    drawBox(hx - 0.18f, hy + 0.3f, hz, 0.18f, 0.18f, 0.18f);
    if (trafficLightTimer < 3.0f) setColor(0.0f, 1.0f, 0.0f); else setColor(0.12f, 0.12f, 0.12f);
    drawBox(hx - 0.18f, hy, hz, 0.18f, 0.18f, 0.18f);
    glEnable(GL_LIGHTING);
}

void drawSEUBuilding(float x, float z) {
    setColor(0.75f, 0.05f, 0.05f);
    drawBox(x, 0.0f, z, 14.0f, 16.0f, 10.0f);

    for(float y = 2.0f; y < 14.0f; y += 3.0f) {
        for(float w = 1.0f; w < 13.0f; w += 2.5f) {
            drawWindowPanel(x + w, y, z + 10.01f, 1.0f, 1.5f);
        }
    }
    setColor(0.15f, 0.15f, 0.15f);
    drawBox(x + 5.5f, 0.0f, z + 10.01f, 3.0f, 4.0f, 0.05f);
    drawText3D(x + 0.5f, 16.5f, z + 10.01f, "SOUTHEAST UNIVERSITY", 0.006f, 1.0f, 1.0f, 1.0f);
}

void drawSchool(float x, float z) {
    setColor(0.9f, 0.85f, 0.6f);
    drawBox(x, 0.0f, z, 10.0f, 5.0f, 8.0f);
    setColor(0.65f, 0.25f, 0.25f);
    drawBox(x - 0.5f, 5.0f, z - 0.5f, 11.0f, 1.2f, 9.0f);
    setColor(0.4f, 0.25f, 0.15f);
    drawBox(x + 4.0f, 0.0f, z + 8.01f, 2.0f, 2.2f, 0.05f);

    for (float wx = 1.0f; wx < 9.0f; wx += 2.5f) {
        if (wx > 3.0f && wx < 6.0f) continue;
        drawWindowPanel(x + wx, 1.5f, z + 8.01f, 1.5f, 2.0f);
    }
    drawText3D(x + 3.0f, 3.8f, z + 8.05f, "SCHOOL", 0.005f, 0.1f, 0.1f, 0.1f);
}

void drawEnvironmentGrid() {
    setColor(0.14f, 0.14f, 0.15f);
    drawBox(-45.0f, 0.0f, -45.0f, 90.0f, 0.01f, 90.0f);

    setColor(0.2f, 0.2f, 0.22f);
    drawBox(-45.0f, 0.01f, -5.0f, 90.0f, 0.01f, 10.0f);
    drawBox(-5.0f, 0.01f, -45.0f, 10.0f, 0.01f, 90.0f);

    setColor(0.9f, 0.9f, 0.95f);
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (float i = -42.0f; i <= 42.0f; i += 6.0f) {
        if (abs(i) < 4.0f) continue;
        glVertex3f(i, 0.02f, 0.0f); glVertex3f(i + 3.0f, 0.02f, 0.0f);
        glVertex3f(0.0f, 0.02f, i); glVertex3f(0.0f, 0.02f, i + 3.0f);
    }
    glEnd();
    glEnable(GL_LIGHTING);

    setColor(0.28f, 0.28f, 0.32f);
    drawBox(-45.0f, 0.02f, -6.4f, 90.0f, 0.14f, 1.4f);
    drawBox(-45.0f, 0.02f, 5.0f, 90.0f, 0.14f, 1.4f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -cameraDistance);
    glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);

    float sunPosition[] = { 45.0f, 75.0f, 30.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, sunPosition);

    drawEnvironmentGrid();
    drawRoadFences();

    drawPedestrian(ped1Pos, 5.4f);
    drawPedestrian(ped2Pos, -5.8f);
    drawPedestrian(ped1Pos + 10.0f, 5.6f);

    // Green Parks & Kids Playground
    drawCityPark(-24.0f, 8.0f, 16.0f, 14.0f);
    drawCityPark(12.0f, 26.0f, 18.0f, 14.0f);
    drawKidsPlayground(10.0f, -22.0f, 14.0f, 11.0f);

    // Supershop (In the left foreground area)
    drawSupershop(-40.0f, 12.0f);

    // NEW: Police Station Added at the foreground empty space (Right side relative to center road)
    drawPoliceStation(28.0f, -42.0f);

    // Fuel Station
    drawFuelStation(-14.0f, 25.0f);

    // SEU Building
    drawSEUBuilding(25.0f, 10.0f);

    // Shopping Mall
    drawShoppingMall(-40.0f, -22.0f);

    // Background Buildings
    drawBuilding(-42.0f, -42.0f, 12.0f, 12.0f, 26.0f, 0.72f, 0.34f, 0.26f);
    drawBuilding(-24.0f, -42.0f, 11.0f, 11.0f, 32.0f, 0.32f, 0.44f, 0.56f);

    // Small Hospital
    drawHospital(6.0f, 10.0f);

    // Car Parking Zone
    drawCarParking(10.0f, -42.0f, 13.0f, 16.0f);

    drawSchool(32.0f, -22.0f);

    drawBirds();

    drawTrafficLight(-9.0f, -9.0f);
    drawTrafficLight(8.0f, -9.0f);
    drawTrafficLight(-9.0f, 8.0f);
    drawTrafficLight(8.0f, 8.0f);

    drawCar(car1Pos, -2.4f, 0.15f, 0.36f, 0.72f, car1WheelRot, true);
    drawCar(car2Pos, 2.4f, 0.82f, 0.18f, 0.18f, car2WheelRot, false);
    drawCar(car3Pos, -2.5f, 0.22f, 0.62f, 0.28f, car3WheelRot, true);
    drawCar(car4Pos, 2.2f, 0.44f, 0.44f, 0.48f, car4WheelRot, false);
    drawCar(car5Pos, -2.2f, 0.72f, 0.48f, 0.12f, car5WheelRot, true);
    drawBus(busPos, -2.3f, true);
    drawTaxi(taxiPos, 2.5f, taxiWheelRot, false);

    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
    leafSway += 0.035f;
    trafficLightTimer += 0.025f;
    if (trafficLightTimer > 6.0f) trafficLightTimer = 0.0f;

    birdWingTimer += 0.4f;
    birdFlyAngle += 0.5f;
    if (birdFlyAngle > 360.0f) birdFlyAngle -= 360.0f;

    bool carsCanMoveEast = (trafficLightTimer < 3.0f);

    if (carsCanMoveEast || car1Pos < -12.0f || car1Pos > -7.0f) { car1Pos += 0.18f; car1WheelRot -= 14.0f; }
    if (carsCanMoveEast || busPos < -15.0f || busPos > -6.0f) { busPos += 0.14f; busWheelRot -= 10.0f; }
    if (carsCanMoveEast || car3Pos < -12.0f || car3Pos > -7.0f) { car3Pos += 0.22f; car3WheelRot -= 16.0f; }
    if (carsCanMoveEast || car5Pos < -12.0f || car5Pos > -7.0f) { car5Pos += 0.16f; car5WheelRot -= 12.0f; }

    car2Pos -= 0.15f; car2WheelRot += 11.0f;
    car4Pos -= 0.20f; car4WheelRot += 15.0f;
    taxiPos -= 0.24f; taxiWheelRot += 18.0f;

    ped1Pos += 0.04f; if (ped1Pos > 45.0f) ped1Pos = -45.0f;
    ped2Pos -= 0.03f; if (ped2Pos < -45.0f) ped2Pos = 45.0f;

    if (car1Pos > 45.0f) car1Pos = -45.0f;
    if (car2Pos < -45.0f) car2Pos = 45.0f;
    if (car3Pos > 45.0f) car3Pos = -45.0f;
    if (car4Pos < -45.0f) car4Pos = 45.0f;
    if (car5Pos > 45.0f) car5Pos = -45.0f;
    if (busPos > 45.0f) busPos = -45.0f;
    if (taxiPos < -45.0f) taxiPos = 45.0f;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
    if (key == 'd' || key == 'D' || key == 'n' || key == 'N') {
        isDay = !isDay;
        if (isDay) glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
        else glClearColor(0.05f, 0.08f, 0.18f, 1.0f);
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) { mouseDragging = true; lastMouseX = x; lastMouseY = y; }
        else { mouseDragging = false; }
    }
    if (button == 3) { cameraDistance -= 2.0f; if (cameraDistance < 5.0f) cameraDistance = 5.0f; glutPostRedisplay(); }
    if (button == 4) { cameraDistance += 2.0f; glutPostRedisplay(); }
}

void motion(int x, int y) {
    if (!mouseDragging) return;
    int dx = x - lastMouseX; int dy = y - lastMouseY;
    cameraAngleY += dx * 0.5f; cameraAngleX += dy * 0.5f;
    if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
    if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
    lastMouseX = x; lastMouseY = y;
    glutPostRedisplay();
}

void init() {
    srand((unsigned int)time(NULL));
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float daylightAmbient[] = { 0.32f, 0.36f, 0.46f, 1.0f };
    float daylightDiffuse[] = { 1.0f, 0.98f, 0.92f, 1.0f };
    float daylightSpecular[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, daylightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, daylightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, daylightSpecular);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1100, 750);
    glutCreateWindow("Realistic Urban City Daylight Walkways Simulation");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}
