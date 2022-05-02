#include "tetraeder.h"
#include <cmath>
#include <qdebug.h>

Tetraeder::Tetraeder()
{

    float grunnflate = -0.3;

    float side = 0.3; //distansen fra en kant til en annen

    const float lengde = sqrt(pow(side, 2) - pow(side/2,2));//distansen fra en kant til siden på motsatt side, Eks: kant1 til siden mellom kant2 og kant3

    const float kant1X = 0;
    const float kant1Y = grunnflate;
    const float kant1Z = -lengde * 2/3;//Sørger for at midtpunktet av tetraederet er i midten av rotasjonen


    const float kant2X = kant1X + side/2;
    const float kant2Y = grunnflate;
    const float kant2Z = kant1Z + lengde;

    const float kant3X = -kant2X;
    const float kant3Y = grunnflate;
    const float kant3Z = kant2Z;

    const float hoyde = sqrt(pow(side, 2) - pow(lengde, 2));

    const float kant4X = 0;
    const float kant4Y = grunnflate + hoyde;
    const float kant4Z = 0;
    //topp = midtpunkt + høyde z

    Vertex k1(kant1X,kant1Y,kant1Z,1,0,0);
    Vertex k2(kant2X,kant2Y,kant2Z,0,0,1);
    Vertex k3(kant3X,kant3Y,kant3Z,0,1,0);
    Vertex k4(kant4X,kant4Y,kant4Z,1,1,0);

    std::vector<Vertex> corners{k1,k2,k3,k4};

    for(int i=1;i<3;i++)
        for(int j=(i+1);j<4;j++)
            if(i==2 && j==4)
                break;
            else
            {
                mVertices.push_back((corners[0]));
                mVertices.push_back((corners[i]));
                mVertices.push_back((corners[j]));
            }

    mVertices.push_back((corners[1]));
    mVertices.push_back((corners[2]));
    mVertices.push_back((corners[3]));
    //writeFile();
}

Tetraeder::~Tetraeder()
{

}

void Tetraeder::init(GLint matrixUniform)
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

void Tetraeder::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void Tetraeder::writeFile()
{
    std::fstream file;
    file.open("../3Dprog22/trofeModell.txt", std::ios::out);
    if(file.is_open())
    {
        for (auto i = 0; i < mVertices.size(); i++)
            file << mVertices[i] << "\n";

        file.close();
    }
}

