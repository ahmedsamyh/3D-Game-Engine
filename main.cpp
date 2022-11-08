#include <iostream>
#include <SFML/Graphics.hpp>
#include "MoGE.h"
#include <vector>
#include <algorithm>

using namespace momo;

class GameEngine3D : public MoGE {
private:
	Mesh mesh_obj;
	Matrix4x4 mat_projection;
	float theta = 0.f;

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

	void custom_init() override {
		mesh_obj.load_from_obj("teapot.obj");

		// Projection Matrix
		mat_projection = Matrix4x4::projection(0.1f, 1000.0f, 90, (float)screen_height/(float)screen_width);


		Vec3f v1 = {1.0f, 1.0f, 1.0f, 1.0f};
		Vec3f v2 = v1.copy();

		v1.print();
		v2.print();

	}

	void update() override {

	}

	void draw() override {
		win.clear();
		Matrix4x4 mat_rotZ = Matrix4x4::rotationZ(theta);
		Matrix4x4 mat_rotX = Matrix4x4::rotationX(theta*0.5);

		theta += delta;
	
		std::vector<Triangle> tri_to_raster;

		// Draw triangles
		for (auto& tri : mesh_obj.triangles) {
			Triangle tri_projected, tri_translated, tri_rotated;

			// Rotate
			tri_rotated.p[0] = Vec3f::mult(tri.p[0], mat_rotZ);
			tri_rotated.p[1] = Vec3f::mult(tri.p[1], mat_rotZ);
			tri_rotated.p[2] = Vec3f::mult(tri.p[2], mat_rotZ);

			tri_rotated.p[0] = Vec3f::mult(tri_rotated.p[0], mat_rotX);
			tri_rotated.p[1] = Vec3f::mult(tri_rotated.p[1], mat_rotX);
			tri_rotated.p[2] = Vec3f::mult(tri_rotated.p[2], mat_rotX);


			// Translate
			tri_translated = tri_rotated;
			tri_translated.p[0].z += 5.0f;
			tri_translated.p[1].z += 5.0f;
			tri_translated.p[2].z += 5.0f;

			// Project 2D -> 3D
			tri_projected.p[0] = Vec3f::mult(tri_translated.p[0], mat_projection, true);
			tri_projected.p[1] = Vec3f::mult(tri_translated.p[1], mat_projection, true);
			tri_projected.p[2] = Vec3f::mult(tri_translated.p[2], mat_projection, true);

			// Scale into view
			tri_projected.p[0].x += 1.0f; tri_projected.p[0].y += 1.0f;
			tri_projected.p[1].x += 1.0f; tri_projected.p[1].y += 1.0f;
			tri_projected.p[2].x += 1.0f; tri_projected.p[2].y += 1.0f;

			tri_projected.p[0].x *= 0.5f * (float)screen_width; tri_projected.p[0].y *= 0.5f * (float)screen_height;
			tri_projected.p[1].x *= 0.5f * (float)screen_width; tri_projected.p[1].y *= 0.5f * (float)screen_height;
			tri_projected.p[2].x *= 0.5f * (float)screen_width; tri_projected.p[2].y *= 0.5f * (float)screen_height;

			tri_to_raster.push_back(tri_projected);
		}



		// Draw the triangles
		for (auto& tri_projected : tri_to_raster) {
			// Filled
			draw_triangle_filled(tri_projected.p[0], tri_projected.p[1], tri_projected.p[2]);

			// Wireframe
			//draw_triangle(tri_projected.p[0], tri_projected.p[1], tri_projected.p[2]);
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
