#include "interactiveobject.h"
#include <fstream>
#include <QDebug>

InteractiveObject::InteractiveObject()
{
    //Oppgave 4
    mesh = new ObjMesh("../Eksamen3DProg/Assets/obj_files/Player.obj", true);
    mesh->init(mMatrixUniform);

    mMatrix.setToIdentity();
}

InteractiveObject::~InteractiveObject()
{

}

void InteractiveObject::init(GLint matrixUniform)
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
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)) );
        glEnableVertexAttribArray(2);

        glGenBuffers ( 1, &mEAB );
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
}

void InteractiveObject::draw()
{
    if(mesh)
        mesh->draw();
    else{
       glBindVertexArray(mVAO);
       glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
       glBindVertexArray(0);
    }

    if(bDrawBox)
        collider->draw();
}

void InteractiveObject::readFile()
{
    std::string file = "../3Dprog22/rainbowCube.txt";
    std::ifstream inn;
     inn.open(file.c_str());

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

void InteractiveObject::collision(VisualObject* gameObject)
{
    if(gameObject->m_name == "red")
        if(collider->isColliding(gameObject, this->mMatrix.getPosition2D()))
            gameObject->bDraw = false;

}

void InteractiveObject::createCollisionBox(bool draw)
{
    collider = new CollisionAABB (this->mesh);
    collider->init(mMatrixUniform);
    bDrawBox = draw;
}
















