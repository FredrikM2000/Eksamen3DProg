#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "visualobject.h"
#include "trianglesurface.h"
#include "collisionaabb.h"
#include "objmesh.h"
#include "collisionsphere.h"
#include "trofee.h"
#include "plan.h"
#include "bomb.h"

#include <QElapsedTimer>


class InteractiveObject : public VisualObject
{
public:
    InteractiveObject();
    ~InteractiveObject() override;
    void init(GLint matrixUniform) override;
    void draw() override;
    void collision(VisualObject* gameObject);
    void createCollisionBox(bool draw);
    void createCollisionSphere();
    void collisionWithBomb(ObjMesh* bomb);
    void spawnBillboard();
    void stun();

    float speed = 0.15;
    int collectedTrophies{0};
private:
   TriangleSurface* curve;
   float mx, my, mz; // posisjon
   float distance{0.15};

   ObjMesh *mesh;
   CollisionAABB *collider;
   CollisionSphere *sphere;
   Plan* plan;

   bool bdrawPLan{false};
   bool bDrawBox;

   QElapsedTimer timer;
};

#endif // INTERACTIVEOBJECT_H
