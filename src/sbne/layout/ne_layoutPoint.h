#ifndef __NE_LAYOUTPOINT_H_
#define __NE_LAYOUTPOINT_H_

#include "sbne/core/ne_core.h"

namespace sbne {

/// @class LPoint
/// @brief The equivalent class for SBML [Point](http://model.caltech.edu/software/libsbml/5.18.0/docs/formatted/cpp-api/class_point.html) .
class LPoint {
public:
    /// Constructors
    LPoint() {
        _x = 0.0;
        _y = 0.0;
        _z = 0.0;
    }
    
    LPoint(double x, double y, double z = 0.0) {
        _x = x;
        _y = y;
        _z = z;
    }
    
    /// Functions
    // set id
    void setId(const std::string& id);
    
    // get id
    const std::string& getId() const;
    
    // show whether the id is set
    const bool isSetId() const { return _isSetId; }
    
    // set x
    void setX(const double& x);
    
    // get x
    const double& x() const;
    
    // set y
    void setY(const double& y);
    
    // get y
    const double& y() const;
    
    // set z
    void setZ(const double& z);
    
    // get z
    const double& z() const;
    
    // Unary minus
    const LPoint operator-() const;
    
    // Return the magitude of this point as if it were a vector
    const double mag() const;
    
    // equal to another point
    LPoint operator = (const LPoint& p);
    
    // equal adds to another point
    LPoint operator += (const LPoint& p);
    
    // equal sub from another point
    LPoint operator -= (const LPoint& p);
    
    // shift
    void shift(const double& dx, const double& dy, const double& dz = 0);
    
    // show the layout point as a string
    const std::string toString() const;
    
protected:
    std::string _id;
    double _x;
    double _y;
    double _z;
    bool _isSetId;
};

// element-wise min
LPoint pointMin(const LPoint& u, const LPoint& v);

// element-wise max
LPoint pointMax(const LPoint& u, const LPoint& v);

// Translation via another point
LPoint operator- (const LPoint& p, const LPoint& q);
LPoint operator+ (const LPoint& p, const LPoint& q);

// Scalar multiplication
LPoint operator* (const LPoint& p, const double s);
LPoint operator* (const double s, const LPoint& p);

// Scalar division
LPoint operator/ (const LPoint& p, const double s);

}

#endif
