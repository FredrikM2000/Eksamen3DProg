#include "cube.h"

Cube::Cube()
{
    construct2();
    //writeFile();
    mMatrix.setToIdentity();
}

Cube::~Cube()
{}

void Cube::side(Vertex m_k1, Vertex m_k2, Vertex m_k3, Vertex m_k4)
{
    mVertices.push_back(m_k1);
    mVertices.push_back(m_k2);
    mVertices.push_back(m_k3);

    mVertices.push_back(m_k2);
    mVertices.push_back(m_k3);
    mVertices.push_back(m_k4);
}

void Cube::init(GLint matrixUniform)
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

void Cube::draw()
{
   glBindVertexArray( mVAO );
   glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void Cube::construct1()
{
    Vertex k1(-0.5,-0.5,0.5,1,0,0);
    Vertex k2(0.5,-0.5,0.5,1,1,0);
    Vertex k3(-0.5,0.5,0.5,1,0,1);
    Vertex k4(0.5,0.5,0.5,1,1,1);
    Vertex k5(0.5,-0.5,-0.5,0,1,1);
    Vertex k6(0.5,0.5,-0.5,0,1,0);
    Vertex k7(-0.5,-0.5,-0.5,0,0,1);
    Vertex k8(-0.5,0.5,-0.5,0,0,0);

    side(k1,k2,k3,k4);
    side(k2,k5,k4,k6);
    side(k5,k7,k6,k8);
    side(k7,k1,k8,k3);
    side(k3,k4,k8,k6);
    side(k7,k5,k1,k2);
}

void Cube::construct2()
{
    Vertex k1(-0.1,-0.1,0.1,1,0,0);
    Vertex k2(0.1,-0.1,0.1,1,1,0);
    Vertex k3(-0.1,0.1,0.1,1,0,1);
    Vertex k4(0.1,0.1,0.1,1,1,1);
    Vertex k5(0.1,-0.1,-0.1,0,1,1);
    Vertex k6(0.1,0.1,-0.1,0,1,0);
    Vertex k7(-0.1,-0.1,-0.1,0,0,0);
    Vertex k8(-0.1,0.1,-0.1,0,0,1);

    side(k1,k2,k3,k4);
    side(k2,k5,k4,k6);
    side(k5,k7,k6,k8);
    side(k7,k1,k8,k3);
    side(k3,k4,k8,k6);
    side(k7,k5,k1,k2);
}

void Cube::writeFile()
{
    std::fstream file;
    file.open("../3Dprog22/rainbowCube.txt", std::ios::out);
    if(file.is_open())
    {
        for (auto i = 0; i < mVertices.size(); i++)
            file << mVertices[i] << "\n";

        file.close();
    }
}
