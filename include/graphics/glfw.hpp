#include <GLFW/glfw3.h>

namespace mm {
namespace viz {

void drawPoint(double x, double y, double radius, int segments, double red, double green, double blue);
void drawLine(double x_initial, double y_initial, double x_final, double y_final, double red, double green, double blue);
void drawRectangle(double x, double y, double width, double height, double red, double green, double blue);
void drawGrid(int rows, int cols, int width, int height);

} // namespace viz
} // namespace mm
