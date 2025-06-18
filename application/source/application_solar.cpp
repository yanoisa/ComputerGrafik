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
	:Application{ resource_path }
	, planet_object{}
	, star_object{}
	, m_view_transform{ glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f}) }
	, m_view_projection{ utils::calculate_projection_matrix(initial_aspect_ratio) }
	, lastMouseX_{ 0.0 }
	, lastMouseY_{ 0.0 }
	, firstMouse_{ true }
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
	GeometryNode* sunGeom = new GeometryNode("SunGeom", sunHold);
	sunGeom->setGeometry(&planet_model);
	sunGeom->setLocalTransform(glm::scale(glm::mat4(1.0f), glm::vec3(3.0f)));
	//Camera 
	CameraNode* camera = new CameraNode("Camera", rootNode, true);
	camera->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(0.0f, 0.0f, 60.0f)));

	//for the scaling factors was a mixture from actual relationships and what look nice
	Node* mercHold = new Node("mercHold", rootNode);
	GeometryNode* mercGeom = new GeometryNode("mercGeom", mercHold);
	mercHold->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(5.0f, 0.0f, 0.0f)));
	mercGeom->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(0.38f)));
	Node* venHold = new Node("venHold", rootNode);
	GeometryNode* venGeom = new GeometryNode("venGeom", venHold);
	venHold->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(7.0f, 0.0f, 0.0f)));
	venGeom->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(0.95f)));
	Node* earthHold = new Node("earthHold", rootNode);
	GeometryNode* earthGeom = new GeometryNode("earthGeom", earthHold);
	earthHold->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(10.0f, 0.0f, 0.0f)));
	earthGeom->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(1.0f)));
	//moon is relative to 
	Node* moonHold = new Node("moonHold", earthGeom);
	moonHold->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(3.0f, 0.0f, 0.0f)));
	GeometryNode* moonGeom = new GeometryNode("Moon", moonHold);
	moonGeom->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(0.6f)));

	Node* marsHold = new Node("marsHold", rootNode);
	GeometryNode* marsGeom = new GeometryNode("marsGeom", marsHold);
	marsHold->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(15.0f, 0.0f, 0.0f)));
	marsGeom->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(0.5f)));
	Node* jupitHold = new Node("jupitHold", rootNode);
	GeometryNode* jupitGeom = new GeometryNode("jupitGeom", jupitHold);
	jupitHold->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(20.0f, 0.0f, 0.0f)));
	jupitGeom->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(3.0f)));
	Node* satHold = new Node("satHold", rootNode);
	GeometryNode* satGeom = new GeometryNode("satGeom", satHold);
	satHold->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(30.0f, 0.0f, 0.0f)));
	satGeom->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(2.5f)));
	Node* uranHold = new Node("uranHold", rootNode);
	GeometryNode* uranGeom = new GeometryNode("uranGeom", uranHold);
	uranHold->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(40.0f, 0.0f, 0.0f)));
	uranGeom->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(2.0f)));
	Node* neptHold = new Node("neptHold", rootNode);
	GeometryNode* neptGeom = new GeometryNode("neptGeom", neptHold);
	neptHold->setLocalTransform(glm::translate(glm::mat4{ 1.0f }, glm::vec3(50.0f, 0.0f, 0.0f)));
	neptGeom->setLocalTransform(glm::scale(glm::mat4{ 1.0f }, glm::vec3(2.0f)));

	mercHold->setRotationSpeed(1.0f);
	venHold->setRotationSpeed(0.9f);
	earthHold->setRotationSpeed(0.8f);
	marsHold->setRotationSpeed(0.7f);
	jupitHold->setRotationSpeed(0.6);
	satHold->setRotationSpeed(0.5);
	uranHold->setRotationSpeed(0.4);
	neptHold->setRotationSpeed(0.3);
	moonHold->setRotationSpeed(5.0);

	scenegraph_ = solarSystem;

	initializeGeometry();
	initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
	glDeleteBuffers(1, &planet_object.vertex_BO);
	glDeleteBuffers(1, &planet_object.element_BO);
	glDeleteVertexArrays(1, &planet_object.vertex_AO);
	glDeleteBuffers(1, &star_object.vertex_BO);
	glDeleteVertexArrays(1, &star_object.vertex_AO);
}

