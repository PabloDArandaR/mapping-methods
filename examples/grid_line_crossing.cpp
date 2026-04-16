#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>
#include "argparse/argparse.hpp"
#include "spdlog/spdlog.h"

#include "graphics/glfw.hpp"
#include "math/grid.hpp"
#include "types.hpp"

using namespace mm;
using namespace mm::viz;
using namespace mm::grid;

struct lineArgs : public argparse::Args {
    std::optional<std::vector<int>>& config =
        kwarg("c,config",
              "Info about the points and grid to plot. Format will depend on the mode (default is: start.x start.y "
              "end.x end.y rows cols)")
            .multi_argument();
    bool& click = flag("d,click", "Click mode active. Config: rows, cols");
    bool& debug = flag("t,test", "Activate debug mode");
};

void colorCells(std::list<Cell> cells, int rows, int cols, int width, int height) {
    glColor3f(1.0f, 0.0f, 0.0f);

    double horizontalSize{double(width) / cols}, verticalSize{double(height) / rows};
    for (Cell cell : cells) {
        drawRectangle(cell.x * horizontalSize, cell.y * verticalSize, horizontalSize, verticalSize, 1.0f, 0.0f, 0.0f);
    }
}

int cli_click_points(std::vector<int> grid_data, bool debug) {

    class clickAppData {
      public:
        clickAppData() {
            this->first = nullptr;
            this->second = nullptr;
            this->recalculate = false;
        }

        static void click_callback(GLFWwindow* window, int button, int action, int mods) {
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
                clickAppData* data = reinterpret_cast<clickAppData*>(glfwGetWindowUserPointer(window));
                if (data->first == nullptr) {
                    double xpos, ypos;
                    glfwGetCursorPos(window, &xpos, &ypos);
                    data->first = new Point(xpos, ypos);
                } else if (data->second == nullptr) {
                    double xpos, ypos;
                    glfwGetCursorPos(window, &xpos, &ypos);
                    data->second = new Point(xpos, ypos);
                } else {
                    data->first = data->second;
                    double xpos, ypos;
                    glfwGetCursorPos(window, &xpos, &ypos);
                    data->second = new Point(xpos, ypos);
                }
                data->recalculate = true;
            }
        }

        Point* getFirst() { return this->first; }
        Point* getSecond() { return this->second; }
        bool getRecalculate() { return this->recalculate; }

        void setRecalculate(bool new_recalculate) { this->recalculate = new_recalculate; }

      private:
        Point* first;
        Point* second;
        bool recalculate;
    };

    GLFWwindow* window;
    int width{1000}, height{1000};

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(width, height, "Line crossing grid", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    clickAppData data;
    glfwSetWindowUserPointer(window, &data);
    glfwSetMouseButtonCallback(window, clickAppData::click_callback);

    std::list<Cell> cell_list;
    int rows{(int)grid_data[0]}, cols{(int)grid_data[1]};

    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        double horizontalSize{double(width) / cols}, verticalSize{double(height) / rows};

        if ((data.getFirst() != nullptr) && (data.getSecond() != nullptr)) {
            if (data.getRecalculate()) {
                spdlog::info("--------------------------------New set of points---------------------------------");
                Point starting_point(data.getFirst()->x / horizontalSize, data.getFirst()->y / verticalSize);
                Point ending_point(data.getSecond()->x / horizontalSize, data.getSecond()->y / verticalSize);
                spdlog::info("Starting point is {}, {}", starting_point.x, starting_point.y);
                spdlog::info("Ending point is {}, {}", ending_point.x, ending_point.y);
                cell_list = crossing_line_cells(starting_point, ending_point);
                data.setRecalculate(false);
            }
            colorCells(cell_list, rows, cols, width, height);
            if (debug) {
                Point starting_point(data.getFirst()->x / horizontalSize, data.getFirst()->y / verticalSize);
                Point ending_point(data.getSecond()->x / horizontalSize, data.getSecond()->y / verticalSize);
                cell_list = crossing_line_cells(starting_point, ending_point);
            }

            drawPoint(data.getFirst()->x, data.getFirst()->y, 3, 10, 0.0, 1.0, 0.0);
            drawPoint(data.getSecond()->x, data.getSecond()->y, 3, 10, 0.0, 1.0, 0.0);
            drawLine(data.getFirst()->x, data.getFirst()->y, data.getSecond()->x, data.getSecond()->y, 0, 0, 1);
        }

        drawGrid(rows, cols, width, height);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

int cli_points_execution(std::vector<int> config) {

    GLFWwindow* window;
    int width{1000}, height{1000};

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(width, height, "Line crossing grid", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    Point starting_point(config[0], config[1]), end_point(config[2], config[3]);
    int rows{(int)config[4]}, cols{(int)config[5]};

    bool setup_cells{true};
    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        double horizontalSize{double(width) / cols}, verticalSize{double(height) / rows};

        std::list<Cell> cell_list = crossing_line_cells(starting_point, end_point);
        colorCells(cell_list, rows, cols, width, height);

        drawGrid(rows, cols, width, height);
        drawPoint(starting_point.x * horizontalSize, starting_point.y * verticalSize, 3, 10, 0.0, 1.0, 0.0);
        drawPoint(end_point.x * horizontalSize, end_point.y * verticalSize, 3, 10, 0.0, 1.0, 0.0);
        drawLine(starting_point.x * horizontalSize, starting_point.y * verticalSize, end_point.x * horizontalSize,
                 end_point.y * verticalSize, 0, 0, 1);

        if (setup_cells) {
            for (Cell cell : cell_list) {
                spdlog::debug("Cell found is: {}, {}", cell.x, cell.y);
            }
            setup_cells = false;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

int main(int argc, char** argv) {
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Test evaluation");

    auto args = argparse::parse<lineArgs>(argc, argv);

    if (args.config.has_value()) {
        if (args.config.value().size() == 6) {
            return cli_points_execution(args.config.value());
        } else if (&args.click) {
            spdlog::info("Starting clicking mode");
            if (args.config.value().size() == 2) {
                return cli_click_points(args.config.value(), args.debug);
            } else {
                spdlog::error("Unhandled case. Invalid amount of config parameters.");
            }
        }
    } else {
        spdlog::warn("No valid CLI selection");
    };
    return 0;
}
