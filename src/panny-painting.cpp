#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

using namespace std;

// Structure to hold our drawing coordinates
struct Point {
    float x, y;
};

// Global storage for our brush strokes
vector<vector<Point>> strokes;
bool isDrawing = false;

// Convert screen coordinates (pixels) to OpenGL coordinates (-1 to 1)
void addPoint(GLFWwindow* window, double xpos, double ypos) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    
    Point p;
    p.x = (float)((xpos / width) * 2.0 - 1.0);
    p.y = (float)(1.0 - (ypos / height) * 2.0);
    strokes.back().push_back(p);
}

// Mouse Button Callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isDrawing = true;
            strokes.push_back(vector<Point>()); // Start a new line
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            addPoint(window, x, y);
        } else if (action == GLFW_RELEASE) {
            isDrawing = false;
        }
    }
}

// Mouse Movement Callback
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (isDrawing) {
        addPoint(window, xpos, ypos);
    }
}

int main() {
    // 1. Initialize the Library
    if (!glfwInit()) return -1;

    // 2. Create the Window
    GLFWwindow* window = glfwCreateWindow(1000, 800, "CT-TOOL-05: PANNY_PAINT_GL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // 3. Main Rendering Loop
    while (!glfwWindowShouldClose(window)) {
        // Clear Screen (Dark CodingTrolling Theme)
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 4. Draw every stroke stored in memory
        glColor3f(0.0f, 1.0f, 0.5f); // Neon Green Panny Color
        glLineWidth(3.0f);

        for (const auto& stroke : strokes) {
            glBegin(GL_LINE_STRIP);
            for (const auto& p : stroke) {
                glVertex2f(p.x, p.y);
            }
            glEnd();
        }

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
