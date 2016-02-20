#ifndef GRAPHICS_DATABASE__DRAWER_H_
#define GRAPHICS_DATABASE__DRAWER_H_

namespace GraphicsDatabase
{

class Drawer
{
public:
    Drawer();
    ~Drawer();
    void drawTriangle3DH(   const double* xyz0,
                            const double* xyz1,
                            const double* xyz2,
                            const double* uv0 = 0,
                            const double* uv1 = 0,
                            const double* uv2 = 0,
                            unsigned c0 = 0xffffffff,
                            unsigned c1 = 0xffffffff,
                            unsigned c2 = 0xffffffff) const;
};

} // namespace GraphicsDatabase

#endif
