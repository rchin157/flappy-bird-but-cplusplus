#define is_key_down(k) inputs -> keys[k].is_key_down
#define is_key_pressed(k) inputs -> keys[k].is_key_down && inputs -> keys[k].state_changed
#define GRAVITY 0.001f
#define GAP_WIDTH 30
#define VERTICALBIRDPOS 0
#define WALLSAFE 20

class Wall {
private:
	int gap_height;
	float x;
	bool passed;
public:
	Wall();
	void step();
	void draw();
	void set_pos(float pos);
	float get_pos();
	bool is_passed();
	void set_passed(bool wall_passed);
};

Wall::Wall() {
	gap_height = rand() % 60 + 20;
	x = -100;
	passed = false;
}

void Wall::step() {
	x -= 0.05f;
}

void Wall::draw() {
	draw_rect_rel_not_centred(x, 0, 10, (float)(gap_height - (GAP_WIDTH / 2)), 0x32CD32);
	draw_rect_rel_not_centred(x, (float)(gap_height + (GAP_WIDTH / 2)), 10, 100, 0x32CD32);
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

float rise_rate = 0;
float bird_pos = 0;
int score = 0;

void add_wall(Node& prev) {
	Node* new_node = new Node;
	prev.set_next(new_node);
	(*new_node).set_wall_position(200.f);
}

void update_walls(Node** head) {
	Node* current = *head;
	Node* last = current;
	while (current != NULL) {
		Wall* temp = (*current).get_wall();
		(*temp).step();
		if ((*temp).get_pos() < 30 && !(*temp).is_passed()) {
			(*temp).set_passed(true);
   			score++;
		} else if ((*temp).get_pos() < -100) {
			Node* old_node = current;
			current = (*current).get_next();
			(*head) = (*old_node).get_next();
			(*old_node).set_next(NULL);
			delete old_node;
			continue;
		}
		last = current;
		current = (*current).get_next();
	}
	if (last != NULL && (*(*last).get_wall()).get_pos() < 160) {
		add_wall(*last);
	}
}

void draw_walls(Node* head) {
	Node* current = head;
	while (current != NULL) {
		(*(*current).get_wall()).draw();
		current = (*current).get_next();
	}
}

void run_game(Inputs* inputs, Node** walls) {
	clear_screen(0x87ceeb);
	if (is_key_pressed(KEY_SPACE))
		rise_rate = 0.15f;
	draw_bird(bird_pos);
	bird_pos += rise_rate;
	rise_rate -= GRAVITY;

	update_walls(walls);
	draw_walls(*walls);
}