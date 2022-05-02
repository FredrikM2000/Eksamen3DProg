#include "interactiveobject.h"
#include <fstream>
#include <QDebug>

InteractiveObject::InteractiveObject()
{
//    mVertices.push_back(Vertex{-0.5,-0.5,-0.5,1,0,0});
//    mVertices.push_back(Vertex{-0.5,-0.5,0.5,1,0,0});
//    mVertices.push_back(Vertex{-0.5,0.5,0.5,1,0,0});

//    mVertices.push_back(Vertex{0.5,0.5,-0.5,0,1,0});
//    mVertices.push_back(Vertex{-0.5,-0.5,-0.5,0,1,0});
//    mVertices.push_back(Vertex{-0.5,0.5,-0.5,0,1,0});

//    mVertices.push_back(Vertex{0.5,-0.5,0.5,0,1,0});
//    mVertices.push_back(Vertex{-0.5,-0.5,-0.5,0,1,0});
//    mVertices.push_back(Vertex{0.5,-0.5,-0.5,0,1,0});

//    mVertices.push_back(Vertex{0.5,0.5,-0.5,0,1,0});
//    mVertices.push_back(Vertex{0.5,-0.5,-0.5,0,1,0});
//    mVertices.push_back(Vertex{-0.5,-0.5,-0.5,0,1,0});

//    mVertices.push_back(Vertex{-0.5,-0.5,-0.5,1,0,0});
//    mVertices.push_back(Vertex{-0.5,0.5,0.5,1,0,0});
//    mVertices.push_back(Vertex{-0.5,0.5,-0.5,1,0,0});

//    mVertices.push_back(Vertex{0.5,-0.5,0.5,0,0,1});
//    mVertices.push_back(Vertex{-0.5,-0.5,0.5,0,0,1});
//    mVertices.push_back(Vertex{-0.5,-0.5,-0.5,0,0,1});

//    mVertices.push_back(Vertex{-0.5,0.5,0.5,0,1,1});
//    mVertices.push_back(Vertex{-0.5,-0.5,0.5,0,1,1});
//    mVertices.push_back(Vertex{0.5,-0.5,0.5,0,1,1});

//    mVertices.push_back(Vertex{0.5,0.5,0.5,1,0,1});
//    mVertices.push_back(Vertex{0.5,-0.5,-0.5,1,0,1});
//    mVertices.push_back(Vertex{0.5,0.5,-0.5,1,0,1});

//    mVertices.push_back(Vertex{0.5,-0.5,-0.5,1,0,1});
//    mVertices.push_back(Vertex{0.5,0.5,0.5,1,0,1});
//    mVertices.push_back(Vertex{0.5,-0.5,0.5,1,0,1});

//    mVertices.push_back(Vertex{0.5,0.5,0.5,1,0,1});
//    mVertices.push_back(Vertex{0.5,0.5,-0.5,1,0,1});
//    mVertices.push_back(Vertex{-0.5,0.5,-0.5,1,0,1});

//    mVertices.push_back(Vertex{0.5,0.5,0.5,0,1,1});
//    mVertices.push_back(Vertex{-0.5,0.5,-0.5,0,1,1});
//    mVertices.push_back(Vertex{-0.5,0.5,0.5,0,1,1});

//    mVertices.push_back(Vertex{0.5,0.5,0.5,0,1,1});
//    mVertices.push_back(Vertex{-0.5,0.5,0.5,0,1,1});
//    mVertices.push_back(Vertex{0.5,-0.5,0.5,0,1,1});
//    readFile();

//    mx = 1.5f;
//    mz = 1.5f;
//    my = curve->function(mx,my) + 1.2f;


//    mVertices.push_back(Vertex{-0.1, -0.1, 0.1,      0,0,1});// 0
//    mVertices.push_back(Vertex{0.1, -0.1, 0.1,       1,0,1});// 1
//    mVertices.push_back(Vertex{0.1, -0.1, -0.1,      0,1,1});// 2
//    mVertices.push_back(Vertex{-0.1, -0.1, -0.1,     1,1,1});// 3
//    mVertices.push_back(Vertex{-0.1, 0.1, 0.1,       1,0,0});// 4
//    mVertices.push_back(Vertex{0.1, 0.1, 0.1,        0,1,0});// 5
//    mVertices.push_back(Vertex{0.1, 0.1, -0.1,       1,1,0});// 6
//    mVertices.push_back(Vertex{-0.1, 0.1, -0.1,      0,0,0});// 7

//    mIndices.insert(mIndices.end(), {0,1,4, 4,1,5, 1,2,5, 5,2,6, 2,3,6, 6,3,7, 3,0,7, 7,0,4, 0,1,2, 0,2,3, 4,5,6, 4,6,7});

    //Oppgave 4
    obj = new ObjMesh("../Eksamen3DProg/Assets/obj_files/Player.obj", true);
    obj->init(mMatrixUniform);

    mx = 2.f;
    mz = 2.f;
    my = 10.f;

    mPosition(0,3) = mx;
    mPosition(1,3) = my;
    mPosition(2,3) = mz;

    mMatrix = mPosition;
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
    if(obj)
        obj->draw();
    else{
    glBindVertexArray( mVAO );
 //    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

//    if(collider)
//        collider->draw();
    }
}

void InteractiveObject::move(float x, float y, float z)
{

//    mx += x;
//    my += y;
//    mz += z;

//    mPosition(0,3) = mx;
//    mPosition(1,3) = curve->function(mx,mz) + 0.5f;
//    mPosition(2,3) = mz;


//    mMatrix = mPosition;

//    mMatrix.translateX(0.2);
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
//    float objX = gameObject->mMatrix.getPosition().getX();
//    float objZ = gameObject->mMatrix.getPosition().getZ();

//    float playerX = mMatrix.getPosition().getX();
//    float playerZ = mMatrix.getPosition().getZ();

//        if (objX - 0.2 < playerX && objX + 0.3 > playerX && objZ - 0.2 < playerZ && objZ + 0.35 > playerZ)
//        {
//                if(gameObject->typeName == "enemy")
//                    qDebug() << "Hit";
//                if(gameObject->typeName == "pickup")
//                    gameObject->bDraw = false;
//        }



    if(collider->isColliding(gameObject, this->mMatrix.getPosition2D()))
    {
        if(gameObject->typeName == "enemy")
            qDebug() << "Hit";// Reset?
        if(gameObject->typeName == "pickup")
            gameObject->bDraw = false;
    }

//    collider->isColliding(gameObject.c);

//    gameObject->typeName;
//    CollisionBox *coll;

//    coll = new CollisionBox();

//    if(collider->isColliding(coll))
//        qDebug() << "it worked";


}

void InteractiveObject::createCollisionBox()
{
    collider = new CollisionAABB (this->mVertices);
    collider->init(mMatrixUniform);
}



















