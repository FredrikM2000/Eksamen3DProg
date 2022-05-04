#include "beziercurve.h"

BezierCurve::BezierCurve()
{

    c[0] = gsl::Vector3D(70,10,65);
    c[1] = gsl::Vector3D(70,10,35);
    c[2] = gsl::Vector3D(30,10,55);
    c[3] = gsl::Vector3D(40,10,60);

    double  dt=0.05;
     for (auto t=0.0; t<1.001; t+=dt)
     {
        gsl::Vector3D punkt = evaluateBezier(t); // returnerer Vector2D eller Vector3D
        Vertex v{punkt.x, punkt.y, punkt.z, 1};
        mVertices.push_back(v);
     }
     mMatrix.setToIdentity();
}

BezierCurve::~BezierCurve()
{

}

gsl::Vector3D BezierCurve::evaluateBezier(float t)
{
    gsl::Vector3D a[4];
    for (int i=0; i<4; i++){
        a[i] = c[i];

        for (int k=d; k>0; k--)
            for(int i=0; i<k; i++)
                a[i] = a[i]*(1-t) + a[i+1] * t;
    }
    return a[0];
}

void BezierCurve::init(GLint matrixUniform)
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

void BezierCurve::draw()
{
    initializeOpenGLFunctions();

    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());
//    if(npc)
//        npc->draw();
}
