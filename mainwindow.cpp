#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(Qt::white));

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    // ----------------------- My STL mesh code BEGIN -----------------------
    // Transform for positioning and scaling
    Qt3DCore::QTransform *stlTransform = new Qt3DCore::QTransform();
    stlTransform->setScale(1.0f);

    // Material for the STL
    Qt3DExtras::QPhongMaterial *stlMaterial = new Qt3DExtras::QPhongMaterial();
    stlMaterial->setAmbient(QColor(Qt::blue));

    // STL mesh
    Qt3DRender::QMesh *stlMesh = new Qt3DRender::QMesh();
    stlMesh->setSource(QUrl::fromLocalFile(":/storm.stl"));
    connect(stlMesh, &Qt3DRender::QMesh::statusChanged, this, &MainWindow::onMeshStatusChanged);    // sanity check

    // Entity for the mesh
    Qt3DCore::QEntity *meshEntity = new Qt3DCore::QEntity(rootEntity);
    meshEntity->addComponent(stlMesh);
    meshEntity->addComponent(stlTransform);
    meshEntity->addComponent(stlMaterial);

    // ----------------------- My STL mesh code END -----------------------

    // Create Torus from Qt example as a sanity check
    Qt3DExtras::QPhongMaterial *torusMaterial = new Qt3DExtras::QPhongMaterial();
    torusMaterial->setAmbient(QColor(Qt::blue));

    Qt3DCore::QEntity *torusEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DExtras::QTorusMesh *torusMesh = new Qt3DExtras::QTorusMesh;
    torusMesh->setRadius(5);
    torusMesh->setMinorRadius(1);
    torusMesh->setRings(100);
    torusMesh->setSlices(20);

    Qt3DCore::QTransform *torusTransform = new Qt3DCore::QTransform;
    torusTransform->setScale3D(QVector3D(1.5, 1, 0.5));
    torusTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 45.0f));

    torusEntity->addComponent(torusMesh);
    torusEntity->addComponent(torusTransform);
    torusEntity->addComponent(torusMaterial);

    // Camera
    Qt3DRender::QCamera *camera = view->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0, 0, 10.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));

    // Camera controller
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setCamera(camera);

    view->setRootEntity(rootEntity);
    view->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMeshStatusChanged(Qt3DRender::QMesh::Status status)
{
    qDebug() << "Status changed to " << status;
}

