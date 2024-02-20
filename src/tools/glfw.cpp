#include <iostream>
#include <vector>
#include "tools/glfw.hpp"
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>


void drawRectangle(float x, float y, float width, float height, float red, float green, float blue){
    glColor3f(red, green, blue);

    glBegin(GL_QUADS);
    glVertex2f(x, y);           // Bottom-left vertex
    glVertex2f(x + width, y);   // Bottom-right vertex
    glVertex2f(x + width, y + height);  // Top-right vertex
    glVertex2f(x, y + height);  // Top-left vertex
    glEnd();

}

void drawPoint(float x, float y, float radius, int segments, float red, float green, float blue){
    glColor3f(red, green, blue);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i{0}; i <= segments ; ++i){
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void drawLine(float x_initial, float y_initial, float x_final, float y_final, float red, float green, float blue){
    glColor3f(red, green, blue);

    glBegin(GL_LINES);
    glVertex2f(x_initial, y_initial);
    glVertex2f(x_final, y_final);
    glEnd();
}


void drawGrid(int rows, int cols, int width, int height) {
    // Set color to gray
    glColor3f(0.5f, 0.5f, 0.5f);

    float horizontalSize {float(width)/cols}, verticalSize {float(height)/rows};

    // Draw horizontal lines
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (int i = 0; i <= rows; ++i) {
        glVertex2f(0.0f, i * verticalSize);
        glVertex2f(cols * horizontalSize, i * verticalSize);
    }
    glEnd();

    // Draw vertical lines
    glBegin(GL_LINES);
    for (int i = 0; i <= cols; ++i) {
        glVertex2f(i * horizontalSize, 0.0f);
        glVertex2f(i * horizontalSize, rows * verticalSize);
    }
    glEnd();
}