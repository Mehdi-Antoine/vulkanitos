#include "Geometry.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 TrackBallCamera::computeViewMatrix()
{
    auto trans = glm::translate(glm::translate(glm::mat4(1.f), target), glm::vec3(0.f, 0.f, -dist));
    auto alphaRot = glm::rotate(glm::mat4(1.f), xAngle, glm::vec3(1.f, 0.f, 0.f));
    auto thetaRot = glm::rotate(glm::mat4(1.f), yAngle, glm::vec3(0.f, 1.f, 0.f));
    return trans * alphaRot * thetaRot;
}

bool Vertex::operator==(const Vertex& other) const
{
    return pos == other.pos
        && color == other.color
        && texCoord == other.texCoord;
}
