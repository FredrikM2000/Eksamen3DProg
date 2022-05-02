#include "xyz.h"

XYZ::XYZ()
{
    mVertices.push_back(Vertex{0,0,0,1,0,0});
    mVertices.push_back(Vertex{0.5,0,0,1,0,0});
    mVertices.push_back(Vertex{0,0,0,0,1,0});
    mVertices.push_back(Vertex{0,0.5,0,0,1,0});
    mVertices.push_back(Vertex{0,0,0,0,0,1});
    mVertices.push_back(Vertex{0,0,0.5,0,0,1});
    mMatrix.setToIdentity();
}

XYZ::~XYZ() { }

void XYZ::init(GLint matrixUniform)
{    
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



void XYZ::draw()
{
   glBindVertexArray( mVAO );
   glDrawArrays(GL_LINES, 0, mVertices.size());
}

