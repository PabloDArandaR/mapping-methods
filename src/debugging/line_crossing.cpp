#include <iostream>
#include <vector>
#include "argparse/argparse.hpp"
#include <GLFW/glfw3.h>
#include "types.hpp"
#include "math/grid.hpp"
#include "tools/glfw.hpp"


struct lineArgs : public argparse::Args
{
    std::vector<float> &config = kwarg("c,config", "Info about the points and grid to plot. Format is: start.x start.y end.x end.y rows cols");
};

void colorCells(std::list<Cell> cells, int rows, int cols, int width, int height){
    glColor3f(1.0f, 0.0f, 0.0f);

    float horizontalSize {float(width)/cols}, verticalSize {float(height)/rows};
    for (Cell cell: cells){
        drawRectangle(cell.x*horizontalSize, cell.y*verticalSize, horizontalSize, verticalSize, 1.0f, 0.0f, 0.0f);
    }
}

int main(int argc, char** argv)
{
    auto args = argparse::parse<lineArgs>(argc, argv);

    GLFWwindow* window;
    int width {1000}, height{1000};

    /* Initialize the library */
    if (!glfwInit()){
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Line crossing grid", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    Point starting_point(args.config[0], args.config[1]), end_point(args.config[2], args.config[3]);
    int rows{(int)args.config[4]}, cols{(int)args.config[5]};
    
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
