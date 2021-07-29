#include "mainwindow.h"
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DRender/QPointLight>
#include <QSlider>

#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QGeometry>

#include "geometry.h"
#include "cfdworker.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init3dwidget(ui->openGLWidget);

//    QTimer* animationTimer = new QTimer(this);
//    animationTimer->setInterval(10);
//    connect(animationTimer, &QTimer::timeout, this, &MainWindow::rotateDetector);
//    animationTimer->start();

    ui->horizontalSlider->setMaximum(1000);
    ui->horizontalSlider->setMinimum(-1000);
//    ui->horizontalSlider->setTracking(false);
    ui->horizontalSlider_2->setMaximum(1000);
    ui->horizontalSlider_2->setMinimum(-1000);
//    ui->horizontalSlider_2->setTracking(false);
    ui->horizontalSlider_3->setMaximum(1000);
    ui->horizontalSlider_3->setMinimum(-1000);
//    ui->horizontalSlider_3->setTracking(false);

//    connect(ui->horizontalSlider, &QSlider::sliderMoved, this, &MainWindow::moveDetector);
//    connect(ui->horizontalSlider_2, &QSlider::sliderMoved, this, &MainWindow::moveDetector);
//    connect(ui->horizontalSlider_3, &QSlider::sliderMoved, this, &MainWindow::moveDetector);

    connect(ui->horizontalSlider, &QSlider::sliderReleased, this, &MainWindow::moveDetector);
    connect(ui->horizontalSlider_2, &QSlider::sliderReleased, this, &MainWindow::moveDetector);
    connect(ui->horizontalSlider_3, &QSlider::sliderReleased, this, &MainWindow::moveDetector);
//    tally = new Tally(Sphere(QVector3D(100, 100, 10), 2.54), 100, 0, 1.0);

    initSpectrum(ui->spectrum->getCanvas());

    CFDWorker *worker = new CFDWorker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::started, worker, &CFDWorker::work);
    connect(worker, &CFDWorker::spectrumChanged, this, &MainWindow::handleResults);
    connect(this, &MainWindow::centerChanged, worker, &CFDWorker::onCenterChanged);
    connect(this, &MainWindow::radiusChanged, worker, &CFDWorker::onRadiusChanged);
    workerThread.start();

    emit started();

}

MainWindow::~MainWindow()
{
    delete ui;
    workerThread.quit();
    workerThread.wait();
}

void MainWindow::init3dwidget(Qt3DWidget* widget)
{
    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    //    // draw origin
    drawLine({ 0, 0, 0 }, { 100, 0, 0 }, Qt::red, rootEntity); // X
    drawLine({ 0, 0, 0 }, { 0, 100, 0 }, Qt::green, rootEntity); // Y
    drawLine({ 0, 0, 0 }, { 0, 0, 100 }, Qt::blue, rootEntity); // Z

    // water cylinder
    // Cylinder shape data
    Qt3DExtras::QCylinderMesh *cylinder = new Qt3DExtras::QCylinderMesh();
    cylinder->setRadius(21.5);
    cylinder->setLength(52);
    cylinder->setRings(20);
    cylinder->setSlices(20);
    // CylinderMesh Transform
    Qt3DCore::QTransform *cylinderTransform = new Qt3DCore::QTransform();
//    cylinderTransform->setScale(1.5f);
    cylinderTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 90.0f));
    cylinderTransform->setTranslation(QVector3D(25, 25, 26));
    Qt3DExtras::QPhongMaterial *cylinderMaterial = new Qt3DExtras::QPhongMaterial();
//    cylinderMaterial->setDiffuse(QColor(QRgb(0x928327)));
    cylinderMaterial->setAmbient("red");
    // Cylinder
    Qt3DCore::QEntity *cylinderEntity = new Qt3DCore::QEntity(rootEntity);
    cylinderEntity->addComponent(cylinder);
    cylinderEntity->addComponent(cylinderMaterial);
    cylinderEntity->addComponent(cylinderTransform);

    // detector sphere
    // Sphere shape data
    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
    sphereMesh->setRings(20);
    sphereMesh->setSlices(20);
    sphereMesh->setRadius(2.54);
    // Sphere mesh transform
    detectorTransform = new Qt3DCore::QTransform();
//    sphereTransform->setScale(1.3f);
    detectorTransform->setTranslation(QVector3D(100.0f, 100.0f, 10.0f));
    Qt3DExtras::QPhongMaterial *sphereMaterial = new Qt3DExtras::QPhongMaterial();
    sphereMaterial->setDiffuse(QColor(QRgb(0xa69929)));
    // Sphere
    Qt3DCore::QEntity* sphereEntity = new Qt3DCore::QEntity(rootEntity);
    sphereEntity->addComponent(sphereMesh);
    sphereEntity->addComponent(sphereMaterial);
    sphereEntity->addComponent(detectorTransform);

    // camera
    Qt3DExtras::QOrbitCameraController *cameraController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    cameraController->setCamera(widget->camera());
    //    widget->camera()->setPosition(QVector3D(-50, -50, 100));
    widget->camera()->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    widget->camera()->setPosition(QVector3D(200, 200, 200.0f));
    widget->camera()->setUpVector(QVector3D(0, 0, -1));
    widget->camera()->setViewCenter(QVector3D(25, 25, 10));

    // light
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(widget->camera()->position());
    lightEntity->addComponent(lightTransform);

    Qt3DExtras::QForwardRenderer *forwardRenderer = new Qt3DExtras::QForwardRenderer;
    forwardRenderer->setCamera(widget->camera());
    forwardRenderer->setClearColor(Qt::gray);
    widget->setActiveFrameGraph(forwardRenderer);

    widget->setRootEntity(rootEntity);
}

