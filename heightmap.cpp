#include "heightmap.h"
#include "logger.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Heightmap::Heightmap()
{
    maxHeight = terrainRadius/5;
    m_localBuffer = stbi_load("../Eksamen3DProg/Assets/EksamenHeightmap.bmp", &m_width, &m_depth, (int*)NULL, 1);
    setPlane();
}

void Heightmap::init(GLint matrixUniform)
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

void Heightmap::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
}

void Heightmap::setPlane()
{
    mVertices.clear();
    mIndices.clear();

//    for ( int z = 0; z < testSize; z++)
//        for(int x = 0; x < testSize; x++)
//        {
      for ( int z = 0; z < m_depth; z++)
          for(int x = 0; x < m_width; x++)
          {
//            float y = rand() % 2;
            float y = m_localBuffer[(z * m_width) + x];
//            float u = x/testSize-1;
//            float v = z/testSize-1;
            float u = (float)x/(float)(m_width-1);
            float v = (float)z/(float)(m_depth-1);

            y = y/255 * maxHeight;
            float newX =  terrainRadius * (x / (float)m_width);
            float newZ =  terrainRadius * (z / (float)m_depth);
            mVertices.push_back(Vertex{newX,y,newZ,y,y,y,u,v});
        }

    for(int j = 0; j < m_depth-1; j++)
        for(int i = 0; i < m_width-1; i++)
        {
            int it = j * m_width + i;

            Vertex v1 = mVertices[it];
            Vertex v2 = mVertices[it+1];
            Vertex v3 = mVertices[it+1+m_width];
            Vertex v4 = mVertices[it+m_width];

            gsl::Vector3D a = v1.to3DVec() - v2.to3DVec();
            gsl::Vector3D b = v3.to3DVec() - v2.to3DVec();
            gsl::Vector3D n = n.cross(a,b);
            n.normalize();
            mVertices[it].setNormal(n.x, n.y, n.z);
            mVertices[it+1].setNormal(n.x, n.y, n.z);
            mVertices[it+1+m_width].setNormal(n.x, n.y, n.z);

            a = v1.to3DVec() - v4.to3DVec();
            b = v3.to3DVec() - v4.to3DVec();
            gsl::Vector3D n2 = n2.cross(b,a);
            n2.normalize();
            mVertices[it].setNormal(n2.x, n2.y, n2.z);
            mVertices[it+1+m_width].setNormal(n2.x, n2.y, n2.z);
            mVertices[it+m_width].setNormal(n2.x, n2.y, n2.z);

            mIndices.push_back(it);
            mIndices.push_back(it + 1);
            mIndices.push_back(it + 1 + m_width );

            mIndices.push_back(it + 1 + m_width );
            mIndices.push_back(it + m_width );
            mIndices.push_back(it);
        }


//    for(int j = 0; j < testSize-1; j++)
//        for(int i = 0; i < testSize-1; i++)
//        {
//            int it = j * testSize + i;

//            Vertex v1 = mVertices[it];
//            Vertex v2 = mVertices[it+1];
//            Vertex v3 = mVertices[it+1+testSize];
//            Vertex v4 = mVertices[it+testSize];

//            Vector3D a = v1.to3DVec() - v2.to3DVec();
//            Vector3D b = v3.to3DVec() - v2.to3DVec();
//            Vector3D n = n.cross(a,b);
//            n.normalize();
//            mVertices[it].setNormal(n.x, n.y, n.z);
//            mVertices[it+1].setNormal(n.x, n.y, n.z);
//            mVertices[it+1+testSize].setNormal(n.x, n.y, n.z);

//            a = v1.to3DVec() - v4.to3DVec();
//            b = v3.to3DVec() - v4.to3DVec();
//            Vector3D n2 = n2.cross(b,a);
//            n2.normalize();
//            mVertices[it].setNormal(n2.x, n2.y, n2.z);
//            mVertices[it+1+testSize].setNormal(n2.x, n2.y, n2.z);
//            mVertices[it+testSize].setNormal(n2.x, n2.y, n2.z);

//            mIndices.push_back(it);
//            mIndices.push_back(it + 1);
//            mIndices.push_back(it + 1 + testSize );

//            mIndices.push_back(it + 1 + testSize );
//            mIndices.push_back(it + testSize );
//            mIndices.push_back(it);
//        }
    mMatrix.setToIdentity();
}

