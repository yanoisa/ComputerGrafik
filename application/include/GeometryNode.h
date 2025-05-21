#ifndef GEOMETRYNODE_HPP
#define GEOMETRYNODE_HPP

#include "Node.h"
#include <glm/glm.hpp>
#include "model_loader.hpp"
#include "model.hpp"

class GeometryNode : public Node
{
public:
    GeometryNode(const std::string& name, Node* parent);

    void setmodel(model const* geometry);
    model const* getmodel() const;

private:
    model const* geometry_;
};

#endif
