#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include "argparse/argparse.hpp"
#include "spdlog/spdlog.h"
#include "types.hpp"
#include "math/grid.hpp"
#include "graphics/glfw.hpp"


struct lineArgs : public argparse::Args
{
    std::optional<std::vector<float>> &config = kwarg("c,config", "Info about the points and grid to plot. Format will depend on the mode (default is: start.x start.y end.x end.y rows cols)").multi_argument();
    bool &click = flag("d,click", "Click mode active. Config: rows, cols");
};

void colorCells(std::list<Cell> cells, int rows, int cols, int width, int height){
    glColor3f(1.0f, 0.0f, 0.0f);

    float horizontalSize {float(width)/cols}, verticalSize {float(height)/rows};
    for (Cell cell: cells){
        drawRectangle(cell.x*horizontalSize, cell.y*verticalSize, horizontalSize, verticalSize, 1.0f, 0.0f, 0.0f);
    }
}

int cli_click_points(std::vector<float> grid_data){

    class clickAppData{
        public:
            clickAppData(){
                this->first = nullptr;
                this->second = nullptr;
                this->recalculate = false;
            }

            static void click_callback(GLFWwindow* window, int button, int action, int mods){
                if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
                    clickAppData* data = reinterpret_cast<clickAppData*>(glfwGetWindowUserPointer(window));
                    if (data->first == nullptr){
                        double xpos, ypos;
                        glfwGetCursorPos(window, &xpos, &ypos);
                        data->first = new Point(xpos, ypos);
                    }
                    else if (data->second == nullptr)
                    {
                        double xpos, ypos;
                        glfwGetCursorPos(window, &xpos, &ypos);
                        data->second = new Point(xpos, ypos);
                    }
                    else {
                        data->first = data->second;
                        double xpos, ypos;
                        glfwGetCursorPos(window, &xpos, &ypos);
                        data->second = new Point(xpos, ypos);
                    }
                    data->recalculate = true;
                    }
            }

            Point* getFirst(){return this->first;}
            Point* getSecond(){return this->second;}
            bool getRecalculate(){return this->recalculate;}

            void setRecalculate(bool new_recalculate) {
                this->recalculate = new_recalculate;
            }

        private:
            Point* first;
            Point* second;
            bool recalculate;
};

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

    clickAppData data;
    glfwSetWindowUserPointer(window, &data);

    glfwSetMouseButtonCallback(window, clickAppData::click_callback);

    std::list<Cell> cell_list;

    int rows {(int)grid_data[0]}, cols {(int)grid_data[1]};

    spdlog::info("Gets before the while loop");
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
        if ((data.getFirst() != nullptr) && (data.getSecond() != nullptr)){
            if (data.getRecalculate()){
                spdlog::info("--------------------------------New iteration---------------------------------");
                Point starting_point (data.getFirst()->x / horizontalSize, data.getFirst()->y / verticalSize);
                Point ending_point (data.getSecond()->x / horizontalSize, data.getSecond()->y / verticalSize);
                spdlog::info("Starting point is {}, {}", starting_point.x, starting_point.y);
                spdlog::info("Ending point is {}, {}", ending_point.x, ending_point.y);
                cell_list = grid::crossing_line_cells(starting_point, ending_point);
                data.setRecalculate(false);
            }
            colorCells(cell_list, rows, cols, width, height);

            // Draw starting and ending point
            drawPoint(data.getFirst()->x, data.getFirst()->y, 3, 10, 0.0, 1.0, 0.0);
            drawPoint(data.getSecond()->x, data.getSecond()->y, 3, 10, 0.0, 1.0, 0.0);
            drawLine(data.getFirst()->x, data.getFirst()->y, data.getSecond()->x, data.getSecond()->y, 0, 0, 1);
        }
        
        // Draw the grid
        drawGrid(rows, cols, width, height); // Draw a 10x10 grid with cell size of 50 pixels

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

int cli_points_execution(std::vector<float> config){

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

    Point starting_point(config[0], config[1]), end_point(config[2], config[3]);
    int rows{(int)config[4]}, cols{(int)config[5]};
    
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
        glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Update sizes according to current window size
        float horizontalSize {float(width)/cols}, verticalSize {float(height)/rows};

        // Find occupied cells
        std::list<Cell> cell_list = grid::crossing_line_cells(starting_point, end_point);
        colorCells(cell_list, rows, cols, width, height);

        // Draw the grid
        drawGrid(rows, cols, width, height); // Draw a 10x10 grid with cell size of 50 pixels

        // Draw starting and ending point
        drawPoint(starting_point.x*horizontalSize, starting_point.y*verticalSize, 3, 10, 0.0, 1.0, 0.0);
        drawPoint(end_point.x*horizontalSize, end_point.y*verticalSize, 3, 10, 0.0, 1.0, 0.0);
        drawLine(starting_point.x*horizontalSize, starting_point.y*verticalSize, end_point.x*horizontalSize, end_point.y*verticalSize, 0, 0, 1);

        if (setup_cells){
            for (Cell cell: cell_list){
                spdlog::debug("Cell found is: {}, {}", cell.x, cell.y);
            }
            setup_cells = false;
        }

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

int main(int argc, char** argv)
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Test evaluation");
    
    auto args = argparse::parse<lineArgs>(argc, argv);

    if (args.config.has_value()){
        if (args.config.value().size() == 6){
            return cli_points_execution(args.config.value());
        }
        else if (&args.click){
            spdlog::info("Starting clicking mode");
            if (args.config.value().size() == 2){
                return cli_click_points(args.config.value());
            }
            else{
                spdlog::error("Unhandled case. Invalid amount of config parameters.");
            }
        }
    }
    else {
        spdlog::warn("No valid CLI selection");
    };
    return 0;
}
