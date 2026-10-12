cmake_minimum_required(VERSION 3.16)
project(SCAPMS VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
add_executable(SCAPMS
  src/main.cpp
  src/model/PatientRecord.cpp
  src/model/Appointment.cpp
  src/model/ClinicDatabase.cpp
  src/controller/ClinicController.cpp
  src/gui/MainWindow.cpp
  src/utils/FileHandler.cpp
  src/utils/Algorithms.cpp
)
target_include_directories(SCAPMS PRIVATE src)
target_link_libraries(SCAPMS PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
if(MSVC)
  target_compile_options(SCAPMS PRIVATE /W4 /permissive-)
else()
  target_compile_options(SCAPMS PRIVATE -Wall -Wextra -Wpedantic)
endif()
