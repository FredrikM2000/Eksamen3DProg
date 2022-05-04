#include "bomb.h"

Bomb::Bomb()
{
    typeName = "bomb";
    mesh = new ObjMesh("../Eksamen3DProg/Assets/obj_files/Sphere.obj", false, 1,0.5,0);

    mMatrix.setToIdentity();
}

Bomb::~Bomb(){}

void Bomb::init(GLint matrixUniform)
{
    mesh->init(mMatrixUniform);
}

void Bomb::draw()
{
    if(mMatrix.getPosition().y < 0)
        mesh->bDraw = false;
    else
    {
        if(time == 1)
            mMatrix.translateY(-0.25);
        mesh->draw();
    }
}
