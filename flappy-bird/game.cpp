#define is_key_down(k) inputs -> keys[k].is_key_down
#define is_key_pressed(k) inputs -> keys[k].is_key_down && inputs -> keys[k].state_changed
#define GRAVITY 150.f
#define GAP_WIDTH 30
#define VERTICALBIRDPOS 0
#define WALLSAFE -30

#include "game.h"

float rise_rate = 0;
float bird_pos = 0;
int score = 0;

void add_wall(Node& prev) {
	Node* new_node = new Node;
	prev.set_next(new_node);
	(*new_node).set_wall_position(100.f);
}

void update_walls(Node** head, float delta) {
	Node* current = *head;
	Node* last = current;
	while (current != NULL) {
		Wall* temp = (*current).get_wall();
		(*temp).step(delta);
		if ((*temp).get_pos() < -30 && !(*temp).is_passed()) {
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
	if (last != NULL && (*(*last).get_wall()).get_pos() < 40) {
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

bool check_collisions(Node* head) {
	if (bird_pos > get_rel_window_height() / 2 || bird_pos < get_rel_window_height() / -2)
		return true;

	Node* current = head;
	while (current != NULL) {
		if ((*(*current).get_wall()).check_colliding(bird_pos))
			return true;
		current = (*current).get_next();
	}

	return false;
}

void init_walls(Node** head) {
	Node* current = *head;
	while (current != NULL) {
		Node* temp = (*current).get_next();
		delete current;
		current = temp;
	}
	*head = new Node;
	(**head).set_wall_position(100);
}

void run_game(Inputs* inputs, Node** walls, float delta) {
	clear_screen(0x87ceeb);
	if (is_key_pressed(KEY_SPACE))
		rise_rate = 80.f;
	draw_bird(bird_pos);
	bird_pos += rise_rate * delta;
	rise_rate -= GRAVITY * delta;

	update_walls(walls, delta);
	draw_walls(*walls);
	if (check_collisions(*walls)) {
		//end game
		
		init_walls(walls);
	}
}
