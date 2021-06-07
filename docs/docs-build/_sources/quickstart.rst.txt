***********
Qucik Start
***********

Using portable C++ library (C++ API)
####################################

To use the portable C++ library, called libSBNE, you first need to:

*  :ref:`Build <installation:Build from Source>` libSBML and libSBNE from source.

*  Add libSBNE include directory (``<root directory>/install/include``) and libSBML include directory (probably something like ``<root directory of libSBML>/install/include``) to the "include directory" of your project.

*  Add libSBNE library (in ``<root directory>/install/lib``) and libSBML library (probably in somewhere like ``<root directory of libSBML>/install/lib``) to the "dependencies" of your project.

* Include the following headers in your script:

    .. code-block:: C++

        #include "sbne/sbml/ne_sbml.h"
        #include "sbne/sbml/ne_layout.h"
        #include "sbne/sbml/ne_render.h"
    
Now you are able to make use of the functions of the API in your code.

C++ example
===========

Here is a simple script which makes use of the API to read the SBML document of an SBML (.xml) file, add Layout and Render features (if not included) to it, and finally write it to the SBML file.

.. code-block:: C++

    #include "sbne/sbml/ne_sbml.h"
    #include "sbne/sbml/ne_layout.h"
    #include "sbne/sbml/ne_render.h"

    using namespace sbne;

    int main () {
        // set inputfile value to the absolute path of the input SBML (.xml) file.
        const std::string inputfile = "Absolute path to the input sbml (.xml) file";
        
        // set outputfile value to the absolute path of the output SBML (.xml) file.
        const std::string outputfile = "Absolute path to the output sbml (.xml) file";

        // read the inputfile and get its SBML document
        SBMLDocument* document = ne_doc_readSBML(inputfile);

        //get layout information (if exists) from the SBML document
        LayoutInfo* li = ne_doc_processLayoutInfo(document);

        // get render information (if exists) from the SBML document
        RenderInfo* ri = ne_doc_processRenderInfo(document);

        // if layout information does not exist
        if (!ne_net_isLayoutSpecified(ne_li_getNetwork(li))) {
            // apply autolayout algorithm
            ne_li_addLayoutFeaturesToNetowrk(li);
        }

        // if render information does not exist
        if (!ne_ven_isRenderSpecified(ne_ri_getVeneer(ri))) {
            // apply autorender algorithm
            ne_ri_addDefaultRenderFeaturesToVeneer(ri);
        }

        // add newly-added or already-included layout information to the SBML document
        document = ne_doc_populateSBMLdocWithLayoutInfo(document, li);
        
        // add newly-added or already-included render information to the SBML document
        document = ne_doc_populateSBMLdocWithRenderInfo(document, ri);

        // write the SBML document to the outputfile
        ne_doc_writeSBML(document, outputfile);
    }

Using Python bindings
#####################

To use the language bindings of SBNE for Python, you first need to:
 
*  :ref:`Build <installation:Build from Source>` libSBML and SBNE with enabled ``WITH_PYTHON`` option.

.. note::

    While building SBNE, if you just enabled ``PYTHON_INSTALL_IN_PREFIX`` option, here you need to add the directory of the built Python library (``<root directory>/install/lib/pyhton[version]/site-packages/libsbne``) to your ``PYTHONPATH``. In case you enabled ``PYTHON_INSTALL_WITH_SETUP`` option, no directory is needed to be added.
    
* ``import _libsbne`` in your script.

Now, you can make use of the functions of the API in your Python script.

Python example
==============

Here is a simple script which makes use of the Python bindings of the API to read the SBML document of an SBML (.xml) file, add Layout and Render features (if not included) to it, and finally write it to the SBML file.

