#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include <string>

#include "shader.h"
#include "mainwindow.h"
#include "logger.h"
#include "texture.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

     Point2D a{20, 70}, b{80, 70}, c{80, 30}, d{20, 30};
     mQuadTre = new QuadTree(a,b,c,d);
     mQuadTre->subDivide(1);


    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);
}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
//    glDeleteVertexArrays( 1, &mVAO );
//    glDeleteBuffers( 1, &mVBO );
}

// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{

    //Get the instance of the utility Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    mLogger = Logger::getInstance();

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Print render version info (what GPU is used):
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));
    mLogger->logText(tempString);

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines - at least with NVidia GPUs
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    checkForGLerrors();

    //*******Compile shaders********
    mShaderProgram[0] = new Shader("../Eksamen3DProg/plainshader.vert", "../Eksamen3DProg/plainshader.frag");
    mLogger->logText("Plain shader program id: " + std::to_string(mShaderProgram[0]->getProgram()));
    mShaderProgram[1] = new Shader("../Eksamen3DProg/textureshader.vert", "../Eksamen3DProg/textureshader.frag");
    mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[1]->getProgram()));
    mShaderProgram[2] = new Shader("../Eksamen3DProg/phongvertex.vert", "../Eksamen3DProg/phongfragment.frag");
    mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[2]->getProgram()));
    checkForGLerrors();

    setupPlainShader(0);
    setupTextureShader(1);
    setupPhongShader(2);

//*************Texture stuff**************

    mTexture[0] = new Texture();
    mTexture[1] = new Texture("../Eksamen3DProg/Assets/PlayerTexture.bmp");
    mTexture[2] = new Texture("../Eksamen3DProg/Assets/Grass.bmp");//Tekstur hentet fra https://www.creativeswall.com/65-free-high-resolution-grass-textures/

    //set the textures loaded to a texture unit (also called texture slot)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture[0]->id());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mTexture[1]->id());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mTexture[2]->id());

    //making objects to be drawn

    mQuadTre->init(mMatrixUniform0);
    mVisualObjects.push_back(mQuadTre);// [0]

    // Oppgave2
    mHeightmap = new Heightmap();
    mHeightmap->init(mMatrixUniform2);
    mVisualObjects.push_back(mHeightmap);// [1]

    //Oppgave 3
    mLight = new Light();
    mLight->init(mMatrixUniform0);
    mLight->mMatrix.translate(5.f, 20.f, -2.f);
    mLight->mMatrix.scale(5.f);
    mVisualObjects.push_back(mLight);// [2]

    //Oppgave 4
    mia = new InteractiveObject();
    mia->init(mMatrixUniform2);
    mia->createCollisionBox(false);
    mia->mMatrix.setPosition(50,0,70);
    mVisualObjects.push_back(mia);// [3]
    mMap.insert(std::pair<std::string, VisualObject*>{"mia", mia});

    //Oppgave 5
    cameraMesh = new ObjMesh("../Eksamen3DProg/Assets/obj_files/Camera.obj", false, 0,0,1);
    cameraMesh->init(mMatrixUniform0);
    mVisualObjects.push_back(cameraMesh);// [4]

    VisualObject *temp = new XYZ();
    temp->init(mMatrixUniform0);
    mVisualObjects.push_back(temp);// [5]

    for(int i = 0; i < 10; i++)
    {
        trofee = new Trofee(true);
        trofee->init(mMatrixUniform0);
        trofee->mMatrix.translate(trofee->redTrophies[i].x,4,trofee->redTrophies[i].z);
        trofee->mMatrix.scale(8);
        trofee->mScaled = 8;
        mVisualObjects.push_back(trofee);// [6 - 15]
        mQuadTre->insert(trofee->getPosition2D(), trofee);
    }

    for(int i = 0; i < 10; i++)
    {
        trofee = new Trofee(false);
        trofee->init(mMatrixUniform0);
        trofee->mMatrix.translate(trofee->blueTrophies[i].x,4,trofee->blueTrophies[i].z);
        trofee->mMatrix.scale(8);
        trofee->mScaled = 8;
        mVisualObjects.push_back(trofee);// [16 - 25]
        mQuadTre->insert(trofee->getPosition2D(), trofee);
    }

    enemy = new Enemy();
    enemy->init(mMatrixUniform0);
    mVisualObjects.push_back(enemy);// [26]

    temp = new CollisionAABB(trofee);
    temp->init(mMatrixUniform0);
    mVisualObjects.push_back(temp);// [27]

    //**********Set up camera************
    mCurrentCamera = new Camera();
    mCurrentCamera->setPosition(gsl::Vector3D(2.0f, 20.f, 15.f));
    mCurrentCamera->pitch(40.f);
    mCurrentCamera->yaw(-5.f);

    // Oppgave 5
    cameraMesh->mMatrix.rotateX(40);
    cameraMesh->mMatrix.rotateY(-5);

    glBindVertexArray(0);
    checkForGLerrors();

    //unbinds any VertexArray - good practice
}

