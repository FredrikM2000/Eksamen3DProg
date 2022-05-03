#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "visualobject.h"
#include "trianglesurface.h"
#include "collisionaabb.h"
#include "objmesh.h"
#include "collisionsphere.h"
#include "trofee.h"

class InteractiveObject : public VisualObject
{
public:
    InteractiveObject();
    ~InteractiveObject() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void readFile();
    void collision(VisualObject* gameObject);
    void createCollisionBox(bool draw);
private:
   TriangleSurface* curve;
   float mx, my, mz; // posisjon
   float distance{0.15};

   ObjMesh *mesh;
   CollisionAABB *collider;

   bool bDrawBox;
};

#endif // INTERACTIVEOBJECT_H