.. code-block:: Python

    import _libsbne as sbne

    # set inputfile value to the absolute path of the input SBML (.xml) file.
    inputfile = "Absolute path to the input sbml (.xml) file"

    # set outputfile value to the absolute path of the output SBML (.xml) file.
    outputfile = "Absolute path to the output sbml (.xml) file"

    # read the inputfile and get its SBML document
    document = sbne.ne_doc_readSBML(inputfile)

    # get layout information (if exists) from the SBML document
    layoutinfo = sbne.ne_doc_processLayoutInfo(document)

    # get render information (if exists) from the SBML document
    renderinfo = sbne.ne_doc_processRenderInfo(document)

    # if layout information does not exist
    if not sbne.ne_net_isLayoutSpecified(sbne.ne_li_getNetwork(layoutinfo)):
        # apply autolayout algorithm
        sbne.ne_li_addLayoutFeaturesToNetowrk(layoutinfo)

    # if render information does not exist
    if not sbne.ne_ven_isRenderSpecified(sbne.ne_ri_getVeneer(renderinfo)):
        # apply autorender algorithm
        sbne.ne_ri_addDefaultRenderFeaturesToVeneer(renderinfo)

    # add newly-added or already-included layout information to the SBML document
    document = sbne.ne_doc_populateSBMLdocWithLayoutInfo(document, layoutinfo)

    # add newly-added or already-included render information to the SBML document
    document = sbne.ne_doc_populateSBMLdocWithRenderInfo(document, renderinfo)

    # write the SBML document to the outputfile
    sbne.ne_doc_writeSBML(document, outputfile)

Using SBNE-GUI (Graphical User Interface)
#########################################

To use the graphical user interface of SBNE, you need to:

* :ref:`Build <installation:Build from Source>` libSBML and libSBNE with enabled ``WITH_GUI`` option.
* Go to ``<root directory>/install/bin``.
* Run SBNE-GUI.

GUI example
===========

Here is a simple example to show how to:

* Read the SBML document of an SBML (.xml) file.
* Add Layout and Render features (if not included) to it.
* Render a biological network for the SBML model using its Layout and Render information.
* Make a change to the network of the model.
* Write the modified SBML document to an SBML file and save it.
 
To do so, once we run SBNE-GUI, we need to load an SBML file. Here, we click on "File" in the menu bar, and then click on the "Open" action.

.. figure:: images/quickstart/1.png
    :width: 400
    :alt: Alternative text
    :align: center
    
    Loading an SBML file into the GUI.

Using the popped-up file dialog, we choose an SBML (*Example.xml*) file and open it. This action reads the SBML document in the file, (if not included) generates the Layout and Render information for the model, and renders the network of the model using this information.

.. figure:: images/quickstart/2.png
    :width: 800
    :alt: Alternative text
    :align: center

    Rendered network of the SBML model.

Now, to become able to modify the features of the rendered network, we need to **double-click on the graphical item whose features we want to change**. Once it is done, a feature menu enabling us to make changes to that graphical item pops up at the left side of the GUI screen (here, we have double-clicked on the Species named *S1*, so an "Item Features" menu for "Species" has appeared). To close this menu, we can simply double-click on an empty zone within the graphics scene of the GUI.

.. figure:: images/quickstart/3.png
    :width: 800
    :alt: Alternative text
    :align: center

    Getting access to the "Item Features" menu for "Species".

As an example, here we want to change the *fill color* of the graphical item representing the *S1* species. So, from the "Item Features" menu, we first expand the **Render Features** submenu, and then, under the **Fill** section, click on the **Color** button. A "Change Fill Color" dialog box, which enables us to choose a new color, pops up.

.. figure:: images/quickstart/4.png
    :width: 800
    :alt: Alternative text
    :align: center

    Getting access to the "Change Fill Color" dialog box.

Then, we click on the "Fill Color" drop-down list and choose a color ("red") from the list of available colors.

.. figure:: images/quickstart/5.png
    :width: 800
    :alt: Alternative text
    :align: center

    Choosing a new color using "Change Fill Color" dialog box.

After we choose our desired color, we click on the "Apply" button in the "Change Fill Color" dialog box, so the *fill color* of the graphical item representing the *S1* species turns into the chosen color.

.. figure:: images/quickstart/6.png
    :width: 800
    :alt: Alternative text
    :align: center
 
    Modified network of the SBML model (the *fill color* of the graphical item representing the *S1* species has turned into "red").

To write the modified features to the existing SBML model file and save it, we click on "File" in the menu bar, and then click on the "Save" action. Using the popped-up file dialog, we can choose a name and directory for the SBML (.xml) file and save it. The GUI also allows us to export a *.jpg* image of the rendered network by clicking on the "Export as JPEG" action under the "File" menu bar.

.. figure:: images/quickstart/7.png
    :width: 400
    :alt: Alternative text
    :align: center

    Saving the modified SBML model and exporting an image of its rendered network.