// Called each frame - doing the rendering!!!
void RenderWindow::render()
{
    handleInput();

//    mCurrentCamera->lookAt(10,10,10);
    mCurrentCamera->update();

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    checkForGLerrors();


    //Oppgave 2
    glUseProgram(mShaderProgram[2]->getProgram() );
    // Heightmap
    glUniformMatrix4fv(vMatrixUniform2,1,GL_TRUE, mCurrentCamera->mViewMatrix.constData());
    glUniformMatrix4fv(pMatrixUniform2,1,GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
    glUniformMatrix4fv(mMatrixUniform2,1,GL_TRUE, mVisualObjects[1]->mMatrix.constData());
    glUniform3f(mLightPositionUniform, mLight->mMatrix.getPosition().x, mLight->mMatrix.getPosition().y, mLight->mMatrix.getPosition().z);
    glUniform3f(mCameraPositionUniform, mCurrentCamera->position().x, mCurrentCamera->position().y, mCurrentCamera->position().z);
    glUniform3f(mLightColorUniform, mLight->mLightColor.x, mLight->mLightColor.y, mLight->mLightColor.y);
    glUniform1f(mSpecularStrengthUniform,mLight->mSpecularStrength);
    //texture
    glUniform1i(mTextureUniform2, 2);

    mVisualObjects[1]->draw();

    //Oppgave 3
    drawObject(0, 2);//Lys
    mLight->mMatrix.translateY(0.03);
    mLight->mMatrix.rotateZ(0.2);

    //Oppgave 4
    glUseProgram(mShaderProgram[2]->getProgram() );
    // Spiller
    glUniformMatrix4fv(vMatrixUniform2,1,GL_TRUE, mCurrentCamera->mViewMatrix.constData());
    glUniformMatrix4fv(pMatrixUniform2,1,GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
    glUniformMatrix4fv(mMatrixUniform2,1,GL_TRUE, mVisualObjects[3]->mMatrix.constData());
    glUniform3f(mLightPositionUniform, mLight->mMatrix.getPosition().x, mLight->mMatrix.getPosition().y, mLight->mMatrix.getPosition().z);
    glUniform3f(mCameraPositionUniform, mCurrentCamera->position().x, mCurrentCamera->position().y, mCurrentCamera->position().z);
    glUniform3f(mLightColorUniform, mLight->mLightColor.x, mLight->mLightColor.y, mLight->mLightColor.y);
    glUniform1f(mSpecularStrengthUniform,mLight->mSpecularStrength);
    //texture
    glUniform1i(mTextureUniform2, 1);

    mVisualObjects[3]->draw();


    // Oppgave 5
    if(specMode)
    {
        drawObject(0,4);
        drawObject(0,5);// Oppgave 6
    }
    else
        cameraMesh->mMatrix.setPosition(mCurrentCamera->getPosition().x, mCurrentCamera->getPosition().y, mCurrentCamera->getPosition().z);



    for(int i = 6; i < 26; i++)
        drawObject(0,i);


    drawObject(0,26);// enemy

//    drawObject(0,27);// trofee collision

    drawObject(0,0);// Quadtre


    //Calculate framerate before
    // checkForGLerrors() because that call takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();
    checkForGLerrors();
    //using our expanded OpenGL debugger to check if everything is OK.

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.
    mContext->swapBuffers(this);
}

void RenderWindow::setupPlainShader(int shaderIndex)
{
    mMatrixUniform0 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "mMatrix");
    vMatrixUniform0 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "vMatrix");
    pMatrixUniform0 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "pMatrix");
}

