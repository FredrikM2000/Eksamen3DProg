#ifndef GRAPH_H
#define GRAPH_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

class Graph : public VisualObject
{
private:
   std::vector<Vertex> mVertices;
   //QOpenGLContext *mContext;

public:
    Graph();
    ~Graph();
    //void setContext(QOpenGLContext *context);
    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // GRAPH_H
