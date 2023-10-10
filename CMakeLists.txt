cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

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

## find sbml package
if (WIN32)
	find_package(libsbml REQUIRED)
	file(GLOB LIBSBML_DLLS "${libsbml_DIR}/../../bin/libsbml.dll")
	set(SBML_INCLUDE_DIR "${libsbml_DIR}/../../include/sbml")
else ()
	set(SBML_PREFIX "SBML_INSTALL_DIR-NOTFOUND" CACHE PATH "sbml root directory containing lib, include, etc.")
	set(SBML_INCLUDE_DIR "${SBML_PREFIX}/include")
	file(GLOB SBML_LIBS "${SBML_PREFIX}/lib/libsbml-static.a")
	set(LIBZ_PREFIX "LIBZ_INSTALL_DIR-NOTFOUND" CACHE PATH "libz root directory containing lib, include, etc.")
	set(LIBZ_INCLUDE_DIR "${LIBZ_PREFIX}/include")
	file(GLOB LIBZ_LIBS "${LIBZ_PREFIX}/lib/libz.a")
	set(LIB_POCO_FOUNDATION_PREFIX "LIB_POCO_FOUNDATION_PREFIX_INSTALL_DIR-NOTFOUND" CACHE PATH "libPocoFoundation root directory containing lib, include, etc.")
	set(LIB_POCO_FOUNDATION_INCLUDE_DIR "${LIB_POCO_FOUNDATION_PREFIX}/include")
	file(GLOB LIB_POCO_FOUNDATION_LIBS "${LIB_POCO_FOUNDATION_PREFIX}/lib/libPocoFoundation.a")
	set(LIB_POCO_XML_PREFIX "LIB_POCO_XML_PREFIX_INSTALL_DIR-NOTFOUND" CACHE PATH "libPocoXML root directory containing lib, include, etc.")
	set(LIB_POCO_XML_INCLUDE_DIR "${LIB_POCO_XML_PREFIX}/include")
	file(GLOB LIB_POCO_XML_LIBS "${LIB_POCO_XML_PREFIX}/lib/libPocoXML.a")
endif()

## Use Graphviz to layout the graphs
option(USE_GRAPHVIZ "Use Graphviz libraries to layout the graphs" OFF)

if (${USE_GRAPHVIZ})
	add_definitions(-DGRAPHVIZ_INCLUDED=true)

	if (WIN32)
		set(GRAPHVIZ_PREFIX "GRAPHVIZ_DIR-NOTFOUND" CACHE PATH "graphviz root directory containing lib, include, etc.")
		set(GRAPHVIZ_INCLUDE_DIR ${GRAPHVIZ_PREFIX}/include)
		set(GRAPHVIZ_LIB_DIR ${GRAPHVIZ_PREFIX}/lib)
		file(GLOB GRAPHVIZ_LIBS_FIRSTHAND "${GRAPHVIZ_LIB_DIR}/gvc.*" "${GRAPHVIZ_LIB_DIR}/cgraph.*" "${GRAPHVIZ_LIB_DIR}/cdt.*")
		set(GRAPHVIZ_DLLS "${GRAPHVIZ_PREFIX}/bin/gvc.dll;${GRAPHVIZ_PREFIX}/bin/cgraph.dll;${GRAPHVIZ_PREFIX}/bin/cdt.dll;${GRAPHVIZ_PREFIX}/bin/pathplan.dll;${GRAPHVIZ_PREFIX}/bin/xdot.dll;${GRAPHVIZ_PREFIX}/bin/expat.dll;${GRAPHVIZ_PREFIX}/bin/gvplugin_neato_layout.dll;${GRAPHVIZ_PREFIX}/bin/gvplugin_pango.dll")
		set(GRAPHVIZ_API_CONFIG "${GRAPHVIZ_PREFIX}/bin/config6")
	else()
		set(GRAPHVIZ_PREFIX "GRAPHVIZ_DIR-NOTFOUND" CACHE PATH "graphviz root directory containing lib, include, etc.")
		set(GRAPHVIZ_INCLUDE_DIR ${GRAPHVIZ_PREFIX}/include)
		set(GRAPHVIZ_LIB_DIR ${GRAPHVIZ_PREFIX}/lib)
		file(GLOB GRAPHVIZ_LIBS "${GRAPHVIZ_LIB_DIR}/libcgraph.a"
				"${GRAPHVIZ_LIB_DIR}/libcdt.a"
				"${GRAPHVIZ_LIB_DIR}/libgvc.a"
				"${GRAPHVIZ_LIB_DIR}/libxdot.a"
				"${GRAPHVIZ_LIB_DIR}/libpathplan.a"
				"${GRAPHVIZ_LIB_DIR}/liblab_gamut.a")
		message(STATUS ${GRAPHVIZ_LIBS})
	endif ()
	if (NOT GRAPHVIZ_LIBS)
		message(FATAL_ERROR "Graphviz dependencies are not found. Set GRAPHVIZ_PREFIX to the graphviz root directory, containing lib, include, etc.")
	endif ()
else ()
	add_definitions(-DGRAPHVIZ_INCLUDED=false)
endif (${USE_GRAPHVIZ})	

## whether to build the static version of the library
set(BUILD_STATIC_LIB ON CACHE BOOL "Build the static version of the library")

if(WIN32)
	## on windows Ws2_32.lib is needed to be included for the dll
    	list(APPEND EXTRA_LIBS "Ws2_32.lib")
elseif(UNIX)
	## set use of RPATH for Mac
	if (APPLE)
		set(CMAKE_MACOSX_RPATH true)
		set(CMAKE_BUILD_RPATH "${CMAKE_BINARY_DIR}/src;${CMAKE_BINARY_DIR}/src/dependencies")
		set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} "@loader_path;@loader_path/dependencies")
		## set C/C++ compiler flags
    		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wno-inline")
    		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fPIC -std=gnu++11")
	else ()
		set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} "$ORIGIN;$ORIGIN/dependencies")
		## set C/C++ compiler flags
    		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -Wno-inline")
    		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fPIC -std=gnu++11 -Wl,--disable-new-dtags")
	endif()

	## set RPATH features
	set(CMAKE_SKIP_BUILD_RPATH FALSE)
	set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
	set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif (WIN32)

## Re-generate the swig bindings
option(WITH_SWIG
	"Regenerate the programming language interface code for Python using SWIG."  ON)

## Which language bindings should be built
option(WITH_PYTHON
	"Generate the Python language interface for SBNE." OFF)

if (WITH_PYTHON)
	set(BUILD_STATIC_LIB ON)
	set(WITH_SWIG ON CACHE BOOL "Regenerate the programming language interface code for Python using SWIG.")
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
		set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} "@loader_path/../Frameworks;@loader_path/../Frameworks/dependencies")
	else ()
		find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport REQUIRED)

		if (UNIX)
			set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} "$ORIGIN/../lib/dependencies")
			file(GLOB Qt_EXTRA "${Qt5_DIR}/../../libicudata.*" "${Qt5_DIR}/../../libicui18n.*" "${Qt5_DIR}/../../libicuuc.*")
		endif (UNIX)
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

add_subdirectory(src)

if (${BUILD_DOCS})
    	add_subdirectory(docs)
endif()

if (${BUILD_PACKAGING})
    	add_subdirectory(packaging)
endif()