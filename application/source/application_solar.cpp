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
  sunLight->setLightColor(glm::vec3(1.0, 1.0, 0.8)); // Yellow light
  sunLight->setLightIntensity(1.0f);
  sunLight->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(0.0f, 0.0f, 0.0f)));
  Node* sunHold = new Node("sunHold", rootNode);

  GeometryNode* sunGeom = new GeometryNode("SunGeom", sunHold); // Give it a unique name
  sunGeom->setGeometry(&planet_model);
  sunGeom->setLocalTransform(glm::scale(glm::mat4(1.0f), glm::vec3(3.0f)));
  sunGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(0.0f, 0.0f, 0.0f)));
  //Camera 
  CameraNode* camera = new CameraNode("Camera", rootNode, true);
  camera->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(10.0f, 0.0f, 20.0f)));

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
  earthGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(10.0f, 0.0f, 0.0f)));
  

  // The moon's 'holding' node should be a child of the moonOrbitNode
  Node* moonHold = new Node("moonHold", earthGeom);
  moonHold->setLocalTransform(glm::mat4(1.0f));
  moonHold->setRotationSpeed(2.0f);
  moonHold->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(0.6f))); // Moon's size relative to Earth
  GeometryNode* moonGeom = new GeometryNode("Moon", moonHold);
  moonGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(2.0f, 0.0f, 0.0f))); // Moon's distance from Earth
  
  Node* marsHold = new Node("marsHold", rootNode);
  GeometryNode* marsGeom = new GeometryNode("marsGeom", marsHold);
  marsHold->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(0.53f)));
  marsGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(13.0f, 0.0f, 0.0f)));
  Node* jupitHold = new Node("jupitHold", rootNode);
  GeometryNode* jupitGeom = new GeometryNode("jupitGeom", jupitHold);
  jupitHold->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(11.0f)));
  jupitGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(15.0f, 0.0f, 0.0f)));
  Node* satHold = new Node("satHold", rootNode);
  GeometryNode* satGeom = new GeometryNode("satGeom", satHold);
  satHold->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(9.1f)));
  satGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(17.0f, 0.0f, 0.0f)));
  Node* uranHold = new Node("uranHold", rootNode);
  GeometryNode* uranGeom = new GeometryNode("uranGeom", uranHold);
  uranHold->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(4.0f)));
  uranGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(19.0f, 0.0f, 0.0f)));
  Node* neptHold = new Node("neptHold", rootNode);
  GeometryNode* neptGeom = new GeometryNode("neptGeom", neptHold);
  neptHold->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(3.9f)));
  neptGeom->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(21.0f, 0.0f, 0.0f)));
  

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
  moonHold->setRotationSpeed(5.0);

  scenegraph_ = solarSystem;

  initializeGeometry();
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

void ApplicationSolar::render() const {
    glUseProgram(m_shaders.at("planet").handle);
    renderNode(scenegraph_.getRoot(), glm::mat4(1.0f)); // Render with identity parent transform
    
}


