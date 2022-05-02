#include "objmesh.h"

ObjMesh::ObjMesh(std::string filename, bool useNormals, float r, float g, float b)
{
    readFile(filename, useNormals, r, g, b);
    mMatrix.setToIdentity();
}

ObjMesh::~ObjMesh(){}


void ObjMesh::readFile(std::string filename, bool useNormals, float r, float g, float b)
{
    std::ifstream fileIn;
    fileIn.open(filename);
    if(!fileIn)
        qDebug() << "Could not open file for reading" << QString::fromStdString(filename);

    std::string oneLine;
    std::string oneWord;

    std::vector<gsl::Vector3D> tempVertices;
    std::vector<gsl::Vector3D> tempNormals;
    std::vector<gsl::Vector2D> tempUVs;

    unsigned int temp_index = 0;

    while(std::getline(fileIn, oneLine))
    {
        std::stringstream sStream;

        sStream << oneLine;

        oneWord = "";
        sStream >> oneWord;

        if(oneWord == "#")
            continue;
        if(oneWord == "")
            continue;
        if(oneWord == "v")
        {
            gsl::Vector3D tempVertex;
            sStream >> oneWord;
            tempVertex.x = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.y = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.z = std::stof(oneWord);

            tempVertices.push_back(tempVertex);

            continue;
        }
        if(oneWord == "vt")
        {
            gsl::Vector2D tempUV;
            sStream >> oneWord;
            tempUV.x = std::stof(oneWord);
            sStream >> oneWord;
            tempUV.z = std::stof(oneWord);

            tempUVs.push_back(tempUV);

            continue;
        }
        if(oneWord == "vn")
        {
            if(!useNormals)
            {
                gsl::Vector3D colors{r,g,b};
                tempNormals.push_back(colors);
            }
            else{
                gsl::Vector3D tempNormal;
                sStream >> oneWord;
                tempNormal.x = std::stof(oneWord);
                sStream >> oneWord;
                tempNormal.y = std::stof(oneWord);
                sStream >> oneWord;
                tempNormal.z = std::stof(oneWord);

                tempNormals.push_back(tempNormal);
                continue;
            }
        }
        if(oneWord == "f")
        {
            int index, normal, uv;
            for(int i = 0; i < 3; i++)
            {
                sStream >> oneWord;
                std::stringstream tempWord(oneWord);
                std::string segment;
                std::vector<std::string> segmentArray;
                while(std::getline(tempWord, segment, '/'))
                {
                    segmentArray.push_back(segment);
                }
                index = std::stoi(segmentArray[0]);
                if(segmentArray[1] != "")
                    uv = std::stoi(segmentArray[1]);
                else
                    uv = 0;

                normal = std::stoi(segmentArray[2]);

                --index;
                --uv;
                --normal;

                if(uv > -1)
                {
                    Vertex tempVert(tempVertices[index], tempNormals[normal], tempUVs[uv]);
                    mVertices.push_back(tempVert);
                } else {

                    Vertex tempVert(tempVertices[index], tempNormals[normal], gsl::Vector2D(0,0));
                    mVertices.push_back(tempVert);
                }
                mIndices.push_back(temp_index++);
            }
            continue;
        }
    }
    fileIn.close();
}

void ObjMesh::init(GLint matrixUniform)
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

void ObjMesh::draw()
{
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
