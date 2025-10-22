cmake_minimum_required(VERSION 3.10)
project(RobotSensorSystem)

# تفعيل C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# تحديد مجلدات التضمين
include_directories(include)

# تحديد ملفات المصدر (كل ملفات cpp في مجلد src)
file(GLOB SOURCES "src/*.cpp")

# إنشاء ملف تنفيذي واحد
add_executable(robot_sensor_system ${SOURCES})