#ifndef XYZ_H
#define XYZ_H

#include <QOpenGLFunctions_4_1_Core>
#include "visualobject.h"

class XYZ : public VisualObject
{
public:
     XYZ();
     ~XYZ() override;
     void init(GLint matrixUniform) override;
     void draw() override;
};

#endif // XYZ_H
