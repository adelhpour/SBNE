************
Installation
************

Build from source
#################

To build SBNE from its source code you need to:

Build libSBML from source (with enabled layout and render packages)
===================================================================
    
As SBNE depends on `libSBML <http://model.caltech.edu/software/libsbml/>`_, first, it is required to build the latest version of libSBML from its `source <http://sourceforge.net/projects/sbml/files/libsbml>`_. In doing so, this `video <https://www.youtube.com/watch?v=e_Lydwzx-Hg>`_ and `instruction page <http://sbml.org/Software/libSBML/5.18.0/docs/cpp-api/libsbml-installation.html>`_ can come in handy for you. As, for SBNE, we need to build libSBML with its Layout and Render extensions, while following these instructions, you must remember to check ``ENABLE_LAYOUT`` and ``ENABLE_RENDER`` boxes in CMake-gui configuration of libSBML.

Build SBNE from source
======================

The steps to build SBNE from its source code are as follows:
      
*  Create a directory for your entire SBNE project. This directory has to be placed in somewhere not admin-blocked and is recommended to have the following file structure:

    .. code-block::

        <root directory>
        ├── build
        ├── install
        ├── source

*  Clone or download `SBNE source <https://github.com/adelhpour/sbne>`_ to `<root directory>/source`.

*  `Download <https://cmake.org/download/>`_ and Install CMake and then open CMake-gui.

    *  Select ``<root directory>/source/sbne-main`` (where the top-level ``CMakeLists.txt`` is located) as the source directory.

    *  Select ``<root directory>/build`` as the build (binary) directory.

    *  Click "Configure" button and choose your compiler.

    *  Set ``CMAKE_INSTALL_PREFIX`` to ``<root directory>/install``.

    *  Set ``LIBSBML_PREFIX`` to the install directory where you installed libSBML libraries in the previous stage (that is probably something like ``<root directory of libSBML>/install``).

    *  Check ``WITH_PYTHON`` box if you want to build the language bindings for Python.
    
        .. note::

            If you check ``WITH_PYTHON`` box at this step, CMake will provide you with ``PYTHON_INSTALL_IN_PREFIX`` and ``PYTHON_INSTALL_WITH_SETUP`` options later. By checking the box associated with the former option, Python binidngs are installed in ``<root directory>/install/lib/pyhton[version]/site-packages/libsbne`` and by checking the box associated with the latter one they will be installed to the standard location for third-party Python modules.

    *  Check ``WITH_GUI`` box if you want to build SBNE-GUI.
    
        .. note::

            To build SBNE-GUI, Qt5 development tools are needed to be already installed on your machine. In that case, if you check ``WITH_GUI`` box at this step, CMake requires you later to set the value of ``Qt5_DIR`` to the directory where ``Qt5Config.cmake`` and other Qt dependencies are located.
    
    *  Click "Configure" button time and again until no red variable row remains.
    
    *  Click "Generate" button.
    
*  Now that you have figured out all the configuration settings, all you need is building the project as follows:

    *  On Windows, you can either click on "Open Project" button on CMake-gui or open the generated ``.sln`` file in ``<root directory>/build`` in Visual Studio. Once Visual Studio is launched, change the configuration to "Release", and build ``ALL-BUILD`` and ``INSTALL``.
    
    *  On macOS and Linux, simply go to ``<root directory>/build`` and run ``make`` and then ``make install`` on terminal.


