#include "collisionaabb.h"

CollisionAABB::CollisionAABB()
{
    mVertices.push_back(Vertex{0,0,0});
    mVertices.push_back(Vertex{1,0,0});
    mVertices.push_back(Vertex{1,0,1});
    mVertices.push_back(Vertex{0,0,1});

    mIndices.insert(mIndices.end(), { 0,1, 1,2, 2,3, 3,0});

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
    std::vector<float> values = findValues(obj);

    mVertices.push_back(Vertex{values[0],1,values[2],   0,0,1});
    mVertices.push_back(Vertex{values[1],1,values[2],   0,0,1});
    mVertices.push_back(Vertex{values[1],1,values[3],   0,0,1});
    mVertices.push_back(Vertex{values[0],1,values[3],   0,0,1});

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

std::vector<float> CollisionAABB::findValues(VisualObject *obj, gsl::Vector2D position)
{
    // Oppgave 8
    std::vector<Vertex> vertices = obj->getVertices();
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

    //Sett riktig posisjon
    if(position.x == 0 && position.z == 0)
    {
        minX += obj->mMatrix.getPosition().x;
        maxX += obj->mMatrix.getPosition().x;
        minZ += obj->mMatrix.getPosition().z;
        maxZ += obj->mMatrix.getPosition().z;
    }
    else
    {
        minX += position.x;
        maxX += position.x;
        minZ += position.z;
        maxZ += position.z;
    }

    std::vector<float> returns = fixScale(minX, maxX, minZ, maxZ, obj->mScaled);

    return std::vector<float> {returns};
}

std::vector<float> CollisionAABB::fixScale(float a, float b, float c, float d, float objScale)
{
    float radiusX = (b - a)/2;
    float radiusZ = (d - c)/2;

    a -= (radiusX * objScale) - radiusX;
    b += (radiusX * objScale) - radiusX;
    c -= (radiusZ * objScale) - radiusZ;
    d += (radiusZ * objScale) - radiusZ;

    return std::vector<float> {a, b, c, d};
}

bool CollisionAABB::isColliding(VisualObject *other, gsl::Vector2D playerPos)
{
    std::vector<float> this_values = findValues(this, playerPos);
    std::vector<float> other_values = findValues(other);

    if(this_values[0] < other_values[1] && this_values[1] > other_values[0]
    && this_values[2] < other_values[3] && this_values[3] > other_values[2])
    {
        return true;
    }
    return false;
}