void RenderWindow::setupTextureShader(int shaderIndex)
{
    mMatrixUniform1 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "mMatrix");
    vMatrixUniform1 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "vMatrix");
    pMatrixUniform1 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "pMatrix");
    mTextureUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "textureSampler");
}

void RenderWindow::setupPhongShader(int shaderIndex)
{
    mMatrixUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "mMatrix");
    vMatrixUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "vMatrix");
    pMatrixUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "pMatrix");

    mLightColorUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "lightColor");
    mObjectColorUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "objectColor");
    mAmbientLightStrengthUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "ambientStrength");
    mLightPositionUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "lightPosition");
    mSpecularStrengthUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "specularStrength");
    mSpecularExponentUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "specularExponent");
    mLightPowerUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "lightPower");
    mCameraPositionUniform = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "cameraPosition");
    mTextureUniform2 = glGetUniformLocation(mShaderProgram[shaderIndex]->getProgram(), "textureSampler");
}

void RenderWindow::drawPhong(int objNum)
{
    glUniform3f(mLightPositionUniform, mLight->mMatrix.getPosition().x, mLight->mMatrix.getPosition().y, mLight->mMatrix.getPosition().z);
    glUniform3f(mCameraPositionUniform, mCurrentCamera->position().x, mCurrentCamera->position().y, mCurrentCamera->position().z);
    glUniform3f(mLightColorUniform, mLight->mLightColor.x, mLight->mLightColor.y, mLight->mLightColor.y);
    glUniform1f(mSpecularStrengthUniform,mLight->mSpecularStrength);
    //texture
    glUniform1i(mTextureUniform2, 1);

    mVisualObjects[objNum]->draw();
}


void RenderWindow::drawObject(int shadeNum, int objNum)
{
    glUseProgram(mShaderProgram[shadeNum]->getProgram() );

    glUniformMatrix4fv(vMatrixUniform0,1,GL_TRUE, mCurrentCamera->mViewMatrix.constData());
    glUniformMatrix4fv(pMatrixUniform0,1,GL_TRUE, mCurrentCamera->mProjectionMatrix.constData());
    glUniformMatrix4fv(mMatrixUniform0,1,GL_TRUE, mVisualObjects[objNum]->mMatrix.constData());

    if(shadeNum == 1)
        glUniform1i(mTextureUniform, 1);

    mVisualObjects[objNum]->draw();
}


//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }

    mCurrentCamera->mProjectionMatrix.perspective(100.f, 1.7f, 0.1f, 100.f);
}

//The way this function is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                                                        // will use VIDEO memory as the source for
                                                        // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.");
                break;
            }
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
//Usually works on Windows machines, but not on Mac...
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}

