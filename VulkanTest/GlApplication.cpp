#include "GlApplication.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void GlApplication::initWindow()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        throw std::runtime_error("failed to initialize glfw");
    }
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    glfwWindowHint(GLFW_DECORATED, GL_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", nullptr, nullptr);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetMouseButtonCallback(window, mousePressCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        const char* str = (const char*) glewGetErrorString(err);
        throw std::runtime_error("failed to initialize glew: " + std::string(str));
    }

//     glfwSwapInterval(1);

    GLenum glerr = GL_NO_ERROR;
    glerr = glGetError();
}

void GlApplication::createTexture()
{
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    checkError("createTexture");
}

void compileShader(GLuint shader, const char* buffer, size_t bufferSize)
{
    GLint length = bufferSize;
    glShaderSource(shader, 1, &buffer, &length);
    glCompileShader(shader);

    int logSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

    if (logSize > 1)
    {
        std::vector<char> log(logSize);
        glGetShaderInfoLog(shader, logSize, &logSize, log.data());
        std::cout << buffer << std::endl;
        std::cout << "Compile: " << log.data() << std::endl;
    }

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        throw std::runtime_error("failed to compile shader");
    }
}

void linkProgram(GLuint program, GLuint vertexShader, GLuint fragmentShader)
{
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int logSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

    if (logSize > 1)
    {
        std::vector<char> log(logSize);
        glGetProgramInfoLog(program, logSize, &logSize, log.data());
        std::cout << "Link: " << log.data() << std::endl;
    }

    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        throw std::runtime_error("failed to link program");
    }
}

void GlApplication::createProgram()
{
    auto vertexShaderCode = readFile("shaders/gl/shader.vert");
    auto fragmentShaderCode = readFile("shaders/gl/shader.frag");

    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    compileShader(vertexShaderId, vertexShaderCode.data(), vertexShaderCode.size());
    compileShader(fragmentShaderId, fragmentShaderCode.data(), fragmentShaderCode.size());

    programId = glCreateProgram();

    linkProgram(programId, vertexShaderId, fragmentShaderId);

    checkError("createProgram");
}

void GlApplication::createBuffers()
{
    glGenVertexArrays(1, &vao);

    glGenBuffers(2, vbos.data());

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[BufferUsage_Indice]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[BufferUsage_Position]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    checkError("createBuffers");
}

void GlApplication::createUniforms()
{
    modelLocation = glGetUniformLocation(programId, "model");
    viewLocation = glGetUniformLocation(programId, "view");
    projLocation = glGetUniformLocation(programId, "proj");
    texLocation = glGetUniformLocation(programId, "tex");
    glProgramUniform1i(programId, texLocation, 0);

    checkError("createUniforms");
}

void GlApplication::initResources()
{
    createTexture();
    createProgram();
    createBuffers();
    createUniforms();
}

void GlApplication::drawFrame()
{
    if (framebufferResized)
    {
        retrieveWindowSize();
        glViewport(0, 0, windowWidth, windowHeight);
        framebufferResized = false;
    }
    

    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glUseProgram(programId);

    glm::mat4 model(1.f);
    glm::mat4 view = camera.computeViewMatrix();
    glm::mat4 proj = glm::perspective(glm::radians(camera.verticalFOV), windowWidth / (float)windowHeight, camera.near, camera.far);

    glProgramUniformMatrix4fv(programId, modelLocation, 1, 0, glm::value_ptr(model));
    glProgramUniformMatrix4fv(programId, viewLocation, 1, 0, glm::value_ptr(view));
    glProgramUniformMatrix4fv(programId, projLocation, 1, 0, glm::value_ptr(proj));

    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

    glfwSwapBuffers(window);

    checkError("drawFrame");
}

void GlApplication::mainLoop()
{
    glViewport(0, 0, windowWidth, windowHeight);

    double total = 0.0;
    int frameCount = 0;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        auto begin = glfwGetTime();
        drawFrame();
        auto end = glfwGetTime();
        total += (end - begin);
        frameCount++;
    }

    double avgFrame = total / frameCount;

    std::cout << "avg frame time (ms): " << avgFrame * 1000.0 << std::endl;
    std::cout << "avg framerate (fps): " << 1.0 / avgFrame << std::endl;
}

void GlApplication::cleanup()
{
    // cleanup gl
    glDeleteTextures(1, &textureId);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(vbos.size(), vbos.data());
    glDeleteProgram(programId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    checkError("cleanup");

    // cleanup window
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GlApplication::checkError(const std::string& section)
{
    int error = glGetError();
    if (error == GL_NO_ERROR)
    {
        return;
    }

    std::string errorStr;
    switch (error)
    {
    case GL_INVALID_ENUM:
        errorStr = "GL_INVALID_ENUM";
        break;
    case GL_INVALID_VALUE:
        errorStr = "GL_INVALID_VALUE";
        break;
    case GL_INVALID_OPERATION:
        errorStr = "GL_INVALID_OPERATION";
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        errorStr = "GL_INVALID_FRAMEBUFFER_OPERATION";
        break;
    case GL_OUT_OF_MEMORY:
        errorStr = "GL_OUT_OF_MEMORY";
        break;
    default:
        errorStr = "UNKNOWN";
        break;
    }

    throw std::runtime_error("gl error (at " + section + ") : " + errorStr);
}


