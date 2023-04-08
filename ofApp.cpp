#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();
	ofSetLineWidth(1.5);

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();
	this->line.clear();

	float phi_deg_step = 5;
	float theta_deg_step = 5;
	float noise_radius = 300;
	float threshold = 0.58;

	for (float radius = 150; radius <= 250; radius += 5) {

		for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

			for (float theta_deg = 0; theta_deg <= 180; theta_deg += theta_deg_step) {

				auto noise_value = ofNoise(
					noise_radius * cos(phi_deg * DEG_TO_RAD) * 0.01,
					noise_radius * sin(phi_deg * DEG_TO_RAD) * 0.01,
					noise_radius * cos(theta_deg * DEG_TO_RAD) * 0.005,
					ofGetFrameNum() * 0.01);

				if (noise_value < threshold) { continue; }

				auto noise_value_1 = ofNoise(
					noise_radius * cos(phi_deg * DEG_TO_RAD) * 0.01,
					noise_radius * sin(phi_deg * DEG_TO_RAD) * 0.01,
					noise_radius * cos((theta_deg - theta_deg_step) * DEG_TO_RAD) * 0.005,
					ofGetFrameNum() * 0.01);
				auto noise_value_2 = ofNoise(
					noise_radius * cos((phi_deg + phi_deg_step) * DEG_TO_RAD) * 0.01,
					noise_radius * sin((phi_deg + phi_deg_step) * DEG_TO_RAD) * 0.01,
					noise_radius * cos(theta_deg * DEG_TO_RAD) * 0.005,
					ofGetFrameNum() * 0.01);
				auto noise_value_3 = ofNoise(
					noise_radius * cos((phi_deg - phi_deg_step) * DEG_TO_RAD) * 0.01,
					noise_radius * sin((phi_deg - phi_deg_step) * DEG_TO_RAD) * 0.01,
					noise_radius * cos(theta_deg * DEG_TO_RAD) * 0.005,
					ofGetFrameNum() * 0.01);
				auto noise_value_4 = ofNoise(
					noise_radius * cos(phi_deg * DEG_TO_RAD) * 0.01,
					noise_radius * sin(phi_deg * DEG_TO_RAD) * 0.01,
					noise_radius * cos((theta_deg + theta_deg_step) * DEG_TO_RAD) * 0.005,
					ofGetFrameNum() * 0.01);

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				auto noise_deg_z = ofMap(ofNoise(radius * 0.001 + ofGetFrameNum() * 0.005), 0, 1, -720, 720);
				auto noise_deg_x = ofMap(ofNoise(radius * 0.001 + ofGetFrameNum() * 0.01), 0, 1, -90, 90);
				auto rotation_z = glm::rotate(glm::mat4(), (float)(noise_deg_z * DEG_TO_RAD), glm::vec3(0, 0, 1));
				auto rotation_x = glm::rotate(glm::mat4(), (float)(noise_deg_x * DEG_TO_RAD), glm::vec3(1, 0, 0));

				vertices.push_back(glm::vec3(
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD)));
				vertices.push_back(glm::vec3(
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg - theta_deg_step * 0.5) * DEG_TO_RAD)));
				vertices.push_back(glm::vec3(
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg + phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD)));
				vertices.push_back(glm::vec3(
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * cos((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * sin((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD) * sin((phi_deg - phi_deg_step * 0.5) * DEG_TO_RAD),
					radius * cos((theta_deg + theta_deg_step * 0.5) * DEG_TO_RAD)));

				for (auto& vertex : vertices) {

					vertex = glm::vec4(vertex, 0) * rotation_x * rotation_z;
				}

				this->face.addVertices(vertices);

				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
				this->face.addIndex(index + 0); this->face.addIndex(index + 3); this->face.addIndex(index + 2);

				ofColor color = ofColor(0, ofMap(radius, 150, 250, 0, 255));
				this->face.addColor(color);
				this->face.addColor(color);
				this->face.addColor(color);
				this->face.addColor(color);

				color = ofColor(255, ofMap(radius, 150, 250, 0, 255));

				if (noise_value_1 < threshold) {

					this->line.addVertex(vertices[0]); this->line.addVertex(vertices[1]);
					this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);

					this->line.addColor(color); this->line.addColor(color);
				}

				if (noise_value_2 < threshold) {

					this->line.addVertex(vertices[0]); this->line.addVertex(vertices[2]);
					this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);

					this->line.addColor(color); this->line.addColor(color);
				}

				if (noise_value_3 < threshold) {

					this->line.addVertex(vertices[1]); this->line.addVertex(vertices[3]);
					this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);

					this->line.addColor(color); this->line.addColor(color);
				}

				if (noise_value_4 < threshold) {

					this->line.addVertex(vertices[2]); this->line.addVertex(vertices[3]);
					this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);

					this->line.addColor(color); this->line.addColor(color);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(270);
	//ofRotateZ(ofGetFrameNum() * 0.66666666666666);

	this->face.draw();
	this->line.draw();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}