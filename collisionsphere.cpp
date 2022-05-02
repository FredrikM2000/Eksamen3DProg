#include "collisionsphere.h"

CollisionSphere::CollisionSphere()
{

}

CollisionSphere::CollisionSphere(VisualObject *obj)
{
    std::vector<float> values = findMinMax(obj);

    values[0] += obj->mMatrix.getPosition().x;
    values[1] += obj->mMatrix.getPosition().x;
    values[2] += obj->mMatrix.getPosition().z;
    values[3] += obj->mMatrix.getPosition().z;

    mVertices.push_back(Vertex{values[0],0,values[2],   0,0,1});
    mVertices.push_back(Vertex{values[1],0,values[2],   0,0,1});
    mVertices.push_back(Vertex{values[1],0,values[3],   0,0,1});
    mVertices.push_back(Vertex{values[0],0,values[3],   0,0,1});

    mIndices.insert(mIndices.end(), {0,1, 1,2, 2,3, 3,0});
    mMatrix.setToIdentity();
}

CollisionSphere::~CollisionSphere(){}

void CollisionSphere::init(GLint matrixUniform)
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

void CollisionSphere::draw()
{
    glBindVertexArray(mVAO);
    glDrawElements(GL_LINE_STRIP, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

bool CollisionSphere::isColliding(VisualObject *other, gsl::Vector2D playerPos)
{
    std::vector<float> this_values = findValues(this);
    std::vector<float> other_values = findValues(other);

    float distX = abs(this_values[0] - other_values[0]);
    float distZ = abs(this_values[1] - other_values[1]);

    float distance = sqrt(pow(distX,2) + pow(distZ,2));

    if (distance < (this_values[2] + other_values[2]))
        return true;
    return false;
}

std::vector<float> CollisionSphere::findValues(VisualObject *obj)
{
    std::vector<float> MinMax = findMinMax(obj);

    float radius = findRadius(MinMax[0], MinMax[0], MinMax[0], MinMax[0]);

    std::vector<float> values = {MinMax[0], MinMax[1], MinMax[2], MinMax[3], radius};
    gsl::Vector2D punkt = gsl::Vector2D(MinMax[0] + radius + obj->mMatrix.getPosition2D().x, MinMax[2] + radius + obj->mMatrix.getPosition2D().z);

    return {punkt.x, punkt.z, radius};
}

float CollisionSphere::findRadius(float a, float b, float c, float d)
{
float length;

    if(a > b)
        length = a;
    else
        length = b;

    if(c && length < c)
        length = c;

    if(d && length < d)
        length = d;

    return length/2;
}

std::vector<float> CollisionSphere::findMinMax(VisualObject *obj)
{
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
    return {minX, maxX, minZ, maxZ};
}
