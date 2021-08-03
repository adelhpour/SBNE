set(CMAKE_INCLUDE_CURRENT_DIR ON)

### files
## sources
set(SBNE_SOURCES 
    sbne/core/ne_core.cpp
    sbne/layout/ne_autoLayout.cpp
    sbne/layout/ne_layoutPoint.cpp
    sbne/layout/ne_layoutBox.cpp
    sbne/render/ne_relAbsVector.cpp
    sbne/render/ne_renderPoint.cpp
    sbne/math/ne_transform.cpp
    sbne/network/ne_network.cpp
    sbne/veneer/ne_veneer.cpp
    sbne/sbml/ne_sbml.cpp
    sbne/sbml/ne_layout.cpp
    sbne/sbml/ne_render.cpp
    )

## headers
set(SBNE_HEADERS 
    sbne/core/ne_core.h
    sbne/layout/ne_autoLayout.h
    sbne/layout/ne_layoutPoint.h
    sbne/layout/ne_layoutBox.h
    sbne/render/ne_relAbsVector.h
    sbne/render/ne_renderPoint.h
    sbne/math/ne_transform.h
    sbne/network/ne_network.h
    sbne/veneer/ne_veneer.h
    sbne/sbml/ne_sbml.h
    sbne/sbml/ne_layout.h
    sbne/sbml/ne_render.h
    )

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

		foreach(lib1fullpath IN LISTS GRAPHVIZ_LIBS)
			get_filename_component(lib1name ${lib1fullpath} NAME)
			add_custom_command(TARGET sbne POST_BUILD
					COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id
					"@rpath/${lib1name}"
					"${DEPENDENCIES_DIR}/${lib1name}")			

			foreach(lib2fullpath IN LISTS GRAPHVIZ_LIBS)
				get_filename_component(lib2name ${lib2fullpath} NAME)				
				add_custom_command(TARGET sbne POST_BUILD
					COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change
					"${lib1fullpath}" "@rpath/${lib1name}"
					 "${DEPENDENCIES_DIR}/${lib2name}")
			endforeach()

			foreach(lib2fullpath IN LISTS GRAPHVIZ_APIS)
				get_filename_component(lib2name ${lib2fullpath} NAME)
				add_custom_command(TARGET sbne POST_BUILD
					COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id
					"@rpath/${lib2name}"
					"${DEPENDENCIES_DIR}/graphviz/${lib2name}")
				
				add_custom_command(TARGET sbne POST_BUILD
					COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change
					"${lib1fullpath}" "@rpath/${lib1name}"
					 "${DEPENDENCIES_DIR}/graphviz/${lib2name}")
			endforeach()
			
			#set(GRAPHVIZ_LIBS_MODIFIED ${GRAPHVIZ_LIBS_MODIFIED}
			#"${DEPENDENCIES_DIR}/${lib1name}")
		endforeach()

		add_custom_command(TARGET sbne POST_BUILD
			COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change
			"/usr/local/opt/graphviz/lib/libgvc.6.dylib"
			"@rpath/libgvc.6.dylib" $<TARGET_FILE:sbne>
			COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change
			"/usr/local/opt/graphviz/lib/libcgraph.6.dylib"
			"@rpath/libcgraph.6.dylib" $<TARGET_FILE:sbne>
			COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change
			"/usr/local/opt/graphviz/lib/libcdt.5.dylib"
			"@rpath/libcdt.5.dylib" $<TARGET_FILE:sbne>)
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