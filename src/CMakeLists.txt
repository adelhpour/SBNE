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
		${GRAPHVIZ_INCLUDE_DIR}
		${SBML_INCLUDE_DIR})

## link libraries
target_link_libraries(sbne
		${GRAPHVIZ_LIBS}
		${SBML_LIBS}
		${LIBZ_LIBS}
		${LIB_POCO_FOUNDATION_LIBS}
		${LIB_POCO_XML_LIBS})

## set library dist
install(TARGETS sbne RUNTIME DESTINATION bin COMPONENT sbne_cpp_api LIBRARY DESTINATION lib COMPONENT sbne_cpp_api ARCHIVE DESTINATION lib COMPONENT sbne_cpp_api)

### static library
if (BUILD_STATIC_LIB)
	## build
	add_library(sbne-static STATIC ${SBNE_SOURCES})

	## include directories
	target_include_directories(sbne-static PRIVATE
			${GRAPHVIZ_INCLUDE_DIR}
			${SBML_INCLUDE_DIR})

	## link libraries
	target_link_libraries(sbne-static
			${GRAPHVIZ_LIBS}
			${SBML_LIBS}
			${LIBZ_LIBS}
			${LIB_POCO_FOUNDATION_LIBS}
			${LIB_POCO_XML_LIBS})

	## set library dist
	install(TARGETS sbne-static RUNTIME DESTINATION bin COMPONENT sbne_cpp_api LIBRARY DESTINATION lib COMPONENT sbne_cpp_api ARCHIVE DESTINATION lib COMPONENT sbne_cpp_api)
endif()

### install
## install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/sbne DESTINATION include COMPONENT sbne_cpp_api)

### build GUI
if (WITH_GUI)
	add_subdirectory(UI)
endif()

### build language bindings
if (WITH_SWIG)
	add_subdirectory(bindings)
endif()