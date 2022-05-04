#ifndef ENEMY_H
#define ENEMY_H

#include "visualobject.h"
#include "trofee.h"
#include "collisionaabb.h"
#include "objmesh.h"

#include <QElapsedTimer>

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
    void stun();


    //Opgave 13
    int time{1};
    int collectedTrophies{0};
private:
    float mx{0}, dt{0.02};
    float speed = 0.1;

    bool hit{false};
    bool bDrawBox{false};


    Trofee trofee;
    CollisionAABB *collider;
    ObjMesh* mesh;
    QElapsedTimer timer;
};

#endif // ENEMY_H
