#include "CameraNode.h"

CameraNode::CameraNode(const std::string& name, Node* parent, bool isPerspective)
    : Node(name, parent), isPerspective_(isPerspective), isEnabled_(true), projectionMatrix_(1.0f)
{
}

bool CameraNode::getPerspective() {
    return isPerspective_;
}

bool CameraNode::getEnabled() {
    return isEnabled_;
}

void CameraNode::setEnabled(bool enabled) {
    isEnabled_ = enabled;
}

glm::mat4 CameraNode::getProjectionMatrix() {
    return projectionMatrix_;
}

void CameraNode::setProjectionMatrix(const glm::mat4& matrix) {
    projectionMatrix_ = matrix;
}
