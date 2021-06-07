#ifndef __NE_RELABSVECTOR_H_
#define __NE_RELABSVECTOR_H_

#include "sbne/core/ne_core.h"

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
    
    /// Convert to String
    const std::string toString() const;
    
protected:
    double _a;
    double _r;
};
}

#endif
