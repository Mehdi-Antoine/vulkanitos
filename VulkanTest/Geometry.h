#ifndef Geometry_h__
#define Geometry_h__

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

struct Matrices
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct TrackBallCamera
{
    glm::vec3 target = { 0.f, 0.f, 0.f };
    float dist = 100.f;
    float yAngle = 0.f;
    float xAngle = 0.f;
    float verticalFOV = 45.f;
    float near = 0.1f;
    float far = 1000.f;

    glm::mat4 computeViewMatrix();
};

struct Vertex
{
    glm::vec3 pos;
    float a;
    glm::vec3 color;
    float b;
    glm::vec2 texCoord;
    glm::vec2 c;

    bool operator==(const Vertex& other) const;
};

namespace std {
template<>
struct hash<Vertex>
{
    size_t operator()(Vertex const & vertex) const
    {
        return ((hash<glm::vec3>()(vertex.pos) ^
            (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                 (hash<glm::vec2>()(vertex.texCoord) << 1);
    }
};
}

#endif // Geometry_h__
