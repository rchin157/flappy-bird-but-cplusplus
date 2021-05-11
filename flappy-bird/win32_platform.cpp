#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include "util.cpp"

bool running = true;

struct Render_Info {
	int height;
	int width;
	void* memory;

	BITMAPINFO bitmap_info;
};

Render_Info renderinfo;

#include "Renderer.cpp"
#include "common.cpp"
#include "game.cpp"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	switch (uMsg) {
		case WM_CLOSE:
		case WM_DESTROY: {
			running = false;
		} break;
		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect);
			renderinfo.width = rect.right - rect.left;
			renderinfo.height = rect.bottom - rect.top;

			int size = renderinfo.width * renderinfo.height * sizeof(unsigned int);

			if (renderinfo.memory)
				VirtualFree(renderinfo.memory, 0, MEM_RELEASE);
			renderinfo.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			renderinfo.bitmap_info.bmiHeader.biSize = sizeof(renderinfo.bitmap_info.bmiHeader);
			renderinfo.bitmap_info.bmiHeader.biWidth = renderinfo.width;
			renderinfo.bitmap_info.bmiHeader.biHeight = renderinfo.height;
			renderinfo.bitmap_info.bmiHeader.biPlanes = 1;
			renderinfo.bitmap_info.bmiHeader.biBitCount = 32;
			renderinfo.bitmap_info.bmiHeader.biCompression = BI_RGB;

		} break;
		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// Create Window Class
	const wchar_t CLASS_NAME[] = L"Window Class";

	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	// Register window
	RegisterClass(&wc);

	// Create window
	HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"I tried making Flappy Bird in C++", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
		NULL, NULL,
		hInstance,
		NULL);

	HDC hdc = GetDC(hwnd);

	if (hwnd == NULL)
		return 0;

	ShowWindow(hwnd, nShowCmd);

	Inputs inputs = {};

	srand(time(NULL));
	Node* head = new Node;
	(*head).set_wall_position(200);
	

	//main loop
	while (running) {
		MSG message;

		for (int i = 0; i < KEY_COUNT; i++)
			inputs.keys[i].state_changed = false;

		while (PeekMessage(&message, hwnd, 0, 0, PM_REMOVE)) {
			switch (message.message) {
			case WM_KEYUP:
			case WM_KEYDOWN: {
				unsigned int vk_code = (unsigned int)message.wParam;
				bool is_key_down = ((message.lParam & (1 << 31)) == 0);

				switch (vk_code) {
				case VK_SPACE: {
					if (inputs.keys[KEY_SPACE].is_key_down != is_key_down)
						inputs.keys[KEY_SPACE].state_changed = true;
					inputs.keys[KEY_SPACE].is_key_down = is_key_down;
				} break;
				}
			} break;
			default:
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}

		run_game(&inputs, &head);

		//render
		StretchDIBits(hdc, 0, 0, renderinfo.width, renderinfo.height, 0, 0, renderinfo.width, renderinfo.height, renderinfo.memory, &renderinfo.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}

	return 0;
}