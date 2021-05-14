#pragma once

#define WALL_WIDTH 10

class Wall {
private:
	int gap_height;
	float x;
	bool passed;
public:
	Wall();
	void step(float delta);
	void draw();
	void set_pos(float pos);
	float get_pos();
	bool is_passed();
	void set_passed(bool wall_passed);
	bool check_colliding(float birdpos);
};

Wall::Wall() {
	gap_height = rand() % 60 - 30;
	x = -100;
	passed = false;
}

void Wall::step(float delta) {
	x -= 20.f * delta;
}

void Wall::draw() {
	//draw_rect_rel_not_centred(x, 0, 10, (float)(gap_height - (GAP_WIDTH / 2)), 0x32CD32);
	//draw_rect_rel_not_centred(x, (float)(gap_height + (GAP_WIDTH / 2)), 10, 100, 0x32CD32);

	draw_rect_relative(x, -50, WALL_WIDTH, (50 + gap_height - (GAP_WIDTH / 2)) * 2, 0x32CD32);
	draw_rect_relative(x, 50, WALL_WIDTH, (50 - gap_height - (GAP_WIDTH / 2)) * 2, 0x32CD32);
}

void Wall::set_pos(float pos) {
	x = pos;
}

float Wall::get_pos() {
	return x;
}

bool Wall::is_passed() {
	return passed;
}

void Wall::set_passed(bool wall_passed) {
	passed = wall_passed;
}

bool Wall::check_colliding(float birdpos) {
	if ((birdpos + (BIRD_HEIGHT / 2) > gap_height + (GAP_WIDTH / 2) ||
		birdpos - (BIRD_HEIGHT / 2) < gap_height - (GAP_WIDTH / 2)) &&
		BIRDX + (BIRD_WIDTH / 2) > x - (WALL_WIDTH / 2) &&
		BIRDX - (BIRD_WIDTH / 2) < x + (WALL_WIDTH / 2)) {
		return true;
	}

	return false;
}

class Node {
private:
	Wall wall;
	Node* next;
public:
	Node() {
		wall = Wall();
		next = NULL;
	}
	Node* get_next();
	Wall* get_wall();
	void set_next(Node* n);
	void set_wall_position(float pos);
};

Node* Node::get_next() {
	return next;
}

Wall* Node::get_wall() {
	return &wall;
}

void Node::set_next(Node* n) {
	next = n;
}

void Node::set_wall_position(float pos) {
	wall.set_pos(pos);
}