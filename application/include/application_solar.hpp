#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include "SceneGraph.h"

// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();

  // react to key input
  void keyCallback(int key, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);
  //handle resizing
  void resizeCallback(unsigned width, unsigned height);

  // draw all objects
  void render() const;
  void renderNode(Node* node, glm::mat4 parent_transform) const;
  glm::vec3 getPlanetColor(const std::string& name) const;
  void renderStars() const;

 protected:
  SceneGraph scenegraph_;
  void initializeShaderPrograms();
  void initializeGeometry();
  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();

  // cpu representation of model
  model_object planet_object;
  // cpu representation of stars
  model_object star_object;
  
  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;
  //They are used for mouse handling
  double lastMouseX_ = 0.0, lastMouseY_ = 0.0;
  bool firstMouse_ = true;
  float yaw_ = -90.0f;  // Horizontal angle, -90 to face forward in OpenGL
  float pitch_ = 0.0f;  // Vertical angle
};

#endif