#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <strstream>
#include "SFML/Graphics.hpp"
namespace momo {
	// Declarations --------------------------------------------------------------------------------------------------------------
	class TextureManager;
	class Sprite;
	class Vec3f;
	class Vec2f;
	class Matrix4x4;
	struct Triangle;

	// Constants -----------------------------------------------------------------------------------------------------------------
	const double PI = 3.14159265358979;
	const double TWO_PI = 6.28318530718;
	// Functions -----------------------------------------------------------------------------------------------------------------
	void set_origin_to_middle(sf::Shape& _shape);
	void set_origin_to_middle(sf::Sprite& _sprite);
	float rad2deg(float _rad);
	float deg2rad(float _deg);
	Vec2f interpolate(Vec2f _from, Vec2f _to, float _t);
	float interpolate(float _from, float _to, float _t);

	// Matrix --------------------------------------------------------------------------------------------------------------------
	class Matrix4x4 {
	public:
		float m[4][4] = { 0.0f };
	public:

		void print() {
			for (int r = 0; r < 4; r++) {
				std::cout << "[ ";
				for (int c = 0; c < 4; c++) {
					std::cout << (float)m[r][c] << "\t";
				}
				std::cout << "]\n";
			}
		}

		static Matrix4x4 rotationX(float _theta);

		static Matrix4x4 rotationY(float _theta);

		static Matrix4x4 rotationZ(float _theta);

		static Matrix4x4 projection(float _near, float _far, float _fov, float _ar);

		static Matrix4x4 mult(Matrix4x4 _m1, Matrix4x4 _m2);

		static Matrix4x4 identity();

		static Matrix4x4 translation(float _x, float _y, float _z);

		static Matrix4x4 point_at(Vec3f& _pos, Vec3f& _target, Vec3f& _up);

		static Matrix4x4 quick_inverse(Matrix4x4& _mat); // only for rotation/translation matrices
	};

	// Vec3f ---------------------------------------------------------------------------------------------------------------------
	class Vec3f {
	public:
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 1.0f;

		// Constructors
		Vec3f() {};
		~Vec3f() {};
		Vec3f(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {};

		Vec3f copy() { return Vec3f(x,y,z); }
		void print();
		std::string to_string();

		float mag();
		float dist();
		void normalize();
		Vec3f get_normalized();

		// vector scalar
		void add(float _n);
		void sub(float _n);
		void mult(float _n);
		void div(float _n);

		// vector vector
		void add(Vec3f _v);
		void sub(Vec3f _v);
		void mult(Vec3f _v);
		void div(Vec3f _v);
		Vec3f cross(Vec3f _v);
		float dot(Vec3f _v);

		// vector matrix
		void mult_matrix(Matrix4x4 _mat, bool divide_everything_by_w=false);

		// static
		static Vec3f mult_matrix(Vec3f _v, Matrix4x4 _m, bool divide_everything_by_w = false);
		static Vec3f cross(Vec3f _v1, Vec3f _v2);
		static float dot(Vec3f _v1, Vec3f _v2);

		// operator
		// scalar
		Vec3f operator + (const float _n);
		Vec3f operator += (const float _n);
		Vec3f operator - (const float _n);
		Vec3f operator -= (const float _n);
		Vec3f operator * (const float _n);
		Vec3f operator *= (const float _n);
		Vec3f operator / (const float _n);
		Vec3f operator /= (const float _n);
		// vector
		Vec3f operator + (const  Vec3f _n);
		Vec3f operator += (const Vec3f  _n);
		Vec3f operator - (const  Vec3f _n);
		Vec3f operator -= (const Vec3f  _n);
		Vec3f operator * (const  Vec3f _n);
		Vec3f operator *= (const Vec3f  _n);
		Vec3f operator / (const  Vec3f _n);
		Vec3f operator /= (const Vec3f  _n);
	};

	// Vec2f ---------------------------------------------------------------------------------------------------------------------
	class Vec2f {
	public:
		float x = 0.0f;
		float y = 0.0f;
	public:
		Vec2f() :x(0.0f), y(0.0f) {};
		Vec2f(float _x, float _y) :x(_x), y(_y) {};
		~Vec2f() {};

