#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "visualobject.h"
#include "trianglesurface.h"
#include "collisionaabb.h"

class InteractiveObject : public VisualObject
{
public:
    InteractiveObject();
    ~InteractiveObject() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void move(float x, float y, float z) override;
    void readFile();
    void collision(VisualObject* gameObject);
    void createCollisionBox();

private:
   TriangleSurface* curve;
   float mx, my, mz; // posisjon
   float distance{0.15};

   CollisionAABB *collider;

};

#endif // INTERACTIVEOBJECT_H
