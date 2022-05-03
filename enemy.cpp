#include "enemy.h"

Enemy::Enemy()
{
    typeName = "enemy";
    mVertices.push_back(Vertex{0.f,0.f,0.f,         1.f,0.f,0.f,     0.f,0.f});
    mVertices.push_back(Vertex{1.f, 0.f, 0.f,       1.f,0.f,0.f,     1.f,0.f});
    mVertices.push_back(Vertex{0.f, 1.f, 0.f,       1.f,0.f,0.f,        0.f,1.f});

    mVertices.push_back(Vertex{1.f, 0.f, 0.f,       1.f,0.f,0.f,     1.f,0.f});
    mVertices.push_back(Vertex{0.f, 1.f, 0.f,       1.f,0.f,0.f,        0.f,1.f});
    mVertices.push_back(Vertex{1.f, 1.f, 0.f,       1.f,0.f,0.f,     1.f,1.f});


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
        glBindVertexArray(0);
}

void Enemy::draw()
{
    moveTowards();
    glBindVertexArray( mVAO );
//    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void Enemy::move()
{
    if(mx > 3 || mx < -3)
        dt = dt * -1;

    mx += dt;
    mMatrix.translateX(dt);


}

void Enemy::moveTowards() //Oppgave 9
{
    gsl::Vector2D trophyXZ;
    trophyXZ.x = trofee.blueTrophies[currentEnemy].x;
    trophyXZ.z = trofee.blueTrophies[currentEnemy].z;
    gsl::Vector2D pos = this->mMatrix.getPosition2D();
    gsl::Vector2D distance = {abs(pos.x - trophyXZ.x), abs(pos.z - trophyXZ.z)};

    distance.normalize();
    mMatrix.translateX(distance.x * speed);
    mMatrix.translateZ(distance.z * speed);
}







