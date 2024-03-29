#include "enemy.h"

Enemy::Enemy()
{
    typeName = "enemy";
    mesh = new ObjMesh("../Eksamen3DProg/Assets/obj_files/Enemy.obj", false, 0,1,0);
    mesh->init(mMatrixUniform);

    mMatrix.setToIdentity();
}

Enemy::~Enemy() {}

void Enemy::init(GLint matrixUniform)
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

void Enemy::draw()
{
    moveTowards();

    if(mesh)
        mesh->draw();
    else
    {
        glBindVertexArray(mVAO);
        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
    if(bDrawBox)
        collider->draw();

    if(timer.elapsed() > 2000)
        speed = 0.1;
}

void Enemy::moveTowards() //Oppgave 9
{//Beveger seg mot neste trofee
    if(collectedTrophies > 9)
        return;
    gsl::Vector2D trophyXZ;
    trophyXZ.x = trofee.blueTrophies[collectedTrophies].x;
    trophyXZ.z = trofee.blueTrophies[collectedTrophies].z;
    gsl::Vector2D pos = this->mMatrix.getPosition2D();
    gsl::Vector2D distance = {trophyXZ.x - pos.x, trophyXZ.z - pos.z};

    distance.normalize();
    mMatrix.translateX(distance.x * speed * time);
    mMatrix.translateZ(distance.z * speed * time);
}

void Enemy::collision(VisualObject* gameObject)//Oppgave 9
{
    if(gameObject->m_name == "blue")
    {
        if(collider->isColliding(gameObject, this->mMatrix.getPosition2D()))
        {
            if(gameObject->bDraw){
                collectedTrophies++;// Denne ble noen ganger kjørt flere ganger for et trofee, så currentEnemy ble høyere enn det det burde være
                gameObject->bDraw = false;
            }
        }
    }
}

void Enemy::createCollisionBox(bool draw)
{
    collider = new CollisionAABB (this->mesh);
    collider->init(mMatrixUniform);
    bDrawBox = draw;
}

void Enemy::stun()
{
    speed = 0;
    timer.start();
}





