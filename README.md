# money-home
Aplicación de registro de gastos (C++) con interfaz gráfica Qt pensada para parejas. Permite introducir gastos por usuario y categoría, crear nuevas categorías y prorratear gastos anuales en los 12 meses siguientes.

## Características
- Usuarios por defecto: "Yo" y "Pareja" (puedes añadir más).
- Categorías predefinidas + creación de nuevas.
- Gastos normales y gastos anuales prorrateados automáticamente en los 12 meses siguientes al mes del gasto.
- Resumen mensual por usuario y por categoría + gráfico de tarta.
- Persistencia en CSV en carpeta de datos de la app (por plataforma).

## Requisitos
- CMake >= 3.12
- Compilador C++17 (GCC/Clang/MSVC)
- Qt 6 (o Qt 5) con módulos `Widgets` y `Charts`

## Compilar (GUI Qt)
```bash
cmake -S . -B build
cmake --build build --config Release
```

Si CMake no encuentra Qt automáticamente, especifica `CMAKE_PREFIX_PATH` (ejemplo MSVC):
```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt_v6_9/6.9.x/msvc2022_64"
cmake --build build --config Release
```

También puedes usar MinGW (si instalaste el kit MinGW de Qt):
```bash
cmake -S . -B build-mingw -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt_v6_9/6.9.x/mingw_64"
cmake --build build-mingw -j 4
```

## Ejecutar (Windows)
- MSVC: `build/Release/money_home_gui.exe`
- MinGW: `build-mingw/money_home_gui.exe`

En MinGW, usa `windeployqt` para copiar las dependencias Qt junto al .exe:
```powershell
C:/Qt_v6_9/6.9.x/mingw_64/bin/windeployqt.exe --release build-mingw/money_home_gui.exe
```

## Android
- Branch: `feature/android-port`
- Módulos: Qt Widgets + Qt Charts (compatibles con Android)
- Empaquetado: `android/` (manifest y strings) vía `QT_ANDROID_PACKAGE_SOURCE_DIR`

Versiones recomendadas:
- Qt 6.9.x for Android (arm64-v8a), Widgets y Charts
- SDK Platform 34, Build-Tools 34.0.0, NDK r25c (25.2.9519653), JDK 17

Cómo compilar en Qt Creator:
- Instala con el Qt Installer/Qt Android Tools: Android SDK/NDK y un kit Android (p. ej. Qt 6.9.x for Android)
- Abre el proyecto en Qt Creator
- Elige el kit Android (ARM64-v8a)
- Run: Qt Creator usa `androiddeployqt` para generar e instalar el APK

Notas de datos: en Android los CSV se guardan en la ruta interna de la app (`QStandardPaths::AppDataLocation`).

### Rutas ejemplo (instalación en C:\Qt_v6_9)
- Host (Windows): `C:\Qt_v6_9\6.9.x\msvc2022_64`
- Android (arm64): `C:\Qt_v6_9\6.9.x\android_arm64_v8a`
- SDK: `C:\Users\TU_USUARIO\AppData\Local\Android\Sdk`
- NDK: `C:\Users\TU_USUARIO\AppData\Local\Android\Sdk\ndk\25.2.9519653`

## Datos
- Windows/Android/iOS: se usan rutas de `QStandardPaths::AppDataLocation` para almacenar `expenses.csv` y `categories.csv`.

## Prorrateo
- Los gastos "Anual prorrateado" se reparten en 12 partes iguales.
- Se aplican a los 12 meses siguientes al mes del gasto (no incluye el mismo mes).

## AGENTES
Consulta `AGENTS.md` para pautas de colaboración de IAs/LLMs.
