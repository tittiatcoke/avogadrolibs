cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(AvogadroLibs)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Request C++11 standard, using new CMake variables.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)
# Set symbol visibility defaults for all targets.
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_VISIBILITY_INLINES_HIDDEN True)

include(BuildType)
include(BuildLocation)
include(CompilerFlags)
include(InstallLocation)
include(DetermineVersion)

# Set up our version.
set(AvogadroLibs_VERSION_MAJOR   "1")
set(AvogadroLibs_VERSION_MINOR   "90")
set(AvogadroLibs_VERSION_PATCH   "0")
set(AvogadroLibs_VERSION
  "${AvogadroLibs_VERSION_MAJOR}.${AvogadroLibs_VERSION_MINOR}.${AvogadroLibs_VERSION_PATCH}")
find_package(Git)
determine_version(${AvogadroLibs_SOURCE_DIR} ${GIT_EXECUTABLE} "AvogadroLibs")

option(BUILD_SHARED_LIBS "Build with shared libraries" ON)

# Before any plugins are defined, and before any add_subdirectory calls:
set_property(GLOBAL PROPERTY AvogadroLibs_PLUGINS)
set_property(GLOBAL PROPERTY AvogadroLibs_STATIC_PLUGINS)

if(MSVC)
  add_definitions("-D_CRT_SECURE_NO_WARNINGS" "-DNOMINMAX")
endif()

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})

option(ENABLE_TESTING "Enable testing and building the tests." OFF)
option(USE_OPENGL "Enable libraries that use OpenGL" ON)
option(USE_HDF5 "Enable optional HDF5 features" OFF)
option(USE_QT "Enable libraries that use Qt 5" ON)
option(USE_VTK "Enable libraries that use VTK" OFF)
option(USE_PROTOCALL "Enable libraries that use ProtoCall" OFF)
option(USE_MOLEQUEUE "Enable the MoleQueue dependent functionality" ON)
option(USE_BOOST_PYTHON "Use Boost Python to wrap some of our API" OFF)

add_subdirectory(utilities)
add_subdirectory(thirdparty)
add_subdirectory(avogadro)

if(ENABLE_TESTING)
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()

option(BUILD_DOCUMENTATION "Build project documentation" OFF)

if(BUILD_DOCUMENTATION)
  add_subdirectory(docs)
endif()

if(USE_BOOST_PYTHON)
  add_subdirectory(boostpython)
endif()

install(
  FILES
    README.md
    CONTRIBUTING.md
    COPYING
  DESTINATION "${INSTALL_DOC_DIR}/avogadrolibs")

# After all add_subdirectory calls, so the list of plugins is complete:
get_property(AvogadroLibs_PLUGINS GLOBAL PROPERTY AvogadroLibs_PLUGINS)
get_property(AvogadroLibs_STATIC_PLUGINS GLOBAL
  PROPERTY AvogadroLibs_STATIC_PLUGINS)

configure_file(${AvogadroLibs_SOURCE_DIR}/cmake/CTestCustom.cmake.in
  ${AvogadroLibs_BINARY_DIR}/CTestCustom.cmake)

configure_file("${AvogadroLibs_SOURCE_DIR}/cmake/AvogadroLibsConfig.cmake.in"
  "${AvogadroLibs_BINARY_DIR}/AvogadroLibsConfig.cmake" @ONLY)
configure_file("${AvogadroLibs_SOURCE_DIR}/cmake/AvogadroLibsConfigVersion.cmake.in"
  "${AvogadroLibs_BINARY_DIR}/AvogadroLibsConfigVersion.cmake" @ONLY)

install(FILES
  "${AvogadroLibs_BINARY_DIR}/AvogadroLibsConfig.cmake"
  "${AvogadroLibs_BINARY_DIR}/AvogadroLibsConfigVersion.cmake"
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindEigen3.cmake"
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindGLEW.cmake"
  DESTINATION "${INSTALL_LIBRARY_DIR}/cmake/avogadrolibs")
install(EXPORT "AvogadroLibsTargets"
  DESTINATION "${INSTALL_LIBRARY_DIR}/cmake/avogadrolibs")
