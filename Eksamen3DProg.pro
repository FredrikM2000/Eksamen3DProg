QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

INCLUDEPATH += ./gsl

SOURCES += main.cpp \
    beziercurve.cpp \
    bomb.cpp \
    camera.cpp \
    collisionaabb.cpp \
    collisionsphere.cpp \
    cube.cpp \
    disc.cpp \
    door.cpp \
    enemy.cpp \
    fence.cpp \
    graph.cpp \
    heightmap.cpp \
    house.cpp \
    interactiveobject.cpp \
    light.cpp \
    logger.cpp \
    mainwindow.cpp \
    gsl/matrix3x3.cpp \
    gsl/matrix4x4.cpp \
    npc.cpp \
    objmesh.cpp \
    octahedronball.cpp \
    plan.cpp \
    quadtree.cpp \
    renderwindow.cpp \
    shader.cpp \
    tetraeder.cpp \
    texture.cpp \
    trianglesurface.cpp \
    trofee.cpp \
    gsl/vector2d.cpp \
    gsl/vector3d.cpp \
    vertex.cpp \
    visualcurve.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    Input.h \
    QuadTre.h \
    beziercurve.h \
    bomb.h \
    camera.h \
    collisionaabb.h \
    collisionsphere.h \
    cube.h \
    disc.h \
    door.h \
    enemy.h \
    fence.h \
    gsl/gltypes.h \
    graph.h \
    heightmap.h \
    house.h \
    interactiveobject.h \
    light.h \
    logger.h \
    mainwindow.h \
    gsl/math_constraints.h \
    gsl/matrix3x3.h \
    gsl/matrix4x4.h \
    npc.h \
    objmesh.h \
    octahedronball.h \
    plan.h \
    quadtree.h \
    renderwindow.h \
    shader.h \
    stb_image.h \
    tetraeder.h \
    texture.h \
    trianglesurface.h \
    trofee.h \
    gsl/vector2d.h \
    gsl/vector3d.h \
    vertex.h \
    visualcurve.h \
    visualobject.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Cube.txt \
    Test.txt \
    matteOblig2.txt \
    matte_oblig1.txt \
    phong_wo_texture.frag \
    phong_wo_texture.vert \
    phongfragment.frag \
    phongvertex.vert \
    plainshader.frag \
    plainshader.vert \
    rainbowCube.txt \
    textureshader.frag \
    textureshader.vert \
    trofeModell.txt \
    xy_function.txt
