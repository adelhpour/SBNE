%module(directors="1") libsbne

%{
#include "libsbne.h"
%}

#pragma SWIG cpperraswarn=1

/**
 * Ignore operator= and operator<< on all SBNE objects.
 */
%ignore *::operator=;
%ignore *::operator<<;
%ignore operator==;
%ignore operator!=;


/**
 * Wrap these files.
 */
%include "std_string.i"
%include sbne/core/ne_core.h
%include sbne/layout/ne_layoutPoint.h
%include sbne/layout/ne_layoutBox.h
%include sbne/math/ne_transform.h
%include sbne/network/ne_network.h
%include sbne/render/ne_relAbsVector.h
%include sbne/render/ne_renderPoint.h
%include sbne/veneer/ne_veneer.h
%include sbne/layout/ne_autoLayout.h
%include sbne/sbml/ne_sbml.h
%include sbne/sbml/ne_layout.h
%include sbne/sbml/ne_render.h






