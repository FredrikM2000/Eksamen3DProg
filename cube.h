#ifndef CUBE_H
#define CUBE_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

class Cube : public VisualObject
{
private:
   std::vector<Vertex> mVertices;
   //QOpenGLContext *mContext;
   void side(Vertex m_k1, Vertex m_k2, Vertex m_k3, Vertex m_k4);


public:
    Cube();
    ~Cube();
    //void setContext(QOpenGLContext *context);
    void init(GLint matrixUniform) override;
    void draw() override;
    Vertex k1, k2, k3, k4, k5, k6, k7, k8;
    void construct1();
    void writeFile();
    void construct2();
};

#endif // CUBE_H
