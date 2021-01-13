# CMakeList.txt : CMake project for rest-api-httplib, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (rest-api-httplib "main.cpp" "json/json.hpp" "httplib/httplib.hpp")

# TODO: Add tests and install targets if needed.
