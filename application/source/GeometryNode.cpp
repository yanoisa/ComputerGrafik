#include "GeometryNode.h"

// Constructor: passes name and parent to base Node constructor
GeometryNode::GeometryNode(const std::string& name, Node* parent, bool isPerspective)
    : Node(name, parent)
    , geometry_() // initialize empty geometry
{
    // Optionally handle isPerspective here, if needed in future
}

// Setter: store pointer to model
void GeometryNode::setGeometry(model* geometry) {
    if (geometry) {
        geometry_ = *geometry;  // copy contents of pointed-to model
    }
}

// Getter: return copy of stored model
model GeometryNode::getGeometry() {
    return geometry_;
}
