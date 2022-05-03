#ifndef TROFEE_H
#define TROFEE_H

#include "visualobject.h"
#include "collisionaabb.h"

class Trofee : public VisualObject
{
public:
    Trofee();
    Trofee(bool red);
    Trofee(std::string name, float mx, float my, float mz);
    ~Trofee();
    void init(GLint matrixUniform) override;
    void draw() override;
    void readFile();

    CollisionAABB collider;

    std::vector<gsl::Vector2D> redTrophies{{25,67},
                                           {46,62},
                                           {29,57},
                                           {72,54},
                                           {25,35},
                                           {47,35},
                                           {52,48},
                                           {66,35},
                                           {53,31},
                                           {74,64}};

    std::vector<gsl::Vector2D> blueTrophies{{58,64},
                                            {40,57},
                                            {24,52},
                                            {35,35},
                                            {41,39},
                                            {40,47},
                                            {60,42},
                                            {54,35},
                                            {77,42},
                                            {55,53}};
};

#endif // TROFEE_H
