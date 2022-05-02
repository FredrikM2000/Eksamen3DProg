#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <matrix4x4.h>
#include <vector>
#include "vertex.h"


class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
   VisualObject();
   VisualObject(std::string name);
   ~VisualObject();
   virtual void init(GLint matrixUniform)=0;
   virtual void draw()=0;
   virtual void move(float x, float y, float z){};
   virtual void move(float dt) {}
   std::pair<float,float> getPosition2D();

   std::string typeName{""};
   std::string m_name{""};

   virtual void open(){};

   //Lagrer posisjon, rotasjon og translasjon i 4x4 matrise
   gsl::Matrix4x4 mPosition;
   gsl::Matrix4x4 mRotation;
   gsl::Matrix4x4 mScale;

   bool bDraw{true};

   gsl::Matrix4x4 mMatrix;

   std::vector<Vertex> getVertices();

protected:
   std::vector<Vertex> mVertices;
   std::vector<GLuint> mIndices;

   GLuint mVAO{0};
   GLuint mVBO{0};
   GLuint mEAB{0};//holds indices
   GLint mMatrixUniform{0};



   //Legger til rette for simulering
   QVector3D mVelocity{1.f,0.f,0.f};
};
#endif // VISUALOBJECT_H
