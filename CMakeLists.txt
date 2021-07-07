cmake_minimum_required(VERSION 3.3)

project(sbne)

include(GNUInstallDirs)

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

## get libsbml directories
if (WIN32)
	set(LIBSBML_PREFIX "C:/Program Files/libsbml" CACHE PATH "libSBML root directory (contains lib, include, etc.)")
elseif(UNIX)
	set(LIBSBML_PREFIX "/usr/local" CACHE PATH "libSBML root directory (contains lib, include, etc.)")
endif()

set(LIBSBML_INCLUDEDIR ${LIBSBML_PREFIX}/include)
set(LIBSBML_LIBDIR ${LIBSBML_PREFIX}/lib)
file(GLOB LIBSBML_LIBS "${LIBSBML_LIBDIR}/libsbml.*")

## Find libsbml libraries
find_library(LIBSBML_LIBRARY
	     NAMES sbml libsbml 
	     PATHS ${LIBSBML_LIBDIR})

find_library(LIBSBML_STATIC_LIBRARY
	     NAMES sbml-static libsbml-static 
	     PATHS ${LIBSBML_LIBDIR})

## whether to build the static version of the library
set(BUILD_STATIC_LIB ON CACHE BOOL "Build the static version of the library")

if(WIN32)
	## on windows Ws2_32.lib is needed to be included for the dll
    	list(APPEND LIBSBML_EXTRA_LIBS "Ws2_32.lib")
elseif(UNIX)
	## set use of RPATH for Mac
	if (APPLE)
		set(CMAKE_MACOSX_RPATH true)
		set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} 						"${CMAKE_BINARY_DIR}/src")
	endif()

	## set RPATH features
	set(CMAKE_SKIP_BUILD_RPATH FALSE)
	set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
	set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
	set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

	## set C/C++ compiler flags
    	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wno-inline")
    	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fPIC -std=gnu++11")
endif (WIN32)

## Re-generate the swig bindings
option(WITH_SWIG
	"Regenerate the programming language interface code for Python using SWIG."  ON)

## Which language bindings should be built
option(WITH_PYTHON
	"Generate the Python language interface for SBNE." OFF)

if (WITH_PYTHON)
	set(BUILD_STATIC_LIB ON)
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
	set(BUILD_STATIC_LIB ON)

	# find Qt5 and required components
	if (APPLE)
		find_package(Qt5 COMPONENTS Core Gui Widgets DBus PrintSupport REQUIRED)
		set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} 						"${CMAKE_INSTALL_PREFIX}/bin/SBNE-GUI.app/Contents/Frameworks")
	else ()
		find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
	endif(APPLE)
endif(WITH_GUI)

## building the Documentation
option(BUILD_DOCS "Build sbne documentation" OFF)

## Package the project
option(BUILD_PACKAGING "Package SBNE into binary distributions" OFF)

## Set build type default.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING
  "Choose the type of build to perform. The options are: Debug, Release, RelWithDebInfo, MinSizeRel.")
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" FORCE)
endif()

## only allow building of sbne targets after the dependencies have been found
if (EXISTS ${LIBSBML_LIBRARY})
	message(STATUS "building src")
	add_subdirectory(src)

	if (${BUILD_DOCS})
    		add_subdirectory(docs)
	endif()

	if (${BUILD_PACKAGING})
    		add_subdirectory(packaging)
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