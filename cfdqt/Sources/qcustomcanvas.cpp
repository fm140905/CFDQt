#include "qcustomcanvas.h"
#include <QFileInfo>
#include <QDebug>

QCustomCanvas::QCustomCanvas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotCanvas)
{
    ui->setupUi(this);
    ui->canvas->setZoomMode(true);
    connect(ui->ClearButton, &QToolButton::clicked, this, &QCustomCanvas::handleClear);
    connect(ui->UnZoomButton, &QToolButton::clicked, this, &QCustomCanvas::handleZoomout);
    connect(ui->LogButton, &QToolButton::clicked, this, &QCustomCanvas::handleLogscaled);
//    #ifdef _WIN32
//    ui->ScreenshotButton->hide();
//    #else
//    connect(ui->ScreenshotButton, &QToolButton::clicked, this, &QCustomCanvas::handleScreenshot);
//    #endif
    connect(ui->ScreenshotButton, &QToolButton::clicked, this, &QCustomCanvas::handleScreenshot);
    connect(ui->SaveButton, &QToolButton::clicked, this, &QCustomCanvas::handleSave2Txt);
}

QCustomCanvas::~QCustomCanvas()
{
    delete ui;
}

int QCustomCanvas::getID()
{
    return id;
}

bool QCustomCanvas::getLogScaled()
{
    return ui->LogButton->isChecked();
}

void QCustomCanvas::setCountsLabel(int counts)
{
    ui->countsLabel->setText(QString("NPS: %1").arg(QString::number(counts)));
}

void QCustomCanvas::handleSave2Txt()
{
    qDebug() << "save canvas to txt";
    emit canvasSaved2Txt(id);
}
void QCustomCanvas::handleClear()
{
    qDebug() << "clear canvas";
    emit canvasCleared(id);
}
void QCustomCanvas::handleZoomout()
{
    qDebug() << "zoom out";
    emit canvasZoomedout(id);
}

void QCustomCanvas::handleScreenshot()
{
    qDebug() << "save canvas to image";
    emit canvasSaved2Image(id);
}

void QCustomCanvas::handleLogscaled()
{
    qDebug() << "Set log scale to " << ui->LogButton->isChecked();
    emit canvasLogscaled(id);
}

void QCustomCanvas::hideLogButton()
{
    ui->LogButton->hide();
}