void ApplicationSolar::render() const {
	glEnable(GL_BLEND); // Ensure blending is on for stars
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive blending for starry glow

	glDepthMask(GL_FALSE); //Disable writing to the depth buffer for stars
	glDepthFunc(GL_LEQUAL);
	glUseProgram(m_shaders.at("point").handle);
	renderStars();
	glDepthMask(GL_TRUE); // enable writing to depth buffer for planets
	glDisable(GL_BLEND);  //Disable blending for planets
	glDepthFunc(GL_LESS);
	//call recursive render on the graph
	glUseProgram(m_shaders.at("planet").handle);
	renderNode(scenegraph_.getRoot(), glm::mat4(1.0f));
}


void ApplicationSolar::uploadView() {
	Node* camera = scenegraph_.getRoot()->getChildren("Camera");
	if (!camera) {
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
// In ApplicationSolar.cpp

void ApplicationSolar::uploadUniforms() {
	// upload the light data to both shaders,
	// so both are ready to be used in renderNode.

	// Calculate light properties once
	Node* root = scenegraph_.getRoot();
	PointLightNode* sunLight = dynamic_cast<PointLightNode*>(root->getChildren("SunLight"));

	glm::vec3 light_color = sunLight->getLightColor();
	float light_intensity = sunLight->getLightIntensity();
	glm::vec3 light_position_world = glm::vec3(sunLight->getWorldTransform()[3]);

	Node* camera_node = scenegraph_.getRoot()->getChildren("Camera");

	glm::mat4 view_matrix = glm::inverse(camera_node->getWorldTransform());
	glm::vec3 light_position_view_space = glm::vec3(view_matrix * glm::vec4(light_position_world, 1.0f));

	// Upload to "planet" shader
	glUseProgram(m_shaders.at("planet").handle);
	glUniform3fv(glGetUniformLocation(m_shaders.at("planet").handle, "LightPosition"), 1, glm::value_ptr(light_position_view_space));
	glUniform3fv(glGetUniformLocation(m_shaders.at("planet").handle, "LightColor"), 1, glm::value_ptr(light_color));
	glUniform1f(glGetUniformLocation(m_shaders.at("planet").handle, "LightIntensity"), light_intensity);

	//  same lighting data to the "cel" shader
	glUseProgram(m_shaders.at("cel").handle);
	glUniform3fv(glGetUniformLocation(m_shaders.at("cel").handle, "LightPosition"), 1, glm::value_ptr(light_position_view_space));
	glUniform3fv(glGetUniformLocation(m_shaders.at("cel").handle, "LightColor"), 1, glm::value_ptr(light_color));
	glUniform1f(glGetUniformLocation(m_shaders.at("cel").handle, "LightIntensity"), light_intensity);

	uploadView();
	uploadProjection();
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
	// are needed for the light and lightning
	m_shaders.at("planet").u_locs["LightPosition"] = -1;
	m_shaders.at("planet").u_locs["LightColor"] = -1;
	m_shaders.at("planet").u_locs["LightIntensity"] = -1;
	m_shaders.at("planet").u_locs["PlanetColor"] = -1;
	m_shaders.at("planet").u_locs["EmissiveColor"] = -1;
	m_shaders.emplace("point", shader_program{ {{GL_VERTEX_SHADER,m_resource_path + "shaders/vao.vert"},
												{GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}} });

	m_shaders.at("point").u_locs["ModelViewMatrix"] = -1;
	m_shaders.at("point").u_locs["ProjectionMatrix"] = -1;

	m_shaders.emplace("cel", shader_program{ {{GL_VERTEX_SHADER, m_resource_path + "shaders/cel.vert"},
											  {GL_FRAGMENT_SHADER, m_resource_path + "shaders/cel.frag"}} });
	// Initialize all uniform locations for the cel shader
	m_shaders.at("cel").u_locs["NormalMatrix"] = -1;
	m_shaders.at("cel").u_locs["ModelMatrix"] = -1;
	m_shaders.at("cel").u_locs["ViewMatrix"] = -1;
	m_shaders.at("cel").u_locs["ProjectionMatrix"] = -1;
	m_shaders.at("cel").u_locs["LightPosition"] = -1;
	m_shaders.at("cel").u_locs["LightColor"] = -1;
	m_shaders.at("cel").u_locs["LightIntensity"] = -1;
	m_shaders.at("cel").u_locs["PlanetColor"] = -1;
	m_shaders.at("cel").u_locs["EmissiveColor"] = -1;


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
	// Initialize Star Object (as Points)

	// Example star data: {x, y, z, r, g, b}
	std::vector<float> star_vertex_data;
	// Generate 10000 random stars
		// otherwise the stars could appear between the planets which could be weird
	for (int i = 0; i < 100000; ++i) {
		float x, y, z;
		//adding stars which has a minimal distance
		do {
			x = (rand() % 10000 - 5000) / 10.0f;
			y = (rand() % 10000 - 5000) / 10.0f;
			z = (rand() % 10000 - 5000) / 10.0f;
		} while (abs(x) + abs(y) + abs(z) < 100);

		// Store valid position
		star_vertex_data.push_back(x);
		star_vertex_data.push_back(y);
		star_vertex_data.push_back(z);

		// Random color
		star_vertex_data.push_back(rand() / (float)RAND_MAX);
		star_vertex_data.push_back(rand() / (float)RAND_MAX);
		star_vertex_data.push_back(rand() / (float)RAND_MAX);

	}


	// Generate Vertex Array Object for stars
	glGenVertexArrays(1, &star_object.vertex_AO);
	glBindVertexArray(star_object.vertex_AO);

	// Generate Vertex Buffer Object for stars
	glGenBuffers(1, &star_object.vertex_BO);
	glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
	glBufferData(GL_ARRAY_BUFFER,
		star_vertex_data.size() * sizeof(float),
		star_vertex_data.data(),
		GL_STATIC_DRAW);
	// Vertex attributes for stars position at location 2, color at location 3)
	GLsizei star_stride = 6 * sizeof(float); // 3 for position + 3 for color

	// Position attribute (location 2)
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,
		3, GL_FLOAT, // 3 components (x,y,z)
		GL_FALSE,
		star_stride,
		(void*)0);     // Offset to the beginning 

	// Color attribute (location 3)
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,
		3, GL_FLOAT, // 3 components (r,g,b)
		GL_FALSE,
		star_stride,
		(void*)(3 * sizeof(float))); // Offset past the position data

	star_object.draw_mode = GL_POINTS;

	//Each star has 6 floats(pos + color)
	star_object.num_elements = GLsizei(star_vertex_data.size() / 6); //


	// Unbind star VAO
	glBindVertexArray(0);
	//-----------------------------------------------------------------------
	//  vertex  sets gl_PointSize
	glEnable(GL_PROGRAM_POINT_SIZE);

}

