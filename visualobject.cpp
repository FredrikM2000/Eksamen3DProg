// VisualObject.cpp
#include "visualobject.h"

VisualObject::VisualObject() {  }

VisualObject::VisualObject(std::string name) : m_name(name)
{

}
VisualObject::~VisualObject() {
   glDeleteVertexArrays( 1, &mVAO );
   glDeleteBuffers( 1, &mVBO );
}

std::pair<float, float> VisualObject::getPosition2D()
{
//   auto col = mMatrix.column(3);
   return std::pair<float,float>(mMatrix.getPosition2D().getX(), mMatrix.getPosition2D().getZ());
   //    return std::pair<float,float>(1.f, 1.f);
}

std::vector<Vertex> VisualObject::getVertices()
{
    return mVertices;
}
