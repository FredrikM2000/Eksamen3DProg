#ifndef OBJMESH_H
#define OBJMESH_H

#include "visualobject.h"

class ObjMesh : public VisualObject
{
public:
    ObjMesh(std::string filename, bool useNormals, float r, float g, float b);
    ~ObjMesh() override;
    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;
private:
    void readFile(std::string filename, bool useNormals, float r = 0, float g = 0, float b = 0);

};

#endif // OBJMESH_H
