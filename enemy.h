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
private:
    float mx{0}, dt{0.02};
    bool hit{false};
    Trofee trofee;
    int currentEnemy{0};
    float speed = 0.1;
    CollisionAABB *collider;
    ObjMesh* mesh;
    bool bDrawBox{false};
};

#endif // ENEMY_H
