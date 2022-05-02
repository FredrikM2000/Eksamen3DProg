#include "graph.h"

Graph::Graph()
{
    float xmin = -2.0f, xmax = 1.0f, ymin = -2.0f, ymax = 1.0f, h = 0.05f;
    for (auto x = xmin; x < xmax; x += h)
    {
        for (auto y = ymin; y < ymax; y += h)
        {
            float z = sin(M_PI*x)*sin(M_PI*y);
            mVertices.push_back(Vertex{x,y,z,x,y,z});
            z = sin(M_PI*(x+h))*sin(M_PI*y);
            mVertices.push_back(Vertex{x+h,y,z,x,y,z});
            z = sin(M_PI*x)*sin(M_PI*(y+h));
            mVertices.push_back(Vertex{x,y,z,x,y,z});
            mVertices.push_back(Vertex{x,y+h,z,x,y,z});
            z = sin(M_PI*(x+h))*sin(M_PI*y);
            mVertices.push_back(Vertex{x+h,y,z,x,y,z});
            z = sin(M_PI*(x+h))*sin(M_PI*(y+h));
            mVertices.push_back(Vertex{x+h,y+h,z,x,y,z});
        }
    }
}

Graph::~Graph()
{}

void Graph::init(GLint matrixUniform)
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

void Graph::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

