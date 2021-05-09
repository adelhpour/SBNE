#ifndef __NE_LAYOUTBOX_H_
#define __NE_LAYOUTBOX_H_

#include "sbne/core/ne_core.h"
#include "sbne/layout/ne_layoutPoint.h"

namespace sbne {

class LBox {
public:
    /// Constructors
    LBox() {
        _x = 0.0;
        _y = 0.0;
        _z = 0.0;
        _width = 0.0;
        _height = 0.0;
        _depth = 0.0;
    }
    
    LBox(const LBox& box)
    : _x(box.x()), _y(box.y()), _z(box.z()), _width(box.width()), _height(box.height()), _depth(box.depth()) {
    }

    LBox(const LPoint& min, const LPoint& max)
    : _x(min.x()), _y(min.y()), _z(min.z()), _width(max.x() - min.x()), _height(max.y() - min.y()), _depth(max.z() - min.z()) {
        if (min.x() > max.x() || min.y() > max.y() || min.z() > max.z())
            std::cerr << "Min/max mismatch\n";
    }

    LBox(const double& x, const double& y, const double& z,const double& width, const double& height, const double& depth)
    : _x(x), _y(y), _z(z), _width(width), _height(height), _depth(depth) {}
    
    LBox(const double& x, const double& y,const double& width, const double& height)
    : _x(x), _y(y), _z(0.0), _width(width), _height(height), _depth(0.0) {}
        
    /// Functions
    LBox operator = (const LBox& b);
    
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
    
    // set width
    void setWidth(const double& width);
    
    // get width
    const double& width() const;
    
    // set height
    void setHeight(const double& height);
    
    // get height
    const double& height() const;
    
    // set depth
    void setDepth(const double& depth);
    
    // get depth
    const double& depth() const;
    
    // set upper-left corner
    void setUpperLeftCorner(const LPoint& p);
    
    // get upper-left corner
    const LPoint upperLeftCorner() const;
    
    // get upper-right corner
    const LPoint upperRightCorner() const;
    
    // get bottom-left corner
    const LPoint bottomLeftCorner() const;
    
    // get bottom-right corner
    const LPoint bottomRightCorner() const;
    
    // get center
    const LPoint center() const;
    
    // diogonal
    const LPoint diagonal() const;
    
    // get maximum dimension
    const double& maxDimension() const;
    
    // get minimum dimension
    const double& minDimension() const;
    
    // get the area
    const double area() const;
    
    // add the value to the width
    void extendWidth(const double& addedWidth);

    // add the value to the height
    void extendHeight(const double& addedHeight);

    // add the vlaue to the depth
    void extendDepth(const double& addedDepth);
    
    // shift position
    void shiftPosition(const double& dx, const double& dy, const double& dz = 0);
     
protected:
    std::string _id;
    double _x;
    double _y;
    double _z;
    double _width;
    double _height;
    double _depth;
    bool _isSetId;
};

LBox packBoxesIntoOptimalBox(const LPoint& startP, std::vector<LBox*> boxes);

}

#endif