		// methods
		void add(float _num) { x += _num; y += _num; };
		void add(float _x, float _y) { x += _x; y += _y; };
		void add(Vec2f _v) { x += _v.x; y += _v.y; };
		void sub(float _num) { x -= _num; y -= _num; }
		void sub(float _x, float _y) { x -= _x; y -= _y; };
		void sub(Vec2f _v) { x -= _v.x; y -= _v.y; };
		void mult(float _num) { x *= _num; y *= _num; };
		void mult(float _x, float _y) { x *= _x; y *= _y; };
		void mult(Vec2f _v) { x *= _v.x; y *= _v.y; };
		void div(float _num) { x /= _num; y /= _num; };
		void div(float _x, float _y) { x /= _x; y /= _y; };
		void div(Vec2f _v) { x /= _v.x; y /= _v.y; };
		float mag() { return sqrtf(x * x + y * y); }
		float dist() { return mag(); }
		float dist_to(float _x, float _y) { return sqrtf((_x - x) * (_x - x) + (_y - y) * (_y - y)); }
		float dist_to(Vec2f _v) { return dist_to(_v.x, _v.y); }
		float dot(float _x, float _y) { return (_x * x) + (_y * y); }
		float dot(Vec2f _v) { return dot(_v.x, _v.y); }
		Vec2f perp() { return Vec2f(-y, x); }
		Vec2f copy() { return Vec2f(x, y); }
		void print() { std::cout << x << ", " << y << "\n"; }
		void normalize();
		Vec2f get_normalized();
		float radians();
		float degrees();
		void set(float _x, float _y) { x = _x; y = _y; }
		bool equals(Vec2f _v) { return (x == _v.x && y == _v.y); }
		void set_mag(float _mag);
		void constrain(float _min_mag, float _max_mag);

		// static
		static Vec2f add(Vec2f _v1, Vec2f _v2);
		static Vec2f add(Vec2f _v1, float _x, float _y);
		static Vec2f add(Vec2f _v1, float _num);
		static Vec2f sub(Vec2f _v1, Vec2f _v2);
		static Vec2f sub(Vec2f _v1, float _x, float _y);
		static Vec2f sub(Vec2f _v1, float _num);
		static Vec2f mult(Vec2f _v1, Vec2f _v2);
		static Vec2f mult(Vec2f _v1, float _x, float _y);
		static Vec2f mult(Vec2f _v1, float _num);
		static Vec2f div(Vec2f _v1, Vec2f _v2);
		static Vec2f div(Vec2f _v1, float _x, float _y);
		static Vec2f div(Vec2f _v1, float _num);
		static float dist(Vec2f _v1, Vec2f _v2);
		static float dist(Vec2f _v1, float _x, float _y);
		static Vec2f from_degrees(float _deg, float _mag = 1);
		static Vec2f from_radians(float _rad, float _mag = 1);

		// Operator overloads
		Vec2f operator + (const Vec2f _rhs) { return Vec2f(x + _rhs.x, y + _rhs.y); }
		Vec2f operator + (const float _rhs) { return Vec2f(x + _rhs, y + _rhs); }
		void operator += (const Vec2f _rhs) { add(_rhs); }
		void operator += (const float _rhs) { add(_rhs); }
		Vec2f operator - (const Vec2f _rhs) { return Vec2f(x - _rhs.x, y - _rhs.y); }
		Vec2f operator - (const float _rhs) { return Vec2f(x - _rhs, y - _rhs); }
		void operator -= (const Vec2f _rhs) { sub(_rhs); }
		void operator -= (const float _rhs) { sub(_rhs); }
		Vec2f operator * (const Vec2f _rhs) { return Vec2f(x * _rhs.x, y * _rhs.y); }
		Vec2f operator * (const float _rhs) { return Vec2f(x * _rhs, y * _rhs); }
		void operator *= (const Vec2f _rhs) { mult(_rhs); }
		void operator *= (const float _rhs) { mult(_rhs); }
		Vec2f operator / (const Vec2f _rhs) { return Vec2f(x / _rhs.x, y / _rhs.y); }
		Vec2f operator / (const float _rhs) { return Vec2f(x / _rhs, y / _rhs); }
		void operator /= (const Vec2f _rhs) { div(_rhs); }
		void operator /= (const float _rhs) { div(_rhs); }
		bool operator == (const Vec2f _rhs) { return equals(_rhs); }
		bool operator != (const Vec2f _rhs) { return !equals(_rhs); }
	};

