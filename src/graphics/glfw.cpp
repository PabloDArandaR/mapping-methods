#include <iostream>
#include <vector>

#include "graphics/glfw.hpp"
#include <GLFW/glfw3.h>

#define _USE_MATH_DEFINES
#include <cmath>

namespace mm {
namespace viz {

void drawRectangle(double x, double y, double width, double height, double red, double green, double blue) {
    glColor3f(red, green, blue);

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawPoint(double x, double y, double radius, int segments, double red, double green, double blue) {
    glColor3f(red, green, blue);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i{0}; i <= segments; ++i) {
        double theta = 2.0f * M_PI * double(i) / double(segments);
        double dx = radius * cosf(theta);
        double dy = radius * sinf(theta);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void drawLine(double x_initial, double y_initial, double x_final, double y_final, double red, double green,
              double blue) {
    glColor3f(red, green, blue);

    glBegin(GL_LINES);
    glVertex2f(x_initial, y_initial);
    glVertex2f(x_final, y_final);
    glEnd();
}

void drawGrid(int rows, int cols, int width, int height) {
    glColor3f(0.5f, 0.5f, 0.5f);

    double horizontalSize{double(width) / cols}, verticalSize{double(height) / rows};

    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (int i = 0; i <= rows; ++i) {
        glVertex2f(0.0f, i * verticalSize);
        glVertex2f(cols * horizontalSize, i * verticalSize);
    }
    glEnd();

    glBegin(GL_LINES);
    for (int i = 0; i <= cols; ++i) {
        glVertex2f(i * horizontalSize, 0.0f);
        glVertex2f(i * horizontalSize, rows * verticalSize);
    }
    glEnd();
}

} // namespace viz
} // namespace mm
