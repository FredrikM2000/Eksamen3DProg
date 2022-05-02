#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "visualobject.h"
//#include "vector3d.h"

class TriangleSurface : public VisualObject
{
public:
    TriangleSurface();
    TriangleSurface(std::string filnavn);
      ~TriangleSurface() override;
      void readFile(std::string filnavn);
      void init(GLint matrixUniform) override;
      void draw() override;
      void construct();
      void writeFile();
      float function(float x, float y);
      void volum();
   };

#endif // TRIANGLESURFACE_H
