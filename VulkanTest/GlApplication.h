#ifndef GlApplication_h__
#define GlApplication_h__

#include "Application.h"
#include <GL/glew.h>
#include <array>

class GlApplication : public Application
{
protected:
    void initWindow() override;
    void initResources() override;
    void mainLoop() override;
    void cleanup() override;

    void checkError(const std::string& section);

    void createTexture();

    void createProgram();
    void createBuffers();
    void createUniforms();
    void drawFrame();

protected:
    GLuint textureId;

    GLuint vertexShaderId;
    GLuint fragmentShaderId;
    GLuint programId;

    GLuint modelLocation;
    GLuint viewLocation;
    GLuint projLocation;
    GLuint texLocation;

    GLuint vao;
    enum BufferUsage : unsigned int
    {
        BufferUsage_Position = 0,
        BufferUsage_Indice
    };
    std::array<GLuint, 2> vbos;
};

#endif // GlApplication_h__
