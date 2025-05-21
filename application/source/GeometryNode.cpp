#include "GeometryNode.h"
//I am not quite sure if this is correct because the secenegraph is not functioning
GeometryNode::GeometryNode(const std::string& name, Node* parent)
    : Node(name, parent), geometry_(nullptr){}

void GeometryNode::setGeometry(model const* geometry) {
    geometry_ = geometry;
}

model const* GeometryNode::getGeometry() const {
    return geometry_;
}
