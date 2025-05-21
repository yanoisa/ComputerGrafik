#include "Node.h"
#include <algorithm>

Node::Node() :
    parent_(nullptr),
    name_(""),
    path_(""),
    depth_(0),
    localTransform_(1.0f),
    worldTransform_(1.0f) {}

Node::Node(const std::string& name, Node* parent) :
    parent_(parent),
    name_(name),
    path_(""),
    depth_(0),
    localTransform_(1.0f),
    worldTransform_(1.0f) {
    if (parent_) {
        parent_->addChildren(this);
        depth_ = parent_->getDepth() + 1;
        path_ = parent_->getPath() + "/" + name_;
    }
    else {
        path_ = name_;
    }
}

//deleting also with all the childs
Node::~Node() {
    for (auto child : children_) {
        delete child;
    }
}

Node* Node::getParent() { 
    return parent_; 
}

//The update is also if we would rearange some nodes so we have also to remove the child for the other parents.
void Node::setParent(Node* parent) {
    if (parent_) {
        parent_->removeChild(name_);
    }
    parent_ = parent;
    if (parent_) {
        depth_ = parent_->getDepth() + 1;
        path_ = parent_->getPath() + "/" + name_;
        parent_->addChildren(this);
    }
    else {
        depth_ = 0;
        path_ = name_;
    }
}

Node* Node::getChildren(const std::string& name) {
    for (auto child : children_) {
        if (child->getName() == name) {
            return child;
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

std::string Node::getPath() { 
    return path_; 
}

int Node::getDepth() { 
    return depth_; 
}

glm::mat4 Node::getLocalTransform() { 
    return localTransform_; 
}

//because the world transformation is the world transformation AND the local Transformation.
glm::mat4 Node::getWorldTransform() {
    if (parent_) {
        return parent_->getWorldTransform() * localTransform_;
    }
    return localTransform_;
}


void Node::setLocalTransform(glm::mat4 localTransform) {
    localTransform_ = localTransform;
}
//here we look also for the parents WorldTransform because that is for the child important too
void Node::setWorldTransform(glm::mat4& globalTransform) {
    if (parent_) {
        localTransform_ = glm::inverse(parent_->getWorldTransform()) * globalTransform;
    }
    else {
        localTransform_ = globalTransform;
    }
}

void Node::addChildren(Node* child) {
    if (child && !getChildren(child->getName())) {
        children_.push_back(child);
        child->parent_ = this;
        child->depth_ = depth_ + 1;
        child->path_ = path_ + "/" + child->getName();
    }
}

Node* Node::removeChild(const std::string& name) {
    auto it = std::find_if(children_.begin(), children_.end(),
        [name](Node* node) { return node->getName() == name; });

    if (it != children_.end()) {
        Node* child = *it;
        children_.erase(it);
        child->parent_ = nullptr;
        child->depth_ = 0;
        child->path_ = child->getName();
        return child;
    }
    return nullptr;
}