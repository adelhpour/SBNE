*************
API Reference
*************

As mentioned :ref:`here <quickstart:Using portable C++ library (C++ API)>`, by making use of the features of C++ API, a user can simply load the SBML document of an SBML file, automatically add Layout and Render features to it, straightforwardly make any change to these features, and finally write the added/modified features to the SBML file. In this regard, instructions on how to effectively use each component of C++ API, including its  **Enumeration**, **Classes**, **Structs**, and **Functions**, are provided here:

Enumerations
############

The enumeration related to **Layout extension**:

.. toctree::
    :maxdepth: 1

    enumerations/layout/GrphObjType.rst
    enumerations/layout/RxnRoleType.rst

The enumeration related to **Render extension**:

.. toctree::
    :maxdepth: 1

    enumerations/render/ColorString.rst
    enumerations/render/GradientSpreadMethod.rst
    enumerations/render/StyleType.rst
    enumerations/render/FillRule.rst
    enumerations/render/FontFamily.rst
    enumerations/render/FontStyle.rst
    enumerations/render/FontWeight.rst
    enumerations/render/HTextAnchor.rst
    enumerations/render/VTextAnchor.rst
    enumerations/render/RenderGroupElementShape.rst
    
Classes and Structs
###################

The classes and structs related to **Layout extension**:

.. toctree::
    :maxdepth: 1
    
    structs/layout/LayoutInfo.rst
    classes/layout/NetworkElement.rst
    classes/layout/Network.rst
    classes/layout/NGraphicalObject.rst
    classes/layout/NCompartment.rst
    classes/layout/NSpecies.rst
    classes/layout/NReaction.rst
    classes/layout/NSpeciesReference.rst
    classes/layout/NText.rst
    classes/layout/LCurve.rst
    classes/layout/LLineSegment.rst
    classes/layout/LCubicBezier.rst
    classes/layout/LBox.rst
    classes/layout/LPoint.rst
    
The classes and structs related to **Render extension**:

.. toctree::
    :maxdepth: 1

    structs/render/RenderInfo.rst
    classes/render/VeneerElement.rst
    classes/render/Veneer.rst
    classes/render/VColorDefinition.rst
    classes/render/VGradientBase.rst
    classes/render/VLinearGradient.rst
    classes/render/VRadialGradient.rst
    classes/render/VGradientStop.rst
    classes/render/VGlobalStyle.rst
    classes/render/VLocalStyle.rst
    classes/render/VLineEnding.rst
    classes/render/VTransformation2D.rst
    classes/render/VGraphicalPrimitive1D.rst
    classes/render/VGraphicalPrimitive2D.rst
    classes/render/VRenderGroup.rst
    classes/render/VImage.rst
    classes/render/RCurve.rst
    classes/render/VText.rst
    classes/render/VRectangle.rst
    classes/render/VEllipse.rst
    classes/render/VPolygon.rst
    classes/render/RenPoint.rst
    classes/render/RCubicBezier.rst
    classes/render/RAVector.rst
    classes/render/RPoint.rst

Functions
#########

The **Get** and **Set** functions:

.. toctree::
    :maxdepth: 1

    functions/ne_get.rst
    functions/ne_set.rst

The functions related to `SBMLDocument <http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_s_b_m_l_document.html>`_:

.. toctree::
    :maxdepth: 1

    functions/SBMLDocument/ne_doc_readSBML.rst
    functions/SBMLDocument/ne_doc_writeSBML.rst
    functions/SBMLDocument/ne_doc_processLayoutInfo.rst
    functions/SBMLDocument/ne_doc_populateSBMLdocWithLayoutInfo.rst
    functions/SBMLDocument/ne_doc_processRenderInfo.rst
    functions/SBMLDocument/ne_doc_populateSBMLdocWithRenderInfo.rst

The functions related to :ref:`APIReference/structs/layout/LayoutInfo:LayoutInfo`:

.. toctree::
    :maxdepth: 1

    functions/LayoutInfo/ne_li_setNetwork.rst
    functions/LayoutInfo/ne_li_getNetwork.rst
    functions/LayoutInfo/ne_li_addLayoutFeaturesToNetowrk.rst

The functions related to :ref:`APIReference/classes/layout/NetworkElement:NetworkElement`:

.. toctree::
    :maxdepth: 1

    functions/NetworkElement/ne_ne_isSetId.rst
    functions/NetworkElement/ne_ne_getId.rst
    functions/NetworkElement/ne_ne_isSetName.rst
    functions/NetworkElement/ne_ne_getName.rst

The functions related to :ref:`APIReference/classes/layout/Network:Network`:

.. toctree::
    :maxdepth: 1
    
    functions/Network/ne_net_get.rst
    functions/Network/ne_net_set.rst
    functions/Network/ne_net_isLayoutSpecified.rst
    functions/Network/ne_net_getNumCompartments.rst
    functions/Network/ne_net_getCompartment.rst
    functions/Network/ne_net_getCompartments.rst
    functions/Network/ne_net_getNumSpecies.rst
    functions/Network/ne_net_getSpecies.rst
    functions/Network/ne_net_getSpecies1.rst
    functions/Network/ne_net_getNumReactions.rst
    functions/Network/ne_net_getReaction.rst
    functions/Network/ne_net_getReactions.rst
    functions/Network/ne_net_getText.rst

The functions related to :ref:`APIReference/classes/layout/NGraphicalObject:NGraphicalObject`:

.. toctree::
    :maxdepth: 1

    functions/NGraphicalObject/ne_go_get.rst
    functions/NGraphicalObject/ne_go_set.rst
    functions/NGraphicalObject/ne_go_isSetGlyphId.rst
    functions/NGraphicalObject/ne_go_getGlyphId.rst
    functions/NGraphicalObject/ne_go_setGlyphId.rst
    functions/NGraphicalObject/ne_go_isSetText.rst
    functions/NGraphicalObject/ne_go_getText.rst
    functions/NGraphicalObject/ne_go_setText.rst
    functions/NGraphicalObject/ne_go_isSetBoundingBox.rst
    functions/NGraphicalObject/ne_go_getBoundingBox.rst
    functions/NGraphicalObject/ne_go_setBoundingBox.rst
    functions/NGraphicalObject/ne_go_unSetBoundingBox.rst
    functions/NGraphicalObject/ne_go_getType.rst

The functions related to :ref:`APIReference/classes/layout/NCompartment:NCompartment`:

.. toctree::
    :maxdepth: 1

    functions/NCompartment/ne_cmp_get.rst
    functions/NCompartment/ne_cmp_set.rst

The functions related to :ref:`APIReference/classes/layout/NSpecies:NSpecies`:

.. toctree::
    :maxdepth: 1

    functions/NSpecies/ne_spc_get.rst
    functions/NSpecies/ne_spc_set.rst
    functions/NSpecies/ne_spc_updateBoundingBox.rst
    functions/NSpecies/ne_spc_isSetCompartment.rst
    functions/NSpecies/ne_spc_getCompartment.rst

The functions related to :ref:`APIReference/classes/layout/NReaction:NReaction`:

.. toctree::
    :maxdepth: 1

    functions/NReaction/ne_rxn_get.rst
    functions/NReaction/ne_rxn_set.rst
    functions/NReaction/ne_rxn_getNumSpeciesReferences.rst
    functions/NReaction/ne_rxn_getSpeciesReference.rst
    functions/NReaction/ne_rxn_getSpeciesReferences.rst
    functions/NReaction/ne_rxn_isSetCurve.rst
    functions/NReaction/ne_rxn_getCurve.rst
    functions/NReaction/ne_rxn_setCurve.rst
    functions/NReaction/ne_rxn_unSetCurve.rst
    functions/NReaction/ne_rxn_isSetExtentBox.rst
    functions/NReaction/ne_rxn_getExtentBox.rst

The functions related to :ref:`APIReference/classes/layout/NSpeciesReference:NSpeciesReference`:

.. toctree::
    :maxdepth: 1
    
    functions/NSpeciesReference/ne_sr_get.rst
    functions/NSpeciesReference/ne_sr_set.rst
    functions/NSpeciesReference/ne_sr_isSetReaction.rst
    functions/NSpeciesReference/ne_sr_getReaction.rst
    functions/NSpeciesReference/ne_sr_isSetSpecies.rst
    functions/NSpeciesReference/ne_sr_getSpecies.rst
    functions/NSpeciesReference/ne_sr_isSetRole.rst
    functions/NSpeciesReference/ne_sr_getRoleAsString.rst
    functions/NSpeciesReference/ne_sr_isSetCurve.rst
    functions/NSpeciesReference/ne_sr_getCurve.rst

