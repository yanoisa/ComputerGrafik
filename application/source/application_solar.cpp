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
 ,lastMouseX_{0.0}
 ,lastMouseY_{0.0}
 ,firstMouse_{true}
{
  SceneGraph solarSystem("Solar System");
  scenegraph_ = solarSystem;
  
  Node* rootNode = new Node();
  rootNode->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(0.0f, 0.0f, 0.0f)));
  solarSystem.setRoot(rootNode);
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);
  //LightSource
  PointLightNode* sunLight = new PointLightNode("SunLight", rootNode);
  sunLight->setLightColor(glm::vec3(1.0, 1.0, 0.8)); // Yellowish light
  sunLight->setLightIntensity(10.0f);
  sunLight->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(0.0f, 0.0f, 0.0f)));
  //Camera 
  CameraNode* camera = new CameraNode("Camera", rootNode, true);


  //for the scaling factors was a mixture from actual relationships and what look nice
  Node* mercHold = new Node("mercHold", rootNode);
  GeometryNode* mercGeom = new GeometryNode("mercGeom", mercHold);
  mercHold->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(0.38f)));
  mercGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(5.0f, 0.0f, 0.0f)));
  Node* venHold = new Node("venHold", rootNode);
  GeometryNode* venGeom = new GeometryNode("venGeom", venHold);
  venHold->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(0.95f)));
  venGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(7.0f, 0.0f, 0.0f)));
  Node* earthHold = new Node("earthHold", rootNode);
  GeometryNode* earthGeom = new GeometryNode("earthGeom", earthHold);
  earthHold->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(1.0f)));
  earthGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(9.0f, 0.0f, 0.0f)));
  Node* moonHold = new Node("moonHold", earthGeom);
  GeometryNode* moonGeom = new GeometryNode("Moon", moonHold);
  Node* marsHold = new Node("marsHold", rootNode);
  GeometryNode* marsGeom = new GeometryNode("marsGeom", marsHold);
  marsHold->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(0.53f)));
  marsGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(11.0f, 0.0f, 0.0f)));
  Node* jupitHold = new Node("jupitHold", rootNode);
  GeometryNode* jupitGeom = new GeometryNode("jupitGeom", jupitHold);
  jupitHold->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(11.0f)));
  jupitGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(13.0f, 0.0f, 0.0f)));
  Node* satHold = new Node("satHold", rootNode);
  GeometryNode* satGeom = new GeometryNode("satGeom", satHold);
  satHold->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(9.1f)));
  satGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(15.0f, 0.0f, 0.0f)));
  Node* uranHold = new Node("uranHold", rootNode);
  GeometryNode* uranGeom = new GeometryNode("uranGeom", uranHold);
  uranHold->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(4.0f)));
  uranGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(17.0f, 0.0f, 0.0f)));
  Node* neptHold = new Node("neptHold", rootNode);
  GeometryNode* neptGeom = new GeometryNode("neptGeom", neptHold);
  neptHold->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(3.9f)));
  neptGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(19.0f, 0.0f, 0.0f)));


  mercGeom->setGeometry(&planet_model);
  venGeom->setGeometry(&planet_model);
  earthGeom->setGeometry(&planet_model);
  marsGeom->setGeometry(&planet_model);
  jupitGeom->setGeometry(&planet_model);
  satGeom->setGeometry(&planet_model);
  uranGeom->setGeometry(&planet_model);
  neptGeom->setGeometry(&planet_model);
  moonGeom->setGeometry(&planet_model);

  mercHold->setRotationSpeed(1.0f);
  venHold->setRotationSpeed(0.9f);
  earthHold->setRotationSpeed(0.8f);
  marsHold->setRotationSpeed(0.7f);
  jupitHold->setRotationSpeed(0.6);
  satGeom->setRotationSpeed(0.5);
  uranGeom->setRotationSpeed(0.4);
  neptGeom->setRotationSpeed(0.3);

  scenegraph_ = solarSystem;

  initializeGeometry();
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}
/*
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
}*/

