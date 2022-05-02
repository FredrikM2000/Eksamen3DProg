#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <fstream>
#include "vector3d.h"
#include "vector2d.h"
class Vertex {
   //! Overloaded ostream operator which writes all vertex data on an open textfile stream
    friend std::ostream& operator<< (std::ostream&, const Vertex&);

      //! Overloaded ostream operator which reads all vertex data from an open textfile stream
      friend std::istream& operator>> (std::istream&, Vertex&);

public:
      float m_xyz[3];       	// position i 3D
      float m_normal[3];    // normal in 3D or rgb colors
      float m_st[2];        	// texture coordinates if used

      Vertex();
      Vertex(float x, float y, float z, float r = 0.f, float g = 0.f, float b = 0.f, float s = 0.f, float t = 0.f);
      Vertex(gsl::Vector3D a, gsl::Vector3D b, gsl::Vector2D c);

      void setXYZ(float x, float y, float z);
      void setNormal(float r, float g, float b);
      void setST(float s, float t);

      void setXYZ(gsl::Vector3D xyz);
      void setNormal(gsl::Vector3D normal);
      void setST(gsl::Vector3D st);

      gsl::Vector3D to3DVec();
};
#endif // VERTEX_H
