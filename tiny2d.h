#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdbool.h>
#include <math.h>

// --- Tipos y colores ---
typedef struct { int x, y, w, h; } Rect;
typedef struct { float x, y; } Vec2;

static struct {
    COLORREF BLACK, WHITE, RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, GRAY, ORANGE;
} color = {
    RGB(0,0,0), RGB(255,255,255), RGB(255,0,0), RGB(0,255,0), RGB(0,0,255),
    RGB(255,255,0), RGB(0,255,255), RGB(255,0,255), RGB(128,128,128), RGB(255,165,0)
};

// --- Estado interno ---
static HWND hwnd = NULL;
static HDC hdcWindow = NULL;
static HDC hdcBuffer = NULL;
static HBITMAP hbmBuffer = NULL;
static int screenWidth = 640, screenHeight = 480;
static bool shouldClose = false;
static bool keys[256] = {0};

// --- Hooks del usuario ---
extern bool tiny2D_Update(float dt);
extern bool tiny2D_Init();

// --- Input por eventos ---
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_KEYDOWN: keys[wParam & 0xFF] = true; return 0;
    case WM_KEYUP:   keys[wParam & 0xFF] = false; return 0;
    case WM_CLOSE:   shouldClose = true; DestroyWindow(hwnd); return 0;
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// --- Inicialización y ciclo principal ---
void InitWindow(int width, int height, const char* title) {
    screenWidth = width; screenHeight = height;
    WNDCLASSA wc = {0}; wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleA(NULL); wc.lpszClassName = "tiny2DWindowClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassA(&wc);
    hwnd = CreateWindowExA(0, wc.lpszClassName, title, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, wc.hInstance, NULL);
    ShowWindow(hwnd, SW_SHOW); UpdateWindow(hwnd);
    SetForegroundWindow(hwnd); SetFocus(hwnd);
    hdcWindow = GetDC(hwnd); hdcBuffer = CreateCompatibleDC(hdcWindow);
    hbmBuffer = CreateCompatibleBitmap(hdcWindow, width, height);
    SelectObject(hdcBuffer, hbmBuffer);
}

void CloseWindow() {
    DeleteObject(hbmBuffer); DeleteDC(hdcBuffer);
    ReleaseDC(hwnd, hdcWindow); DestroyWindow(hwnd);
}

bool WindowShouldClose() { return shouldClose; }

void BeginDrawing() {}
void EndDrawing() { BitBlt(hdcWindow, 0, 0, screenWidth, screenHeight, hdcBuffer, 0, 0, SRCCOPY); }

void ClearBackground(COLORREF color) {
    HBRUSH brush = CreateSolidBrush(color);
    RECT rect = {0, 0, screenWidth, screenHeight};
    FillRect(hdcBuffer, &rect, brush); DeleteObject(brush);
}

// --- Dibujo ---
void DrawPixel(int x, int y, COLORREF color) { SetPixel(hdcBuffer, x, y, color); }

void DrawLine(int x1, int y1, int x2, int y2, COLORREF color) {
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HPEN old = (HPEN)SelectObject(hdcBuffer, pen);
    MoveToEx(hdcBuffer, x1, y1, NULL); LineTo(hdcBuffer, x2, y2);
    SelectObject(hdcBuffer, old); DeleteObject(pen);
}

void DrawRect(int x, int y, int w, int h, COLORREF color) {
    HBRUSH brush = CreateSolidBrush(color);
    RECT r = {x, y, x + w, y + h};
    FillRect(hdcBuffer, &r, brush); DeleteObject(brush);
}

void DrawCircle(int cx, int cy, int radius, COLORREF color) {
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HPEN old = (HPEN)SelectObject(hdcBuffer, pen);
    Arc(hdcBuffer, cx - radius, cy - radius, cx + radius, cy + radius, 0, 0, 0, 0);
    SelectObject(hdcBuffer, old); DeleteObject(pen);
}

// --- Input ---
bool IsKeyDown(int vk_code) { return keys[vk_code & 0xFF]; }
bool IsMouseButtonDown(int button) { return (GetAsyncKeyState(button) & 0x8000) != 0; }
int GetMouseX() { POINT p; GetCursorPos(&p); ScreenToClient(hwnd, &p); return p.x; }
int GetMouseY() { POINT p; GetCursorPos(&p); ScreenToClient(hwnd, &p); return p.y; }

// --- Utilidades ---
bool CheckCollision(Rect a, Rect b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x &&
            a.y < b.y + b.h && a.y + a.h > b.y);
}

HBITMAP LoadBitmapFromFile(const char* filename) {
    return (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0,
                               LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

void DrawBitmap(HBITMAP bmp, int x, int y) {
    if (!bmp) return;
    HDC hdcMem = CreateCompatibleDC(hdcBuffer);
    HBITMAP old = (HBITMAP)SelectObject(hdcMem, bmp);
    BITMAP bm; GetObject(bmp, sizeof(bm), &bm);
    BitBlt(hdcBuffer, x, y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, old); DeleteDC(hdcMem);
}

// --- Punto de entrada universal ---
int tiny2D_Run(int width, int height, const char* title) {
    InitWindow(width, height, title);
    if (&tiny2D_Init) tiny2D_Init();

    LARGE_INTEGER freq, t1, t2;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t1);

    const float targetFPS = 60.0f;
    const float frameTime = 1.0f / targetFPS;
    float accumulator = 0.0f;

    while (!WindowShouldClose()) {
        // Procesar mensajes primero
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg); DispatchMessage(&msg);
        }

        // Calcular dt
        QueryPerformanceCounter(&t2);
        float dt = (float)(t2.QuadPart - t1.QuadPart) / freq.QuadPart;
        if (dt > 0.1f) dt = 0.1f;
        t1 = t2;
        accumulator += dt;

        // Simular a pasos fijos
        while (accumulator >= frameTime) {
            if (!tiny2D_Update(frameTime)) goto salir;
            accumulator -= frameTime;
        }

        BeginDrawing();
        EndDrawing();
        Sleep(1);
    }

salir:
    CloseWindow();
    return 0;
}
