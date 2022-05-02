#ifndef COLLISIONSPHERE_H
#define COLLISIONSPHERE_H

#include "visualobject.h"

class CollisionSphere : public VisualObject
{
public:
    CollisionSphere();
    CollisionSphere(VisualObject *object);
    ~CollisionSphere();
    void init(GLint matrixUniform) override;
    void draw() override;
    bool isColliding(VisualObject *other, gsl::Vector2D playerPos);
    std::vector<float> findValues(VisualObject *obj);
    float findRadius(float a, float b, float c=0, float d=0);
    std::vector<float> findMinMax(VisualObject *obj);
};

#endif // COLLISIONSPHERE_H
