#ifndef ENEMY_H
#define ENEMY_H

#include "visualobject.h"
#include "trofee.h"

class Enemy : public VisualObject
{
public:
    Enemy();
    ~Enemy();
    void init(GLint matrixUniform) override;
    void draw() override;
    void move();
    void moveTowards();
    void construct();

private:
    float mx{0}, dt{0.02};
    bool hit{false};
    Trofee trofee;
    int currentEnemy{1};
    float speed = 0.1;
};

#endif // ENEMY_H
