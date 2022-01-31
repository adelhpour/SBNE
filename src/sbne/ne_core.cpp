#include "sbne/ne_core.h"
#include "sbne/layout/ne_layout.h"
#include "sbne/render/ne_render.h"

namespace sbne {

bool compareChar(const char& c1, const char& c2) {
        if (c1 == c2)
            return true;
        else if (std::toupper(c1) == std::toupper(c2))
            return true;
        return false;
}

bool stringCompare(const std::string& str1, const std::string& str2) {
        return ((str1.size() == str2.size()) && std::equal(str1.begin(), str1.end(), str2.begin(), &compareChar));
}

std::string getKeyValue(std::unordered_map<std::string, std::string> info, const std::string& key) {
    std::unordered_map<std::string, std::string>::iterator it;
    for (it = info.begin(); it != info.end(); ++it) {
        if (stringCompare(it->first, key))
            return it->second;
    }
    
    return std::string();
}

bool isNumber(const std::string& str) {
    if (!str.empty())
        return std::regex_match( str, std::regex( ( "((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?" ) ) );
    
    return false;
}

bool isBoolean(const std::string& str) {
    if (stringCompare(str, "true") || stringCompare(str, "false") || stringCompare(str, "0") || stringCompare(str, "1"))
        return true;
    
    return false;
}

bool stringToBoolean(const std::string& str) {
    if (stringCompare(str, "false") || stringCompare(str, "0"))
        return false;
    
    return true;
}

// -- Input --

SBMLDocument* ne_doc_readSBML(const std::string& filename) {
    SBMLDocument* document = readSBMLFromFile(filename.c_str());
    
    if (document) {
        if (!document->getNumErrors())
            return document;
        else {
            std::cerr << "Failed to parse SBML correctly because of : \n";
            for (int i = 0; i < document->getNumErrors(); ++i)
                std::cerr << i + 1 << ": " << document->getError(i)->getMessage() << std::endl;
        }
    }
        
    return document;
}

// -- Output --

int ne_doc_writeSBML(SBMLDocument* doc, const std::string& filename) {
    SBMLWriter writer;
    if (writer.writeSBML(doc, filename.c_str()))
        return 0;
    
    std::cerr << "Failed to save SBML\n";
    return -1;
}

// -- API --

const std::string ne_get(std::unordered_map<std::string, std::string> infoList) {
    SBMLDocument* doc = ne_doc_readSBML(getKeyValue(infoList, "filename"));
    if (doc) {
        std::string value;
        // layout
        LayoutInfo* lInfo = ne_doc_processLayoutInfo(doc);
        if (lInfo) {
            value = ne_net_get(lInfo->net, infoList);
            if (!value.empty())
                return value;
        }
        // render
        RenderInfo* rInfo = ne_doc_processRenderInfo(doc);
        if (rInfo) {
            value = ne_ven_get(rInfo->ven, infoList);
            if (!value.empty())
                return value;
            
            value = ne_ven_get(rInfo->ven, lInfo->net, infoList);
            if (!value.empty())
                return value;
        }
    }
    
    return std::string();
}

int ne_set(std::unordered_map<std::string, std::string> infoList) {
    SBMLDocument* doc = ne_doc_readSBML(getKeyValue(infoList, "filename"));
    
    if (doc) {
        // layout
        LayoutInfo* lInfo = ne_doc_processLayoutInfo(doc);
        if (lInfo && !ne_net_set(lInfo->net, infoList)) {
            doc = ne_doc_populateSBMLdocWithLayoutInfo(doc, lInfo);
            
            if (doc)
                return ne_doc_writeSBML(doc, getKeyValue(infoList, "filename"));
        }
        // render
        RenderInfo* rInfo = ne_doc_processRenderInfo(doc);
        if (rInfo && (!ne_ven_set(rInfo->ven, infoList) || !ne_ven_set(rInfo->ven, lInfo->net, infoList))) {
            doc = ne_doc_populateSBMLdocWithRenderInfo(doc, rInfo);
            
            if (doc)
                return ne_doc_writeSBML(doc, getKeyValue(infoList, "filename"));
        }
    }
    
    return -1;
}

}

