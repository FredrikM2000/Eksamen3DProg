#ifndef NPC_H
#define NPC_H

#include "visualobject.h"
#include "visualcurve.h"
#include "bomb.h"

class NPC : public VisualObject
{
public:
    NPC();
    NPC(std::vector<Vertex> vertices);
    ~NPC();
    void init(GLint matrixUniform) override;
    void draw() override;
    void moveTowards();

    float time{1};
    bool turn{false};
private:
    Vertex k1, k2, k3, k4, k5, k6, k7, k8;
    void side(Vertex m_k1, Vertex m_k2, Vertex m_k3, Vertex m_k4);
    VisualCurve* curve;
    bool bCurrentGraph{false};
    std::vector<Vertex> otherVertices;
    float speed{0.15};
    int i = 1;

    Bomb* bomb;

};

#endif // NPC_H
