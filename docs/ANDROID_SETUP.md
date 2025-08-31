# Android Build Setup

Objetivo: compilar e instalar `money_home_gui` en Android (Pixel 9 Pro) con Qt.

## Versiones recomendadas
- Qt for Android: 6.7.2 (arm64-v8a)
- Qt módulos: Widgets, Charts
- Android SDK Platform: API 34 (Android 14)
- Android Build-Tools: 34.0.0
- Android NDK: r25c (25.2.9519653)
- JDK: OpenJDK 17
- CMake/Ninja: incluidos en el Installer de Qt o Qt Android Tools

Qt Android Tools 1.4: válido para instalar/configurar SDK/NDK/JDK/Gradle y crear el kit.

## Pasos en Qt Creator
1) Instala desde el Qt Installer:
   - Qt 6.7.2 for Android (arm64-v8a) + Widgets + Charts
   - Herramientas: Android SDK, Android NDK (r25c), OpenJDK 17, CMake, Ninja
2) Qt Creator → Options → Devices → Android
   - Verifica rutas de SDK/NDK/JDK sin avisos
3) Abre el proyecto y elige el kit "Qt 6.7.2 for Android (arm64-v8a)"
4) Run/Deploy con el Pixel conectado (depuración USB). Qt Creator generará el APK y lo instalará.

## Firma (release)
1) Crear keystore (una vez):
   ```bash
   keytool -genkeypair -v \
     -keystore moneyhome-release.keystore \
     -alias moneyhome \
     -keyalg RSA -keysize 2048 -validity 3650
   ```
2) Guardar el keystore fuera del repo o en local (el repo lo ignora).
3) Qt Creator → Projects → Build Android APK → Sign package
   - Selecciona el keystore, alias y contraseñas (solo para Release)
4) Build → Build for Android → Build (Release) → Deploy/Export APK/AAB

## Build por línea de comandos (opcional)
Ejemplo (ajusta rutas):
```bash
cmake -S . -B build-android \
  -DCMAKE_TOOLCHAIN_FILE="C:/Users/<you>/AppData/Local/Android/Sdk/ndk/25.2.9519653/build/cmake/android.toolchain.cmake" \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-34 \
  -DQT_HOST_PATH="C:/Qt/6.7.2/msvc2022_64" \
  -DCMAKE_FIND_ROOT_PATH="C:/Qt/6.7.2/android_arm64_v8a" \
  -DANDROID_SDK_ROOT="C:/Users/<you>/AppData/Local/Android/Sdk" \
  -DANDROID_NDK_ROOT="C:/Users/<you>/AppData/Local/Android/Sdk/ndk/25.2.9519653"

cmake --build build-android -j 8
```
El APK se genera con `androiddeployqt` al hacer "Install" desde Qt Creator, o con `--target install` en algunos kits.

## Almacenamiento
En Android se usa `QStandardPaths::AppDataLocation` como directorio de datos, no requiere permisos extra.

