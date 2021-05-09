#ifndef __NE_TRANSFORM_H_
#define __NE_TRANSFORM_H_

#include "sbne/core/ne_core.h"

namespace sbne {
    
class Affine2d {
public:
    /// Constructors
    Affine2d() {
        _e[0][0] = 1.; _e[0][1] = 0.; _e[0][2] = 0.;
        _e[1][0] = 0.; _e[1][1] = 1.; _e[1][2] = 0.;
        _e[2][0] = 0.; _e[2][1] = 0.; _e[2][2] = 1.;
    }
    
    Affine2d(double a, double b, double c,
             double u, double v, double w,
             double x, double y, double z) {
        _e[0][0] = a; _e[0][1] = b; _e[0][2] = c;
        _e[1][0] = u; _e[1][1] = v; _e[1][2] = w;
        _e[2][0] = x; _e[2][1] = y; _e[2][2] = z;
    }

    /// Functions
    // Set the value of an element
    void setElement(const int& r, const int& c, const double& value);
    
    // Get access to an element
    const double element(const int& r, const int& c) const;
    
    // Invert
    Affine2d invert() const;
    
    // Det
    double determinent() const;
    
    // Operations:
    
    Affine2d operator*(const double& k) const;
    
    Affine2d operator/(const double& k) const;
    
    Affine2d operator*(const Affine2d& u) const;
    
protected:
    double _e[3][3];
};

}

#endif
