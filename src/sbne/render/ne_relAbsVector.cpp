#include "sbne/render/ne_relAbsVector.h"

namespace sbne {

    void RAVector::setA(const double& a) {
        _a = a;
    }
    
    const double& RAVector::a() const {
        return _a;
    }
    
    void RAVector::setR(const double& r) {
        _r = r;
    }
    
    const double& RAVector::r() const {
        return _r;
    }

	RAVector RAVector::operator = (const RAVector& r) {
		_a = r.a();
        _r = r.r();
		return *this;
	}

	const RAVector RAVector::operator-() const {
		return RAVector(-_a, -_r);
	}

    const std::string RAVector::toString() const {
        std::stringstream ss;
        ss << "(" << _a << " ";
        
        if (_r > 1.0)
            ss << _r << "%" << ")";
        else
            ss << 100. * _r << "%" << ")";
        
        return ss.str();
    }

    RAVector* RAVector::stringToRAVector(const std::string& str) {
        RAVector* rav = NULL;
        std::stringstream ss;
        ss << str;
        std::string temp;
        int i = 0;
        double abs, rel, extra;
        while (!ss.eof()) {
            ss >> temp;
            
            switch (i) {
                case 0:
                    if (std::stringstream(temp) >> abs)
                        ++i;
                    break;
                case 1:
                    if (std::stringstream(temp) >> rel)
                        ++i;
                    break;
                case 2:
                    if (std::stringstream(temp) >> extra)
                        ++i;
                    break;
                default:
                    break;
            }
            
            temp = "";
        }
        
        if (i == 2)
            rav = new RAVector(abs, rel);
        
        return rav;
    }
}
