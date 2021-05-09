## find python packages
find_package(Python COMPONENTS Interpreter Development)

## if swing version is less than 3.0
if (${SWIG_FOUND} AND ${Python_VERSION} VERSION_LESS 3.0)
  if (${SWIG_VERSION} VERSION_GREATER "3.0.7")
    list(APPEND SWIG_EXTRA_ARGS -DUSE_SWIG_PYTHON_2_UNICODE)
    message(STATUS SWIG_VERSION " ${SWIG_VERSION}")
  else()
    message(WARNING 
"Python ${PYTHON_VERSION} detected and SWIG version ${SWIG_VERSION}
 < 3.0.8, libSBNE Python 
 bindings only accept strings as arguments. Please upgrade Swig to a
 version greater than 3.0.8 to enable unicode input in Python 2. See 
 http://www.swig.org/Doc3.0/Python.html for more details.)")
  endif()
endif()

## determine local dependencies, so as to re-swig if one of them changed
file(GLOB SWIG_DEPENDENCIES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.i 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/../swig/*.i
    ${CMAKE_CURRENT_SOURCE_DIR}/../swig/*.h
    )

## Swig bindings
add_custom_command(

    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libsbne_wrap.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/libsbne.py


    COMMAND "${SWIG_EXECUTABLE}"
    ARGS  -I${CMAKE_CURRENT_SOURCE_DIR}/../swig/
        -I${CMAKE_CURRENT_SOURCE_DIR} 
        -I${CMAKE_CURRENT_BINARY_DIR} 
        -I${SBNE_ROOT_BINARY_DIR}/src
        -I${SBNE_ROOT_SOURCE_DIR}/src
        -I${SBNE_ROOT_BINARY_DIR}/src/sbne
        -I${SBNE_ROOT_SOURCE_DIR}/src/sbne
        -DGENERATING_DOCS
        -c++
        -python   
        ${SWIG_EXTRA_ARGS}        
        -o ${CMAKE_CURRENT_BINARY_DIR}/libsbne_wrap.cpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/libsbne.i


    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/libsbne.i
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
        ${SWIG_DEPENDENCIES} 

    COMMENT "Swig Python source")

add_custom_target(binding_python_swig ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libsbne_wrap.cpp )

# Build native library
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../swig)
include_directories(${SBNE_ROOT_SOURCE_DIR}/src)
include_directories(${Python_INCLUDE_DIRS})
include_directories(${LIBSBML_INCLUDEDIR})
include_directories(BEFORE ${SBNE_ROOT_BINARY_DIR}/src)
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

add_library(binding_python_lib SHARED libsbne_wrap.cpp)

target_link_libraries(binding_python_lib sbne-static ${LIBSBML_LIBRARY} ${Python_LIBRARIES})


add_dependencies(binding_python_lib binding_python_swig)
set_target_properties (binding_python_lib PROPERTIES OUTPUT_NAME "_libsbne")

if (UNIX)
    set_target_properties (binding_python_lib PROPERTIES PREFIX "")
    set_target_properties (binding_python_lib PROPERTIES SUFFIX ".so")
else()    
    if (CYGWIN)
        set_target_properties (binding_python_lib PROPERTIES PREFIX "")
        set_target_properties (binding_python_lib PROPERTIES SUFFIX ".dll")
    else()
        set_target_properties (binding_python_lib PROPERTIES SUFFIX ".pyd")  
    endif()
endif()

if (APPLE)
	add_custom_command(TARGET binding_python_lib 
    	POST_BUILD COMMAND 
    	${CMAKE_INSTALL_NAME_TOOL} -change "libsbml.5.dylib" 					"${LIBSBML_LIBDIR}/libsbml.5.dylib"
    	$<TARGET_FILE:binding_python_lib>)
endif()

## Determine the python installation directory
set(PYTHON_PACKAGE_INSTALL_DIR)
if (UNIX OR CYGWIN) 
    execute_process(COMMAND "${Python_EXECUTABLE}" -c "import sys;import platform; sys.stdout.write(platform.python_version()[:3])"
        OUTPUT_VARIABLE Python_VERSION)
    set(PYTHON_PACKAGE_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/python${Python_VERSION}/site-packages)
else()
    set(PYTHON_PACKAGE_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/python${Python_VERSION}/site-packages)
endif()

## install using the configured install prefix
option (PYTHON_INSTALL_IN_PREFIX
    "Install python binaries using the specified install prefix." ON)
if (PYTHON_INSTALL_IN_PREFIX)
    install(TARGETS binding_python_lib DESTINATION ${PYTHON_PACKAGE_INSTALL_DIR}/libsbne )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libsbne.py  DESTINATION ${PYTHON_PACKAGE_INSTALL_DIR}/libsbne)

    configure_file(setup.py.cmake setup.py)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/setup.py DESTINATION ${PYTHON_PACKAGE_INSTALL_DIR}/libsbne)
endif(PYTHON_INSTALL_IN_PREFIX)

## copy lib dependencies to the build and install directory (for windows)
if (SBNE_WIN_COPY_DEPS_TO_INSTALL)
	add_custom_command(TARGET binding_python_lib POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBSBML_PREFIX}/bin/libsbml.dll 	$<TARGET_FILE_DIR:binding_python_lib>)

	if (PYTHON_INSTALL_IN_PREFIX)
		install(FILES ${LIBSBML_PREFIX}/bin/libsbml.dll DESTINATION 				${PYTHON_PACKAGE_INSTALL_DIR}/libsbne)
	endif()
endif()

