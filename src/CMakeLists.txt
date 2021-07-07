### files
## sources
set(SOURCES 
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

set(SBNE_SOURCES ${SOURCES})

## headers
set(HEADERS 
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

set(SBNE_HEADERS ${HEADERS})

### shared library
## build
add_library(sbne SHARED ${SBNE_SOURCES})

## add include directories
target_include_directories(sbne PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
        ${LIBSBML_INCLUDEDIR})

## add libraries
target_link_libraries(sbne
	${LIBSBML_LIBRARY}
	${LIBSBML_EXTRA_LIBS})

### static library
if (BUILD_STATIC_LIB)
	## build
	add_library(sbne-static STATIC ${SBNE_SOURCES})

	## add include directories
	target_include_directories(sbne-static PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
        	${LIBSBML_INCLUDEDIR})

	## add libraries
	target_link_libraries(sbne-static PRIVATE 
		${LIBSBML_STATIC_LIBRARY})
endif()

if (WIN32)
	add_custom_command(TARGET sbne POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBSBML_PREFIX}/bin/libsbml.dll
	$<TARGET_FILE_DIR:sbne>)

elseif (APPLE)
	add_custom_command(TARGET sbne POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBSBML_LIBS}
	$<TARGET_FILE_DIR:sbne>)

	add_custom_command(TARGET sbne POST_BUILD
		COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change
		"libsbml.5.dylib" "@rpath/libsbml.5.dylib"
		$<TARGET_FILE:sbne>)
endif(WIN32)

### install
## set library dist
install(TARGETS sbne RUNTIME DESTINATION bin COMPONENT sbne_cpp_api LIBRARY DESTINATION lib COMPONENT sbne_cpp_api ARCHIVE DESTINATION lib COMPONENT sbne_cpp_api)
install(TARGETS sbne-static RUNTIME DESTINATION bin COMPONENT sbne_cpp_api LIBRARY DESTINATION lib COMPONENT sbne_cpp_api ARCHIVE DESTINATION lib COMPONENT sbne_cpp_api)

## install headers
foreach(dir core layout math network render sbml veneer)
    file(GLOB header_files "${CMAKE_CURRENT_SOURCE_DIR}/sbne/${dir}/*.h")
    install(FILES ${header_files} DESTINATION include/sbne/${dir} COMPONENT sbne_cpp_api)
endforeach(dir)

## install lib dependencies
if (WIN32)
	install(FILES ${LIBSBML_PREFIX}/bin/libsbml.dll DESTINATION bin COMPONENT 		sbne_cpp_api)
else ()
	install(FILES ${LIBSBML_LIBS} DESTINATION lib COMPONENT sbne_cpp_api)
endif(WIN32)

### build GUI
if (WITH_GUI)
	add_subdirectory(UI)
endif()

### build language bindings
if (WITH_SWIG)
	add_subdirectory(bindings)
endif()


