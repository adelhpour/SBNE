## file from the qt5_wrap_ui call below
set(CMAKE_INCLUDE_CURRENT_DIR ON)

## set the required sources and headers
set(gui_sources ne_mainwindow.cpp ne_main.cpp ne_SBMLDocument.cpp ne_featureMenu.cpp)
set(gui_headers ne_mainwindow.h ne_featureMenu.h)
set(gui_ui ne_mainwindow.ui)

## wrap the ui files to a c++ header
qt5_wrap_ui(ui_wrap ${gui_ui})
qt5_wrap_cpp(moc_sources ${gui_headers})

## add other headers
set(gui_headers ${gui_headers} ne_SBMLDocument.h)

## set include directories
include_directories (
	${SBNE_ROOT_SOURCE_DIR}/src
	${SBNE_ROOT_BINARY_DIR}/src)

## create the executable
add_executable(SBNE-GUI WIN32 MACOSX_BUNDLE 
	${gui_sources} 
	${gui_headers}
	${ui_wrap} 
	${moc_sources})

## link libraries
target_link_libraries(SBNE-GUI PUBLIC
	Qt5::Core
	Qt5::Gui 
	Qt5::Widgets
	PRIVATE
	sbne-static
	${LIBSBML_LIBRARY})

## set the info plist for macOS	
if (APPLE)
	set_target_properties(SBNE-GUI PROPERTIES MACOSX_BUNDLE_INFO_PLIST 			${SBNE_ROOT_SOURCE_DIR}/src/UI/Info.Plist)
endif()

## set the directory of the dependencies as full path in macOS
if (APPLE)
	add_custom_command(TARGET SBNE-GUI 
    	POST_BUILD COMMAND 
    	${CMAKE_INSTALL_NAME_TOOL} -change "libsbml.5.dylib"					"${LIBSBML_LIBDIR}/libsbml.5.dylib"
    	$<TARGET_FILE:SBNE-GUI>)
endif()

## install the executable to the install directory
install(TARGETS SBNE-GUI DESTINATION bin)

include (InstallRequiredSystemLibraries)

## copy lib dependencies to the build and install directory (for windows)
if (SBNE_WIN_COPY_DEPS_TO_INSTALL)
	add_custom_command(TARGET SBNE-GUI POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> 			$<TARGET_FILE_DIR: SBNE-GUI>
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> 			$<TARGET_FILE_DIR: SBNE-GUI>
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> 		$<TARGET_FILE_DIR: SBNE-GUI>
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBSBML_PREFIX}/bin/libsbml.dll 	$<TARGET_FILE_DIR: SBNE-GUI>)

	add_custom_command(TARGET SBNE-GUI POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> 	$<TARGET_FILE_DIR:SBNE-GUI>/platforms/qwindows.dll)
	install(FILES $<TARGET_FILE:Qt5::Core> $<TARGET_FILE:Qt5::Gui> 				$<TARGET_FILE:Qt5::Widgets> ${LIBSBML_PREFIX}/bin/libsbml.dll DESTINATION bin)
	install(FILES $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> 	DESTINATION bin/platforms)
endif()
