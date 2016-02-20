#include "GraphicsDatabase/Drawer.h"
#include <iostream>
#include <sstream>

namespace GraphicsDatabase
{

Drawer::Drawer() {}

Drawer::~Drawer() {}

void Drawer::drawTriangle3DH(   const double* xyz0,
                                const double* xyz1,
                                const double* xyz2,
                                const double* uv0,
                                const double* uv1,
                                const double* uv2,
                                unsigned c0,
                                unsigned c1,
                                unsigned c2) const
{
    std::ostringstream oss;

    oss << "{";
    oss << "(" << xyz0[0] << ", " << xyz0[1] << ", " << xyz0[2] << "), ";
    oss << "(" << xyz1[0] << ", " << xyz1[1] << ", " << xyz1[2] << "), ";
    oss << "(" << xyz2[0] << ", " << xyz2[1] << ", " << xyz2[2] << ")";

    if (uv0 && uv1 && uv2)
    {
        oss << ", ";
        oss << "(" << uv0[0] << ", " << uv0[1] << ", " << uv0[2] << "), ";
        oss << "(" << uv1[0] << ", " << uv1[1] << ", " << uv1[2] << "), ";
        oss << "(" << uv2[0] << ", " << uv2[1] << ", " << uv2[2] << ") ";
    }

    oss << "}";

    std::cerr << oss.str() << std::endl;
}

} // namespace GraphicsDatabase
