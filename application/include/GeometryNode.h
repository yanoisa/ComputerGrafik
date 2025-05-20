#ifndef GEOMETRYNODE_HPP
#define GEOMETRYNODE_HPP

#include "Node.h"
#include <glm/glm.hpp>
#include "model_loader.hpp"
#include "model.hpp"

class GeometryNode : public Node
{
public:
    GeometryNode(const std::string& name, Node* parent = nullptr, bool isPerspective = true);

    void setGeometry(model* geometry);
    model getGeometry();

private:
    model geometry_;
};

#endif
