# Tiny2D
English:

P.S.: Don't export as PNG. After opening the file explorer, renaming the file to BMP and saving it won't work for Tiny2D. You have to export to PNG and then convert it. A strange but useful example is to use Paint to import the image and then export it.​


Tiny2D is licensed under: BSL 1.0 (Boost Software License)

Tiny2D is a 2D rendering library programmed in C. This library allows loading BMP files, handling input, graphics, etc.

It also allows the use of Base64 encoding of bitmap (BMP) images.

Developed to allow users to program games, applications, and tools without getting bogged down in licensing issues, the BSL license permits the use, modification, redistribution, and commercialization of the software, with the sole condition that if the library is distributed in its open-source form, the copyright notice must be retained and the license must be made accessible within the source code download. This does not apply to its binary form (if it has been compiled, this condition does not apply).

Español:

PD: No exportes como PNG, luego que se abra el explorador de archivos y renombrar el archivo a BMP y guardar, no funcionará para Tiny2D, has de exportar a PNG y luego convertirlo, un ejemplo extraño pero útil es, usar Paint para importar la imagen y luego exportarla.


Tiny2D licenciado bajo: BSL 1.0 (Boost Software License)

Tiny2D es una librería de renderizado 2D programado en C, esta librería permite la carga de archivos BMP, manejo de input, gráficos, etc.

Por otro lado, permite el uso de Base64 de imágenes bitmap (BMP).

Desarrollado con el fin de permitir al usuario programar juegos, aplicaciones y herramientas sin liarse con licencias, la BSL permite el uso, la modificación, la redistribución y la comercialización del software, con la única condición de que si se distribuye la librería en su forma de código abierto, se ha de mantener el aviso de copyright y mantener la licencia en algún lugar accesible dentro de la descarga del código fuente, pero esto no se aplica en su forma binaria (si ha sido compilado, esta condición no se aplicará)​

​

# tiny2D — Guía completa en español 🇪🇸

**tiny2D** es un motor gráfico 2D minimalista y autocontenido para Windows, escrito en C/C++, diseñado para ser extremadamente simple, reproducible y sin dependencias externas. Está pensado para desarrolladores que valoran el control total, la claridad del código y la facilidad de distribución.

---

## 🧱 ¿Qué es tiny2D?

Es un único archivo `.h` que te permite:

- Crear una ventana con bucle de juego

- Dibujar primitivas (líneas, rectángulos, círculos, píxeles)

- Cargar y mostrar bitmaps (`.bmp`)

- Detectar entrada de teclado y ratón

- Incrustar sprites como texto Base64 (sin archivos externos)

- Usar `main`, `WinMain` o `wWinMain` sin fricción

---

## 🚀 ¿Cómo se usa?

### 1. Incluir el archivo

    #include "tiny2D.h"

---

### 2. Definir las funciones principales

    bool tiny2D_Init();           // Se ejecuta una vez al iniciar

    bool tiny2D_Update(float dt); // Se ejecuta cada frame con tiempo fijo

---

### 3. Iniciar el motor

    int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

        return tiny2D_Run(800, 600, "Mi Juego");

    }

---

## 🎨 Dibujo

Dentro de `tiny2D_Update()`, puedes usar funciones como:

    ClearBackground(color.BLACK);

    DrawRect(100, 100, 64, 64, color.GREEN);

    DrawLine(0, 0, 800, 600, color.RED);

    DrawCircle(400, 300, 50, color.YELLOW);

---

## 🎮 Entrada

Puedes detectar teclas y ratón fácilmente:

    if (IsKeyDown(VK_LEFT))  jugador.x -= 5;

    if (IsKeyDown(VK_RIGHT)) jugador.x += 5;

    if (IsMouseButtonDown(VK_LBUTTON)) {

        int mx = GetMouseX();

        int my = GetMouseY();

        // lógica de clic

    }

---

## 🖼️ Cargar sprites

### Desde archivo `.bmp`:

    HBITMAP bmp = LoadBitmapFromFile("sprite.bmp");

    DrawBitmap(bmp, x, y);

> Asegúrate de que el archivo esté junto al `.exe` y sea un BMP de 24 bits sin compresión.

---

### Desde Base64 (sin archivos externos):

