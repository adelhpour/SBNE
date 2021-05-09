#ifndef __NE_RENDERPOINT_H_
#define __NE_RENDERPOINT_H_

#include "sbne/render/ne_relAbsVector.h"
#include "sbne/layout/ne_layoutPoint.h"

namespace sbne {
    class RPoint {
    public:
        /// Constructors
        RPoint() {
            _x = RAVector(0., 0.);
            _y = RAVector(0., 0.);
            _z = RAVector(0., 0.);
        }
        
        RPoint(RAVector x, RAVector y, RAVector z = RAVector(0., 0.)) {
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
        void setX(const RAVector& x);
        
        // get x
        const RAVector& x() const;
        
        // set y
        void setY(const RAVector& y);
        
        // get y
        const RAVector& y() const;
        
        // set z
        void setZ(const RAVector& z);
        
        // get z
        const RAVector& z() const;
        
        RPoint operator = (const RPoint& rP);
        
        const RPoint operator-() const;
        
        const std::string toString() const;
        
    protected:
        std::string _id;
        RAVector _x;
        RAVector _y;
        RAVector _z;
        bool _isSetId;
    };
}

#endif
