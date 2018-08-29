#include "Application.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <unordered_map>

void Application::loadModel(const char* path)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path))
    {
        throw std::runtime_error(err);
    }

    std::unordered_map<Vertex, int> uniqueVertices = {};

    glm::vec3 barycenter;

    for (auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex = {};
            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 2], // model in z up
                attrib.vertices[3 * index.vertex_index + 1],
            };
            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1],
            };
            vertex.color = { 1.0f, 1.0f, 1.0f };


            if (!uniqueVertices.count(vertex))
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                barycenter += vertex.pos;
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }

    barycenter /= (float)uniqueVertices.size();

    for (auto& vertice : vertices)
    {
        vertice.pos -= barycenter;
    }
}

void Application::loadTexture(const char* path)
{
    texture = stbi_load(path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

}

void Application::releaseTexture()
{
    stbi_image_free(static_cast<stbi_uc*>(texture));
}

void Application::run()
{
    initWindow();
    initResources();
    mainLoop();
    cleanup();
}

void Application::retrieveWindowSize()
{
    int width = 0, height = 0;
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }
    windowWidth = width;
    windowHeight = height;
}

void Application::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

void Application::mouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
{
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));

    if (!app->mousePressed)
    {
        app->cursorX = xPos;
        app->cursorY = yPos;
        return;
    }

    double deltaY = (xPos - app->cursorX) * 0.01;
    double deltaX = (yPos - app->cursorY) * 0.01;

    app->camera.xAngle = app->xAngleOnPress + (float)deltaX;
    app->camera.yAngle = app->yAngleOnPress + (float)deltaY;
}

void Application::mousePressCallback(GLFWwindow* window, int button, int action, int mods)
{
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            app->mousePressed = true;
            app->xAngleOnPress = app->camera.xAngle;
            app->yAngleOnPress = app->camera.yAngle;
        }
        else
        {
            app->mousePressed = false;
        }
    }
}

void Application::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));

    auto& dist = app->camera.dist;

    dist -= (float)yoffset * 0.3f;
    dist -= (float)xoffset * 0.3f;
    dist = glm::clamp(dist, 0.5f, 10.f);
}

std::vector<char> Application::readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("failed to open " + filename);
    }

    auto fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}
