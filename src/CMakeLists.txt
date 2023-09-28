set(CMAKE_INCLUDE_CURRENT_DIR ON)

### files
## sources
set(SBNE_SOURCES 
    	sbne/ne_core.cpp
    	sbne/layout/ne_autoLayout.cpp
    	sbne/layout/ne_layout.cpp
	sbne/layout/ne_layoutBox.cpp
    	sbne/layout/ne_layoutPoint.cpp
	sbne/layout/ne_network.cpp
    	sbne/render/ne_relAbsVector.cpp
	sbne/render/ne_render.cpp
    	sbne/render/ne_renderPoint.cpp
    	sbne/render/ne_transform.cpp
    	sbne/render/ne_veneer.cpp)

## headers
set(SBNE_HEADERS 
	sbne/ne_core.h
    	sbne/layout/ne_autoLayout.h
    	sbne/layout/ne_layout.h
	sbne/layout/ne_layoutBox.h
    	sbne/layout/ne_layoutPoint.h
	sbne/layout/ne_network.h
    	sbne/render/ne_relAbsVector.h
	sbne/render/ne_render.h
    	sbne/render/ne_renderPoint.h
    	sbne/render/ne_transform.h
    	sbne/render/ne_veneer.h)

### shared library
## build
add_library(sbne SHARED ${SBNE_SOURCES})

## include directories
target_include_directories(sbne PRIVATE
		${GRAPHVIZ_INCLUDEDIR})

## link libraries
target_link_libraries(sbne
		${GRAPHVIZ_LIBS_FIRSTHAND})

if (WIN32)
	target_link_libraries(sbne
			libsbml
			${EXTRA_LIBS})
else ()
	target_link_libraries(sbne
			sbml)
endif ()
	
# copy sbne dependencies to the build directory
if (APPLE)
	set(DEPENDENCIES_DIR ${CMAKE_BINARY_DIR}/src/dependencies)
	add_custom_command(TARGET sbne POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${DEPENDENCIES_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SBML_LIBS}
		${DEPENDENCIES_DIR}
		COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change
		"libsbml.5.dylib"
		"@rpath/libsbml.5.dylib" $<TARGET_FILE:sbne>)

	if (${USE_GRAPHVIZ})
		add_custom_command(TARGET sbne POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory ${DEPENDENCIES_DIR}/graphviz
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GRAPHVIZ_LIBS}
			${DEPENDENCIES_DIR}
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GRAPHVIZ_APIS}
			${DEPENDENCIES_DIR}/graphviz
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GRAPHVIZ_API_CONFIG}
			${DEPENDENCIES_DIR}/graphviz)
	endif(${USE_GRAPHVIZ})
endif(APPLE)

## set library dist
install(TARGETS sbne RUNTIME DESTINATION bin COMPONENT sbne_cpp_api LIBRARY DESTINATION lib COMPONENT sbne_cpp_api ARCHIVE DESTINATION lib COMPONENT sbne_cpp_api)

### static library
if (BUILD_STATIC_LIB)
	## build
	add_library(sbne-static STATIC ${SBNE_SOURCES})

	## include directories
	target_include_directories(sbne-static PRIVATE
			${GRAPHVIZ_INCLUDEDIR})

	## link libraries
	target_link_libraries(sbne-static
			${GRAPHVIZ_LIBS_FIRSTHAND})

	if (WIN32)
		target_link_libraries(sbne-static
				libsbml
				${EXTRA_LIBS})
	else ()
		target_link_libraries(sbne-static
				sbml)
	endif ()

	## set library dist
	install(TARGETS sbne-static RUNTIME DESTINATION bin COMPONENT sbne_cpp_api LIBRARY DESTINATION lib COMPONENT sbne_cpp_api ARCHIVE DESTINATION lib COMPONENT sbne_cpp_api)
endif()

### install
## install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/sbne DESTINATION include COMPONENT sbne_cpp_api
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${SBML_INCLUDEDIR}" DESTINATION include COMPONENT sbne_cpp_api FILES_MATCHING PATTERN "*.h")

## install lib dependencies
if (WIN32)
	include (InstallRequiredSystemLibraries)
	install(FILES ${LIBSBML_DLLS} ${GRAPHVIZ_DLLS} ${GRAPHVIZ_API_CONFIG} DESTINATION bin COMPONENT sbne_cpp_api)
elseif (APPLE)
	install(DIRECTORY ${DEPENDENCIES_DIR} DESTINATION lib COMPONENT sbne_cpp_api)
elseif(UNIX)
	install(FILES ${SBML_LIBS} ${GRAPHVIZ_LIBS} DESTINATION lib/dependencies COMPONENT sbne_cpp_api)
	install(FILES ${GRAPHVIZ_APIS} ${GRAPHVIZ_API_CONFIG} DESTINATION lib/dependencies/graphviz COMPONENT sbne_cpp_api)
endif()

### build GUI
if (WITH_GUI)
	add_subdirectory(UI)
endif()

### build language bindings
if (WITH_SWIG)
	add_subdirectory(bindings)
endif()