void RenderWindow::handleInput()
{
    float speed;
    if(!specMode)
    {
        speed = 0.15;
        if(mInput.W)
            mMap["mia"]->mMatrix.translateZ(-speed);
        if(mInput.A)
            mMap["mia"]->mMatrix.rotateY(-2);
//            mMap["mia"]->mMatrix.translateX(-speed);
        if(mInput.S)
            mMap["mia"]->mMatrix.translateZ(speed);
        if(mInput.D){
            mMap["mia"]->mMatrix.rotateY(2);

        }

        float y = mHeightmap->SetYCoord(mMap["mia"]->mMatrix.getPosition().getX(), mMap["mia"]->mMatrix.getPosition().getZ());
        mMap["mia"]->mMatrix.setPositionY(y + 1);
//        qDebug() << mMap["mia"]->mMatrix.getPosition();

        mCurrentCamera->setPosition(mMap["mia"]->mMatrix.getPosition() + gsl::Vector3D(2, 20, 15));
    }
    else{
        speed = 0.8;
        if(mInput.W)
            mCurrentCamera->moveForward(speed);
        if(mInput.A)
            mCurrentCamera->moveRight(-speed);
        if(mInput.S)
            mCurrentCamera->moveForward(-speed);
        if(mInput.D)
            mCurrentCamera->moveRight(speed);
        if(mInput.E)
            mCurrentCamera->updateHeigth(speed);
        if(mInput.Q)
            mCurrentCamera->updateHeigth(-speed);
    }

    auto posisjon = mMap["mia"]->getPosition2D();
    auto subtre = mQuadTre->find(posisjon);
    for (auto it=subtre->m_sub_objects.begin();it!=subtre->m_sub_objects.end();it++)
        if((*it)->typeName == "pickup")
        {
//            qDebug() << (*it)->mMatrix.getPosition();
            mia->collision(*it);
        }
}


//Event sent from Qt when program receives a keyPress
// NB - see renderwindow.h for signatures on keyRelease and mouse input
void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        mMainWindow->close();       //Shuts down the whole program
    }

    //You get the keyboard input like this
    if(event->key() == Qt::Key_A)
        mInput.A = true;
    if(event->key() == Qt::Key_S)
        mInput.S = true;
    if(event->key() == Qt::Key_W)
        mInput.W = true;
    if(event->key() == Qt::Key_D)
        mInput.D = true;

    if(event->key() == Qt::Key_Shift)
        mInput.LShift = true;
    if(event->key() == Qt::Key_Control)
        mInput.LCtrl = true;



    if(event->key() == Qt::Key_X)
    {
        if(specMode)
        {
            specMode = false;
//            mCurrentCamera->pitch(mCurrentCamera->getPitch() + 40);
//            mCurrentCamera->yaw(mCurrentCamera->getYaw() - 5);

            mCurrentCamera->switchView();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            poly = false;
        }
        else if(!specMode){
            specMode = true;
            mCurrentCamera->switchView();
        }
    }

    if(event->key() == Qt::Key_E)
        mInput.E = true;
    if(event->key() == Qt::Key_Q)
        mInput.Q = true;

    if(event->key() == Qt::Key_Space)
    {
        if(poly)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            poly = false;
        }
        else if(!poly && specMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            poly = true;
        }
    }
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
        mInput.W = false;
    if(event->key() == Qt::Key_A)
        mInput.A = false;
    if(event->key() == Qt::Key_S)
        mInput.S = false;
    if(event->key() == Qt::Key_D)
        mInput.D = false;

    if(event->key() == Qt::Key_Shift)
        mInput.LShift = false;
    if(event->key() == Qt::Key_Control)
        mInput.LCtrl = false;

    if(event->key() == Qt::Key_E)
        mInput.E = false;
    if(event->key() == Qt::Key_Q)
        mInput.Q = false;
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        mInput.RMB = true;
    }
}
void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        mInput.RMB = false;
    }
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(mInput.RMB && specMode)
    {
        mouseX = event->pos().x() - mouseX;
        mouseY = event->pos().y() - mouseY;
//        qDebug() << mouseX;
//        qDebug() << mouseY;
        mCurrentCamera->yaw(mouseX * 0.1);
        mCurrentCamera->pitch(mouseY * 0.1);

    }
    mouseX = event->pos().x();
    mouseY = event->pos().y();
}







