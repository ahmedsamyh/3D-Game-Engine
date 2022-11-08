#include "MoGE.h"

namespace momo {
	// Functions ---------------------------------------------------------------------------------------------------------------------
	void set_origin_to_middle(sf::Shape& _shape) {
		float w = _shape.getLocalBounds().width;
		float h = _shape.getLocalBounds().height;

		_shape.setOrigin(w / 2, h / 2);
	}

	void set_origin_to_middle(sf::Sprite& _sprite) {
		float w = _sprite.getLocalBounds().width;
		float h = _sprite.getLocalBounds().height;

		_sprite.setOrigin(w / 2, h / 2);
	}

	float deg2rad(float _deg) {
		return (_deg / 180) * PI;
	}

	Vec2f interpolate(Vec2f _from, Vec2f _to, float _t) {
		Vec2f v;
		v.x = (_to.x - _from.x) * _t + _from.x;
		v.y = (_to.y - _from.y) * _t + _from.y;
		return v;
	}

	float interpolate(float _from, float _to, float _t) {
		float val = (_to - _from) * _t + _from;
		return val;
	}

	float rad2deg(float _rad) {
		return (_rad / PI) * 180;
	}

	// MoGE --------------------------------------------------------------------------------------------------------------------------
	MoGE::MoGE() {
		tm = new TextureManager();
	}

	MoGE::MoGE(std::string _name, int _w, int _h) {
		init(_name, _w, _h);
	}

	MoGE::~MoGE() {
		delete tm;
	}

	bool MoGE::init(std::string _name, int _w, int _h) {
		game_name = _name;
		screen_width = _w;
		screen_height = _h;

		// Create the Render Window
		win.create(sf::VideoMode(screen_width, screen_height), game_name);

		// Set the fps to the default refresh rate of the monitor
		win.setVerticalSyncEnabled(true);

		// Load font
		if (font.loadFromFile("Default MoGE Font.ttf"))
			text.setFont(font);

		text.setCharacterSize(16);

		// Init the Keys
		for (int i = 0; i <= 101; i++) {
			Key k;
			k.key = i;
			keys.push_back(k);
			prev_keys.push_back(k);
		}

		custom_init();


		return true;
	}

