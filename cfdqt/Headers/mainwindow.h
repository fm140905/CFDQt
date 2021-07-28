#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "qcustomcanvas.h"
#include "qt3dwidget.h"

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
    void initSpectrum();
    Qt3DCore::QTransform *detectorTransform;
private slots:
    void rotateDetector();
    void moveDetector();
    void updateSpectrum();
signals:

};

void drawLine(const QVector3D& start, const QVector3D& end, const QColor& color, Qt3DCore::QEntity *_rootEntity);

#endif // MAINWINDOW_H
