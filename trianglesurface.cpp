#include "trianglesurface.h"
#include <fstream>


TriangleSurface::TriangleSurface() : VisualObject()
{

    //std::string file = "../3Dprog22/Cube.txt";

    //readFile(file);
    construct();
    //writeFile();
    mMatrix.setToIdentity();
    //volum();
}


TriangleSurface::TriangleSurface(std::string filnavn)
{
  //  construct();
    readFile(filnavn);
    mMatrix.setToIdentity();
}

TriangleSurface::~TriangleSurface(){ }

void TriangleSurface::readFile(std::string filnavn)
{
    std::ifstream inn;
     inn.open(filnavn.c_str());

     if (inn.is_open()) {
         //int n;
         Vertex vertex;
         //inn >> n;
         //mVertices.reserve(n);
         while (true) {
              inn >> vertex;
              if(inn.eof())break;
              mVertices.push_back(vertex);
         }
         inn.close();
     }
}

void TriangleSurface::init(GLint matrixUniform){
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
        glBufferData( GL_ARRAY_BUFFER,      //what buffer type
                      mVertices.size()*sizeof( Vertex ),   //how big buffer do we need
                      mVertices.data(),             //the actual vertices
                      GL_STATIC_DRAW        //should the buffer be updated on the GPU
                      );

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glVertexAttribPointer(
                    0,                  // attribute. No particular reason for 0, but must match layout(location = 0) in the vertex shader.
                    3,                  // size / number of elements of data type
                    GL_FLOAT,           // data type
                    GL_FALSE,           // normalize data
                    sizeof(Vertex),  // stride
                    reinterpret_cast<GLvoid*>(0)  );          // array buffer offset
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
}

void TriangleSurface::draw(){
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

//void TriangleSurface::construct()
//{
//  float xmin=-5.0f, xmax=5.0f, ymin=-5.0f, ymax=5.0f, h=0.5f;
//  for (auto x=xmin; x<xmax; x+=h)
//      for (auto y=ymin; y<ymax; y+=h)
//      {
//          float z = sin(M_PI*x)*sin(M_PI*y);
//          mVertices.push_back(Vertex{x,y,z,x,y,z});
//          z = sin(M_PI*(x+h))*sin(M_PI*y);
//          mVertices.push_back(Vertex{x+h,y,z,x,y,z});
//          z = sin(M_PI*x)*sin(M_PI*(y+h));
//          mVertices.push_back(Vertex{x,y+h,z,x,y,z});
//          mVertices.push_back(Vertex{x,y+h,z,x,y,z});
//          z = sin(M_PI*(x+h))*sin(M_PI*y);
//          mVertices.push_back(Vertex{x+h,y,z,x,y,z});
//          z = sin(M_PI*(x+h))*sin(M_PI*(y+h));
//          mVertices.push_back(Vertex{x+h,y+h,z,x,y,z});
//      }
//}

void TriangleSurface::construct()
{
  float xmin=-5.5f, xmax=5.0f, ymin=-5.5f, ymax=5.0f, h=0.25f;
  for (auto x=xmin; x<xmax; x+=h)
      for (auto y=ymin; y<ymax; y+=h)
      {
          float z = function(x,y);
          mVertices.push_back(Vertex{x,z,y,x,y,z});
          z = function(x+h,y);
          mVertices.push_back(Vertex{x+h,z,y,x,y,z});
          z = function(x,y+h);
          mVertices.push_back(Vertex{x,z,y+h,x,y,z});
          mVertices.push_back(Vertex{x,z,y+h,x,y,z});
          z = function(x+h,y);
          mVertices.push_back(Vertex{x+h,z,y,x,y,z});
          z = function(x+h,y+h);
          mVertices.push_back(Vertex{x+h,z,y+h,x,y,z});
      }
}

//void TriangleSurface::construct()
//{
//  float xmin=-1.0f, xmax=1.0f, ymin=-1.0f, ymax=1.0f, h=0.25f;
//  for (auto x=xmin; x<xmax; x+=h)
//      for (auto y=ymin; y<ymax; y+=h)
//      {
//          float z = function(x,y);
//          mVertices.push_back(Vertex{x,y,z,x,y,z});
//          z = function(x+h,y);
//          mVertices.push_back(Vertex{x+h,y,z,x,y,z});
//          z = function(x,y+h);
//          mVertices.push_back(Vertex{x,y+h,z,x,y,z});
//          mVertices.push_back(Vertex{x,y+h,z,x,y,z});
//          z = function(x+h,y);
//          mVertices.push_back(Vertex{x+h,y,z,x,y,z});
//          z = function(x+h,y+h);
//          mVertices.push_back(Vertex{x+h,y+h,z,x,y,z});
//      }
//}

void TriangleSurface::writeFile()
{
    std::fstream file;
    file.open("../3Dprog22/Cube.txt", std::ios::out);
    if(file.is_open())
    {
        for (auto i = 0; i < mVertices.size(); i++)
            file << mVertices[i] << "\n";

        file.close();
    }
}

float TriangleSurface::function(float x, float y)
{
//    return (-0.0341* x * x + (0.7019* x) + 0.2915);
//    return sin(M_PI*x)*sin(M_PI*y);
//    return (x * y);

//    return (-0.0341* x * x + (0.7019 * x) + 0.2915);
        //return 0.417*(x*x*x)-3.75*(x*x)+10.333*x-6;
//        return x + y + 3 * y + y * x + 2.5;
    return sin(x * 0.8) * sin(y * 0.8) - 1;
    //y = z / x
//    return (x/2 * sin(x) * sin(y)) / x/2 * sin(x);
}

void TriangleSurface::volum()
{
    std::fstream file;
    file.open("../3Dprog22/matte_oblig1.txt", std::ios::out);
    if(file.is_open())
    {
        float xmin=0.0f, xmax=0.5f, ymin=0.0f, ymax=0.5f, h=0.005f, volume = 0.0f;
            for (auto x=xmin; x<xmax; x+=h)
                for (auto y=ymin; y<ymax; y+=h)
                {
                    volume += h*h*(1-x-y);
                }
     file << "Volumet under funksjonen er " << volume;
     file.close();
    }
}









