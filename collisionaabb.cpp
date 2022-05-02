#include "collisionaabb.h"

CollisionAABB::CollisionAABB()
{

    mVertices.push_back(Vertex{2.85,0.0f,1.0866});
    mVertices.push_back(Vertex{2.85,10.0,1.0866});

    mIndices.insert(mIndices.end(), { 0,1 });

    mMatrix.setToIdentity();
}

CollisionAABB::CollisionAABB(std::vector<Vertex> object)
{
    for(int i = 0; i < object.size(); i++)
    {
        mVertices.push_back(object[i]);
        mVertices[i].setNormal(0,0,1);
    }

    mIndices.insert(mIndices.end(), {0,1, 5,6, 2,3, 7,4, 0,3, 7,6, 2,1, 5,4});
    mMatrix.setToIdentity();
}

CollisionAABB::CollisionAABB(VisualObject *obj)
{
    std::vector<float> values = findValues(obj->getVertices());

//    float lengthx = (values[1] - values[0])/2;
//    float lengthz = (values[3] - values[2])/2;

//    float minX = values[0] - (lengthx * 3);//temporary
//    float maxX = values[1] + (lengthx * 3);
//    float minZ = values[2] - (lengthz * 3);
//    float maxZ = values[3] + (lengthz * 3);

//    float minX = values[0] - (lengthx * obj->mMatrix.scaled);
//    float maxX = values[1] + (lengthx * obj->mMatrix.scaled);
//    float minZ = values[2] - (lengthz * obj->mMatrix.scaled);
//    float maxZ = values[3] + (lengthz * obj->mMatrix.scaled);

//    minX += obj->mMatrix.getPosition().x;
//    maxX += obj->mMatrix.getPosition().x;
//    minZ += obj->mMatrix.getPosition().z;
//    maxZ += obj->mMatrix.getPosition().z;
    values[0] += obj->mMatrix.getPosition().x;
    values[1] += obj->mMatrix.getPosition().x;
    values[2] += obj->mMatrix.getPosition().z;
    values[3] += obj->mMatrix.getPosition().z;

//    obj->mMatrix.scaled;

//    mVertices.push_back(Vertex{minX,0,minZ,   0,0,1});
//    mVertices.push_back(Vertex{maxX,0,minZ,   0,0,1});
//    mVertices.push_back(Vertex{maxX,0,maxZ,   0,0,1});
//    mVertices.push_back(Vertex{minX,0,maxZ,   0,0,1});
    mVertices.push_back(Vertex{values[0],0,values[2],   0,0,1});
    mVertices.push_back(Vertex{values[1],0,values[2],   0,0,1});
    mVertices.push_back(Vertex{values[1],0,values[3],   0,0,1});
    mVertices.push_back(Vertex{values[0],0,values[3],   0,0,1});

    mIndices.insert(mIndices.end(), {0,1, 1,2, 2,3, 3,0});
    mMatrix.setToIdentity();
}

CollisionAABB::~CollisionAABB(){}

void CollisionAABB::init(GLint matrixUniform)
{
    mMatrixUniform =matrixUniform;
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

void CollisionAABB::draw()
{
    glBindVertexArray(mVAO);
    glDrawElements(GL_LINE_STRIP, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

std::vector<float> CollisionAABB::findValues(std::vector<Vertex> vertices)
{
    float minX = vertices[0].m_xyz[0];
    float maxX = vertices[0].m_xyz[0];
    float minZ = vertices[0].m_xyz[2];
    float maxZ = vertices[0].m_xyz[2];

    for (int i = 1; i < vertices.size(); i++)
    {
        if(vertices[i].m_xyz[0] < minX)
            minX = vertices[i].m_xyz[0];
        if(vertices[i].m_xyz[0] > maxX)
            maxX = vertices[i].m_xyz[0];
        if(vertices[i].m_xyz[2] < minZ)
            minZ = vertices[i].m_xyz[2];
        if(vertices[i].m_xyz[2] > maxZ)
            maxZ = vertices[i].m_xyz[2];
    }


    return std::vector<float> {minX, maxX, minZ, maxZ};
}

bool CollisionAABB::isColliding(VisualObject *other, gsl::Vector2D playerPos)
{
    std::vector<float> this_values = findValues(this->mVertices);
    std::vector<float> other_values = findValues(other->getVertices());


//    qDebug() << other->mMatrix.getPosition();
//    qDebug() << playerPos.x << playerPos.z;

    this_values[0] += playerPos.x;
    this_values[1] += playerPos.x;
    this_values[2] += playerPos.z;
    this_values[3] += playerPos.z;

    //                                              Temporary
    other_values[0] += other->mMatrix.getPosition().x/* - 0.45'*/;
    other_values[1] += other->mMatrix.getPosition().x/* + 0.45'*/;
    other_values[2] += other->mMatrix.getPosition().z/* - 0.39'*/;
    other_values[3] += other->mMatrix.getPosition().z/* + 0.39'*/;


//    qDebug() << "PminX" << this_values[0] << "PmaxX" << this_values[1]
//             << "PminZ" << this_values[2] << "PmaxZ" << this_values[3] << "||"
//             << "OminX" << other_values[0] << "OmaxX" << other_values[1]
//             << "OminZ" << other_values[2] << "OmaxZ" << other_values[3] << '\n';



    if(this_values[0] < other_values[1] && this_values[1] > other_values[0]
    && this_values[2] < other_values[3] && this_values[3] > other_values[2])
    {
        return true;
    }
    return false;
}

//void CollisionBox::createBox()
//{

//}












