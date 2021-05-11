struct Key_State {
	bool is_key_down;
	bool state_changed;
};

enum {
	KEY_SPACE,

	KEY_COUNT
};

struct Inputs {
	Key_State keys[KEY_COUNT];
};