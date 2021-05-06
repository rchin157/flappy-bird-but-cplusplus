#include <Windows.h>

bool running = true;

void* buffer;
int buffer_width;
int buffer_height;
BITMAPINFO buffer_bitmap_info;

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
			buffer_width = rect.right - rect.left;
			buffer_height = rect.bottom - rect.top;

			int buffer_size = buffer_width * buffer_height * sizeof(unsigned int);

			if (buffer)
				VirtualFree(buffer, 0, MEM_RELEASE);
			buffer = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
			buffer_bitmap_info.bmiHeader.biWidth = buffer_width;
			buffer_bitmap_info.bmiHeader.biHeight = buffer_height;
			buffer_bitmap_info.bmiHeader.biPlanes = 1;
			buffer_bitmap_info.bmiHeader.biBitCount = 32;
			buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;

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

	while (running) {
		MSG message;
		while (PeekMessage(&message, hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		StretchDIBits(hdc, 0, 0, buffer_width, buffer_height, 0, 0, buffer_width, buffer_height, buffer, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}

	return 0;
}