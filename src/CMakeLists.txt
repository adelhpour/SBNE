## the source files
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

## the headers
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

set(SBNE_SOURCES ${SOURCES})
set(SBNE_HEADERS ${HEADERS})

## libSBNE shared library
## build
add_library(sbne SHARED ${SBNE_SOURCES})

## add include directories
target_include_directories(sbne PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LIBSBML_INCLUDEDIR})

#add libraries
target_link_libraries(sbne PRIVATE 
	${LIBSBML_LIBRARY}
	${LIBSBML_EXTRA_LIBS})

## set the directory of the dependencies as full path in macOS
if(APPLE)
	add_custom_command(TARGET sbne 
    	POST_BUILD COMMAND 
    	${CMAKE_INSTALL_NAME_TOOL} -change "libsbml.5.dylib"					"${LIBSBML_LIBDIR}/libsbml.5.dylib"
    	$<TARGET_FILE:sbne>)
endif()

## libSBNE static library
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

## install
## set library dist
install(TARGETS sbne RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(TARGETS sbne-static RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

## copy headers to include directory
foreach(dir core layout math network render sbml veneer)
    file(GLOB header_files "${CMAKE_CURRENT_SOURCE_DIR}/sbne/${dir}/*.h")
    install(FILES ${header_files} DESTINATION include/sbne/${dir})
endforeach(dir)

## copy lib dependencies to the build and install directory (for windows)
if (SBNE_WIN_COPY_DEPS_TO_INSTALL)
	add_custom_command(TARGET SBNE-GUI POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBSBML_PREFIX}/bin/libsbml.dll 	$<TARGET_FILE_DIR:sbne>)
	install(FILES ${LIBSBML_PREFIX}/bin/libsbml.dll DESTINATION lib)
endif()

## create GUI
if (WITH_GUI)
	add_subdirectory(UI)
endif()

## create language bindings
if (WITH_SWIG)
	add_subdirectory(bindings)
endif()