gsl::Vector3D Heightmap::BarycCoordinates(const gsl::Vector2D& p1, const gsl::Vector2D& p2, const gsl::Vector2D& p3, const gsl::Vector2D& player)
{
    gsl::Vector2D p12 = p2-p1;
    gsl::Vector2D p13 = p3-p1;
    gsl::Vector3D n = n.cross(gsl::Vector3D{p13.x, 0, p13.z}, gsl::Vector3D{p12.x, 0, p12.z});

    float areal_123 = n.length(); // dobbelt areal
    gsl::Vector3D baryc; // til retur. Husk
    // u
    gsl::Vector2D p = p2 - player;
    gsl::Vector2D q = p3 - player;
    n = n.cross(gsl::Vector3D{q.x, 0, q.z}, gsl::Vector3D{p.x, 0, p.z});

    baryc.x = n.y/areal_123;
    // v
    p = p3 - player;
    q = p1 - player;

    n = n.cross(gsl::Vector3D{q.x, 0, q.z}, gsl::Vector3D{p.x, 0, p.z});

    baryc.y = n.y/areal_123;

    // w
    p = p1 - player;
    q = p2 - player;
    n = n.cross(gsl::Vector3D{q.x, 0, q.z}, gsl::Vector3D{p.x, 0, p.z});

    baryc.z = n.y/areal_123;

    return baryc;
}


float Heightmap::SetYCoord(const float x, const float z)
{
    if(x < 0 || x > terrainRadius - ((float)1/m_width*terrainRadius)
    || z < 0 || z > terrainRadius - ((float)1/m_depth*terrainRadius))
        return 0;
//    if(x < 0 || z < 0 || x > testSize - 1  || z > testSize - 1)
//        return 0;


    float yCoord = 0;

//    int newX = floor(x);
//    int newZ = floor(z);

    int newX = floor(x / terrainRadius * m_width);
    int newZ = floor(z / terrainRadius * m_depth);

//    qDebug() << "x " << x << "z " << z;
//    qDebug() << (float)1/m_depth*terrainRadius;
    int it = (newZ * m_width) + newX;

    int a = it;
    int b = it + 1;
    int c = it + 1 + m_width;
    int d = it + m_width;
//    int it = (newZ * testSize) + newX;

//    int a = it;
//    int b = it + 1;
//    int c = it + 1 + testSize;
//    int d = it + testSize;

    gsl::Vector2D p1 = gsl::Vector2D(mVertices[a].m_xyz[0], mVertices[a].m_xyz[2]);
    gsl::Vector2D p2 = gsl::Vector2D(mVertices[b].m_xyz[0], mVertices[b].m_xyz[2]);
    gsl::Vector2D p3 = gsl::Vector2D(mVertices[c].m_xyz[0], mVertices[c].m_xyz[2]);
    gsl::Vector2D p4 = gsl::Vector2D(mVertices[d].m_xyz[0], mVertices[d].m_xyz[2]);
    gsl::Vector2D p = gsl::Vector2D(x, z);

    gsl::Vector3D bary = BarycCoordinates(p1,p2,p3,p);

    if(bary.getX() >= 0 && bary.getX() <= 1 && bary.getY() >= 0 && bary.getY() <= 1 && bary.getZ() >= 0 && bary.getZ() <= 1)
    {
        yCoord += mVertices[a].m_xyz[1] * bary.x;
        yCoord += mVertices[b].m_xyz[1] * bary.y;
        yCoord += mVertices[c].m_xyz[1] * bary.z;
    } else {
        bary = BarycCoordinates(p1,p3,p4,p);
        yCoord += mVertices[a].m_xyz[1] * bary.x;
        yCoord += mVertices[c].m_xyz[1] * bary.y;
        yCoord += mVertices[d].m_xyz[1] * bary.z;
    }
    return yCoord;
}



