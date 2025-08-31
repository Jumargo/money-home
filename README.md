# money-home
Aplicación de registro de gastos (C++) con interfaz gráfica Qt pensada para parejas. Permite introducir gastos por usuario y categoría, crear nuevas categorías y prorratear gastos anuales en los 12 meses siguientes.

## Características
- Usuarios por defecto: "Yo" y "Pareja" (puedes añadir más).
- Categorías predefinidas + creación de nuevas.
- Gastos normales y gastos anuales prorrateados automáticamente en los 12 meses siguientes al mes del gasto.
- Resumen mensual por usuario y por categoría + gráfico de tarta.
- Persistencia en CSV en `data/`.

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
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.7.2/msvc2022_64"
cmake --build build --config Release
```

También puedes usar MinGW (si instalaste el kit MinGW de Qt):
```bash
cmake -S . -B build-mingw -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/6.7.2/mingw_64"
cmake --build build-mingw -j 4
```

## Ejecutar
- MSVC: `build/Release/money_home_gui.exe`
- MinGW: `build-mingw/money_home_gui.exe`

En MinGW, usa `windeployqt` para copiar las dependencias Qt junto al .exe:
```powershell
C:/Qt/6.7.2/mingw_64/bin/windeployqt.exe --release build-mingw/money_home_gui.exe
```

## Datos
Los ficheros se guardan en `data/expenses.csv` y `data/categories.csv`.

## Prorrateo
- Los gastos "Anual prorrateado" se reparten en 12 partes iguales.
- Se aplican a los 12 meses siguientes al mes del gasto (no incluye el mismo mes).

## AGENTES
Consulta `AGENTS.md` para pautas de colaboración de IAs/LLMs.

