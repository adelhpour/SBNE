#   Sphinx
#
find_package(Python COMPONENTS Interpreter)
if (NOT ${Python_FOUND})
    message(STATUS "Python cannot be found")
endif ()


message(STATUS "Python_EXECUTABLE ${Python_EXECUTABLE}")
get_filename_component(PYTHON_ROOT ${Python_EXECUTABLE} DIRECTORY)
find_program(SPHINX_EXECUTABLE
        NAMES sphinx-build sphinx-build.exe
        PATHS
        "${PYTHON_ROOT}/Scripts/sphinx-build.exe"
        "${PYTHON_ROOT}/Scripts/sphinx-build"
        REQUIRED
        COMMENT
        "absolute path to the sphinx-build executable. If you do not have sphinx-build
you will need to install it. In your conda environment do: $conda activate <env>;
pip install breathe sphinx sphinxcontrib-bibtex sphinx-tabs sphinx_rtd_theme; which sphinx-build. Then copy the value of which sphinx-build into
-DSPHINX_EXECUTABLE"
        )
message(STATUS "SPHINX_EXECUTABLE: ${SPHINX_EXECUTABLE}")


# find Doxygen package
find_package(Doxygen REQUIRED)

# Find all the public headers
file(GLOB_RECURSE SBNE_PUBLIC_HEADERS ${SBNE_ROOT_SOURCE_DIR}/src/sbne/*.h)

set(DOXYGEN_INPUT_DIR "${SBNE_ROOT_SOURCE_DIR}/src/sbne")
set(DOXYGEN_OUTPUT_DIR "${SBNE_ROOT_SOURCE_DIR}/docs/doxygen-output")
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# Replace variables inside @@ with the current values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Doxygen won't create this for us
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

# Only regenerate Doxygen when the Doxyfile or public headers change
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
        	   DEPENDS ${SBNE_PUBLIC_HEADERS} 
        	   COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        	   MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
        	   COMMENT "Generating docs"
        	   VERBATIM)

# Nice named target so we can run the job easily
add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

# Only regenerate Sphinx when:
# - Doxygen has rerun
# - Our doc files have been updated
# - The Sphinx config has been updated
add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
                   COMMAND 
                     ${SPHINX_EXECUTABLE} -b html
                     # Tell Breathe where to find the Doxygen output
                     -Dbreathe_projects.SBNE=${DOXYGEN_OUTPUT_DIR}/xml
                   ${SPHINX_SOURCE} ${SPHINX_BUILD}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   DEPENDS
                   # Other docs files you want to track should go here                   		   ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
                   ${DOXYGEN_INDEX_FILE}
                   MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/conf.py
                   COMMENT "Generating documentation with Sphinx")

# Nice named target so we can run the job easily
add_custom_target(Sphinx ALL DEPENDS ${SPHINX_INDEX_FILE})

# Add an install target to install the docs
include(GNUInstallDirs)
install(DIRECTORY ${SPHINX_BUILD}
DESTINATION ${CMAKE_INSTALL_DOCDIR})