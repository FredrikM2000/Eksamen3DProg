#ifndef LIGHT_H
#define LIGHT_H

#include "visualobject.h"
#include "objmesh.h"
class Light : public VisualObject
{
public:
    Light();
    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;

    GLfloat mAmbientStrength{0.3f};
    gsl::Vector3D mAmbientColor{0.3f, 0.3f, 0.3f};

    GLfloat mLightStrength{0.7f};
    gsl::Vector3D mLightColor{0.9f, 0.9f, 0.3f};

    GLfloat mSpecularStrength{0.9f};
    GLint mSpecularExponent{4};

private:
    ObjMesh * obj;
};

#endif // LIGHT_H
