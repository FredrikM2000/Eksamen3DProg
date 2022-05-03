#ifndef ENEMY_H
#define ENEMY_H

#include "visualobject.h"
#include "trofee.h"
#include "collisionaabb.h"
#include "objmesh.h"

class Enemy : public VisualObject
{
public:
    Enemy();
    ~Enemy();
    void init(GLint matrixUniform) override;
    void draw() override;
    void move();
    void moveTowards();
    void collision(VisualObject* gameObject);
    void construct();
    void createCollisionBox(bool draw);

    //Opgave 13
    int time{1};
private:
    float mx{0}, dt{0.02};
    float speed = 0.1;

    bool hit{false};
    bool bDrawBox{false};

    int currentEnemy{0};

    Trofee trofee;
    CollisionAABB *collider;
    ObjMesh* mesh;
};

#endif // ENEMY_H
