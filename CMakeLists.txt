project(configurator)

cmake_minimum_required(VERSION "${MIN_VER_CMAKE}" FATAL_ERROR)

################# PLATFORM IDENTITY #################
set(PLATFORM 0)
if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL x86_64)
  set(PLATFORM 1)
  add_definitions(-DPLATFORM=1)
  message("PLATFORM: x86_64")
elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL aarch64)
  set(PLATFORM 2)
  add_definitions(-DPLATFORM=2)
  message("PLATFORM: aarch64")
else()
  message("UNKNOWN PLATFORM: " ${CMAKE_HOST_SYSTEM_PROCESSOR})
  return()
endif()
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 NEW)
endif()

find_package(Qt5Widgets REQUIRED)

################# CMake config ##############
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

################# output config ################
# Путь для выходного файла.
if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/debug)
else()
  set(CMAKE_BUILD_TYPE Release)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/release)
endif()

# Qt et common
if(${PLATFORM} EQUAL 1)
  set(COMMON_LIBS_DIR /usr/lib/x86_64-linux-gnu)
else()
  set(COMMON_LIBS_DIR /usr/lib/aarch64-linux-gnu)
endif()

# Хедеры
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# Исходники
file(GLOB SRC_LIST ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB INCLUDE_LIST ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB FORMS_LIST ${PROJECT_SOURCE_DIR}/forms/*.ui)

# ui files
qt5_wrap_ui(FORMS_HEADERS ${FORMS_LIST})

################# Target definition #################
# create executable app
add_executable(configurator ${INCLUDE_LIST} ${SRC_LIST} ${FORMS_LIST})

################# link connect #################

# COMMON_LIBS
link_directories(${COMMON_LIBS_DIR})

# Boost
target_link_libraries(${PROJECT_NAME} libboost_system.so)
target_link_libraries(${PROJECT_NAME} libboost_filesystem.so)

# UI
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
