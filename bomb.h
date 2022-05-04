#ifndef BOMB_H
#define BOMB_H

#include "visualobject.h"
#include "objmesh.h"

//Oppgave 7
class Bomb : public VisualObject
{
public:
    Bomb();
    ~Bomb();
    void init(GLint matrixUniform) override;
    void draw() override;

    ObjMesh* mesh;
    int time;
};

#endif // BOMB_H
