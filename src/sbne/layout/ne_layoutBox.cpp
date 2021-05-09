#include "sbne/layout/ne_layoutBox.h"

namespace sbne {

LBox LBox::operator = (const LBox& b) { _x = b._x; _y = b._y; _z = b._z; _width = b._width; _height = b._height; _depth = b._depth;  return *this;
}

void LBox::setId(const std::string& id) {
    _id = id;
    _isSetId = true;
}
   
const std::string& LBox::getId() const {
    return _id;
}

void LBox::setX(const double& x) {
    _x = x;
}

const double& LBox::x() const {
    return _x;
}

void LBox::setY(const double& y) {
    _y = y;
}

const double& LBox::y() const {
    return _y;
}

void LBox::setZ(const double& z) {
    _z = z;
}

const double& LBox::z() const {
    return _z;
}

void LBox::setWidth(const double& width) {
    _width = width;
}


const double& LBox::width() const {
    return _width;
}

void LBox::setHeight(const double& height) {
    _height = height;
}

const double& LBox::height() const {
    return _height;
}

void LBox::setDepth(const double& depth) {
    _depth = depth;
}

const double& LBox::depth() const {
    return _depth;
}

void LBox::setUpperLeftCorner(const LPoint& p) {
    _x = p.x();
    _y = p.y();
}

const LPoint LBox::upperLeftCorner() const {
    return LPoint(_x, _y, _z);
}

const LPoint LBox::upperRightCorner() const {
    return LPoint(_x + _width, _y, _z);
}

const LPoint LBox::bottomLeftCorner() const {
    return LPoint(_x, _y + _height, _z);
}

const LPoint LBox::bottomRightCorner() const {
    return LPoint(_x + _width, _y + _height, _z);
}

const LPoint LBox::center() const {
    return LPoint(_x + 0.5 * _width, _y + 0.5 * _height, _z + 0.5 * _depth);
}

const LPoint LBox::diagonal() const {
    return LPoint(_width, _height, _depth);
}

const double& LBox::maxDimension() const {
    return _width > _height ? _width : _height;
}

const double& LBox::minDimension() const {
    return _width < _height ? _width : _height;
}

const double LBox::area() const {
    return _width * _height;
}

void LBox::extendWidth(const double& addedWidth) {
    _width += addedWidth;
}

void LBox::extendHeight(const double& addedHeight) {
    _height += addedHeight;
}

void LBox::extendDepth(const double& addedDepth) {
    _depth += addedDepth;
}

void LBox::shiftPosition(const double& dx, const double& dy, const double& dz) {
    _x += dx;
    _y += dy;
    _z += dz;
}

LBox packBoxesIntoOptimalBox(const LPoint& startP, std::vector<LBox*> boxes) {
    struct rightLimit {
        double x;
        double y1, y2;
    };
    struct bottomLimit {
        double x1, x2;
        double y;
    };
    
    LBox optimalBox(0.0, 0.0, 0.0, 0.0);
    if (boxes.size() != 0) {
        // sort the boxes based on their area
        std::sort(boxes.begin(), boxes.end(), [](LBox* a, LBox* b) -> bool { return a->area() > b->area(); });
        
        // pack the biggest box into the optimal box
        std::vector<LBox*>::iterator bIt = boxes.begin();
        (*bIt)->setUpperLeftCorner(startP);
        optimalBox = *(*bIt);
        
        double minArea;
        int minIndex;
        std::vector<rightLimit> rLimits;
        std::vector<rightLimit>::iterator rLIt;
        
        // set the the right side of the optimal box as the first right limit
        rightLimit rLimit, *_rLimit;
        rLimit.x = optimalBox.upperRightCorner().x();
        rLimit.y1 = optimalBox.upperRightCorner().y();
        rLimit.y2 = optimalBox.bottomRightCorner().y();
        rLimits.push_back(rLimit);
        
        // set the the bottom side of the optimal box as the initial bottom limit
        bottomLimit bLimit;
        bLimit.y = optimalBox.bottomLeftCorner().y();
        bLimit.x1 = optimalBox.bottomLeftCorner().x();
        bLimit.x2 = optimalBox.bottomRightCorner().x();
        
        for (bIt = boxes.begin() + 1 ; bIt != boxes.end(); ++bIt) {
            minArea = INT_MAX;
            
            // check all the positions beside the right limits
            for (rLIt = rLimits.begin(); rLIt != rLimits.end(); ++rLIt) {
                if (((*bIt)->height() <= ((*rLIt).y2 - (*rLIt).y1)) &&
                    ((std::max((*rLIt).x - optimalBox.x() + (*bIt)->width(), optimalBox.width())) * optimalBox.height() <= minArea)) {
                    minArea = (std::max((*rLIt).x - optimalBox.x() + (*bIt)->width(), optimalBox.width())) * optimalBox.height();
                    minIndex = rLIt - rLimits.begin();
                }
            }
            
            // check the position below the bottom limit, if it leads to min area, pack the box there
            if (((*bIt)->width() <= (bLimit.x2 - bLimit.x1)) && (((bLimit.y - optimalBox.y()+ (*bIt)->height()) * optimalBox.width()) < minArea)) {
                
                // pack the box below the bottom limit
                (*bIt)->setUpperLeftCorner(LPoint(bLimit.x1, bLimit.y));
                
                // modify the bottom limit
                bLimit.x2 = (*bIt)->x() + (*bIt)->width();
                bLimit.y += (*bIt)->height();
                
                // add a right limit beside the newly packed box
                _rLimit = new rightLimit();
                _rLimit->x = (*bIt)->x() + (*bIt)->width();
                _rLimit->y1 = (*bIt)->y();
                _rLimit->y2 = (*bIt)->y() + (*bIt)->height();
                rLimits.push_back(*_rLimit);
                
                // modify the optimal box dimensions
                optimalBox.setHeight(bLimit.y - optimalBox.y());
                
                minIndex = -1;
            }
            
            // if the box is not packed below the bottom limit
            if (minIndex != -1) {
                rLIt = rLimits.begin() + minIndex;
                
                // pack the box beisde the right limit so that it leads to min area
                (*bIt)->setUpperLeftCorner(LPoint((*rLIt).x, (*rLIt).y1));
                
                // add a right limit beside the newly packed box
                if ((*bIt)->height() < ((*rLIt).y2 - (*rLIt).y1)) {
                    _rLimit = new rightLimit();
                    _rLimit->x = (*rLIt).x;
                    _rLimit->y1 = (*rLIt).y1 + (*bIt)->height();
                    _rLimit->y2 = (*rLIt).y2;
                    rLimits.push_back(*_rLimit);
                }
                
                // modify the right limit beside which the box is packed
                rLIt = rLimits.begin() + minIndex;
                (*rLIt).x = (*rLIt).x + (*bIt)->width();
                (*rLIt).y2 = (*bIt)->y() + (*bIt)->height();
                
                // modfiy the optimal box dimensions
                if ((*rLIt).x > optimalBox.x() + optimalBox.width())
                    optimalBox.setWidth((*rLIt).x - optimalBox.x());
            }
        }
    }
    
    return optimalBox;
}

}
