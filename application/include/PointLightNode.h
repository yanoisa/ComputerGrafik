#ifndef POINTLIGHTNODE_HPP
#define POINTLIGHTNODE_HPP

#include "Node.h"
#include <glm/glm.hpp>

class PointLightNode : public Node
{
public:
    PointLightNode(const std::string& name, Node* parent = nullptr);

    void setLightColor(const glm::vec3& color);
    glm::vec3 getLightColor() const;

    void setLightIntensity(float intensity);
    float getLightIntensity() const;

private:
    glm::vec3 lightColor_;
    float lightIntensity_;
};

#endif