1. Codifica tu BMP como Base64 (puedes usar [base64-image.de](https://www.base64-image.de/) o similar).

2. Pega el string en tu código:

        const char sprite_b64[] = "Qk2...==";

3. Cárgalo así:

        HBITMAP bmp = LoadBitmapFromBase64(sprite_b64);

        DrawBitmap(bmp, x, y);

---

## 🧪 Ejemplo de colisión con el mouse

    Rect boton = {300, 200, 150, 50};

    bool tiny2D_Update(float dt) {

        ClearBackground(color.BLACK);

        Rect mouse = { GetMouseX(), GetMouseY(), 1, 1 };

        bool hover = CheckCollision(mouse, boton);

        DrawRect(boton.x, boton.y, boton.w, boton.h, hover ? color.YELLOW : color.GRAY);

        if (hover && IsMouseButtonDown(VK_LBUTTON)) {

            DrawRect(boton.x + 4, boton.y + 4, boton.w - 8, boton.h - 8, color.RED);

        }

        return true;

    }

---

## 🔧 Cómo inicializar (`tiny2D_Init()`)

Esta función se llama una vez al iniciar. Úsala para:

- Cargar sprites

- Inicializar variables

- Preparar el estado inicial del juego

Ejemplo:

    HBITMAP jugador;

    bool tiny2D_Init() {

        jugador = LoadBitmapFromFile("jugador.bmp");

        return jugador != NULL;

    }

---

## 🧾 Licencia

Distribuido bajo la Boost Software License 1.0    

Puedes usar, modificar y distribuir este archivo libremente. No se requiere atribución en binarios.

# tiny2D — Complete Guide in English 🇺🇸

**tiny2D** is a minimalist, self-contained 2D graphics engine for Windows, written in C/C++, designed to be extremely simple, reproducible, and without external dependencies. It's intended for developers who value total control, code clarity, and ease of distribution.

---

## 🧱 What is tiny2D?

It's a single `.h` file that allows you to:

- Create a window with a game loop

- Draw primitives (lines, rectangles, circles, pixels)

- Load and display bitmaps (`.bmp`)

- Detect keyboard and mouse input

- Embed sprites as Base64 text (without external files)

- Use `main`, `WinMain`, or `wWinMain` seamlessly

---

## 🚀 How to use it?

### 1. Include the file

#include "tiny2D.h"

---

### 2. Define the main functions

bool tiny2D_Init(); // Runs once at startup

bool tiny2D_Update(float dt); // Runs every frame at a fixed time

---

### 3. Start the engine

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

return tiny2D_Run(800, 600, "My Game");

}

---

## 🎨 Drawing

Within `tiny2D_Update()`, you can use functions like:

ClearBackground(color.BLACK);

DrawRect(100, 100, 64, 64, color.GREEN);

DrawLine(0, 0, 800, 600, color.RED);

DrawCircle(400, 300, 50, color.YELLOW);

---

## 🎮 Input

You can easily detect keys and mouse:

if (IsKeyDown(VK_LEFT)) player.x -= 5;

if (IsKeyDown(VK_RIGHT)) player.x += 5;

if (IsMouseButtonDown(VK_LBUTTON)) {

int mx = GetMouseX();

int my = GetMouseY();

// click logic

}

---

## 🖼️ Load sprites

### From a `.bmp` file:

HBITMAP bmp = LoadBitmapFromFile("sprite.bmp");

DrawBitmap(bmp, x, y);

> Make sure the file is next to the `.exe` and is an uncompressed 24-bit BMP.

---

### From Base64 (without external files):

1. Encode your BMP as Base64 (you can use [base64-image.de](https://www.base64-image.de/) or similar).

2. Paste the string into your code:

const char sprite_b64[] = "Qk2...==";

3. Load it like this:

HBITMAP bmp = LoadBitmapFromBase64(sprite_b64);

DrawBitmap(bmp, x, y);

---

## 🧪 Mouse collision example

Rect button = {300, 200, 150, 50};

bool tiny2D_Update(float dt) {
ClearBackground(color.BLACK);

Rect mouse = { GetMouseX(), GetMouseY(), 1, 1 };
bool hover = CheckCollision(mouse, button);

DrawRect(button.x, button.y, button.w, button.h, hover ? color.YELLOW : color.GRAY);

if (hover && IsMouseButtonDown(VK_LBUTTON)) {
DrawRect(button.x + 4, button.y + 4, button.w - 8, button.h - 8, color.RED);
}

return true;
}

---

## 🔧 How to initialize (`tiny2D_Init()`)

This function is called once on startup. Use it to:

- Load sprites

- Initialize variables

- Prepare the initial game state

Example:

HBITMAP player;

bool tiny2D_Init() {

player = LoadBitmapFromFile("player.bmp");

return player != NULL;

}

---

## 🧾 License

Distributed under the Boost Software License 1.0

You may freely use, modify, and distribute this file. No attribution is required for binaries.
​