///////////////////////////// callback functions for window events ////////////
// handle key input
//the steps are much faster with many frames than a low framerate
void ApplicationSolar::keyCallback(int key, int action, int mods) {
	Node* m_cameraNode_ = scenegraph_.getRoot()->getChildren("Camera");
	if (!m_cameraNode_) return;

	glm::mat4 current_cam_local_transform = m_cameraNode_->getLocalTransform();
	glm::vec3 translation_vector{ 0.0f };
	float move_speed = 0.5f; // 

	// relative to world axes 
	// w for z-axis
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

		glm::vec3 forward = -glm::vec3(m_cameraNode_->getWorldTransform()[2]);
		current_cam_local_transform = glm::translate(current_cam_local_transform, glm::normalize(forward) * move_speed);
	}
	//s for z axis
	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		glm::vec3 forward = -glm::vec3(m_cameraNode_->getWorldTransform()[2]);
		current_cam_local_transform = glm::translate(current_cam_local_transform, -glm::normalize(forward) * move_speed);
	}
	// a for x-axis
	else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		glm::vec3 right = glm::vec3(m_cameraNode_->getWorldTransform()[0]);
		current_cam_local_transform = glm::translate(current_cam_local_transform, -glm::normalize(right) * move_speed);
	}
	// d for x-axis
	else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		glm::vec3 right = glm::vec3(m_cameraNode_->getWorldTransform()[0]);
		current_cam_local_transform = glm::translate(current_cam_local_transform, glm::normalize(right) * move_speed);
	}
	//space for y-axis
	else if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
		current_cam_local_transform = glm::translate(current_cam_local_transform, world_up * move_speed);
	}
	//left control for y axis
	else if (key == GLFW_KEY_LEFT_CONTROL && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

		glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
		current_cam_local_transform = glm::translate(current_cam_local_transform, -world_up * move_speed);
	}

	m_cameraNode_->setLocalTransform(current_cam_local_transform);
	//uploadView(); // Update the view matrix in the shader
	uploadUniforms(); // updates view, projection, and light positions correctly
	//changes the class variable
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		cel_shading_active_ = false;
		std::cout << "Standard shading enabled.\n";
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		cel_shading_active_ = true;
		std::cout << "Cel shading enabled.\n";
	}

}