	void MoGE::run() {
		// game loop
		while (win.isOpen()) {
			// calculate delta and fps
			delta = clock.restart().asSeconds();
			FPS = int(1 / delta);

			// Display the FPS and Screen Width and Screen Height in the TitleBar
			win.setTitle(game_name + " - " + "Size: " + std::to_string(screen_width) + "," + std::to_string(screen_height) + " - " + "FPS: " + std::to_string(FPS) + "fps - " + "Delta: " + std::to_string(delta) + "s");

			// event loop
			sf::Event event;
			while (win.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					win.close();
			}

			// get mouse pos
			mouse.pos.x = sf::Mouse::getPosition(win).x;
			mouse.pos.y = sf::Mouse::getPosition(win).y;

			// Held
			mouse.held[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left);
			mouse.held[1] = sf::Mouse::isButtonPressed(sf::Mouse::Right);

			// Pressed
			mouse.pressed[0] = !prev_mouse.held[0] && mouse.held[0];
			mouse.pressed[1] = !prev_mouse.held[1] && mouse.held[1];

			// Released
			mouse.released[0] = prev_mouse.held[0] && !mouse.held[0];
			mouse.released[1] = prev_mouse.held[1] && !mouse.held[1];

			prev_mouse.held[0] = mouse.held[0];
			prev_mouse.held[1] = mouse.held[1];


			// Key
			// Current state
			for (int i = 0; i < keys.size(); i++)
				keys[i].held = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keys[i].key);
			// Pressed
			for (int i = 0; i < keys.size(); i++)
				keys[i].pressed = !prev_keys[i].held && keys[i].held;
			// Released
			for (int i = 0; i < keys.size(); i++)
				keys[i].released = prev_keys[i].held && !keys[i].held;
			// Prev state
			for (int i = 0; i < keys.size(); i++)
				prev_keys[i].held = keys[i].held;


			// update
			update();

			// draw
			draw();
		}
	}

	void MoGE::draw_line(float _x1, float _y1, float _x2, float _y2, sf::Color _color) {
		// set position
		line[0].position = { _x1, _y1 };
		line[1].position = { _x2, _y2 };
		// set color
		line[0].color = _color;
		line[1].color = _color;

		win.draw(line, 2, sf::Lines);
	}

	void MoGE::draw_line(Vec2f _v1, Vec2f _v2, sf::Color _color) {
		draw_line(_v1.x, _v1.y, _v2.x, _v2.y, _color);
	}

	void MoGE::draw_triangle(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3, sf::Color _color) {
		draw_line(_x1, _y1, _x2, _y2, _color);
		draw_line(_x2, _y2, _x3, _y3, _color);
		draw_line(_x3, _y3, _x1, _y1, _color);
	}

	void MoGE::draw_triangle(Vec2f _v1, Vec2f _v2, Vec2f _v3, sf::Color _color) {
		draw_triangle(_v1.x, _v1.y, _v2.x, _v2.y, _v3.x, _v3.y, _color);
	}

	void MoGE::draw_triangle(Vec3f _v1, Vec3f _v2, Vec3f _v3, sf::Color _color) {
		draw_triangle(_v1.x, _v1.y, _v2.x, _v2.y, _v3.x, _v3.y, _color);
	}

	void MoGE::draw_triangle_filled(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3, sf::Color _color) {
		// set position
		line[0].position = { _x1, _y1 };
		line[1].position = { _x2, _y2 };
		line[2].position = { _x3, _y3 };

		// set color
		line[0].color = _color;
		line[1].color = _color;
		line[2].color = _color;

		win.draw(line, 3, sf::Triangles);
	}

	void MoGE::draw_triangle_filled(Vec2f _v1, Vec2f _v2, Vec2f _v3, sf::Color _color) {
		draw_triangle_filled(_v1.x, _v1.y, _v2.x, _v2.y, _v3.x, _v3.y, _color);
	}

	void MoGE::draw_triangle_filled(Vec3f _v1, Vec3f _v2, Vec3f _v3, sf::Color _color) {
		draw_triangle_filled(_v1.x, _v1.y, _v2.x, _v2.y, _v3.x, _v3.y, _color);
	}

	void MoGE::draw_text(std::string _text, float _x, float _y, sf::Color _color, sf::Color _outline_color) {
		text.setString(_text);
		text.setPosition({ _x, _y });
		text.setFillColor(_color);
		if (_outline_color != sf::Color::White)
			text.setOutlineThickness(1);
		text.setOutlineColor(_outline_color);

		win.draw(text);
	}

	// TextureManager ----------------------------------------------------------------------------------------------------------------

	void TextureManager::load_texture(std::string _name, std::string _filepath, std::string _suffix, std::string _prefix) {
		// Check if the texture already exists
		for (auto& pair : textures)
			if (pair.first == _name)
				return;

		// Make pair
		std::pair<std::string, sf::Texture*> pair;
		pair.first = _name;
		pair.second = new sf::Texture();
		pair.second->loadFromFile(_suffix + _filepath + _prefix);
		textures.push_back(pair);
	}

	sf::Texture* TextureManager::get_texture(std::string _name) {
		// Return the texture pointer if the name matches
		for (auto& pair : textures)
			if (pair.first == _name)
				return pair.second;

		// Return nullptr if the texture doesnt exist
		return nullptr;
	}

	// Sprite ------------------------------------------------------------------------------------------------------------------------

	void Sprite::set_origin_middle(bool _o) {
		origin_middle = _o;
		if (_o)
			set_origin_to_middle(current_sprite);
		else
			current_sprite.setOrigin(0, 0);
	}

	Sprite::Sprite(TextureManager* _tm, std::string _name, int _total_frames) {
		init(_tm, _name, _total_frames);
	}

	void Sprite::init(TextureManager* _tm, std::string _name, int _total_frames) {
		if (_tm == nullptr)
			return;
		name = _name;
		total_frames = _total_frames;
		for (int i = 0; i < total_frames; i++) {
			_tm->load_texture(_name + std::to_string(i), _name + std::to_string(i));
			frames.push_back(_tm->get_texture(_name + std::to_string(i)));
		}
		current_sprite.setTexture(*frames[0]);
		set_origin_middle(true);
		size.x = _tm->get_texture(_name + "0")->getSize().x;
		size.y = _tm->get_texture(_name + "0")->getSize().y;
	}

	void Sprite::draw(sf::RenderWindow& _win, sf::RenderStates _render_states) {
		_win.draw(current_sprite, _render_states);
	}

	void Sprite::update(float _delta) {
		if (total_frames > 1) {
			accumulated_time += _delta;
			if (accumulated_time >= time_for_each_frame) {
				accumulated_time = 0.0f;
				if (current_frame + 1 > total_frames - 1)
					current_frame = 0;
				else
					current_frame++;
				current_sprite.setTexture(*frames[current_frame]);
			}
		}
		// Auto update pos
		if (!prev_pos.equals(pos))
			current_sprite.setPosition({ pos.x, pos.y });
		prev_pos = pos;
		// Auto update rotation
		if (prev_rotation != rotation)
			current_sprite.setRotation(rotation);
		prev_rotation = rotation;
	}
	// Vec3f -------------------------------------------------------------------------------------------------------------------------
	void Vec3f::mult_matrix(Matrix4x4 _m, bool divide_everything_by_w) {
		Vec3f v;
		v.x = x * _m.m[0][0] + y * _m.m[1][0] + z * _m.m[2][0] + _m.m[3][0];
		v.y = x * _m.m[0][1] + y * _m.m[1][1] + z * _m.m[2][1] + _m.m[3][1];
		v.z = x * _m.m[0][2] + y * _m.m[1][2] + z * _m.m[2][2] + _m.m[3][2];
		v.w = x * _m.m[0][3] + y * _m.m[1][3] + z * _m.m[2][3] + _m.m[3][3];

		if (divide_everything_by_w && v.w != 0.0f) {
			v.x /= v.w;
			v.y /= v.w;
			v.z /= v.w;
		}
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}
	Vec3f momo::Vec3f::mult_matrix(Vec3f _v, Matrix4x4 _m, bool divide_everything_by_w) {
		Vec3f o = _v.copy();
		o.mult_matrix(_m, divide_everything_by_w);
		return o;
	}
	Vec3f momo::Vec3f::cross(Vec3f _v) {
		Vec3f output;
		output.x = y * _v.z - z * _v.y;
		output.y = z * _v.x - x * _v.z;
		output.z = x * _v.y - y * _v.x;
		*this = output.copy();
		return output;
	}

	float Vec3f::dot(Vec3f _v){
		return (x*_v.x) + (y*_v.y) + (z*_v.z);
	}

	Vec3f momo::Vec3f::cross(Vec3f _v1, Vec3f _v2) {
		Vec3f output;
		output = _v1.cross(_v2);
		return output;
	}

	float Vec3f::dot(Vec3f _v1, Vec3f _v2){
		return _v1.dot(_v2);
	}

	// Scalar
	Vec3f Vec3f::operator+(const float _n){
		Vec3f v = this->copy();
		v.add(_n);
		return v;
	}

	Vec3f Vec3f::operator+=(const float _n){
		this->add(_n);
		return this->copy();
	}

	Vec3f Vec3f::operator-(const float _n){
		Vec3f v = this->copy();
		v.sub(_n);
		return v;
	}

	Vec3f Vec3f::operator-=(const float _n){
		this->sub(_n);
		return this->copy();
	}

	Vec3f Vec3f::operator*(const float _n){
		Vec3f v = this->copy();
		v.mult(_n);
		return v;
	}

	Vec3f Vec3f::operator*=(const float _n){
		this->mult(_n);
		return this->copy();
	}

	Vec3f Vec3f::operator/(const float _n){
		Vec3f v = this->copy();
		v.div(_n);
		return v;
	}

	Vec3f Vec3f::operator/=(const float _n){
		this->div(_n);
		return this->copy();
	}

	// Vector
	Vec3f Vec3f::operator+(const  Vec3f _n) {
		Vec3f v = this->copy();
		v.add(_n);
		return v;
	}
								   
	Vec3f Vec3f::operator+=(const Vec3f  _n) {
		this->add(_n);
		return this->copy();
	}
								   
	Vec3f Vec3f::operator-(const  Vec3f _n) {
		Vec3f v = this->copy();
		v.sub(_n);
		return v;
	}
								   
	Vec3f Vec3f::operator-=(const Vec3f  _n) {
		this->sub(_n);
		return this->copy();
	}
								   
	Vec3f Vec3f::operator*(const  Vec3f _n) {
		Vec3f v = this->copy();
		v.mult(_n);
		return v;
	}
								   
	Vec3f Vec3f::operator*=(const Vec3f  _n) {
		this->mult(_n);
		return this->copy();
	}
								   
	Vec3f Vec3f::operator/(const  Vec3f _n) {
		Vec3f v = this->copy();
		v.div(_n);
		return v;
	}
								   
	Vec3f Vec3f::operator/=(const Vec3f  _n) {
		this->div(_n);
		return this->copy();
	}

	void Vec3f::print() {
		std::cout << x << ", " << y << ", " << z << "\n";
	}

	std::string Vec3f::to_string(){
		return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
	}

	float Vec3f::mag(){
		return sqrtf(x*x + y*y + z*z);
	}

	float Vec3f::dist(){
		return this->mag();
	}

	void Vec3f::normalize(){
		float l = this->mag();
		if (l <= 0.0f)
			return;
		x /= l;
		y /= l;
		z /= l;
	}

	Vec3f Vec3f::get_normalized(){
		Vec3f v = this->copy();
		v.normalize();
		return v;
	}

	void Vec3f::add(float _n){
		x += _n;
		y += _n;
		z += _n;
	}

	void Vec3f::sub(float _n){
		x -= _n;
		y -= _n;
		z -= _n;
	}

	void Vec3f::mult(float _n){
		x *= _n;
		y *= _n;
		z *= _n;
	}

	void Vec3f::div(float _n){
		x /= _n;
		y /= _n;
		z /= _n;
	}

	void Vec3f::add(Vec3f _v){
		x += _v.x;
		y += _v.y;
		z += _v.z;
	}

	void Vec3f::sub(Vec3f _v){
		x -= _v.x;
		y -= _v.y;
		z -= _v.z;
	}

	void Vec3f::mult(Vec3f _v){
		x *= _v.x;
		y *= _v.y;
		z *= _v.z;
	}

	void Vec3f::div(Vec3f _v){
		x /= _v.x;
		y /= _v.y;
		z /= _v.z;
	}

	// Vec2f -------------------------------------------------------------------------------------------------------------------------

	void Vec2f::normalize() {
		float l = this->mag();
		if (l <= 0)
			return;
		x /= l;
		y /= l;
	}

	Vec2f Vec2f::get_normalized() {
		Vec2f v = this->copy();
		v.normalize();
		return v;
	}

	float Vec2f::radians() {
		float rad = atan2(y, x);
		if (rad < 0.0f)
			return rad + TWO_PI;
		return rad;
	}

	float Vec2f::degrees() {
		return rad2deg(radians());
	}

	void Vec2f::set_mag(float _mag) {
		this->normalize();
		this->mult(_mag);
	}

	void Vec2f::constrain(float _min_mag, float _max_mag) {
		float mag = std::max(_min_mag, std::min(_max_mag, this->mag()));
		this->set_mag(mag);
	}

	/// static start ///

	Vec2f Vec2f::add(Vec2f _v1, Vec2f _v2)
	{
		Vec2f _v = _v1.copy();
		_v.add(_v2);
		return _v;
	}

	Vec2f Vec2f::add(Vec2f _v1, float _x, float _y) {
		return Vec2f::add(_v1, Vec2f(_x, _y));
	}

	Vec2f Vec2f::add(Vec2f _v1, float _num) {
		return Vec2f::add(_v1, _num, _num);
	}

	Vec2f Vec2f::sub(Vec2f _v1, Vec2f _v2) {
		Vec2f v = _v1.copy();
		v.sub(_v2);
		return v;
	}

	Vec2f Vec2f::sub(Vec2f _v1, float _x, float _y) {
		return Vec2f::sub(_v1, Vec2f(_x, _y));
	}

	Vec2f Vec2f::sub(Vec2f _v1, float _num) {
		return Vec2f::sub(_v1, _num, _num);
	}

	Vec2f Vec2f::mult(Vec2f _v1, Vec2f _v2) {
		Vec2f v = _v1.copy();
		v.mult(_v2);
		return v;
	}

	Vec2f Vec2f::mult(Vec2f _v1, float _x, float _y) {
		return Vec2f::mult(_v1, Vec2f(_x, _y));
	}

	Vec2f Vec2f::mult(Vec2f _v1, float _num) {
		return Vec2f::mult(_v1, _num, _num);
	}

	Vec2f Vec2f::div(Vec2f _v1, Vec2f _v2) {
		Vec2f v = _v1.copy();
		v.div(_v2);
		return v;
	}

	Vec2f Vec2f::div(Vec2f _v1, float _x, float _y) {
		return Vec2f::div(_v1, Vec2f(_x, _y));
	}

	Vec2f Vec2f::div(Vec2f _v1, float _num) {
		return Vec2f::div(_v1, _num, _num);
	}

	float Vec2f::dist(Vec2f _v1, Vec2f _v2) {
		return _v1.dist_to(_v2);
	}

	float Vec2f::dist(Vec2f _v1, float _x, float _y) {
		return Vec2f::dist(_v1, Vec2f(_x, _y));
	}

	Vec2f Vec2f::from_degrees(float _deg, float _mag) {
		Vec2f v;
		v.x = cosf(deg2rad(_deg)) * _mag;
		v.y = sinf(deg2rad(_deg)) * _mag;

		return v;
	}

	Vec2f Vec2f::from_radians(float _rad, float _mag) {
		return Vec2f::from_degrees(rad2deg(_rad), _mag);
	}

	/// static end ///

	// Matrix4x4 ---------------------------------------------------------------------------------------------------------------------

	Matrix4x4 Matrix4x4::rotationX(float _theta) {
		Matrix4x4 mat_rotX;
		mat_rotX.m[0][0] = 1.0f;
		mat_rotX.m[1][1] = cosf(_theta);
		mat_rotX.m[1][2] = sinf(_theta);
		mat_rotX.m[2][1] = -sinf(_theta);
		mat_rotX.m[2][2] = cosf(_theta);
		mat_rotX.m[3][3] = 1.0f;
		return mat_rotX;
	}

	Matrix4x4 Matrix4x4::rotationY(float _theta) {
		Matrix4x4 mat_rotY;
		mat_rotY.m[0][0] = cosf(_theta);
		mat_rotY.m[0][2] = sinf(_theta);
		mat_rotY.m[2][0] = -sinf(_theta);
		mat_rotY.m[1][1] = 1.0f;
		mat_rotY.m[2][2] = cosf(_theta);
		mat_rotY.m[3][3] = 1.0f;
		return mat_rotY;
	}

	Matrix4x4 Matrix4x4::rotationZ(float _theta) {
		Matrix4x4 mat_rotZ;
		mat_rotZ.m[0][0] = cosf(_theta);
		mat_rotZ.m[0][1] = sinf(_theta);
		mat_rotZ.m[1][0] = -sinf(_theta);
		mat_rotZ.m[1][1] = cosf(_theta);
		mat_rotZ.m[2][2] = 1.0f;
		mat_rotZ.m[3][3] = 1.0f;
		return mat_rotZ;
	}

	Matrix4x4 Matrix4x4::projection(float _near, float _far, float _fov, float _ar) {
		float near = _near;
		float far = _far;
		float fov = _fov;
		float aspect_ratio = _ar;
		float fov_rad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);
		Matrix4x4 mat_proj;

		mat_proj.m[0][0] = aspect_ratio * fov_rad;
		mat_proj.m[1][1] = fov_rad;
		mat_proj.m[2][2] = far / (far - near);
		mat_proj.m[3][2] = (-far * near) / (far - near);
		mat_proj.m[2][3] = 1.0f;
		return mat_proj;
	}

	Matrix4x4 Matrix4x4::mult(Matrix4x4 m1, Matrix4x4 m2) {
		Matrix4x4 matrix;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
		return matrix;
	}

	Matrix4x4 Matrix4x4::identity() {
		Matrix4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	Matrix4x4 Matrix4x4::translation(float _x, float _y, float _z) {
		Matrix4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		matrix.m[3][0] = _x;
		matrix.m[3][1] = _y;
		matrix.m[3][2] = _z;
		return matrix;
	}

	Matrix4x4 Matrix4x4::point_at(Vec3f& _pos, Vec3f& _target, Vec3f& _up){
		// Calculate new forward direction
		Vec3f new_forward = _pos - _target;
		new_forward.normalize();

		// Calculate new up direction
		Vec3f a = new_forward * Vec3f::dot(_up, new_forward);
		Vec3f new_up = _up - a;
		new_up.normalize();

		// Create new right direction
		Vec3f new_right = Vec3f::cross(new_up, new_forward);

		// Construct Matrix
		Matrix4x4 matrix;
		matrix.m[0][0] = new_right.x;	matrix.m[0][1] = new_right.y;	matrix.m[0][2] = new_right.z;	matrix.m[0][3] = 0.f;
		matrix.m[1][0] = new_up.x;		matrix.m[1][1] = new_up.y;		matrix.m[1][2] = new_up.z;		matrix.m[1][3] = 0.f;
		matrix.m[2][0] = new_forward.x; matrix.m[2][1] = new_forward.y; matrix.m[2][2] = new_forward.z; matrix.m[2][3] = 0.f;
		matrix.m[3][0] = _pos.x;		matrix.m[3][1] = _pos.y;		matrix.m[3][2] = _pos.z;		matrix.m[3][3] = 1.f;
		return matrix;
	}

	Matrix4x4 Matrix4x4::quick_inverse(Matrix4x4& m){
		Matrix4x4 matrix;
		matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	// Triangle ----------------------------------------------------------------------------------------------------------------------

	Triangle Triangle::copy(){
		return {p[0], p[1], p[2]};
	}

	void Triangle::print(){
		std::cout << "{\n";
		for (int i = 0; i < 3; i++) p[i].print();
		std::cout << "}\n";
	}

	Triangle Triangle::mult_matrix(Matrix4x4 _m, bool divide_everything_by_w){
		for (int i = 0; i < 3; i++)
			p[i].mult_matrix(_m, divide_everything_by_w);
		return this->copy();
	}

	Triangle Triangle::mult_matrix(Triangle _t, Matrix4x4 _m, bool divide_everything_by_w){
		Triangle t = _t.copy();
		t.mult_matrix(_m, divide_everything_by_w);
		return t;
	}

	// operator
	// scalar
	Triangle Triangle::operator+(const  float _n){
		Triangle t = this->copy();
		t.add(_n);
		return t;
	}
	Triangle Triangle::operator+=(const float _n){
		this->add(_n);
		return this->copy();
	}
	Triangle Triangle::operator-(const  float _n){
		Triangle t = this->copy();
		t.sub(_n);
		return t;
	}
	Triangle Triangle::operator-=(const float _n){
		this->sub(_n);
		return this->copy();
	}
	Triangle Triangle::operator*(const  float _n){
		Triangle t = this->copy();
		t.mult(_n);
		return t;
	}
	Triangle Triangle::operator*=(const float _n){
		this->mult(_n);
		return this->copy();
	}
	Triangle Triangle::operator/(const  float _n){
		Triangle t = this->copy();
		t.div(_n);
		return t;
	}
	Triangle Triangle::operator/=(const float _n){
		this->div(_n);
		return this->copy();
	}
	// vector
	Triangle Triangle::operator+(const  Vec3f _n) {
		Triangle t = this->copy();
		t.add(_n);
		return t;
	}
	Triangle Triangle::operator+=(const Vec3f _n) {
		this->add(_n);
		return this->copy();
	}
	Triangle Triangle::operator-(const  Vec3f _n) {
		Triangle t = this->copy();
		t.sub(_n);
		return t;
	}
	Triangle Triangle::operator-=(const Vec3f _n) {
		this->sub(_n);
		return this->copy();
	}
	Triangle Triangle::operator*(const  Vec3f _n) {
		Triangle t = this->copy();
		t.mult(_n);
		return t;
	}
	Triangle Triangle::operator*=(const Vec3f _n) {
		this->mult(_n);
		return this->copy();
	}
	Triangle Triangle::operator/(const  Vec3f _n) {
		Triangle t = this->copy();
		t.div(_n);
		return t;
	}
	Triangle Triangle::operator/=(const Vec3f _n) {
		this->div(_n);
		return this->copy();
	}
	// triangle
	Triangle Triangle::operator+(const  Triangle _n) {
		Triangle t = this->copy();
		t.add(_n);
		return t;
	}
	Triangle Triangle::operator+=(const Triangle _n) {
		this->add(_n);
		return this->copy();
	}
	Triangle Triangle::operator-(const  Triangle _n) {
		Triangle t = this->copy();
		t.sub(_n);
		return t;
	}
	Triangle Triangle::operator-=(const Triangle _n) {
		this->sub(_n);
		return this->copy();
	}
	Triangle Triangle::operator*(const  Triangle _n) {
		Triangle t = this->copy();
		t.mult(_n);
		return t;
	}
	Triangle Triangle::operator*=(const Triangle _n) {
		this->mult(_n);
		return this->copy();
	}
	Triangle Triangle::operator/(const  Triangle _n) {
		Triangle t = this->copy();
		t.div(_n);
		return t;
	}
	Triangle Triangle::operator/=(const Triangle _n) {
		this->div(_n);
		return this->copy();
	}

	// Scalar
	void Triangle::add(float _n) {
		for (int i = 0; i < 3; i++)
			p[i].add(_n);
	}
	void Triangle::sub(float _n){
		for (int i = 0; i < 3; i++)
			p[i].sub(_n);
	}
	void Triangle::mult(float _n){
		for (int i = 0; i < 3; i++)
			p[i].mult(_n);
	}
	void Triangle::div(float _n){
		for (int i = 0; i < 3; i++)
			p[i].div(_n);
	}
	// Vector
	void Triangle::add(Vec3f _v){
		for (int i = 0; i < 3; i++)
			p[i].add(_v);
	}
	void Triangle::sub(Vec3f _v){
		for (int i = 0; i < 3; i++)
			p[i].sub(_v);
	}
	void Triangle::mult(Vec3f _v){
		for (int i = 0; i < 3; i++)
			p[i].mult(_v);
	}
	void Triangle::div(Vec3f _v){
		for (int i = 0; i < 3; i++)
			p[i].div(_v);
	}
	// Triangle
	void Triangle::add(Triangle _t){
		for (int i = 0; i < 3; i++)
			p[i].add(_t.p[i]);
	}
	void Triangle::sub(Triangle _t){
		for (int i = 0; i < 3; i++)
			p[i].sub(_t.p[i]);
	}
	void Triangle::mult(Triangle _t){
		for (int i = 0; i < 3; i++)
			p[i].mult(_t.p[i]);
	}
	void Triangle::div(Triangle _t){
		for (int i = 0; i < 3; i++)
			p[i].div(_t.p[i]);
	}
}