#include "SceneGraph.h"
#include "Node.h"

SceneGraph::SceneGraph() :name_("SceneGraph"), root_(nullptr) {}
SceneGraph::SceneGraph(const std::string& name, Node* root) : name_(name), root_(root){}
std::string Scenegraph::getName() {
	return name_;
}
void SceneGraph::setName(std::string name) {
	this-> name_ = name;
}
Node* SceneGraph::getRoot() {
	return root_;
}
void SceneGraph::setRoot(Node* root) {
	this->root_ = root;
}