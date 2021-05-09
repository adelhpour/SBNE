# SBNE: SBML Network Editor
SBNE is a network editor tool allowing the users and developers of systems/synthetic biology to straightforwardly read, manipulate, and write to the layout and render extensions of an SBML model using libSBML. Its “autolayout” and “autorender” algorithms can also be implemented to automatically generate all the necessary information about the layout and render extensions of an SBML model as well. It is built on the basis of <a href="https://github.com/sys-bio/sbnw">sbnw</a>.

## Contents
SBNE is made up of a portable library called libSBNE, originally written in C++ with language bindings for Python, and a software tool called SBNE-GUI, a graphical interface enabling users to load, display, edit, and save an SBML network.

## How to Build

### Build libSBML from source (with enabled layout and render packages)
As SBNE depeneds on libSBML library, first, it is required for you to build the latest version of libSBML from its <a href="http://sourceforge.net/projects/sbml/files/libsbml/">source</a>. This <a href="https://www.youtube.com/watch?v=e_Lydwzx-Hg">video</a> and <a href="http://sbml.org/Software/libSBML/5.18.0/docs/cpp-api/libsbml-installation.html">instruction page</a> can come in handy for you to do so. The only point you need to pay attention to, while you are following the instructions, is checking the `ENABLE_LAYOUT` and `ENABLE_RENDER` boxes in CMake-gui configuration of libSBML.

### Build SBNE from source
The steps to build SBNE from its source code are as follows:
* Create a directory for your entire SBNE project. This directory has to be placed in somewhere not admin-blocked and is recommended to have the following file structure:

```
<root directory>
├── build
├── install
├── source
```

* Clone or download <a href="https://github.com/adelhpour/sbne/">SBNE source</a> to the `<root directory>/source`.

* <a href="https://cmake.org/download/">Download</a> and Install CMake and then open CMake-gui.

    + Select `<root directory>/source/sbne-master` (where the top-level `CMakeLists.txt` is located) as the source directory.
    
    + Select `<root directory>/build` as the build (binary) directory.

    + Click "Configure" button and choose your compiler.

    + Set `CMAKE_INSTALL_PREFIX` to `<root directory>/install`.

    + Set `LIBSBML_PREFIX` to the install directory where libSBML libraries were built in the [previous stage](###Build_libSBML_from_source_(with_enabled_layout_and_render_packages)) (that must be something like `<root directory of libSBML>/install`).
    
    + Check `WITH_PYTHON` box if you want to build the language bindings for Python.

    + Check `WITH_GUI` box if you want to build SBNE-GUI.

        - ******NOTE******: To build SBNE-GUI, Qt5 development tools are needed to be already installed on your machine. In that case, if you check `WITH_GUI` box at this step, CMake requires you later to set the value of `Qt5_DIR` to the directory where `Qt5Config.cmake` and other Qt dependencies are located.

    + Click "Configure" button time and again until all red variable rows turn into white.

    + Click "Generate" button.

* Now that you have figured out all the configuration settings, all you need is building the project as follows:

    + On Windows, you can either click on "Open Project" button on CMake-gui or open the generated `.sln` file in  `<root directory>/build` in Visual Studio.  Once the Visual Studio is launched, change the configuration to "Release", and build `ALL-BUILD` and `INSTALL`.

    + On Linux and macOS, simply go to `<root directory>/build` and  run `make` and then `make install`  on terminal.

## How to Use

### Use libSBNE library (C++ API)
To use libSBNE, you first need to:
* [build](##How_to_Build) libSBML and libSBNE. 

* Add libSBNE include directory (`<root directory>/install/include`)  and libSBML include directory (something like `<root directory of libSBML>/install/include`) to the "include directory" of your project.

* Add libSBNE library (in `<root directory>/install/lib` ) and libSBML library  (in somewhere like `<root directory of libSBML>/install/lib`) to the "dependencies" of your project.

Then, include the following headers in your script:
```
    #include "sbne/sbml/ne_sbml.h"
    #include "sbne/sbml/ne_layout.h"
    #include "sbne/sbml/ne_render.h"
```

And now you are able to make use of API functions in your code.

******NOTE******: A simple script which makes use of the API to read the SBML docoument of an SBML (xml) file, (if not included) add layout and render features to it, and finally write it to an SBML (xml) file is included in `<root directory>/source/include/tests`.

### Use Python bindings
To use SBNE Python bindings, you first need to:

* [build](##How_to_Build) libSBML and libSBNE with enabled `WITH_PYTHON` option.

* Add the directory of the built Python library (`<root directory>/install/lib/pyhton[version]/site-packages/libsbne`)  to your `PYTHONPATH`.

By doing so, you are able to import the _libsbne package and make use of API functions in your script.

### Use SBNE-GUI (Graphical Interface)
To use the graphical interface, you need to:

* [build](##How_to_Build) libSBML and libSBNE with enabled `WITH_GUI` option.

* Go to  `<root directory>/install/bin`, and run SBNE-GUI.