void ApplicationSolar::uploadView() {
    Node* camera = scenegraph_.getRoot()->getChildren("Camera");
  if (!camera) { // Safety check
      // Handle error: camera node not initialized
      // default view 
      glm::fmat4 default_view_matrix = glm::lookAt(glm::vec3(0, 0, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
      glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
          1, GL_FALSE, glm::value_ptr(default_view_matrix));
      return;
  }
  //view matrix is the inverse camera's world transform
  glm::fmat4 view_matrix = glm::inverse(camera->getWorldTransform());

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
    glUseProgram(m_shaders.at("planet").handle);

    uploadView();
    uploadProjection();

    Node* root = scenegraph_.getRoot();
    PointLightNode* sunLight = dynamic_cast<PointLightNode*>(root->getChildren("SunLight"));

    // Ensure sunLight is valid before dereferencing
    if (!sunLight) {
        std::cerr << "Error: SunLight node not found!" << std::endl;
        // default lightning
        glUniform3fv(glGetUniformLocation(m_shaders.at("planet").handle, "LightPosition"), 1, glm::value_ptr(glm::vec3(0.0f)));
        glUniform3fv(glGetUniformLocation(m_shaders.at("planet").handle, "LightColor"), 1, glm::value_ptr(glm::vec3(0.0f)));
        glUniform1f(glGetUniformLocation(m_shaders.at("planet").handle, "LightIntensity"), 0.0f);
        return;
    }

    glm::mat4 sunWorldTransform = sunLight->getWorldTransform();
    glm::vec3 light_color = sunLight->getLightColor();
    float light_intensity = sunLight->getLightIntensity();
    glm::vec3 light_position_world = glm::vec3(sunWorldTransform[3]);

    Node* camera_node = scenegraph_.getRoot()->getChildren("Camera");
    // Ensure camera_node is valid before dereferencing
    if (!camera_node) {
        std::cerr << "Error: Camera node not found!" << std::endl;
        // Use a default view matrix if camera is not found
        glm::mat4 default_view_matrix = glm::lookAt(glm::vec3(0, 0, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::vec3 light_position_view_space = glm::vec3(default_view_matrix * glm::vec4(light_position_world, 1.0f));
        glUniform3fv(glGetUniformLocation(m_shaders.at("planet").handle, "LightPosition"), 1, glm::value_ptr(light_position_view_space));
    }
    else {
        glm::mat4 view_matrix = glm::inverse(camera_node->getWorldTransform());
        glm::vec3 light_position_view_space = glm::vec3(view_matrix * glm::vec4(light_position_world, 1.0f));
        glUniform3fv(glGetUniformLocation(m_shaders.at("planet").handle, "LightPosition"), 1, glm::value_ptr(light_position_view_space));
    }

    glUniform3fv(glGetUniformLocation(m_shaders.at("planet").handle, "LightColor"), 1, glm::value_ptr(light_color));
    glUniform1f(glGetUniformLocation(m_shaders.at("planet").handle, "LightIntensity"), light_intensity);
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
    m_shaders.emplace("planet", shader_program{ {{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                                {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}} });
    m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
    m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
    m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
    m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
    m_shaders.at("planet").u_locs["LightPosition"] = -1; // Make sure these are also there
    m_shaders.at("planet").u_locs["LightColor"] = -1;
    m_shaders.at("planet").u_locs["LightIntensity"] = -1;
    m_shaders.at("planet").u_locs["PlanetColor"] = -1;
    m_shaders.at("planet").u_locs["EmissiveColor"] = -1; // <--- RE-ADD THIS
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
    Node* m_cameraNode_ = scenegraph_.getRoot()->getChildren("Camera");
    if (!m_cameraNode_) return; // Don't do anything if camera isn't set

    glm::mat4 current_cam_local_transform = m_cameraNode_->getLocalTransform();
    glm::vec3 translation_vector{ 0.0f };
    float move_speed = 0.5f; // Adjust as needed

    // Movement relative to world axes (as per original m_view_transform logic)
    // If you want movement relative to camera's orientation, logic would be different
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        // Move camera forward along its local Z-axis (standard FPS)
        // Get camera's forward vector (negative Z-axis of its transform)
        glm::vec3 forward = -glm::vec3(m_cameraNode_->getWorldTransform()[2]);
        current_cam_local_transform = glm::translate(current_cam_local_transform, glm::normalize(forward) * move_speed);
    }
    else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        // Move camera backward along its local Z-axis
        glm::vec3 forward = -glm::vec3(m_cameraNode_->getWorldTransform()[2]);
        current_cam_local_transform = glm::translate(current_cam_local_transform, -glm::normalize(forward) * move_speed);
    }
    else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        // Strafe camera left along its local X-axis
        glm::vec3 right = glm::vec3(m_cameraNode_->getWorldTransform()[0]);
        current_cam_local_transform = glm::translate(current_cam_local_transform, -glm::normalize(right) * move_speed);
    }
    else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        // Strafe camera right along its local X-axis
        glm::vec3 right = glm::vec3(m_cameraNode_->getWorldTransform()[0]);
        current_cam_local_transform = glm::translate(current_cam_local_transform, glm::normalize(right) * move_speed);
    }
    else if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)) { // Using Space for Up
        // Move camera up along world Y-axis (or camera's local Y if preferred)
        glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
        current_cam_local_transform = glm::translate(current_cam_local_transform, world_up * move_speed);
    }
    else if (key == GLFW_KEY_LEFT_CONTROL && (action == GLFW_PRESS || action == GLFW_REPEAT)) { // Using Left Ctrl for Down
        // Move camera down along world Y-axis
        glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
        current_cam_local_transform = glm::translate(current_cam_local_transform, -world_up * move_speed);
    }

    m_cameraNode_->setLocalTransform(current_cam_local_transform);
    uploadView(); // Update the view matrix in the shader
}