//handle delta mouse movement input
//it rotates quite extreme. So therefore if the spehre is not visible anymore it is a good idea to 
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
	Node* m_cameraNode_ = scenegraph_.getRoot()->getChildren("Camera");
	if (!m_cameraNode_) return;

	if (firstMouse_) {
		lastMouseX_ = pos_x;
		lastMouseY_ = pos_y;
		firstMouse_ = false;
	}

	float xoffset = pos_x - lastMouseX_;
	float yoffset = lastMouseY_ - pos_y; // reversed since y-coordinates go from bottom to top
	lastMouseX_ = pos_x;
	lastMouseY_ = pos_y;

	const float sensitivity = 0.1f; // adjust sensitivity so movement is easier
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	glm::mat4 cam_transform = m_cameraNode_->getLocalTransform();

	//  rotation around the world's Y-axis (or camera's parent's Y-axis)
	// this provides stability doesn't depend on camera's roll.
	cam_transform = glm::rotate(glm::mat4(1.0f), glm::radians(xoffset), glm::vec3(0.0f, 1.0f, 0.0f)) * cam_transform;

	// rotation around the camera's local X-axis
	glm::vec3 cam_local_right = glm::normalize(glm::vec3(cam_transform[0]));
	cam_transform = glm::rotate(glm::mat4(1.0f), glm::radians(yoffset), cam_local_right) * cam_transform;

	m_cameraNode_->setLocalTransform(cam_transform);
	//uploadView(); // Update the view matrix in the shader
	uploadUniforms(); // updates view, projection, and light positions correctly

}


//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
	// recalculate projection matrix for new aspect ration
	m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
	// upload new projection matrix
	uploadProjection();
}

// In ApplicationSolar.cpp

