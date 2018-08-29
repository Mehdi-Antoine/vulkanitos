#ifndef Application_h__
#define Application_h__

#include "Geometry.h"

#include <vector>

struct GLFWwindow;

class Application
{
public:
    void loadModel(const char* path);
    void loadTexture(const char* path);
    void releaseTexture();

    void run();

    void retrieveWindowSize();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
    static void mousePressCallback(GLFWwindow* window, int button, int action, int mods);
    static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

protected:
    virtual void initWindow() = 0;
    virtual void initResources() = 0;
    virtual void mainLoop() = 0;
    virtual void cleanup() = 0;

    static std::vector<char> readFile(const std::string& filename);

protected:
    std::vector<Vertex> vertices;
    std::vector<int> indices;

    void* texture = nullptr;
    int texWidth, texHeight, texChannels;

    TrackBallCamera camera;

    // window properties
    int windowWidth = 800;
    int windowHeight = 600;

    GLFWwindow* window;

    bool framebufferResized = false;
    float xAngleOnPress = 0;
    float yAngleOnPress = 0;
    double cursorX = 0.0;
    double cursorY = 0.0;
    bool mousePressed = false;
};

#endif // Application_h__
