#include "GeometryNode.h"

GeometryNode::GeometryNode(const std::string& name, Node* parent)
    : Node(name, parent), geometry_(nullptr)
{}

void GeometryNode::setmodel(model const* geometry) {
    geometry_ = geometry;
}

model const* GeometryNode::getmodel() const {
    return geometry_;
}
