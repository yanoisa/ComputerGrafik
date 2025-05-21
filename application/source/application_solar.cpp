#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "SceneGraph.h"
#include "Node.h"
#include "GeometryNode.h"
#include "CameraNode.h"
#include "PointLightNode.h"

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  SceneGraph solarSystem("Solar System");
  scenegraph_ = solarSystem;
  
  Node* rootNode = new Node();
  solarSystem.setRoot(rootNode);

  //Camera 
  CameraNode* camera = new CameraNode("Camera", rootNode, true);

  Node* mercHold = new Node("mercHold", rootNode);

  GeometryNode* mercGeom = new GeometryNode("mercGeom", mercHold);
  Node* venHold = new Node("venHold", rootNode);
  GeometryNode* venGeom = new GeometryNode("venGeom", venHold);
  Node* earthHold = new Node("earthHold", rootNode);
  GeometryNode* earthGeom = new GeometryNode("earthGeom", earthHold);
  Node* marsHold = new Node("marsHold", rootNode);
  GeometryNode* marsGeom = new GeometryNode("marsGeom", marsHold);
  Node* jupitHold = new Node("jupitHold", rootNode);
  GeometryNode* jupitGeom = new GeometryNode("jupitGeom", jupitHold);
  Node* satHold = new Node("satHold", rootNode);
  GeometryNode* satGeom = new GeometryNode("satGeom", satHold);
  Node* uranHold = new Node("uranHold", rootNode);
  GeometryNode* uranGeom = new GeometryNode("uranGeom", uranHold);
  Node* neptHold = new Node("neptHold", rootNode);
  GeometryNode* neptGeom = new GeometryNode("neptGeom", neptHold);
  initializeGeometry();
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

void ApplicationSolar::render() const {
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);

  glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});
  model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, -1.0f});
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));

  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));

  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  // draw bound vertex array using bound shader
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
}

void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at("planet").handle);
  // upload uniform values to new locations
  uploadView();
  uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  planet_object.num_elements = GLsizei(planet_model.indices.size());
}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}


// exe entry point
int main(int argc, char* argv[]) {
  //Setup SceneGraph
  SceneGraph solarSystem("Solar System");
  Node* rootNode = new Node();
  solarSystem.setRoot(rootNode);

  //Camera 
  CameraNode* camera = new CameraNode("Camera", rootNode, true);

  Node* mercHold = new Node("mercHold", rootNode);
  
  GeometryNode* mercGeom = new GeometryNode("mercGeom",mercHold);
  Node* venHold = new Node("venHold", rootNode);
  GeometryNode* venGeom = new GeometryNode("venGeom", venHold);
  Node* earthHold = new Node("earthHold", rootNode);
  GeometryNode* earthGeom = new GeometryNode("earthGeom", earthHold);
  Node* marsHold = new Node("marsHold", rootNode);
  GeometryNode* marsGeom = new GeometryNode("marsGeom", marsHold);
  Node* jupitHold = new Node("jupitHold", rootNode);
  GeometryNode* jupitGeom = new GeometryNode("jupitGeom", jupitHold);
  Node* satHold = new Node("satHold", rootNode);
  GeometryNode* satGeom = new GeometryNode("satGeom", satHold);
  Node* uranHold = new Node("uranHold", rootNode);
  GeometryNode* uranGeom = new GeometryNode("uranGeom", uranHold);
  Node* neptHold = new Node("neptHold", rootNode);
  GeometryNode* neptGeom = new GeometryNode("neptGeom", neptHold);
  
  const std::list<Node*>& temp = rootNode->getChildrenList();
  for (Node* node : temp)
  {
      std::cout << node->getName() << "\n";
  }

  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}