#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <QOpenGLFunctions_4_1_Core>
#include "visualobject.h"

class BezierCurve : public VisualObject
{
private:
//    std::vector<int> c;
    int d = 3;
//    double p(double a, double b, double c, double d, double x);

    gsl::Vector3D c[4];
public:
    BezierCurve();
    ~BezierCurve();
    gsl::Vector3D evaluateBezier(float t);
    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // BEZIERCURVE_H
