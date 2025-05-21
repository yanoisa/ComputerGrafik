#include "SceneGraph.h"
#include "Node.h"
SceneGraph::SceneGraph():name_("root"), root_(nullptr) {};
SceneGraph::SceneGraph(const std::string& name) :name_(name), root_(nullptr) {}
SceneGraph::SceneGraph(const std::string& name, Node* root) : name_(name), root_(root){}
std::string SceneGraph::getName() {
	return name_;
}
void SceneGraph::setName(std::string name) {
	this-> name_ = name;
}
Node* SceneGraph::getRoot() const {
	return root_;
}
void SceneGraph::setRoot(Node* root) {
	this->root_ = root;
}