The functions related to :ref:`APIReference/classes/layout/NText:NText`:

.. toctree::
    :maxdepth: 1

    functions/NText/ne_gtxt_get.rst
    functions/NText/ne_gtxt_set.rst
    functions/NText/ne_gtxt_isSetPlainText.rst
    functions/NText/ne_gtxt_getPlainText.rst
    functions/NText/ne_gtxt_setPlainText.rst
    functions/NText/ne_gtxt_unSetPlainText.rst

The functions related to :ref:`APIReference/classes/layout/LCurve:LCurve`:

.. toctree::
    :maxdepth: 1

    functions/LCurve/ne_crv_get.rst
    functions/LCurve/ne_crv_set.rst
    functions/LCurve/ne_crv_getNumElements.rst
    functions/LCurve/ne_crv_getElement.rst
    functions/LCurve/ne_crv_getElements.rst
    functions/LCurve/ne_crv_addElement.rst
    functions/LCurve/ne_crv_removeElement.rst
    functions/LCurve/ne_crv_getExtentBox.rst

The functions related to :ref:`APIReference/classes/layout/LLineSegment:LLineSegment`:

.. toctree::
    :maxdepth: 1

    functions/LLineSegment/ne_ls_get.rst
    functions/LLineSegment/ne_ls_set.rst
    functions/LLineSegment/ne_ls_isSetStart.rst
    functions/LLineSegment/ne_ls_getStart.rst
    functions/LLineSegment/ne_ls_setStart.rst
    functions/LLineSegment/ne_ls_isSetEnd.rst
    functions/LLineSegment/ne_ls_getEnd.rst
    functions/LLineSegment/ne_ls_setEnd.rst
    functions/LLineSegment/ne_ls_isCubicBezier.rst

The functions related to :ref:`APIReference/classes/layout/LCubicBezier:LCubicBezier`:

.. toctree::
    :maxdepth: 1

    functions/LCubicBezier/ne_cb_isSetBasePoint1.rst
    functions/LCubicBezier/ne_cb_getBasePoint1.rst
    functions/LCubicBezier/ne_cb_setBasePoint1.rst
    functions/LCubicBezier/ne_cb_isSetBasePoint2.rst
    functions/LCubicBezier/ne_cb_getBasePoint2.rst
    functions/LCubicBezier/ne_cb_setBasePoint2.rst

The functions related to :ref:`APIReference/classes/layout/LBox:LBox`:

.. toctree::
    :maxdepth: 1

    functions/LBox/ne_bb_get.rst
    functions/LBox/ne_bb_set.rst
    functions/LBox/ne_bb_getX.rst
    functions/LBox/ne_bb_setX.rst
    functions/LBox/ne_bb_getY.rst
    functions/LBox/ne_bb_setY.rst
    functions/LBox/ne_bb_getWidth.rst
    functions/LBox/ne_bb_setWidth.rst
    functions/LBox/ne_bb_getHeight.rst
    functions/LBox/ne_bb_setHeight.rst

The functions related to :ref:`APIReference/classes/layout/LPoint:LPoint`:

.. toctree::
    :maxdepth: 1

    functions/LPoint/ne_point_get.rst
    functions/LPoint/ne_point_set.rst
    functions/LPoint/ne_point_getX.rst
    functions/LPoint/ne_point_setX.rst
    functions/LPoint/ne_point_getY.rst
    functions/LPoint/ne_point_setY.rst

The functions related to :ref:`APIReference/structs/render/RenderInfo:RenderInfo`:

.. toctree::
    :maxdepth: 1

    functions/RenderInfo/ne_ri_setVeneer.rst
    functions/RenderInfo/ne_ri_getVeneer.rst
    functions/RenderInfo/ne_ri_addDefaultRenderFeaturesToVeneer.rst

The functions related to :ref:`APIReference/classes/render/VeneerElement:VeneerElement`:

.. toctree::
    :maxdepth: 1

    functions/VeneerElement/ne_ve_isSetId.rst
    functions/VeneerElement/ne_ve_getId.rst
    functions/VeneerElement/ne_ve_setId.rst
    functions/VeneerElement/ne_ve_isSetName.rst
    functions/VeneerElement/ne_ve_getName.rst
    functions/VeneerElement/ne_ve_setName.rst