void ApplicationSolar::renderNode(Node* node, glm::mat4 parent_transform) const {
	glm::mat4 local_transform = node->getLocalTransform();

	// Apply rotation only if there's a rotation speed
	float speed = node->getRotationSpeed();
	if (speed != 0.0f) {
		float angle = glfwGetTime() * speed;
		local_transform = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * local_transform;
	}

	glm::mat4 current_transform = parent_transform * local_transform;
	node->setWorldTransform(current_transform);

	if (auto geo_node = dynamic_cast<GeometryNode*>(node)) {

		// selecting shader program based on the attribute
		const shader_program& current_shader = cel_shading_active_ ? m_shaders.at("cel") : m_shaders.at("planet");
		glUseProgram(current_shader.handle);

		// upload matrices and other uniforms to the currently active shader
		glUniformMatrix4fv(current_shader.u_locs.at("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(current_transform));

		Node* camera_node = scenegraph_.getRoot()->getChildren("Camera");
		glm::mat4 view_matrix = camera_node ? glm::inverse(camera_node->getWorldTransform()) : m_view_transform;
		glm::mat4 model_view_matrix = view_matrix * current_transform;
		glm::mat4 normal_matrix = glm::inverseTranspose(model_view_matrix);

		// These are needed by both shaders for their calculations
		glUniformMatrix4fv(current_shader.u_locs.at("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(current_shader.u_locs.at("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normal_matrix));
		glUniformMatrix4fv(current_shader.u_locs.at("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(m_view_projection));


		// color properties 
		glm::vec3 emissive_color(0.0f);
		glm::vec3 planet_color(1.0f);

		if (node->getName() == "SunGeom") {
			emissive_color = glm::vec3(1.0f, 0.8f, 0.0f);
		}
		else {
			// setting the color for each planet
			if (node->getName() == "mercGeom") planet_color = glm::vec3(0.6f, 0.6f, 0.6f);     // Dark gray
			else if (node->getName() == "venGeom") planet_color = glm::vec3(0.9f, 0.6f, 0.2f);   // Golden amber
			else if (node->getName() == "earthGeom") planet_color = glm::vec3(0.2f, 0.4f, 0.9f); // Deep blue
			else if (node->getName() == "Moon") planet_color = glm::vec3(0.8f, 0.8f, 0.8f);      // Light gray-white
			else if (node->getName() == "marsGeom") planet_color = glm::vec3(0.9f, 0.3f, 0.1f);  // Reddish orange
			else if (node->getName() == "jupitGeom") planet_color = glm::vec3(0.9f, 0.7f, 0.4f); // Tawny beige
			else if (node->getName() == "satGeom") planet_color = glm::vec3(0.8f, 0.7f, 0.3f);   // Buttery yellow
			else if (node->getName() == "uranGeom") planet_color = glm::vec3(0.3f, 0.9f, 0.9f);  // Bright cyan
			else if (node->getName() == "neptGeom") planet_color = glm::vec3(0.2f, 0.4f, 1.0f);  // Vivid azure

		}

		// Upload the color properties to the active shader
		glUniform3fv(current_shader.u_locs.at("EmissiveColor"), 1, glm::value_ptr(emissive_color));
		glUniform3fv(current_shader.u_locs.at("PlanetColor"), 1, glm::value_ptr(planet_color));

		//draw the objects
		glBindVertexArray(planet_object.vertex_AO);
		glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, nullptr);
	}

	// Recursive render children
	for (Node* child : node->getChildrenList()) {
		renderNode(child, current_transform);
	}
}
void ApplicationSolar::renderStars() const {
	// Enable blending for stars
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// Disable depth testing for stars (they should always appear behind)
	//glDisable(GL_DEPTH_TEST);

	glUseProgram(m_shaders.at("point").handle);

	// Set up matrices for the stars
	// Stars are in world space, so their model matrix can be identity.
	glm::mat4 star_model_matrix = glm::mat4(1.0f);

	// Get the view matrix from the camera.
	glm::mat4 view_matrix;
	Node* camera_node = scenegraph_.getRoot()->getChildren("Camera");
	if (camera_node) {
		view_matrix = glm::inverse(camera_node->getWorldTransform());
	}
	else {
		// Fallback if camera node isn't found (or use m_view_transform directly)
		view_matrix = glm::lookAt(glm::vec3(0, 0, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Default if no camera
		if (camera_node) {
			view_matrix = glm::inverse(camera_node->getWorldTransform());
		}
	}

	glm::mat4 model_view_matrix = view_matrix * star_model_matrix;

	// Upload uniforms for stars
	glUniformMatrix4fv(m_shaders.at("point").u_locs.at("ModelViewMatrix"),
		1, GL_FALSE, glm::value_ptr(model_view_matrix));
	glUniformMatrix4fv(m_shaders.at("point").u_locs.at("ProjectionMatrix"),
		1, GL_FALSE, glm::value_ptr(m_view_projection));

	// Bind the VAO for stars and draw
	glBindVertexArray(star_object.vertex_AO);
	glDrawArrays(star_object.draw_mode, 0, star_object.num_elements);

	// Unbind VAO 
	glBindVertexArray(0);

	// Restore state
	//glEnable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);
}

// exe entry point
int main(int argc, char* argv[]) {


	Application::run<ApplicationSolar>(argc, argv, 3, 2);
}