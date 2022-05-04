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

InteractiveObject::~InteractiveObject(){}

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

    if(timer.elapsed() > 2000)
    {
        speed = 0.15;
    }
}

void InteractiveObject::collision(VisualObject* gameObject)
{
    //Oppgave 10
    gsl::Vector2D playerPos = mMatrix.getPosition2D();
    if(gameObject->typeName == "fence")
        if(collider->isColliding(gameObject, playerPos))
        {
            //Setter spiller tilbake så mye som den farta den kom fra, og passer på at den gjør det for rett side av gjerdet
            if(playerPos.x < gameObject->mMatrix.getPosition2D().x)
                mMatrix.setPositionX(playerPos.x-speed);
            if(playerPos.x > gameObject->mMatrix.getPosition2D().x)
                mMatrix.setPositionX(playerPos.x+speed);
            if(playerPos.z < gameObject->mMatrix.getPosition2D().z)
                mMatrix.setPositionZ(playerPos.z-speed);
            if(playerPos.z > gameObject->mMatrix.getPosition2D().z)
                mMatrix.setPositionZ(playerPos.z+speed);
        }
    //Oppgave 8
    if(gameObject->m_name == "red")
        if(collider->isColliding(gameObject, playerPos))
            if(gameObject->bDraw)
            {
                gameObject->bDraw = false;
                collectedTrophies++;
            }

//    if(gameObject->typeName == "bomb")
//        if(sphere->isColliding(gameObject, playerPos))
//            qDebug() << "Funka";

}

void InteractiveObject::createCollisionBox(bool draw)
{
    collider = new CollisionAABB (this->mesh);
    collider->init(mMatrixUniform);
    bDrawBox = draw;
}

void InteractiveObject::stun()// Oppgave 7
{
    speed = 0;
    timer.start();
}
















