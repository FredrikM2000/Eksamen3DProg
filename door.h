#ifndef DOOR_H
#define DOOR_H

#include "visualobject.h"

class Door : public VisualObject
{
public:
    Door();
    ~Door();
    void init(GLint matrixUniform) override;
    void draw() override;
    void rotate();
    void collision();
    void open() override;
    bool bOpen {false};

protected:
    std::vector<GLuint>mIndices;
    GLuint mIBO{0};

    float mRadius{0.5};
        std::vector<Vertex> mVertices;
};

#endif // DOOR_H
