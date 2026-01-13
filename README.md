# 🚀 tiny2D — Manual de Referencia / Reference Manual

PD: Si tu compilarás sin consola, recuerda usar: int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd), en vez del int main() tradicional. Usa -lgdi32 -luser32 -lwinmm -lmsimg32 para compilar con MSYS2. **No hace falta mantener la licencia del motor, solo con dejar tiny2d.h en tu código fuente sin quitar el comentario del principio del código, ya cumple con la licencia.**

PS: If you will compile without a console, remember to use: int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd), instead of the traditional int main(). Use -lgdi32 -luser32 -lwinmm -lmsimg32 to compile with MSYS2. **You don't need to maintain the engine license; simply leaving tiny2d.h in your source code without removing the comment at the beginning of the code will comply with the license.**

**tiny2D** es un motor gráfico 2D minimalista y autocontenido para **Windows**, escrito en **C/C++**. Está diseñado para ser extremadamente simple, reproducible y sin dependencias externas, permitiendo el control total sobre el código en un solo archivo `.h`.

## 🔗 Enlace al Proyecto / Project Link
**GitHub:** [AREStudio - tiny2D](https://github.com/AREStudio-itchio/Tiny2D)

---

## 🛠️ 1. Configuración y Ventana / Configuration & Window

Control total sobre el comportamiento y estado de la aplicación.

| Función / Function | Descripción (ES) | Description (EN) |
| :--- | :--- | :--- |
| `InitWindow(w, h, title)` | Inicializa la ventana y el backbuffer. | Initializes the window and backbuffer. |
| `ConfigureWindow(...)` | Configura botones del sistema (cerrar, min, max). | Configures system buttons (close, min, max). |
| `SetFullscreen(bool)` | Alterna entre ventana y pantalla completa. | Toggles between window and fullscreen. |
| `IsFullscreen()` | Devuelve el estado de la pantalla. | Returns the current screen state. |
| `WindowShouldClose()` | Detecta si se ha cerrado la ventana. | Detects if the window has been closed. |

---

## 🎨 2. Primitivas de Dibujo / Drawing Primitives

El motor utiliza un sistema de doble búfer para garantizar un renderizado suave y sin parpadeos.

* **`ClearBackground(color)`**: Limpia la pantalla con un color específico (ej. `color.BLACK`).
* **`DrawPixel(x, y, color)`**: Dibuja un punto individual.
* **`DrawLine(x1, y1, x2, y2, color)`**: Traza una línea entre dos puntos.
* **`DrawRect(x, y, w, h, color)`**: Dibuja un rectángulo sólido.
* **`DrawCircle(cx, cy, r, color)`**: Dibuja el contorno de un círculo.
* **`DrawBitmap(bmp, x, y)`**: Renderiza una imagen cargada en pantalla.

---

## 🎮 3. Entrada de Usuario / User Input

Detección precisa de teclado y ratón basada en estados de frame.

### Teclado / Keyboard
* **`IsKeyDown(vk)`**: Detecta si la tecla se mantiene presionada.
* **`IsKeyPressed(vk)`**: Se activa solo en el frame inicial de la pulsación.
* **`IsKeyReleased(vk)`**: Se activa solo cuando se suelta la tecla.

### Ratón / Mouse
* **`GetMouseX()` / `GetMouseY()`**: Coordenadas del cursor relativas a la ventana.
* **`IsMouseButtonDown(button)`**: Detecta clics (ej. `VK_LBUTTON` para clic izquierdo).

---

## 🖼️ 4. Gestión de Assets / Asset Management

Soporte nativo para imágenes y detección de colisiones.

* **`LoadBitmapFromFile(path)`**: Carga archivos BMP desde el disco.
* **Base64 Support**: Permite cargar imágenes embebidas como texto directamente en el código.
* **`CheckCollision(Rect a, Rect b)`**: Comprueba si dos rectángulos se interceptan.

> [!IMPORTANT]
> **Compatibilidad de Imagen / Image Compatibility**:
> Tiny2D requiere archivos BMP de 24 bits sin compresión. No cambies la extensión de un PNG manualmente; usa **Paint** para importar tu imagen y guardarla como **BMP** para asegurar el funcionamiento.

---

## 🔄 5. Estructura del Código / Code Structure

```c
#include "tiny2D.h"

// Se ejecuta una vez al inicio / Runs once at startup
bool tiny2D_Init() {
    // Carga tus recursos aquí / Load assets here
    return true; 
}

// Lógica y dibujo por frame / Logic and drawing per frame
bool tiny2D_Update(float dt) {
    ClearBackground(color.BLACK);
    
    // Tu lógica aquí / Your logic here
    if (IsKeyDown(VK_ESCAPE)) return false; 

    return true; 
}

int main() {
    // Resolución y título / Resolution and title
    return tiny2D_Run(800, 600, "Proyecto - AREStudio");
}
