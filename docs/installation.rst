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
    
        .. note::

            At this step, CMake requires you to set the value of ``sbml_DIR`` or ``libsbml_DIR`` (depneding on your operationg system) to the directory where ``sbml-config.cmake`` or ``libsbml-config.cmake`` is located under the install directory of libSBML.

    *  Set ``CMAKE_INSTALL_PREFIX`` to ``<root directory>/install``.
    
    *  Check ``USE_GRAPHVIZ`` box if you want your SBNE to layout the network of models by  `Graphviz layout algorithms <https://graphviz.org/about>`_.
    
        .. note::

            To integrate graphivz libraries into SBNE, you must have already installed `Graphviz development tools <https://graphviz.org/download>`_. If you have done so, you can check ``USE_GRAPHVIZ`` box at this step, and then CMake requires you to set the value of ``GRAPHVIZ_PREFIX`` to the root directory where graphviz is installed on your machine.

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
    
Install Python package
######################

The Python bindings of SBNE API can be installed using a "pip install" as a collection of pip packages (*currently on TestPyPI*):

.. code-block:: bash

    $ pip install -i https://test.pypi.org/simple/ libsbne

Download SBNE binaries
######################

Installation with precompiled binaries is provided for users who intend to use SBNE without changing its source code. Using Qt Installer Framework, we have created installers for Microsoft Windows (.exe), macOS (.dmg), and Linux (.run) platforms, which are available in the `Releases page <https://github.com/adelhpour/SBNE/releases>`_ of the repository on GitHub.
