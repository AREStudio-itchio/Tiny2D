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
static bool prevKeys[256] = {0};
static bool isFullscreen = false;

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

void ConfigureWindow(bool allowClose, bool allowMinimize, bool allowMaximize, int startState)
{
    if (!hwnd) return;

    LONG style = GetWindowLongA(hwnd, GWL_STYLE);

    // Botón cerrar (forma parte de WS_SYSMENU)
    if (allowClose)
        style |= WS_SYSMENU;
    else
        style &= ~WS_SYSMENU;

    // Botón minimizar
    if (allowMinimize)
        style |= WS_MINIMIZEBOX;
    else
        style &= ~WS_MINIMIZEBOX;

    // Botón maximizar
    if (allowMaximize)
        style |= WS_MAXIMIZEBOX;
    else
        style &= ~WS_MAXIMIZEBOX;

    // Aplicar cambios
    SetWindowLongA(hwnd, GWL_STYLE, style);
    SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

    // Estado inicial
    switch (startState) {
        case 1: ShowWindow(hwnd, SW_SHOWMAXIMIZED); break;
        case 2: ShowWindow(hwnd, SW_SHOWMINIMIZED); break;
        default: ShowWindow(hwnd, SW_SHOWNORMAL); break;
    }
}

void SetFullscreen(bool enabled)
{
    if (!hwnd) return;

    static RECT windowedRect = {0};

    if (enabled && !isFullscreen)
    {
        // Guardar tamaño original
        GetWindowRect(hwnd, &windowedRect);

        // Quitar bordes
        LONG style = GetWindowLongA(hwnd, GWL_STYLE);
        style &= ~(WS_OVERLAPPEDWINDOW);
        style |= WS_POPUP;
        SetWindowLongA(hwnd, GWL_STYLE, style);

        // Obtener monitor actual
        HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = { sizeof(mi) };
        GetMonitorInfoA(monitor, &mi);

        // Redimensionar ventana al monitor
        SetWindowPos(hwnd, NULL,
            mi.rcMonitor.left, mi.rcMonitor.top,
            mi.rcMonitor.right - mi.rcMonitor.left,
            mi.rcMonitor.bottom - mi.rcMonitor.top,
            SWP_NOZORDER | SWP_FRAMECHANGED);

        // Actualizar tamaño interno
        screenWidth  = mi.rcMonitor.right  - mi.rcMonitor.left;
        screenHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;

        // Recrear backbuffer
        DeleteObject(hbmBuffer);
        hbmBuffer = CreateCompatibleBitmap(hdcWindow, screenWidth, screenHeight);
        SelectObject(hdcBuffer, hbmBuffer);

        // Forzar focus
        SetForegroundWindow(hwnd);
        SetFocus(hwnd);

        isFullscreen = true;
    }
    else if (!enabled && isFullscreen)
    {
        // Restaurar estilo
        LONG style = GetWindowLongA(hwnd, GWL_STYLE);
        style &= ~(WS_POPUP);
        style |= WS_OVERLAPPEDWINDOW;
        SetWindowLongA(hwnd, GWL_STYLE, style);

        // Restaurar tamaño original
        SetWindowPos(hwnd, NULL,
            windowedRect.left, windowedRect.top,
            windowedRect.right - windowedRect.left,
            windowedRect.bottom - windowedRect.top,
            SWP_NOZORDER | SWP_FRAMECHANGED);

        // Actualizar tamaño interno
        screenWidth  = windowedRect.right  - windowedRect.left;
        screenHeight = windowedRect.bottom - windowedRect.top;

        // Recrear backbuffer
        DeleteObject(hbmBuffer);
        hbmBuffer = CreateCompatibleBitmap(hdcWindow, screenWidth, screenHeight);
        SelectObject(hdcBuffer, hbmBuffer);

        SetForegroundWindow(hwnd);
        SetFocus(hwnd);

        isFullscreen = false;
    }
}

bool IsFullscreen() {
    return isFullscreen;
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
bool IsKeyPressed(int vk) {
    vk &= 0xFF;
    return keys[vk] && !prevKeys[vk];
}
bool IsKeyReleased(int vk) {
    vk &= 0xFF;
    return !keys[vk] && prevKeys[vk];
}
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

    // 1. Guardar estado anterior ANTES de procesar mensajes
    for (int i = 0; i < 256; i++)
        prevKeys[i] = keys[i];

    // 2. Procesar mensajes (esto actualiza keys[])
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 3. Calcular dt
    QueryPerformanceCounter(&t2);
    float dt = (float)(t2.QuadPart - t1.QuadPart) / freq.QuadPart;
    if (dt > 0.1f) dt = 0.1f;
    t1 = t2;
    accumulator += dt;

    // 4. Simular
    while (accumulator >= frameTime) {
        if (!tiny2D_Update(frameTime)) goto salir;
        accumulator -= frameTime;
    }

    // 5. Dibujar
    BeginDrawing();
    EndDrawing();
    Sleep(1);
}

salir:
    CloseWindow();
    return 0;
}
