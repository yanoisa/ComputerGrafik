#ifndef NODE_HPP
#define NODE_HPP

#include <list>
#include <string>
#include <glm/glm.hpp>

class Node
{
public:
    Node();
    Node(const std::string& name, Node* parent);
    virtual ~Node(); // WICHTIG für Polymorphismus

    Node* getParent();
    void setParent(Node* parent);

    Node* getChildren(const std::string& name);
    const std::list<Node*>& getChildrenList() const;

    std::string getName();
    std::string getPath();
    int getDepth();

    glm::mat4 getLocalTransform();
    glm::mat4 getWorldTransform();
    void setLocalTransform(glm::mat4 localTransform);
    void setWorldTransform(glm::mat4& globalTransform);

    void addChildren(Node* child);
    Node* removeChild(const std::string& name);
    void setRotationSpeed(float speed);
    float getRotationSpeed();

private:
    //the _ for the member variables is that setter and getter are easier to construct with less confusion
    Node* parent_;
    std::string name_;
    std::list<Node*> children_;
    std::string path_;
    int depth_;
    glm::mat4 localTransform_;
    glm::mat4 worldTransform_;
    float rotationSpeed_ = 0.0f;
};
#endif