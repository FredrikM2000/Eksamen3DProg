#ifndef TROFEE_H
#define TROFEE_H

#include "visualobject.h"
#include "collisionaabb.h"

class Trofee : public VisualObject
{
public:
    Trofee();
    Trofee(std::string name, float mx, float my, float mz);
    ~Trofee();
    void init(GLint matrixUniform) override;
    void draw() override;
    void readFile();

    CollisionAABB collider;
};

#endif // TROFEE_H
