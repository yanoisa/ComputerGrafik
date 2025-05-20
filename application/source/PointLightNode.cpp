#include "PointLightNode.hpp"

PointLightNode::PointLightNode(const std::string& name, Node* parent)
    : Node(name, parent), lightColor_(1.0f, 1.0f, 1.0f), lightIntensity_(1.0f)
{
}

void PointLightNode::setLightColor(const glm::vec3& color) {
    lightColor_ = color;
}

glm::vec3 PointLightNode::getLightColor() const {
    return lightColor_;
}

void PointLightNode::setLightIntensity(float intensity) {
    lightIntensity_ = intensity;
}

float PointLightNode::getLightIntensity() const {
    return lightIntensity_;
}
