#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "visualobject.h"

class Heightmap : public VisualObject
{
public:
    Heightmap();
    void init(GLint matrixUniform) override;
    void draw() override;
    void setPlane();
    gsl::Vector3D BarycCoordinates(const gsl::Vector2D& p1, const gsl::Vector2D& p2, const gsl::Vector2D& p3, const gsl::Vector2D& player);
    float SetYCoord(const float x, const float z);
private:
    int m_width, m_depth;
    unsigned char* m_localBuffer;
    class Logger *mLogger{nullptr};
    int testSize{10};
    float terrainRadius{100};
    float maxHeight{20};
};

#endif // HEIGHTMAP_H
