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
    std::vector<float> findValues(std::vector<Vertex> vertices);
    bool isColliding(VisualObject *other, gsl::Vector2D player);
//    void createBox(std::vector<Vertex> object);
};

#endif // COLLISIONBOX_H