	// Triangle ------------------------------------------------------------------------------------------------------------------
	struct Triangle {
		momo::Vec3f p[3];
		sf::Color color = sf::Color::White;
		sf::Color final_color;

		Triangle copy();
		void print();

		// vector scalar
		void add(float _n);
		void sub(float _n);
		void mult(float _n);
		void div(float _n);

		// vector vector
		void add(Vec3f _v);
		void sub(Vec3f _v);
		void mult(Vec3f _v);
		void div(Vec3f _v);

		// triangle
		void add(Triangle  _t);
		void sub(Triangle  _t);
		void mult(Triangle _t);
		void div(Triangle  _t);


		// vector matrix
		Triangle mult_matrix(Matrix4x4 _m, bool divide_everything_by_w = false);

		// static
		// vector matrix
		static Triangle mult_matrix(Triangle _t, Matrix4x4 _m, bool divide_everything_by_w = false);

		// operator
		// scalar
		Triangle operator + (const	 float _n);
		Triangle operator += (const float _n);
		Triangle operator - (const	 float _n);
		Triangle operator -= (const float _n);
		Triangle operator * (const  float _n);
		Triangle operator *= (const float _n);
		Triangle operator / (const  float _n);
		Triangle operator /= (const float _n);
		// vector
		Triangle operator + (const  Vec3f _n);
		Triangle operator += (const Vec3f  _n);
		Triangle operator - (const  Vec3f _n);
		Triangle operator -= (const Vec3f  _n);
		Triangle operator * (const  Vec3f _n);
		Triangle operator *= (const Vec3f  _n);
		Triangle operator / (const  Vec3f _n);
		Triangle operator /= (const Vec3f  _n);
		// triangle
		Triangle operator + (const  Triangle _n);
		Triangle operator += (const Triangle  _n);
		Triangle operator - (const  Triangle _n);
		Triangle operator -= (const Triangle  _n);
		Triangle operator * (const  Triangle _n);
		Triangle operator *= (const Triangle  _n);
		Triangle operator / (const  Triangle _n);
		Triangle operator /= (const Triangle  _n);
	};

	// Mesh ----------------------------------------------------------------------------------------------------------------------
	struct Mesh {
		std::vector<momo::Triangle> triangles;

		bool load_from_obj(std::string _filename) {
			std::ifstream f(_filename);
			if (!f.is_open())
				return false;

			// verts cache
			std::vector<momo::Vec3f> verts;

			while (!f.eof()) {
				char line[128];
				f.getline(line, 128);

				//std::cout << line;

				std::strstream s;
				s << line;
				char junk;

				if (line[0] == 'v') {
					momo::Vec3f v;
					s >> junk >> v.x >> v.y >> v.z;
					verts.push_back(v);
				}

				if (line[0] == 'f') {
					int f[3];
					s >> junk >> f[0] >> f[1] >> f[2];
					triangles.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
				}
			}
			return true;
		}
	};

	// Mouse ---------------------------------------------------------------------------------------------------------------------
	struct Mouse {
		Vec3f pos;
		bool held[2];
		bool pressed[2];
		bool released[2];
	};

