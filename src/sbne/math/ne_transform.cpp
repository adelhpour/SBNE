#include "sbne/math/ne_transform.h"

namespace sbne {

void Affine2d::setElement(const int& r, const int& c, const double& value) {
    if (r >= 0 && r < 3 && c >= 0 && c < 3)
        _e[r][c] = value;
}

const double Affine2d::element(const int& r, const int& c) const {
    if (r >= 0 && r < 3 && c >= 0 && c < 3)
        return _e[r][c];

    return 0.0;
}

Affine2d Affine2d::invert() const {
    Affine2d trasnpose(_e[1][1] * _e[2][2] - _e[1][2] * _e[2][1], -_e[0][1] * _e[2][2] + _e[0][2] * _e[2][1], _e[0][1] * _e[1][2] - _e[0][2] * _e[1][1], -_e[1][0] * _e[2][2] + _e[1][2] * _e[2][0], _e[0][0] * _e[2][2] - _e[0][2] * _e[2][0], -_e[0][0] * _e[1][2] + _e[0][2] * _e[1][0], _e[1][0] * _e[2][1] - _e[1][1] * _e[2][0], -_e[0][0] * _e[2][1] + _e[0][1] * _e[2][0], _e[0][0] * _e[1][1] - _e[0][1] * _e[1][0]);
    
    return trasnpose/determinent();
}

double Affine2d::determinent() const {
    return _e[0][0] * (_e[1][1] * _e[2][2] - _e[2][1] * _e[1][2]) -
           _e[0][1] * (_e[1][0] * _e[2][2] - _e[2][0] * _e[1][2]) +
           _e[0][2] * (_e[1][0] * _e[2][1] - _e[2][0] * _e[1][1]);
}

Affine2d Affine2d::operator*(const double& k) const {
    return Affine2d(k * _e[0][0], k * _e[0][1], k * _e[0][2],
                    k * _e[1][0], k * _e[1][1], k * _e[1][2],
                    k * _e[2][0], k * _e[2][1], k * _e[2][2]);
}

Affine2d Affine2d::operator/(const double& k) const {
    return (*this)*(1./k);
}

Affine2d Affine2d::operator*(const Affine2d& u) const {
    Affine2d a;
    for(int r = 0; r < 3; ++r)
        for(int c = 0; c < 3; ++c) {
            a.setElement(r, c, 0.);
            for(int i = 0; i < 3; ++i)
                a.setElement(r, c, a.element(r, c) + _e[r][i] * u.element(i, c));
        }
    
    return a;
}

}
