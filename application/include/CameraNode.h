#ifndef CAMERANODE_HPP
#define CAMERANODE_HPP

#include "Node.h"
#include <glm/glm.hpp>

class CameraNode : public Node
{
public:
    CameraNode(const std::string& name, Node* parent = nullptr, bool isPerspective = true);

    bool getPerspective();
    bool getEnabled();
    void setEnabled(bool enabled);
    glm::mat4 getProjectionMatrix();
    void setProjectionMatrix(const glm::mat4& matrix);

private:
    bool isPerspective_;
    bool isEnabled_;
    glm::mat4 projectionMatrix_;
};

#endif
