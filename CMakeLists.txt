cmake_minimum_required(VERSION 3.12)
project(money_home LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

## CLI eliminado: dejamos solo la app GUI

# GUI (Qt) target
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Charts QUIET)
if(QT_FOUND)
  find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Charts REQUIRED)
  add_executable(money_home_gui
    src/gui/main_qt.cpp
    src/gui/MainWindow.cpp
    src/gui/ExpenseModel.cpp
    src/Storage.cpp
  )
  target_include_directories(money_home_gui PRIVATE src src/gui)
  target_link_libraries(money_home_gui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts)
  target_compile_features(money_home_gui PRIVATE cxx_std_17)
  # Android packaging overlay (Qt's androiddeployqt leerá este directorio si existe)
  set_property(TARGET money_home_gui PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/android")
else()
  message(FATAL_ERROR "Qt no encontrado. Instala Qt o define CMAKE_PREFIX_PATH.")
endif()
