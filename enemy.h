#ifndef ENEMY_H
#define ENEMY_H

#include "visualobject.h"

class Enemy : public VisualObject
{
public:
    Enemy();
    ~Enemy();
    void init(GLint matrixUniform) override;
    void draw() override;
    void move();
    void construct();

private:
    float mx{0}, dt{0.02};
    bool hit{false};
};

#endif // ENEMY_H
