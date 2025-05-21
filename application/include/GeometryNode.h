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

    void setGeometry(model const* geometry);
    model const* getGeometry() const;

private:
    //the model is to use at the end the sphere.obj it is the same class used in application solar
    model const* geometry_;
};

#endif
