#include <iostream>
#include <SFML/Graphics.hpp>
#include "MoGE.h"
#include <vector>
#include <algorithm>

using namespace momo;


struct DirectionalLight {
	Vec3f dir;
	sf::Color color;

	DirectionalLight() {
		color = sf::Color::White;
	}
};

class GameEngine3D : public MoGE {
private:
	Mesh mesh_obj;
	Matrix4x4 mat_projection;
	Vec3f camera;
	DirectionalLight light;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
	Vec3f translater;
	bool DRAW_DEBUG = true;
	bool AUTO_ROTATE = false;
	bool RAINBOW_LIGHT = false;

	Vec3f matrix_vector_multiply(Vec3f& _v, Matrix4x4& _m) {
		Vec3f o;
		o.x = _v.x * _m.m[0][0] + _v.y * _m.m[1][0] + _v.z * _m.m[2][0] + _m.m[3][0];
		o.y = _v.x * _m.m[0][1] + _v.y * _m.m[1][1] + _v.z * _m.m[2][1] + _m.m[3][1];
		o.z = _v.x * _m.m[0][2] + _v.y * _m.m[1][2] + _v.z * _m.m[2][2] + _m.m[3][2];
		o.w = _v.x * _m.m[0][3] + _v.y * _m.m[1][3] + _v.z * _m.m[2][3] + _m.m[3][3];

		if (o.w != 0.0f) {
			o.x /= o.w;
			o.y /= o.w;
			o.z /= o.w;
		}
		return o;
	}

public:
	GameEngine3D() {};
	~GameEngine3D() {};

	void reset_values() {
		translater.x = 0.0f;
		translater.y = 0.0f;
		translater.z = 5.0f;
		theta_x = 0.0f;
		theta_y = 0.0f;
		theta_z = 0.0f;
		light.dir.z = -1.0f;
		light.color = sf::Color::Red;
	}

	void custom_init() override {
		mesh_obj.load_from_obj("axis.obj");

		// Projection Matrix
		mat_projection = Matrix4x4::projection(0.1f, 1000.0f, 90, (float)screen_height/(float)screen_width);

		reset_values();
	}

	void update() override {

		if (keys[KEY::Tab].pressed)
			DRAW_DEBUG = !DRAW_DEBUG;

		if (keys[KEY::R].pressed)
			AUTO_ROTATE = !AUTO_ROTATE;

		if (keys[KEY::T].pressed)
			RAINBOW_LIGHT = !RAINBOW_LIGHT;
		
		// Input
		float speed = 10.0f;
		translater.x += ((keys[KEY::D].held) - (keys[KEY::A].held)) * speed * delta;
		translater.y += ((keys[KEY::S].held) - (keys[KEY::W].held)) * speed * delta;
		translater.z += ((keys[KEY::E].held) - (keys[KEY::Q].held)) * speed * delta;

		// Reset
		if (keys[KEY::Escape].pressed) {
			reset_values();
		}

		// theta manipulation
		theta_x += ((keys[KEY::K].held) - (keys[KEY::I].held)) * 5.0f * delta;
		theta_y += ((keys[KEY::L].held) - (keys[KEY::J].held)) * 5.0f * delta;
		theta_z += ((keys[KEY::O].held) - (keys[KEY::U].held)) * 5.0f * delta;

		// Auto Rotate
		if (AUTO_ROTATE) {
			theta_x += delta * 1.0f;
			theta_y += delta * 2.0f;
			theta_z += delta * 3.0f;
		}

		// set light position to mouse position
		light.dir.x = ((mouse.pos.x / screen_width) * 2.0f) - 1.0f;
		light.dir.y = ((mouse.pos.y / screen_height) * 2.0f) - 1.0f;
		light.dir.normalize();

		// Rainbow light
		/*if (RAINBOW_LIGHT) {
			light.color.r += 1.0f; light.color.r %= 256;
			light.color.g += 1.0f; light.color.g %= 256;
			light.color.b += 1.0f; light.color.b %= 256;
		}*/
	}

	void draw() override {
		win.clear();
		Matrix4x4 mat_rotX = Matrix4x4::rotationX(theta_x);
		Matrix4x4 mat_rotY = Matrix4x4::rotationY(theta_y);
		Matrix4x4 mat_rotZ = Matrix4x4::rotationZ(theta_z);
	
		std::vector<Triangle> tri_to_raster;

		// Draw triangles
		for (auto& tri : mesh_obj.triangles) {
			Triangle tri_projected, tri_translated, tri_rotated;

			// Rotate
			tri_rotated = Triangle::mult_matrix(tri, mat_rotZ).mult_matrix(mat_rotY).mult_matrix(mat_rotX);

			// Translate
			tri_translated = tri_rotated + translater;

			// Get normal of triangle to see which one i can see
			Vec3f normal, line1, line2;
			line1 = tri_translated.p[1] - tri_translated.p[0];
			line2 = tri_translated.p[2] - tri_translated.p[0];
			normal = Vec3f::cross(line1, line2);
			normal.normalize();

			Vec3f camera_ray = tri_translated.p[0] - camera;
			float dot = camera_ray.dot(normal);

			if (dot < 0.0f) {
				// Project 2D -> 3D
				tri_projected = Triangle::mult_matrix(tri_translated, mat_projection, true);

				// Scale into view
				tri_projected += momo::Vec3f(1.0f, 1.0f, 0.0f);
				tri_projected *= 0.5f; tri_projected *= momo::Vec3f(screen_width, screen_height, 0);


				// Calculate color
				float color = light.dir.dot(normal);
				color += 1.0f;
				color *= 0.5f;
				tri_projected.final_color.r = interpolate(light.color.r, tri_projected.color.r, 0.5) * color;
				tri_projected.final_color.g = interpolate(light.color.g, tri_projected.color.g, 0.5) * color;
				tri_projected.final_color.b = interpolate(light.color.b, tri_projected.color.b, 0.5) * color;

				tri_to_raster.push_back(tri_projected);
			}
		}



		// Draw the triangles
		for (auto& tri_projected : tri_to_raster) {
			// Filled
			draw_triangle_filled(tri_projected.p[0], tri_projected.p[1], tri_projected.p[2], tri_projected.final_color);

			// Wireframe
			//draw_triangle(tri_projected.p[0], tri_projected.p[1], tri_projected.p[2]);
		}


		// draw debug
		if (DRAW_DEBUG) {
			draw_text("Translation: " + translater.to_string(), 16, text.getCharacterSize() * 1, sf::Color::White, sf::Color::Black);
			draw_text("Rotation: {" + std::to_string(momo::rad2deg(theta_x)) + ", " +
				std::to_string(momo::rad2deg(theta_y)) + ", " +
				std::to_string(momo::rad2deg(theta_z)) + " }", 16, text.getCharacterSize() * 2, sf::Color::White, sf::Color::Black);
		}
		win.display();
	}
};



int main() {
	GameEngine3D game;
	game.init("3D Game Engine", 1280, 720);
	game.run();

	return 0;
}
