#ifndef __NE_RELABSVECTOR_H_
#define __NE_RELABSVECTOR_H_

#include "sbne/ne_core.h"

namespace sbne {

/// @class RAVector
/// @brief The equivalent class for SBML [RelAbsVector](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_rel_abs_vector.html) .
class RAVector {
public:
    /// Constructors
    RAVector() {
        _a = 0.0;
        _r = 0.0;
    }
    
    RAVector(double a, double r) {
        _a = a;
        _r = r;
    }
    
    RAVector(const RAVector& rav) {
        // absolute
        _a = rav.a();
        
        // relative
        _r = rav.r();
    }
    
    /// Functions
    // set abs
    void setA(const double& a);
    
    // get abs
    const double& a() const;
    
    // set rel
    void setR(const double& r);
    
    // get rel
    const double& r() const;
    
    RAVector operator = (const RAVector& r);
    
    const RAVector operator-() const;
    
    // convert to String
    const std::string toString() const;
    
    // extract a relabsvector from a string
    static RAVector* stringToRAVector(const std::string& str);
    
protected:
    double _a;
    double _r;
};

}

#endif
