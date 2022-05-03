#ifndef FENCE_H
#define FENCE_H

#include "visualobject.h"
//Oppgave 10
class Fence : public VisualObject
{
public:
    Fence();
    ~Fence();
    void init(GLint matrixUniform);
    void draw();

};

#endif // FENCE_H