//handle delta mouse movement input
//it rotates quite extreme. So therefore if the spehre is not visible anymore it is a good idea to 
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
    Node* m_cameraNode_ = scenegraph_.getRoot()->getChildren("Camera");
    if (!m_cameraNode_) return; // Don't do anything if camera isn't set

    if (firstMouse_) {
        lastMouseX_ = pos_x;
        lastMouseY_ = pos_y;
        firstMouse_ = false;
    }

    float xoffset = pos_x - lastMouseX_;
    float yoffset = lastMouseY_ - pos_y; // Reversed since y-coordinates go from bottom to top
    lastMouseX_ = pos_x;
    lastMouseY_ = pos_y;

    const float sensitivity = 0.1f; // Adjust sensitivity
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    glm::mat4 cam_transform = m_cameraNode_->getLocalTransform();

    // Yaw: Rotation around the world's Y-axis (or camera's parent's Y-axis)
    // This provides a stable yaw that doesn't depend on camera's roll.
    cam_transform = glm::rotate(glm::mat4(1.0f), glm::radians(xoffset), glm::vec3(0.0f, 1.0f, 0.0f)) * cam_transform;

    // Pitch: Rotation around the camera's local X-axis
    // Get the camera's current right vector from the (potentially yawed) transform
    glm::vec3 cam_local_right = glm::normalize(glm::vec3(cam_transform[0]));
    cam_transform = glm::rotate(glm::mat4(1.0f), glm::radians(yoffset), cam_local_right) * cam_transform;

    m_cameraNode_->setLocalTransform(cam_transform);
    uploadView(); // Update the view matrix in the shader
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

    // Apply rotation only if there's a rotation speed
    float speed = node->getRotationSpeed();
    if (speed != 0.0f) {
        float angle = glfwGetTime() * speed;
        // Planet resolves around Y axis
        local_transform = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * local_transform;
    }

    glm::mat4 current_transform = parent_transform * local_transform;
    node->setWorldTransform(current_transform); 

    if (auto geo_node = dynamic_cast<GeometryNode*>(node)) {
        glUseProgram(m_shaders.at("planet").handle);

        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
            1, GL_FALSE, glm::value_ptr(current_transform));

        // Calculate NormalMatrix from ModelView matrix
        Node* camera_node = scenegraph_.getRoot()->getChildren("Camera");
        glm::mat4 model_view_matrix;
        if (camera_node) {
            model_view_matrix = glm::inverse(camera_node->getWorldTransform()) * current_transform;
        }
        else {
            //  if camera not found 
            model_view_matrix = glm::mat4(1.0f) * current_transform;
        }
        glm::mat4 normal_matrix = glm::inverseTranspose(model_view_matrix);
        glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
            1, GL_FALSE, glm::value_ptr(normal_matrix));

        // --- Determine if this is the Sun or a Planet ---
        if (node->getName() == "SunGeom") {
            // Sun: set emissive color 
            glUniform3fv(m_shaders.at("planet").u_locs.at("EmissiveColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.8f, 0.0f))); // Bright yellow/orange
            glUniform3fv(m_shaders.at("planet").u_locs.at("PlanetColor"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f))); // No base color for lighting
        }
        else {
            // no self-illumination
            glm::vec3 planet_color = glm::vec3(1.0f); // Default to white

            if (node->getName() == "mercGeom") {
                planet_color = glm::vec3(0.7f, 0.7f, 0.7f); // Grey
            }
            else if (node->getName() == "venGeom") {
                planet_color = glm::vec3(0.8f, 0.7f, 0.5f); // Dull yellow/orange
            }
            else if (node->getName() == "earthGeom") {
                planet_color = glm::vec3(0.2f, 0.3f, 0.8f); // Blue/Green
            }
            else if (node->getName() == "Moon") {
                planet_color = glm::vec3(0.7f, 0.7f, 0.7f); // Light grey
            }
            else if (node->getName() == "marsGeom") {
                planet_color = glm::vec3(0.8f, 0.4f, 0.2f); // Reddish-brown
            }
            else if (node->getName() == "jupitGeom") {
                planet_color = glm::vec3(0.7f, 0.6f, 0.4f); // Striped brown/orange (basic color)
            }
            else if (node->getName() == "satGeom") {
                planet_color = glm::vec3(0.8f, 0.7f, 0.5f); // Pale yellow
            }
            else if (node->getName() == "uranGeom") {
                planet_color = glm::vec3(0.6f, 0.8f, 0.9f); // Cyan
            }
            else if (node->getName() == "neptGeom") {
                planet_color = glm::vec3(0.3f, 0.5f, 0.9f); // Blue

            }
            glUniform3fv(m_shaders.at("planet").u_locs.at("EmissiveColor"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f))); // No emissive for planets
            glUniform3fv(m_shaders.at("planet").u_locs.at("PlanetColor"), 1, glm::value_ptr(planet_color));
        }
        // --- END Sun/Planet determination ---

        glBindVertexArray(planet_object.vertex_AO);
        glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, nullptr);
    }

    // Recursively render children
    for (Node* child : node->getChildrenList()) {
        renderNode(child, current_transform);
    }
}

// exe entry point
int main(int argc, char* argv[]) {
  

  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}