The functions related to :ref:`APIReference/classes/render/Veneer:Veneer`:

.. toctree::
    :maxdepth: 1

    functions/Veneer/ne_ven_get1.rst
    functions/Veneer/ne_ven_get2.rst
    functions/Veneer/ne_ven_set1.rst
    functions/Veneer/ne_ven_set2.rst
    functions/Veneer/ne_ven_isRenderSpecified.rst
    functions/Veneer/ne_ven_isSetBackgroundColor.rst
    functions/Veneer/ne_ven_getBackgroundColor.rst
    functions/Veneer/ne_ven_setBackgroundColor.rst
    functions/Veneer/ne_ven_getNumColors.rst
    functions/Veneer/ne_ven_getColor.rst
    functions/Veneer/ne_ven_getColors.rst
    functions/Veneer/ne_ven_addNewColor.rst
    functions/Veneer/ne_ven_getNumGradients.rst
    functions/Veneer/ne_ven_getGradient.rst
    functions/Veneer/ne_ven_getGradients.rst
    functions/Veneer/ne_ven_getNumLineEndings.rst
    functions/Veneer/ne_ven_getLineEnding.rst
    functions/Veneer/ne_ven_getLineEndings.rst
    functions/Veneer/ne_ven_addNewLineEnding.rst
    functions/Veneer/ne_ven_getLocalLineEndingFromGlobalLineEnding.rst
    functions/Veneer/ne_ven_getNumStyles.rst
    functions/Veneer/ne_ven_getStyle.rst
    functions/Veneer/ne_ven_getStyles.rst
    functions/Veneer/ne_ven_addNewGlobalStyle.rst
    functions/Veneer/ne_ven_addNewLocalStyle.rst
    functions/Veneer/ne_ven_findStyle.rst
    functions/Veneer/ne_ven_getLocalStyleFromGlobalStyle.rst

The functions related to :ref:`APIReference/classes/render/VColorDefinition:VColorDefinition`:

.. toctree::
    :maxdepth: 1

    functions/VColorDefinition/ne_clr_get.rst
    functions/VColorDefinition/ne_clr_set.rst
    functions/VColorDefinition/ne_clr_isSetValue.rst
    functions/VColorDefinition/ne_clr_getValue.rst
    functions/VColorDefinition/ne_clr_setValue.rst

The functions related to :ref:`APIReference/classes/render/VGradientBase:VGradientBase`:

.. toctree::
    :maxdepth: 1

    functions/VGradientBase/ne_grd_get.rst
    functions/VGradientBase/ne_grd_set.rst
    functions/VGradientBase/ne_grd_isSetSpreadMethod.rst
    functions/VGradientBase/ne_grd_getSpreadMethod.rst
    functions/VGradientBase/ne_grd_getNumStops.rst
    functions/VGradientBase/ne_grd_getStop.rst
    functions/VGradientBase/ne_grd_isLinearGradient.rst
    functions/VGradientBase/ne_grd_isRadialGradient.rst

The functions related to :ref:`APIReference/classes/render/VLinearGradient:VLinearGradient`:

.. toctree::
    :maxdepth: 1
    
    functions/VLinearGradient/ne_grd_isSetX1.rst
    functions/VLinearGradient/ne_grd_getX1.rst
    functions/VLinearGradient/ne_grd_setX1.rst
    functions/VLinearGradient/ne_grd_isSetY1.rst
    functions/VLinearGradient/ne_grd_getY1.rst
    functions/VLinearGradient/ne_grd_setY1.rst
    functions/VLinearGradient/ne_grd_isSetX2.rst
    functions/VLinearGradient/ne_grd_getX2.rst
    functions/VLinearGradient/ne_grd_setX2.rst
    functions/VLinearGradient/ne_grd_isSetY2.rst
    functions/VLinearGradient/ne_grd_getY2.rst
    functions/VLinearGradient/ne_grd_setY2.rst

The functions related to :ref:`APIReference/classes/render/VRadialGradient:VRadialGradient`:

.. toctree::
    :maxdepth: 1

    functions/VRadialGradient/ne_grd_isSetCx.rst
    functions/VRadialGradient/ne_grd_getCx.rst
    functions/VRadialGradient/ne_grd_setCx.rst
    functions/VRadialGradient/ne_grd_isSetCy.rst
    functions/VRadialGradient/ne_grd_getCy.rst
    functions/VRadialGradient/ne_grd_setCy.rst
    functions/VRadialGradient/ne_grd_isSetFx.rst
    functions/VRadialGradient/ne_grd_getFx.rst
    functions/VRadialGradient/ne_grd_setFx.rst
    functions/VRadialGradient/ne_grd_isSetFy.rst
    functions/VRadialGradient/ne_grd_getFy.rst
    functions/VRadialGradient/ne_grd_setFy.rst
    functions/VRadialGradient/ne_grd_isSetR.rst
    functions/VRadialGradient/ne_grd_getR.rst
    functions/VRadialGradient/ne_grd_setR.rst

The functions related to :ref:`APIReference/classes/render/VGradientStop:VGradientStop`:

.. toctree::
    :maxdepth: 1

    functions/VGradientStop/ne_gstp_get.rst
    functions/VGradientStop/ne_gstp_set.rst
    functions/VGradientStop/ne_gstp_isSetOffset.rst
    functions/VGradientStop/ne_gstp_getOffset.rst
    functions/VGradientStop/ne_gstp_setOffset.rst
    functions/VGradientStop/ne_gstp_isSetColor.rst
    functions/VGradientStop/ne_gstp_getColor.rst
    functions/VGradientStop/ne_gstp_setColor.rst

The functions related to :ref:`APIReference/classes/render/VLineEnding:VLineEnding`:

.. toctree::
    :maxdepth: 1

    functions/VLineEnding/ne_le_get.rst
    functions/VLineEnding/ne_le_set.rst
    functions/VLineEnding/ne_le_setLineEndingValues.rst
    functions/VLineEnding/ne_le_isSetBoundingBox.rst
    functions/VLineEnding/ne_le_getBoundingBox.rst
    functions/VLineEnding/ne_le_setBoundingBox.rst
    functions/VLineEnding/ne_le_isSetGroup.rst
    functions/VLineEnding/ne_le_getGroup.rst
    functions/VLineEnding/ne_le_setGroup.rst
    functions/VLineEnding/ne_le_isSetEnableRotation.rst
    functions/VLineEnding/ne_le_getEnableRotation.rst
    functions/VLineEnding/ne_le_enableRotation.rst

The functions related to :ref:`APIReference/classes/render/VGlobalStyle:VGlobalStyle`:

.. toctree::
    :maxdepth: 1

    functions/VGlobalStyle/ne_stl_get.rst
    functions/VGlobalStyle/ne_stl_set.rst
    functions/VGlobalStyle/ne_stl_setStyleValues.rst
    functions/VGlobalStyle/ne_stl_isSetGroup.rst
    functions/VGlobalStyle/ne_stl_getGroup.rst
    functions/VGlobalStyle/ne_stl_setGroup.rst
    functions/VGlobalStyle/ne_stl_addToTypeList.rst
    functions/VGlobalStyle/ne_stl_addToRoleList.rst
    
The functions related to :ref:`APIReference/classes/render/VLocalStyle:VLocalStyle`:

.. toctree::
    :maxdepth: 1

    functions/VLocalStyle/ne_stl_addToIdList.rst
    
The functions related to :ref:`APIReference/classes/render/VRenderGroup:VRenderGroup`:

.. toctree::
    :maxdepth: 1

    functions/VRenderGroup/ne_grp_get.rst
    functions/VRenderGroup/ne_grp_set.rst
    functions/VRenderGroup/ne_grp_isSetStrokeColor.rst
    functions/VRenderGroup/ne_grp_getStrokeColor.rst
    functions/VRenderGroup/ne_grp_setStrokeColor.rst
    functions/VRenderGroup/ne_grp_isSetStrokeWidth.rst
    functions/VRenderGroup/ne_grp_getStrokeWidth.rst
    functions/VRenderGroup/ne_grp_setStrokeWidth.rst
    functions/VRenderGroup/ne_grp_isSetStrokeDashArray.rst
    functions/VRenderGroup/ne_grp_getNumStrokeDashes.rst
    functions/VRenderGroup/ne_grp_getStrokeDash.rst
    functions/VRenderGroup/ne_grp_getStrokeDashArray.rst
    functions/VRenderGroup/ne_grp_setStrokeDashArray.rst
    functions/VRenderGroup/ne_grp_isSetFillColor.rst
    functions/VRenderGroup/ne_grp_getFillColor.rst
    functions/VRenderGroup/ne_grp_setFillColor.rst
    functions/VRenderGroup/ne_grp_isSetFillRule.rst
    functions/VRenderGroup/ne_grp_getFillRule.rst
    functions/VRenderGroup/ne_grp_setFillRule.rst
    functions/VRenderGroup/ne_grp_isSetFontFamily.rst
    functions/VRenderGroup/ne_grp_getFontFamily.rst
    functions/VRenderGroup/ne_grp_setFontFamily.rst
    functions/VRenderGroup/ne_grp_isSetFontSize.rst
    functions/VRenderGroup/ne_grp_getFontSize.rst
    functions/VRenderGroup/ne_grp_setFontSize.rst
    functions/VRenderGroup/ne_grp_isSetFontStyle.rst
    functions/VRenderGroup/ne_grp_getFontStyle.rst
    functions/VRenderGroup/ne_grp_setFontStyle.rst
    functions/VRenderGroup/ne_grp_isSetFontWeight.rst
    functions/VRenderGroup/ne_grp_getFontWeight.rst
    functions/VRenderGroup/ne_grp_setFontWeight.rst
    functions/VRenderGroup/ne_grp_isSetHTextAnchor.rst
    functions/VRenderGroup/ne_grp_getHTextAnchor.rst
    functions/VRenderGroup/ne_grp_setHTextAnchor.rst
    functions/VRenderGroup/ne_grp_isSetVTextAnchor.rst
    functions/VRenderGroup/ne_grp_getVTextAnchor.rst
    functions/VRenderGroup/ne_grp_setVTextAnchor.rst
    functions/VRenderGroup/ne_grp_isSetStartHead.rst
    functions/VRenderGroup/ne_grp_getStartHead.rst
    functions/VRenderGroup/ne_grp_setStartHead.rst
    functions/VRenderGroup/ne_grp_isSetEndHead.rst
    functions/VRenderGroup/ne_grp_getEndHead.rst
    functions/VRenderGroup/ne_grp_setEndHead.rst
    functions/VRenderGroup/ne_grp_getNumGeometricShapes.rst
    functions/VRenderGroup/ne_grp_getGeometricShape.rst
    functions/VRenderGroup/ne_grp_removeGeometricShapes.rst
    functions/VRenderGroup/ne_grp_addGeometricShape.rst
    
The functions related to :ref:`APIReference/classes/render/VTransformation2D:VTransformation2D`:

.. toctree::
    :maxdepth: 1

    functions/VTransformation2D/ne_gs_get.rst
    functions/VTransformation2D/ne_gs_set.rst
    functions/VTransformation2D/ne_gs_getShape.rst
    functions/VTransformation2D/ne_gs_setShapeFeatures.rst
    functions/VTransformation2D/ne_gs_isSetStrokeColor.rst
    functions/VTransformation2D/ne_gs_getStrokeColor.rst
    functions/VTransformation2D/ne_gs_setStrokeColor.rst
    functions/VTransformation2D/ne_gs_isSetStrokeWidth.rst
    functions/VTransformation2D/ne_gs_getStrokeWidth.rst
    functions/VTransformation2D/ne_gs_setStrokeWidth.rst
    functions/VTransformation2D/ne_gs_isSetStrokeDashArray.rst
    functions/VTransformation2D/ne_gs_getNumStrokeDashes.rst
    functions/VTransformation2D/ne_gs_getStrokeDash.rst
    functions/VTransformation2D/ne_gs_getStrokeDashArray.rst
    functions/VTransformation2D/ne_gs_setStrokeDashArray.rst
    functions/VTransformation2D/ne_gs_isSetFillColor.rst
    functions/VTransformation2D/ne_gs_getFillColor.rst
    functions/VTransformation2D/ne_gs_setFillColor.rst
    functions/VTransformation2D/ne_gs_isSetFillRule.rst
    functions/VTransformation2D/ne_gs_getFillRule.rst
    functions/VTransformation2D/ne_gs_setFillRule.rst
    
The functions related to :ref:`APIReference/classes/render/VImage:VImage`:

.. toctree::
    :maxdepth: 1

    functions/VImage/ne_img_get.rst
    functions/VImage/ne_img_set.rst
    functions/VImage/ne_img_isSetPositionX.rst
    functions/VImage/ne_img_getPositionX.rst
    functions/VImage/ne_img_setPositionX.rst
    functions/VImage/ne_img_isSetPositionY.rst
    functions/VImage/ne_img_getPositionY.rst
    functions/VImage/ne_img_setPositionY.rst
    functions/VImage/ne_img_isSetDimensionWidth.rst
    functions/VImage/ne_img_getDimensionWidth.rst
    functions/VImage/ne_img_setDimensionWidth.rst
    functions/VImage/ne_img_isSetDimensionHeight.rst
    functions/VImage/ne_img_getDimensionHeight.rst
    functions/VImage/ne_img_setDimensionHeight.rst
    functions/VImage/ne_img_isSetHref.rst
    functions/VImage/ne_img_getHref.rst
    functions/VImage/ne_img_setHref.rst

The functions related to :ref:`APIReference/classes/render/RCurve:RCurve`:

.. toctree::
    :maxdepth: 1

    functions/RCurve/ne_rc_get.rst
    functions/RCurve/ne_rc_set.rst
    functions/RCurve/ne_rc_getNumVertices.rst
    functions/RCurve/ne_rc_getVertex.rst
    functions/RCurve/ne_rc_addVertex.rst
    functions/RCurve/ne_rc_removeVertex.rst
    functions/RCurve/ne_rc_isSetStartHead.rst
    functions/RCurve/ne_rc_getStartHead.rst
    functions/RCurve/ne_rc_setStartHead.rst
    functions/RCurve/ne_rc_isSetEndHead.rst
    functions/RCurve/ne_rc_getEndHead.rst
    functions/RCurve/ne_rc_setEndHead.rst

The functions related to :ref:`APIReference/classes/render/VText:VText`:

.. toctree::
    :maxdepth: 1

    functions/VText/ne_txt_get.rst
    functions/VText/ne_txt_set.rst
    functions/VText/ne_txt_isSetPositionX.rst
    functions/VText/ne_txt_getPositionX.rst
    functions/VText/ne_txt_setPositionX.rst
    functions/VText/ne_txt_isSetPositionY.rst
    functions/VText/ne_txt_getPositionY.rst
    functions/VText/ne_txt_setPositionY.rst
    functions/VText/ne_txt_isSetFontFamily.rst
    functions/VText/ne_txt_getFontFamily.rst
    functions/VText/ne_txt_setFontFamily.rst
    functions/VText/ne_txt_isSetFontSize.rst
    functions/VText/ne_txt_getFontSize.rst
    functions/VText/ne_txt_setFontSize.rst
    functions/VText/ne_txt_isSetFontStyle.rst
    functions/VText/ne_txt_getFontStyle.rst
    functions/VText/ne_txt_setFontStyle.rst
    functions/VText/ne_txt_isSetFontWeight.rst
    functions/VText/ne_txt_getFontWeight.rst
    functions/VText/ne_txt_setFontWeight.rst
    functions/VText/ne_txt_isSetHTextAnchor.rst
    functions/VText/ne_txt_getHTextAnchor.rst
    functions/VText/ne_txt_setHTextAnchor.rst
    functions/VText/ne_txt_isSetVTextAnchor.rst
    functions/VText/ne_txt_getVTextAnchor.rst
    functions/VText/ne_txt_setVTextAnchor.rst
    
The functions related to :ref:`APIReference/classes/render/VRectangle:VRectangle`:

.. toctree::
    :maxdepth: 1

    functions/VRectangle/ne_rec_get.rst
    functions/VRectangle/ne_rec_set.rst
    functions/VRectangle/ne_rec_isSetPositionX.rst
    functions/VRectangle/ne_rec_getPositionX.rst
    functions/VRectangle/ne_rec_setPositionX.rst
    functions/VRectangle/ne_rec_isSetPositionY.rst
    functions/VRectangle/ne_rec_getPositionY.rst
    functions/VRectangle/ne_rec_setPositionY.rst
    functions/VRectangle/ne_rec_isSetDimensionWidth.rst
    functions/VRectangle/ne_rec_getDimensionWidth.rst
    functions/VRectangle/ne_rec_setDimensionWidth.rst
    functions/VRectangle/ne_rec_isSetDimensionHeight.rst
    functions/VRectangle/ne_rec_getDimensionHeight.rst
    functions/VRectangle/ne_rec_setDimensionHeight.rst
    functions/VRectangle/ne_rec_isSetCornerCurvatureRX.rst
    functions/VRectangle/ne_rec_getCornerCurvatureRX.rst
    functions/VRectangle/ne_rec_setCornerCurvatureRX.rst
    functions/VRectangle/ne_rec_isSetCornerCurvatureRY.rst
    functions/VRectangle/ne_rec_getCornerCurvatureRY.rst
    functions/VRectangle/ne_rec_setCornerCurvatureRY.rst
    functions/VRectangle/ne_rec_isSetRatio.rst
    functions/VRectangle/ne_rec_getRatio.rst
    functions/VRectangle/ne_rec_setRatio.rst
    
The functions related to :ref:`APIReference/classes/render/VEllipse:VEllipse`:

.. toctree::
    :maxdepth: 1

    functions/VEllipse/ne_elp_get.rst
    functions/VEllipse/ne_elp_set.rst
    functions/VEllipse/ne_elp_isSetPositionCX.rst
    functions/VEllipse/ne_elp_getPositionCX.rst
    functions/VEllipse/ne_elp_setPositionCX.rst
    functions/VEllipse/ne_elp_isSetPositionCY.rst
    functions/VEllipse/ne_elp_getPositionCY.rst
    functions/VEllipse/ne_elp_setPositionCY.rst
    functions/VEllipse/ne_elp_isSetDimensionRX.rst
    functions/VEllipse/ne_elp_getDimensionRX.rst
    functions/VEllipse/ne_elp_setDimensionRX.rst
    functions/VEllipse/ne_elp_isSetDimensionRY.rst
    functions/VEllipse/ne_elp_getDimensionRY.rst
    functions/VEllipse/ne_elp_setDimensionRY.rst
    functions/VEllipse/ne_elp_isSetRatio.rst
    functions/VEllipse/ne_elp_getRatio.rst
    functions/VEllipse/ne_elp_setRatio.rst

The functions related to :ref:`APIReference/classes/render/VPolygon:VPolygon`:

.. toctree::
    :maxdepth: 1

    functions/VPolygon/ne_plg_get.rst
    functions/VPolygon/ne_plg_set.rst
    functions/VPolygon/ne_plg_getNumVertices.rst
    functions/VPolygon/ne_plg_getVertex.rst
    functions/VPolygon/ne_plg_addVertex.rst
    functions/VPolygon/ne_plg_removeVertex.rst

The functions related to :ref:`APIReference/classes/render/RenPoint:RenPoint`:

.. toctree::
    :maxdepth: 1
    
    functions/RenPoint/ne_vrx_get.rst
    functions/RenPoint/ne_vrx_set.rst
    functions/RenPoint/ne_vrx_getRenderPoint.rst
    functions/RenPoint/ne_vrx_setRenderPoint.rst
    functions/RenPoint/ne_vrx_isRenderCubicBezier.rst
    functions/RenPoint/ne_vrx_getBasePoint1.rst
    functions/RenPoint/ne_vrx_setBasePoint1.rst
    functions/RenPoint/ne_vrx_getBasePoint2.rst
    functions/RenPoint/ne_vrx_setBasePoint2.rst
    
The functions related to :ref:`APIReference/classes/render/RPoint:RPoint`:

.. toctree::
    :maxdepth: 1

    functions/RPoint/ne_rp_get.rst
    functions/RPoint/ne_rp_set.rst
    functions/RPoint/ne_rp_getX.rst
    functions/RPoint/ne_rp_setX.rst
    functions/RPoint/ne_rp_getY.rst
    functions/RPoint/ne_rp_setY.rst

The functions related to :ref:`APIReference/classes/render/RAVector:RAVector`:

.. toctree::
    :maxdepth: 1

    functions/RAVector/ne_rav_getAbsoluteValue.rst
    functions/RAVector/ne_rav_getRelativeValue.rst
    functions/RAVector/ne_rav_setAbsoluteValue.rst
    functions/RAVector/ne_rav_setRelativeValue.rst
