#include "GeometryNode.h"

// Konstruktor-Implementierung
GeometryNode::GeometryNode(const std::string& name, Node* parent)
    : Node(name, parent), geometry_(nullptr) {}

// Setzt das zugeh�rige Modell
void GeometryNode::setGeometry(model_object* geometry) {
    geometry_ = geometry;
}

// Gibt das Modell zur�ck
model const* getmodel() const{
    return geometry_;
}
