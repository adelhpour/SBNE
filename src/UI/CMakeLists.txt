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

## create the executable
add_executable(SBNE-GUI WIN32 MACOSX_BUNDLE 
	${gui_sources} 
	${gui_headers}
	${ui_wrap} 
	${moc_sources})

## link libraries
target_link_libraries(SBNE-GUI
	Qt5::Core
	Qt5::Gui 
	Qt5::Widgets
	sbne-static
	${LIBSBML_LIBRARY})

## set include directories
target_include_directories(SBNE-GUI PUBLIC
	${CMAKE_SOURCE_DIR}/src
	${LIBSBML_INCLUDEDIR})

if (APPLE)
	## set the info plist for macOS	
	set_target_properties(SBNE-GUI PROPERTIES MACOSX_BUNDLE_INFO_PLIST 			${CMAKE_SOURCE_DIR}/src/UI/Info.Plist)

	add_custom_command(TARGET SBNE-GUI POST_BUILD
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change                                  "libsbml.5.dylib" "@rpath/libsbml.5.dylib" $<TARGET_FILE:SBNE-GUI>
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:SBNE-GUI>/../Frameworks/QtCore.framework/Versions/5/QtCore
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:SBNE-GUI>/../Frameworks/QtGui.framework/Versions/5/QtGui
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:SBNE-GUI>/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::DBus>  $<TARGET_FILE_DIR:SBNE-GUI>/../Frameworks/QtDBus.framework/Versions/5/QtDBus
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::PrintSupport>
            $<TARGET_FILE_DIR:SBNE-GUI>/../Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QCocoaIntegrationPlugin>						$<TARGET_FILE_DIR:SBNE-GUI>/../PlugIns/platforms/libqcocoa.dylib
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QMacStylePlugin>							$<TARGET_FILE_DIR:SBNE-GUI>/../PlugIns/styles/libqmacstyle.dylib)
	install(TARGETS SBNE-GUI BUNDLE DESTINATION bin COMPONENT sbne_gui)
elseif (UNIX)
	install(FILES
	$<TARGET_FILE:Qt5::Core> "$<TARGET_FILE_DIR:Qt5::Core>/libQt5Core.so.5"
	$<TARGET_FILE:Qt5::Gui> "$<TARGET_FILE_DIR:Qt5::Gui>/libQt5Gui.so.5"
	$<TARGET_FILE:Qt5::Widgets> "$<TARGET_FILE_DIR:Qt5::Widgets>/libQt5Widgets.so.5"
	DESTINATION lib COMPONENT sbne_gui)
	install(TARGETS SBNE-GUI DESTINATION bin COMPONENT sbne_gui)
elseif (WIN32)
	add_custom_command(TARGET SBNE-GUI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core>
        $<TARGET_FILE_DIR:SBNE-GUI>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui>
        $<TARGET_FILE_DIR:SBNE-GUI>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets>
        $<TARGET_FILE_DIR:SBNE-GUI>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin>
        $<TARGET_FILE_DIR:SBNE-GUI>/platforms
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWindowsVistaStylePlugin>
        $<TARGET_FILE_DIR:SBNE-GUI>/styles
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core>
        $<TARGET_FILE_DIR:SBNE-GUI>)

	include (InstallRequiredSystemLibraries)
	install(FILES $<TARGET_FILE:Qt5::Core> $<TARGET_FILE:Qt5::Gui>          $<TARGET_FILE:Qt5::Widgets> DESTINATION bin COMPONENT sbne_gui)
	install(FILES $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> DESTINATION bin/platforms   COMPONENT sbne_gui)
	install(FILES $<TARGET_FILE:Qt5::QWindowsVistaStylePlugin> DESTINATION bin/styles       COMPONENT sbne_gui)
	install(TARGETS SBNE-GUI DESTINATION bin COMPONENT sbne_gui)
endif (APPLE)