void ApplicationSolar::render() const {
    glUseProgram(m_shaders.at("planet").handle);

    // Render manually animated planet
    /*glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});
    model_matrix = glm::translate(model_matrix, glm::fvec3{ 0.0f, 0.0f, -1.0f });
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
        1, GL_FALSE, glm::value_ptr(model_matrix));

    glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
        1, GL_FALSE, glm::value_ptr(normal_matrix));

    glBindVertexArray(planet_object.vertex_AO);
    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
    */
    
    renderNode(scenegraph_.getRoot(), glm::mat4(1.0f)); // Render with identity parent transform
    
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
  Node* root = scenegraph_.getRoot();
  PointLightNode* sunLight = nullptr;
  sunLight = dynamic_cast<PointLightNode*>(root->getChildren("SunLight"));
  
  //Extracting the values for the point light
  glm::mat4 sunWorldTransform = sunLight->getWorldTransform();
  glm::vec3 light_color = sunLight->getLightColor();
  float light_intensity = sunLight->getLightIntensity();
  glm::vec3 light_position = glm::vec3(sunWorldTransform[3]);  // translation column

  glUniform3fv(glGetUniformLocation(m_shaders.at("planet").handle, "LightPosition"), 1, glm::value_ptr(light_position));
  glUniform3fv(glGetUniformLocation(m_shaders.at("planet").handle, "LightColor"), 1, glm::value_ptr(light_color));
  GLint loc_intensity = glGetUniformLocation(m_shaders.at("planet").handle, "LightIntensity");
  glUniform1f(loc_intensity, light_intensity);
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
//the steps are much faster with many frames than a low framerate
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadView();
  }
  // The other four are only copy and paste and a different key and a different row of the Matrix
  else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      m_view_transform = glm::translate(m_view_transform, glm::fvec3{ -0.1f, 0.0f, 0.0f });
      uploadView();
  }
  else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      m_view_transform = glm::translate(m_view_transform, glm::fvec3{ 0.1f, 0.0f, 0.0f });
      uploadView();
  }
  else if (key == GLFW_KEY_Y && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      m_view_transform = glm::translate(m_view_transform, glm::fvec3{ 0.0f, 0.1f, 0.0f });
      uploadView();
  }
  else if (key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      m_view_transform = glm::translate(m_view_transform, glm::fvec3{ 0.0f, -0.1f, 0.0f });
      uploadView();
  }
}

//handle delta mouse movement input
//it rotates quite extreme. So therefore if the spehre is not visible anymore it is a good idea to 
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {

    if (firstMouse_) {
        lastMouseX_ = pos_x;
        lastMouseY_ = pos_y;
        firstMouse_ = false;
    }
    //calculating of the difference of the mouse 
    float deltax = pos_x - lastMouseX_;
    float deltay = lastMouseY_ - pos_y;
    lastMouseX_ = pos_x;
    lastMouseY_ = pos_y;
    //sensitivity otherwise the rotation is to much
    const float sensitivity = 0.01f;
    deltax *= sensitivity;
    deltay *= sensitivity;

    glm::vec3 right = glm::vec3(m_view_transform[0]);
    glm::vec3 up = glm::vec3(m_view_transform[1]);

    m_view_transform = glm::rotate(m_view_transform, glm::radians(deltax), up);
    m_view_transform = glm::rotate(m_view_transform, glm::radians(deltay), right);

    uploadView();
}


//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}

void ApplicationSolar::renderNode(Node* node, glm::mat4 parent_transform) const {
    glm::mat4 local_transform = node->getLocalTransform();

    float speed = node->getRotationSpeed();
    if (speed != 0.0f) {
        float angle = glfwGetTime() * speed;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
        local_transform = rotation * local_transform;
    }

    glm::mat4 current_transform = parent_transform * local_transform;

    if (auto geo_node = dynamic_cast<GeometryNode*>(node)) {
        glUseProgram(m_shaders.at("planet").handle);

        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
            1, GL_FALSE, glm::value_ptr(current_transform));

        glm::mat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * current_transform);
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
            1, GL_FALSE, glm::value_ptr(normal_matrix));

        glBindVertexArray(planet_object.vertex_AO);
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, nullptr);
    }

    for (Node* child : node->getChildrenList()) {
        renderNode(child, current_transform);
    }
}

glm::vec3 ApplicationSolar::getPlanetColor(const std::string& name) const {
    if (name == "mercGeom") return glm::vec3(0.5f, 0.5f, 0.5f);      // Mercury - gray
    if (name == "venGeom") return glm::vec3(1.0f, 0.8f, 0.5f);      // Venus - yellowish
    if (name == "earthGeom") return glm::vec3(0.2f, 0.4f, 1.0f);    // Earth - blue
    if (name == "marsGeom") return glm::vec3(1.0f, 0.3f, 0.2f);     // Mars - reddish
    if (name == "jupitGeom") return glm::vec3(1.0f, 0.9f, 0.7f);    // Jupiter - beige
    if (name == "satGeom") return glm::vec3(1.0f, 0.9f, 0.5f);      // Saturn - pale yellow
    if (name == "uranGeom") return glm::vec3(0.5f, 0.8f, 1.0f);     // Uranus - light blue
    if (name == "neptGeom") return glm::vec3(0.3f, 0.5f, 1.0f);     // Neptune - blue
    return glm::vec3(1.0f, 1.0f, 1.0f);                             // default white
}






// exe entry point
int main(int argc, char* argv[]) {
  //Setup SceneGraph
    Node* root = new Node();
    root->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(0.0f, 0.0f, 0.0f)));

    Node* child = new Node("Child", root);
    child->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(0.0f, 3.0f, 0.0f)));

    glm::vec3 root_pos = glm::vec3(root->getWorldTransform()[3]);   // (5, 0, 0)
    glm::vec3 child_pos = glm::vec3(child->getWorldTransform()[3]); // (5, 3, 0)

    std::cout << "Light Position: ("
        << root_pos.x << ", "
        << root_pos.y << ", "
        << root_pos.z << ")\n";

    std::cout << "Light Position: ("
        << child_pos.x << ", "
        << child_pos.y << ", "
        << child_pos.z << ")\n";
  

  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}