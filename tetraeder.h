#ifndef TETRAEDER_H
#define TETRAEDER_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

class Tetraeder : public VisualObject
{
private:
   std::vector<Vertex> mVertices;
   //QOpenGLContext *mContext;

public:
    Tetraeder();
    ~Tetraeder();
    //void setContext(QOpenGLContext *context);
    void init(GLint matrixUniform) override;
    void draw() override;
    float utregning(float s, float g);
    void writeFile();
};

#endif // TETRAEDER_H
