#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "ui_mainwindow.h"
#include "qcustomcanvas.h"
#include "qt3dwidget.h"
#include "cfd.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    void init3dwidget(Qt3DWidget* widget);
    void initSpectrum(CustomPlotZoom* customPlot);
    void updateSpectrum(CustomPlotZoom* customPlot, const Tally& tally);
    Qt3DCore::QTransform *detectorTransform;
    QThread workerThread;
    bool saveSpectrum2txt(const std::string fileName);
//    Tally* tally;
private slots:
    void rotateDetector();
    void moveDetector();
public slots:
    void handleResults(Tally tally);
    void handleUnZoom(int id=1);
    void handleScreenShot(int id=1);
    void handleClear(int id=1);
    void handleSave2txt(int id=1);
    void handleLogscale(int id=1);
signals:
    void centerChanged(QVector3D newCenter);
    void radiusChanged(double newRadius);
    void started();
};

void drawLine(const QVector3D& start, const QVector3D& end, const QColor& color, Qt3DCore::QEntity *_rootEntity);

#endif // MAINWINDOW_H
