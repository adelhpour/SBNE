# set CPack variables
string(TOUPPER ${CMAKE_PROJECT_NAME} CPACK_PACKAGE_NAME)
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SBNE: Systems Biology Network Editor")
set(CPACK_PACKAGE_VENDOR "Prof. Herbert Sauro's Lab")

set(CPACK_PACKAGE_VERSION_MAJOR ${SBNE_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${SBNE_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${SBNE_VERSION_PATCH})

set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN TRUE)
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED TRUE)
set(CPACK_COMPONENTS_ALL sbne_gui sbne_cpp_api sbne_python_bindings Unspecified)
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_VERBATIM_VARIABLES TRUE)
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_GENERATOR IFW)
set(CPACK_IFW_VERBOSE ON)


if (WIN32)
	# set the default install directory
	set(CPACK_IFW_TARGET_DIRECTORY "C:/Program Files/${CPACK_PACKAGE_FILE_NAME}")
	
	# set the name of package in start menu
	set(CPACK_IFW_PACKAGE_START_MENU_DIRECTORY CPACK_PACKAGE_NAME)
elseif(UNIX)
	# set the default install directory
	set(CPACK_IFW_TARGET_DIRECTORY "/usr/local/${CPACK_PACKAGE_FILE_NAME}")
endif(WIN32)


find_program(CPACK_IFW_BINARYCREATOR_EXECUTABLE
        NAMES binarycreator binarycreator.exe
        REQUIRED
        COMMENT "absolute path to the binarycreator executable. If you do not have 		binarycreator you will need to install Qt Installer Framework")

include(CPack REQUIRED)
include(CPackIFW REQUIRED)

cpack_add_component(
        sbne_gui
        DISPLAY_NAME "SBNE-GUI"
        DESCRIPTION "Graphical User Interface of SBNE"
    )

cpack_ifw_configure_component(
        sbne_gui
	LICENSES License ${CPACK_RESOURCE_FILE_LICENSE}
        DEFAULT TRUE)

cpack_add_component(
        sbne_cpp_api
        DISPLAY_NAME "C++ API"
        DESCRIPTION "Portable C++ library (C++ API) of SBNE"
    )

cpack_ifw_configure_component(
        sbne_cpp_api
	LICENSES License ${CPACK_RESOURCE_FILE_LICENSE}
        DEFAULT TRUE)

cpack_add_component(
        sbne_python_bindings
        DISPLAY_NAME "Python Bindings"
        DESCRIPTION "Language bindings of the SBNE C++ library for Python"
    )

cpack_ifw_configure_component(
        sbne_python_bindings
	LICENSES License ${CPACK_RESOURCE_FILE_LICENSE}
        DEFAULT TRUE)