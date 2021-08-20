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
%include "std_unordered_map.i"

namespace std {
    %template(unordered_map_string_string) unordered_map<string, string>;
}

%include sbne/ne_core.h
%include sbne/layout/ne_layoutPoint.h
%include sbne/layout/ne_layoutBox.h
%include sbne/layout/ne_network.h
%include sbne/layout/ne_autoLayout.h
%include sbne/layout/ne_layout.h
%include sbne/render/ne_relAbsVector.h
%include sbne/render/ne_renderPoint.h
%include sbne/render/ne_transform.h
%include sbne/render/ne_veneer.h
%include sbne/render/ne_render.h







