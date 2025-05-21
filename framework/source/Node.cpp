#include "Node.h"
#include <glm/glm.hpp>

Node::Node():name_("root"),parent_(nullptr),path_("\root"), depth_(0), localTransform_(1.0f), worldTransform_(1.0f) {}
Node::Node(const std::string& name, Node* parent)
    : name_(name),
    parent_(parent),
    path_(""),                
    depth_(0),                
    localTransform_(1.0f),    
    worldTransform_(1.0f)     
{
    if (parent) {
        parent->addChildren(this);
        depth_ = parent->getDepth() + 1;
        path_ = parent->getPath() + "/" + name;
    }
    else {
        path_ = "/" + name;
    }
}


Node* Node::getParent() {
    return parent_;
}
void Node::setParent(Node* parent) {
    this->parent_ = parent;
    if (parent != nullptr) {
        parent->addChildren(this);
    }
}

Node* Node::getChildren(const std::string& name) {
    for (auto it = children_.begin(); it != children_.end(); ++it) {
        if ((*it)->getName() == name) {
            Node* searchedChild = *it;
            return searchedChild;
        }
    }
    return nullptr;
}
const std::list<Node*>& Node::getChildrenList() const {
    return children_;
}


std::string Node::getName() {
    return name_;
}
std::string Node::getPath(){
    return path_;
}
int Node::getDepth() {
    return depth_;
}
glm::mat4 Node::getLocalTransform() {
    return localTransform_;
}
glm::mat4 Node::getWorldTransform() {
    return worldTransform_;
}
void Node::setLocalTransform(glm::mat4& localTransform) {
    this->localTransform_ = localTransform;
}
void Node::setWorldTransform(glm::mat4& globalTransform) {
    this->worldTransform_ = worldTransform_;
}
void Node::addChildren(Node* child) {
    Node::children_.push_back(child);
}
Node* Node::removeChild(const std::string& name) {
    for (auto it = children_.begin(); it != children_.end(); ++it) {
        if ((*it)->getName() == name) {
            Node* removedChild = *it;
            children_.erase(it);
            return removedChild;
        }
    }
    return nullptr;
}

Node::~Node() {
    // Cleanup if needed (currently nothing to delete)
}