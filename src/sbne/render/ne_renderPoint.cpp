#include "sbne/render/ne_renderPoint.h"

namespace sbne {

    void RPoint::setId(const std::string& id) {
        _id = id;
        _isSetId = true;
    }
       
    const std::string& RPoint::getId() const {
        return _id;
    }

    void RPoint::setX(const RAVector& x) {
        _x = x;
    }
    
    const RAVector& RPoint::x() const {
        return _x;
    }
    
    void RPoint::setY(const RAVector& y) {
        _y = y;
    }
    
    const RAVector& RPoint::y() const {
        return _y;
    }

    void RPoint::setZ(const RAVector& z) {
        _z = z;
    }
    
    const RAVector& RPoint::z() const {
        return _z;
    }

    RPoint RPoint::operator = (const RPoint& rP){
        _x = rP.x();
        _y = rP.y();
        _z = rP.z();
        return *this;
    }

	const RPoint RPoint::operator-() const {
		return RPoint(-_x, -_y, -_z);
	}
        
    const std::string RPoint::toString() const {
        std::stringstream ss;
        ss << "[" << _x.toString() << ", " << _y.toString() << ", " << _z.toString() << "]";
        
        return ss.str();
    }
}