	struct Key {
		int key;
		bool held = false;
		bool pressed = false;
		bool released = false;
	};

// MoGE --------------------------------------------------------------------------------------------------------------------------
	class MoGE {
	protected:
		std::string game_name = "Default Game Name";
		int screen_width = 0;
		int screen_height = 0;
		sf::RenderWindow win;
		float delta = 0.0f;
		int FPS = 0;
		sf::Clock clock;
		Mouse mouse;
		Mouse prev_mouse;
		std::vector<Key> keys;
		std::vector<Key> prev_keys;
		sf::Text text;
		sf::Font font;
		enum KEY {
			A = 0, B, C,
			D, E, F, G,
			H, I, J, K,
			L, M, N, O,
			P, Q, R, S,
			T, U, V, W,
			X, Y, Z, Num0,
			Num1, Num2, Num3, Num4,
			Num5, Num6, Num7, Num8,
			Num9, Escape, LControl, LShift,
			LAlt, LSystem, RControl, RShift,
			RAlt, RSystem, Menu, LBracket,
			RBracket, Semicolon, Comma, Period,
			Quote, Slash, Backslash, Tilde,
			Equal, Hyphen, Space, Enter,
			Backspace, Tab, PageUp, PageDown,
			End, Home, Insert, Delete,
			Add, Subtract, Multiply, Divide,
			Left, Right, Up, Down,
			Numpad0, Numpad1, Numpad2, Numpad3,
			Numpad4, Numpad5, Numpad6, Numpad7,
			Numpad8, Numpad9, F1, F2,
			F3, F4, F5, F6,
			F7, F8, F9, F10,
			F11, F12, F13, F14,
			F15, Pause, KeyCount, Dash = Hyphen,
			BackSpace = Backspace, BackSlash = Backslash, SemiColon = Semicolon, Return = Enter
		};
		TextureManager* tm;
		sf::Vertex line[4];

	public:
		MoGE();
		MoGE(std::string _name, int _w, int _h);
		~MoGE();

		bool init(std::string _name, int _w, int _h);

		// methods to be overriden by the user
		virtual void custom_init() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;

		void run();

		// drawing functions
		void draw_line(float _x1, float _y1, float _x2, float _y2, sf::Color _color=sf::Color::White);
		void draw_line(Vec2f _v1, Vec2f _v2, sf::Color _color = sf::Color::White);
		void draw_triangle(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3, sf::Color _color = sf::Color::White);
		void draw_triangle(Vec2f _v1, Vec2f _v2, Vec2f _v3, sf::Color _color = sf::Color::White);
		void draw_triangle(Vec3f _v1, Vec3f _v2, Vec3f _v3, sf::Color _color = sf::Color::White);
		void draw_triangle_filled(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3, sf::Color _color = sf::Color::White);
		void draw_triangle_filled(Vec2f _v1, Vec2f _v2, Vec2f _v3, sf::Color _color = sf::Color::White);
		void draw_triangle_filled(Vec3f _v1, Vec3f _v2, Vec3f _v3, sf::Color _color = sf::Color::White);
		void draw_text(std::string _text, float _x, float _y, sf::Color _color = sf::Color::White, sf::Color _outline_color = sf::Color::White);
	};

	// Texture Manager -----------------------------------------------------------------------------------------------------------
	class TextureManager {
	private:
		std::vector<std::pair<std::string, sf::Texture*>> textures;
	public:
		TextureManager() {};
		~TextureManager() {};

		void load_texture(std::string _name, std::string _filepath, std::string _suffix="Resources/GFX/", std::string _prefix=".png");

		sf::Texture* get_texture(std::string _name);
	};

	// Sprite --------------------------------------------------------------------------------------------------------------------
	class Sprite {
	private:
		std::vector<sf::Texture*> frames;
		int total_frames = 0;
		int current_frame = 0;
		sf::Sprite current_sprite;
		float accumulated_time = 0.0f;
		float time_for_each_frame = 0.1f;
		std::string name = "default string name";
		float speed = 1.0f;
		bool origin_middle = true;
		Vec2f prev_pos;
		float prev_rotation = 0.f;
	public:
		Vec2f pos;
		float rotation = 0.f; // in degrees
		Vec2f size;

		// setters
		void set_origin_middle(bool _o);

		Sprite(TextureManager* _tm=nullptr, std::string _name="default string name", int _total_frames=1);
		~Sprite() {};

		void init(TextureManager* _tm, std::string _name="default string name", int _total_frames = 1);

		void draw(sf::RenderWindow& _win, sf::RenderStates _render_states=sf::RenderStates::Default);

		void update(float _delta);
	};
}