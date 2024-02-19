#include "types.hpp"
#include "math/grid.hpp"
#include <stdio.h>
#include <iostream>

#include <GLFW/glfw3.h>

void drawRectangle(float x, float y, float width, float height, float red, float green, float blue){
    glColor3f(red, green, blue);

    glBegin(GL_QUADS);
    glVertex2f(x, y);           // Bottom-left vertex
    glVertex2f(x + width, y);   // Bottom-right vertex
    glVertex2f(x + width, y + height);  // Top-right vertex
    glVertex2f(x, y + height);  // Top-left vertex
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

void colorCells(std::list<Cell> cells, int rows, int cols, int width, int height){
    glColor3f(1.0f, 0.0f, 0.0f);

    float horizontalSize {float(width)/cols}, verticalSize {float(height)/rows};
    for (Cell cell: cells){
        drawRectangle(cell.x*horizontalSize, cell.y*verticalSize, horizontalSize, verticalSize, 1.0f, 0.0f, 0.0f);
    }
}

int main(void)
{
    GLFWwindow* window;
    int width {1000}, height{1000};

    /* Initialize the library */
    if (!glfwInit()){
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    int cols {10}, rows {10};
    
    bool setup_cells {true};
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Set the viewport size
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Set the coordinate system
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Update sizes according to current window size
        float horizontalSize {float(width)/cols}, verticalSize {float(height)/rows};

        // Find occupied cells
        Point starting_point(5.2, 5.3), end_point(2.5f, 2.1f);
        std::list<Cell> cell_list = grid::crossing_line_cells(starting_point, end_point);
        colorCells(cell_list, 10, 10, width, height);
        if (setup_cells){
            for (Cell cell: cell_list){
                std::cout << "Cell found is: " << cell.x << ", " << cell.y << std::endl;
            }

            setup_cells = false;
        }

        // Draw the grid
        drawGrid(rows, cols, width, height); // Draw a 10x10 grid with cell size of 50 pixels

        // Draw starting and ending point
        drawPoint(starting_point.x*horizontalSize, starting_point.y*verticalSize, 10, 10, 0.0, 1.0, 0.0);
        drawPoint(end_point.x*horizontalSize, end_point.y*verticalSize, 10, 10, 0.0, 1.0, 0.0);
        drawLine(starting_point.x*horizontalSize, starting_point.y*verticalSize, end_point.x*horizontalSize, end_point.y*verticalSize, 0, 0, 1);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}