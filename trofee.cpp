#include "trofee.h"
//#include <fstream>

Trofee::Trofee()
{
    typeName = "pickup";
    Vertex k1(0,-0.3,-0.1732,1,0,0);
    Vertex k2(0.15,-0.3,0.0866,0,0,1);
    Vertex k3(-0.15,-0.3,0.0866,0,1,0);
    Vertex k4(0,-0.15,0,1,1,0);

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

    mMatrix.setToIdentity();
}

Trofee::Trofee(std::string name, float mx, float my, float mz)
{
    typeName = "pickup";
    m_name = name;
    Vertex k1(0,-0.3,-0.1732,1,0,0);
    Vertex k2(0.15,-0.3,0.0866,0,0,1);
    Vertex k3(-0.15,-0.3,0.0866,0,1,0);
    Vertex k4(0,-0.15,0,1,1,0);

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

    mPosition(0,3) = mx;
    mPosition(1,3) = my;
    mPosition(2,3) = mz;

    mMatrix.translate(mx,my,mz);
    mMatrix.setToIdentity();
}

Trofee::~Trofee(){}

void Trofee::init(GLint matrixUniform)
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

void Trofee::draw()
{
    if(!bDraw)
        return;
    else{
        glBindVertexArray( mVAO );
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    }
}

void Trofee::readFile()
{
    std::string filnavn = "../3Dprog22/trofeModell.txt";
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
