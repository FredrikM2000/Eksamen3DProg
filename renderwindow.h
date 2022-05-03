#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <unordered_map>

#include <vector>
#include "camera.h"
//#include "QuadTre.h"

#include "quadtree.h"

#include "vertex.h"
#include "graph.h"
#include "cube.h"
#include "xyz.h"
#include "trianglesurface.h"
#include "octahedronball.h"
#include "tetraeder.h"
#include "plan.h"
#include "visualobject.h"
#include "interactiveobject.h"
#include "beziercurve.h"
#include "disc.h"
#include "house.h"
#include "trofee.h"
#include "door.h"
#include "npc.h"
#include "visualcurve.h"
#include "light.h"
#include "enemy.h"
#include "heightmap.h"
#include "objmesh.h"
#include "collisionaabb.h"

#include "Input.h"


class QOpenGLContext;
class Shader;
class MainWindow;
class Texture;

/// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using "glad" and "glw" from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;  //gets called when app is shown and resized




    bool mRotate{true};     //Check if triangle should rotate

private slots:
    void render();          //the actual render - function

private:
    std::vector<VisualObject*> mObjects;
    std::unordered_map<std::string, VisualObject*>mMap;
    QuadTree* mQuadTre;

    //    gsml::Point2D a{-4, -4}, b{4, -4}, c{4, 4}, d{-4, 4}; // må gjøres ordentlig
//    gsml::QuadTre<std::string, VisualObject*>mQuadTre(std::pair<float, float>(1.f, 1.f));
//  QuadTre* mQuadTre;
    Camera mCamera;

    Light *mLight;
    Heightmap *mHeightmap;
    BezierCurve* bez;
    OctahedronBall* ball;
    Disc* disc;
    InteractiveObject* mia;
    Tetraeder* tet;
    Door* dor;
    NPC* npc;
    VisualCurve* curve;
    VisualCurve* curve1;
    ObjMesh* cameraMesh;
    Enemy* enemy;
    Trofee* trofee;



    void init();            //initialize things we need before rendering
    std::vector<Vertex>mVertices;

    QOpenGLContext *mContext{nullptr};  //Our OpenGL context
    bool mInitialized{false};

    //Shader *mShaderProgram{nullptr};    //holds pointer the GLSL shader program

    GLint  mPmatrixUniform;              //OpenGL reference to the Uniform in the shader program
    GLint  mVmatrixUniform;              //OpenGL reference to the Uniform in the shader program
    GLint  mMmatrixUniform;              //OpenGL reference to the Uniform in the shader program

    GLuint mVAO;                        //OpenGL reference to our VAO
    GLuint mVBO;                        //OpenGL reference to our VBO

    QMatrix4x4 *mMVPmatrix{nullptr};         //The matrix with the transform for the object we draw
    QMatrix4x4 *mPmatrix{nullptr};
    QMatrix4x4 *mVmatrix{nullptr};

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;               //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};  //helper class to get some clean debug info from OpenGL
    class Logger *mLogger{nullptr};         //logger - Output Log in the application

    ///Helper function that uses QOpenGLDebugLogger or plain glGetError()
    void checkForGLerrors();

    void calculateFramerate();          //as name says

    ///Starts QOpenGLDebugLogger if possible
    void startOpenGLDebugger();

    void handleInput();

    void setupPlainShader(int shaderIndex);
    GLint mMatrixUniform0{-1};
    GLint vMatrixUniform0{-1};
    GLint pMatrixUniform0{-1};

    void setupTextureShader(int shaderIndex);
    GLint mMatrixUniform1{-1};
    GLint vMatrixUniform1{-1};
    GLint pMatrixUniform1{-1};
    GLint mTextureUniform{-1};

    void setupPhongShader(int shaderIndex);
    GLint mMatrixUniform2{-1};
    GLint vMatrixUniform2{-1};
    GLint pMatrixUniform2{-1};

    //other light shader variables
    GLint mLightColorUniform{-1};
    GLint mObjectColorUniform{-1};
    GLint mAmbientLightStrengthUniform{-1};
    GLint mLightPositionUniform{-1};
    GLint mCameraPositionUniform{-1};
    GLint mSpecularStrengthUniform{-1};
    GLint mSpecularExponentUniform{-1};
    GLint mLightPowerUniform{-1};
    GLint mTextureUniform2{-1};


    Texture *mTexture[4]{nullptr};
    Shader *mShaderProgram[4]{nullptr};

    Camera *mCurrentCamera;
    int mouseX{0};
    int mouseY{0};

    std::vector<VisualObject*> mVisualObjects;

    Input mInput;

    bool specMode{false};// Spectator mode
    bool poly{false};

    void drawObject(int shadeNum, int objNum);
    void drawPhong(int objNum);

protected:
    //The QWindow that we inherit from have these functions to capture
    // - mouse and keyboard.
    // Uncomment to use (you also have to make the definitions of
    // these functions in the cpp-file to use them of course!)
    //
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    //    void wheelEvent(QWheelEvent *event) override{}
};

#endif // RENDERWINDOW_H
