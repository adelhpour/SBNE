cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0020 NEW)

project(sbne)

set(SBNE_ROOT_SOURCE_DIR "${CMAKE_SOURCE_DIR}" CACHE PATH
    "Path to the SBNE root source directory")

set(SBNE_ROOT_BINARY_DIR "${CMAKE_BINARY_DIR}" CACHE PATH
    "Path to the SBNE root build directory")

if (${CMAKE_VERSION} VERSION_GREATER 2.8.4)
	include(GNUInstallDirs)
else ()
	set (CMAKE_INSTALL_LIBDIR "lib")
	set (CMAKE_INSTALL_INCLUDEDIR "include")
	set (CMAKE_INSTALL_BINDIR "bin")
	set (CMAKE_INSTALL_DATADIR "share")
	set (CMAKE_INSTALL_FULL_LIBDIR "${CMAKE_INSTALL_PREFIX}/lib")
	set (CMAKE_INSTALL_FULL_INCLUDEDIR "${CMAKE_INSTALL_PREFIX}/include")
endif ()

## Determine the version
set(SBNE_VERSION_MAJOR)
set(SBNE_VERSION_MINOR)
set(SBNE_VERSION_PATCH)
set(SBNE_VERSION_RELEASE)
set(SBNE_DOTTED_VERSION)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION.txt")

    file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION.txt" VersionString NEWLINE_CONSUME)

    string(STRIP "${VersionString}" VersionString)

    set(SBNE_DOTTED_VERSION ${VersionString})

    string(REPLACE "." ";" VersionString "${VersionString}" )
    string(REPLACE "-" ";" VersionString "${VersionString}" )

    list(LENGTH VersionString versionLength)

    list(GET VersionString 0 SBNE_VERSION_MAJOR )
    list(GET VersionString 1 SBNE_VERSION_MINOR )
    list(GET VersionString 2 SBNE_VERSION_PATCH )

    if(${versionLength} GREATER 3)
        list(GET VersionString 3 SBNE_VERSION_RELEASE )
    endif()

endif()

## add external projects including libsbml
include(ExternalProject)

## get libsbml directories
if (WIN32)
	set(LIBSBML_PREFIX "C:/Program Files/libsbml" CACHE PATH "libSBML root directory (contains lib, include, etc.)")
elseif(UNIX)
	set(LIBSBML_PREFIX "/usr/local" CACHE PATH "libSBML root directory (contains lib, include, etc.)")
endif()

set(LIBSBML_INCLUDEDIR ${LIBSBML_PREFIX}/include)
set(LIBSBML_LIBDIR ${LIBSBML_PREFIX}/lib)

link_directories(${LIBSBML_LIBDIR})
include_directories(${LIBSBML_INCLUDEDIR})

## Find libsbml libraries
find_library(LIBSBML_LIBRARY sbml libsbml 
	HINTS ${LIBSBML_PREFIX}/lib)

find_library(LIBSBML_STATIC_LIBRARY sbml-static libsbml-static 
	HINTS ${LIBSBML_PREFIX}/lib)

## link to static libsbml
set(SBNE_LINK_TO_STATIC_LIBSBML ON CACHE BOOL
	"Link to a static version of libSBML where available")

## whether to build the static version of the library
set(BUILD_STATIC_LIB ON CACHE BOOL "Build the static version of the library")

if(WIN32)
	set(SBNE_WIN_COPY_DEPS_TO_INSTALL ON CACHE BOOL "When true, libraries that 		sbne and guide depends on (libSBML and its dependencies) will be copied to the 		sbne installation directory")

	## on windows Ws2_32.lib is needed to be included for the dll
    	list(APPEND LIBSBML_EXTRA_LIBS "Ws2_32.lib")
elseif(UNIX)
	## turn off windows specific flag
    	set(SBNE_WIN_COPY_DEPS_TO_INSTALL OFF CACHE BOOL 
		"Should be off for Linux and MAC")

	## enable use of RPATH
    	set(ENABLE_FULL_RPATH 1 CACHE BOOL 
		"Enable full RPath in generated executable code")

	## set use of RPATH for Mac
	if (APPLE)
		set(CMAKE_MACOSX_RPATH true)
	endif()

	## set C/C++ compiler flags
    	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wno-inline")
    	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fPIC -std=gnu++11")
endif ()

## handling the path
## http://www.cmake.org/Wiki/CMake_RPATH_handling
if (ENABLE_FULL_RPATH)
	set(CMAKE_SKIP_BUILD_RPATH FALSE)
	set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
	set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
	set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
	list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
		"${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
	if("${isSystemDir}" STREQUAL "-1")
		set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
	endif("${isSystemDir}" STREQUAL "-1")
endif(ENABLE_FULL_RPATH)

## Re-generate the swig bindings
option(WITH_SWIG
	"Regenerate the programming language interface code for Python using SWIG."  ON)

## Which language bindings should be built
option(WITH_PYTHON
	"Generate the Python language interface for SBNE." OFF)

if (WITH_PYTHON)
	set(BUILD_STATIC_LIB ON CACHE BOOL "Build the static version of the library")
	set(WITH_SWIG ON CACHE BOOL "Regenerate the programming language interface code 	for Python using SWIG.")
endif()

if(WITH_SWIG)
    find_package(SWIG REQUIRED)
    set(SWIG_EXTRA_ARGS -DLIBSBNE_COMPILED_IN_SRC)
    include(${SWIG_USE_FILE})
endif()

## building the GUI
option(WITH_GUI "Generate QT_GUI" OFF)

if (WITH_GUI)
	# find Qt5 and required components
	find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
	set(BUILD_STATIC_LIB ON CACHE BOOL "Build the static version of the library")
endif()

## building the Documentation
option(BUILD_DOCS "Build sbne documentation" OFF)

## Set build type default.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING
  "Choose the type of build to perform. The options are: Debug, Release, RelWithDebInfo, MinSizeRel.")
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" FORCE)
endif()

## only allow building of sbne targets after the dependencies have been found
if (EXISTS ${LIBSBML_PREFIX})
	message(STATUS "building src")
	add_subdirectory(src)

	if (${BUILD_DOCS})
    		add_subdirectory(docs)
	endif()
else()
	message(ERROR
            "SBNE dependencies have not been found.")
endif() 

## show variables and their values
message(STATUS "
CMAKE Variables
    CMAKE_SOURCE_DIR                        ${CMAKE_SOURCE_DIR}
    CMAKE_BINARY_DIR                        ${CMAKE_CURRENT_BINARY_DIR}
    CMAKE_INSTALL_PREFIX                    ${CMAKE_INSTALL_PREFIX}
    CMAKE_BUILD_TYPE                        ${CMAKE_BUILD_TYPE}

Libsbml Variables
    LIBSBML_PREFIX                          ${LIBSBML_PREFIX}
    LIBSBML_LIBRARY                         ${LIBSBML_LIBRARY}
    LIBSBML_STATIC_LIBRARY                  ${LIBSBML_STATIC_LIBRARY}
    LIBSBML_EXTRA_LIBS                      ${LIBSBML_EXTRA_LIBS}
")

	




















	













