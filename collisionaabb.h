#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include "visualobject.h"

class CollisionAABB : public VisualObject
{
public:
    CollisionAABB();
    CollisionAABB(std::vector<Vertex> object);
    CollisionAABB(VisualObject *obj);
    ~CollisionAABB();
    void init(GLint matrixUniform) override;
    void draw() override;
    std::vector<float> findValues(VisualObject *obj, gsl::Vector2D position = 0);
    std::vector<float> fixScale(float a, float b, float c, float d, float objScale);
    bool isColliding(VisualObject *other, gsl::Vector2D player);
};

#endif // COLLISIONBOX_H
