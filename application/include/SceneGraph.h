#ifndef SCENEGRAPH_HPP
#define SCENEGRAPH_HPP

#include <string>
#include "Node.h"
class SceneGraph
{
public:
    SceneGraph(const std::string& name);          // Constructor
    SceneGraph(const std::string& name, Node* root);
    std::string getName();
    void setName(std::string name);
    Node* getRoot() const;
    void setRoot(Node* root);
private:
    std::string name_;
    Node* root_;
};
#endif