void MainWindow::rotateDetector()
{
    static double angle = 0;
//    angle += 1;
    detectorTransform->setMatrix(detectorTransform->rotateAround(QVector3D(25,25,10), angle++, QVector3D(0,0,1)));
}

void MainWindow::moveDetector()
{
    static QVector3D initPos = QVector3D(100, 100, 10);
    const double stepSize(0.01);
    QObject* obj = sender();
    QVector3D newPos{0, 0, 0};
    if( obj == ui->horizontalSlider )
    {
        newPos = QVector3D(ui->horizontalSlider->value(), 0, 0);
    }
    else if( obj == ui->horizontalSlider_2 )
    {
        newPos = QVector3D(0, ui->horizontalSlider_2->value(), 0);
    }
    else if( obj == ui->horizontalSlider_3 )
    {
        newPos = QVector3D(0, 0, ui->horizontalSlider_3->value());
    }
    newPos = newPos * stepSize + initPos;
    // update 3d view
    detectorTransform->setTranslation(newPos);
    // update spectrum
    QSlider* slider = qobject_cast<QSlider*>(sender());
//    if(!slider->isSliderDown())
    {
        emit centerChanged(newPos);
    }
}

void drawLine(const QVector3D& start, const QVector3D& end, const QColor& color, Qt3DCore::QEntity *_rootEntity)
{
    auto *geometry = new Qt3DRender::QGeometry(_rootEntity);

    // position vertices (start and end)
    QByteArray bufferBytes;
    bufferBytes.resize(3 * 2 * sizeof(float)); // start.x, start.y, start.end + end.x, end.y, end.z
    float *positions = reinterpret_cast<float*>(bufferBytes.data());
    *positions++ = start.x();
    *positions++ = start.y();
    *positions++ = start.z();
    *positions++ = end.x();
    *positions++ = end.y();
    *positions++ = end.z();

    auto *buf = new Qt3DRender::QBuffer(geometry);
    buf->setData(bufferBytes);

    auto *positionAttribute = new Qt3DRender::QAttribute(geometry);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(buf);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(2);
    geometry->addAttribute(positionAttribute); // We add the vertices in the geometry

    // connectivity between vertices
    QByteArray indexBytes;
    indexBytes.resize(2 * sizeof(unsigned int)); // start to end
    unsigned int *indices = reinterpret_cast<unsigned int*>(indexBytes.data());
    *indices++ = 0;
    *indices++ = 1;

    auto *indexBuffer = new Qt3DRender::QBuffer(geometry);
    indexBuffer->setData(indexBytes);

    auto *indexAttribute = new Qt3DRender::QAttribute(geometry);
    indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(2);
    geometry->addAttribute(indexAttribute); // We add the indices linking the points in the geometry

    // mesh
    auto *line = new Qt3DRender::QGeometryRenderer(_rootEntity);
    line->setGeometry(geometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    auto *material = new Qt3DExtras::QPhongMaterial(_rootEntity);
    material->setAmbient(color);

    // entity
    auto *lineEntity = new Qt3DCore::QEntity(_rootEntity);
    lineEntity->addComponent(line);
    lineEntity->addComponent(material);
}

void MainWindow::initSpectrum(CustomPlotZoom* customPlot)
{
    //TODO
    // configure axis rect:
    customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
    customPlot->axisRect()->setupFullAxesBox(true);
    customPlot->xAxis->setLabel("Energy (MeV)");
    customPlot->yAxis->setLabel("Fluence per unit NPS");

    // setup step line plot
    customPlot->addGraph();
//    plotdata->mMutex.lock();
//    customPlot->graph()->setData(plotdata->getCoinBinCenters(), plotdata->getCoinBinCounts(), true);
//    ui->spectrumCanvas->setCountsLabel(plotdata->getCoinTotalCounts());
//    plotdata->mMutex.unlock();
    customPlot->graph()->setLineStyle(QCPGraph::lsStepCenter);
    customPlot->graph()->setBrush(QBrush(QColor(0, 0, 255, 20))); // fill with translucent blue
    //    customPlot->rescaleAxes();
    customPlot->yAxis->setRangeLower(0);
    //    customPlot->yAxis->setRange(0, 500);

}

void MainWindow::updateSpectrum(CustomPlotZoom* customPlot, const Tally& tally)
{
    // TODO
    customPlot->graph()->data().clear();
    int counts = tally.getNPS();
    std::vector<double> bins = tally.hist.getBinCenters();
    std::vector<double> flux = tally.hist.getBinContents();
    customPlot->graph()->setData(QVector<double>(bins.begin(), bins.end()),
                                 QVector<double>(flux.begin(), flux.end()),true);

    ui->spectrum->setCountsLabel(counts);
    if (counts > 0 && customPlot->isAutoScaled())
    {
        handleUnZoom();
        //        customPlot->rescaleAxes();
    }
    if(counts > 0 && ui->spectrum->getLogScaled())
        customPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    else
        customPlot->yAxis->setScaleType(QCPAxis::stLinear);
    customPlot->replot();
}

void MainWindow::handleUnZoom()
{
    ui->spectrum->getCanvas()->rescaleAxes();
    ui->spectrum->getCanvas()->yAxis->scaleRange(1.1);
    if(!ui->spectrum->getLogScaled())
    {
        ui->spectrum->getCanvas()->yAxis->setRangeLower(0);
    }
    else
    {
        ui->spectrum->getCanvas()->yAxis->setRangeLower(1e-7);
    }
    ui->spectrum->getCanvas()->setAutoScale(true);

}
void MainWindow::handleResults(Tally tally)
{
    updateSpectrum(ui->spectrum->getCanvas(), tally);
}
