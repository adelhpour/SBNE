#include "sbne/layout/ne_layoutPoint.h"

namespace sbne {

void LPoint::setId(const std::string& id) {
    _id = id;
    _isSetId = true;
}
   
const std::string& LPoint::getId() const {
    return _id;
}

void LPoint::setX(const double& x) {
    _x = x;
}

const double& LPoint::x() const {
    return _x;
}

void LPoint::setY(const double& y) {
    _y = y;
}

const double& LPoint::y() const {
    return _y;
}

void LPoint::setZ(const double& z) {
    _z = z;
}

const double& LPoint::z() const {
    return _z;
}

const LPoint LPoint::operator-() const {
    return LPoint(-_x, -_y, -_z);
}

const double LPoint::mag() const {
    return sqrt(_x * _x + _y * _y + _z * _z);
}

LPoint LPoint::operator = (const LPoint& p) { _x = p._x; _y = p._y; _z = p._z;  return *this; }

LPoint LPoint::operator+= (const LPoint& p) { _x += p._x; _y+= p._y; _z += p._z; return *this; }

LPoint LPoint::operator-= (const LPoint& p) { _x -= p._x; _y -= p._y; _z -= p._z; return *this; }

void LPoint::shift(const double& dx, const double& dy, const double& dz) {
    _x += dx;
    _y += dy;
    _z += dz;
}

const std::string LPoint::toString() const {
    std::stringstream ss;
    ss << "[" << _x << ", " << _y << ", " << _z << "]";
    
    return ss.str();
}

LPoint pointMin(const LPoint& u, const LPoint& v) {
    return LPoint(std::min(u.x(), v.x()), std::min(u.y(), v.y()), std::min(u.z(), v.z()));
}

LPoint pointMax(const LPoint& u, const LPoint& v) {
    return LPoint(std::max(u.x(), v.x()), std::max(u.y(), v.y()), std::max(u.z(), v.z()));
}

LPoint operator- (const LPoint& p, const LPoint& q) {
    return LPoint(p.x() - q.x(), p.y() - q.y(), p.z() - q.z());
}

LPoint operator+ (const LPoint& p, const LPoint& q) {
    return LPoint(p.x() + q.x(), p.y() + q.y(), p.z() + q.z());
}

LPoint operator* (const LPoint& p, const double s) {
    return LPoint(p.x() * s, p.y() * s, p.z() * s);
}

LPoint operator* (const double s, const LPoint& p) {
    return LPoint(p.x() * s, p.y() * s, p.z() * s);
}

LPoint operator/ (const LPoint& p, const double s) {
    return (1 / s) * p;
}

}
