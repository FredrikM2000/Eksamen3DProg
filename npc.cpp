#include "npc.h"
NPC::NPC()
{
    Vertex k1(-0.1,-0.1,0.1,0,0,1);
    Vertex k2(0.1,-0.1,0.1,0,0,1);
    Vertex k3(-0.1,0.1,0.1,0,0,1);
    Vertex k4(0.1,0.1,0.1,0,0,1);
    Vertex k5(0.1,-0.1,-0.1,0,0,1);
    Vertex k6(0.1,0.1,-0.1,0,0,1);
    Vertex k7(-0.1,-0.1,-0.1,0,0,1);
    Vertex k8(-0.1,0.1,-0.1,0,0,1);

    side(k1,k2,k3,k4);
    side(k2,k5,k4,k6);
    side(k5,k7,k6,k8);
    side(k7,k1,k8,k3);
    side(k3,k4,k8,k6);
    side(k7,k5,k1,k2);

    mMatrix.setToIdentity();
}

NPC::NPC(std::vector<Vertex> vertices)
{

    mVertices.push_back(Vertex{0,0,0, 0,0,5});
    mVertices.push_back(Vertex{3,0,0, 0,0,5});
    mVertices.push_back(Vertex{3,0,3, 0,0,5});
    mVertices.push_back(Vertex{0,0,3, 0,0,5});
    mVertices.push_back(Vertex{0,3,0, 0,0,5});
    mVertices.push_back(Vertex{3,3,0, 0,0,5});
    mVertices.push_back(Vertex{3,3,3, 0,0,5});
    mVertices.push_back(Vertex{0,3,3, 0,0,5});

    mIndices.insert(mIndices.end(), {0,1,4, 1,4,5, 1,2,5, 5,2,6, 2,3,6, 6,3,7, 3,0,7, 7,0,2, 0,1,2, 0,2,3, 4,5,6, 4,6,7});

    otherVertices = vertices;

    mMatrix.setToIdentity();
}

NPC::~NPC()
{

}

void NPC::init(GLint matrixUniform)
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

void NPC::draw()
{
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void NPC::moveTowards()//Finner ut hvilken retning neste trofee er, og beveger seg mot den
{//Oppgave 7
    if(i > otherVertices.size()-1)
    {
        turn = true;
        i--;
    }

    if(i < 0)
    {
        turn = false;
        i += 2;
    }

        float x = otherVertices[i].m_xyz[0];
        float y = otherVertices[i].m_xyz[1];
        float z = otherVertices[i].m_xyz[2];

        gsl::Vector3D pos = mMatrix.getPosition();
        gsl::Vector3D distance = {x - pos.x, y - pos.y, z - pos.z};

//        qDebug() << distance << i;


        {
        if(abs(distance.x) < 0.1 && abs(distance.y) < 0.1 && abs(distance.z) < 0.1)

        {
            if(!turn)
                i++;
            else
                i--;
        }
        distance.normalize();

        mMatrix.translateX(distance.x * speed * time);
        mMatrix.translateY(distance.y * speed * time);
        mMatrix.translateZ(distance.z * speed * time);
    }
}



void NPC::side(Vertex m_k1, Vertex m_k2, Vertex m_k3, Vertex m_k4)
{
    mVertices.push_back(m_k1);
    mVertices.push_back(m_k2);
    mVertices.push_back(m_k3);

    mVertices.push_back(m_k2);
    mVertices.push_back(m_k3);
    mVertices.push_back(m_k4);
}
