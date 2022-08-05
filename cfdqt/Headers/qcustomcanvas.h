#ifndef QCUSTOMCANVAS_H
#define QCUSTOMCANVAS_H

#include "ui_plotcanvas.h"
#include "customplotzoom.h"
#include <QMainWindow>

namespace Ui {
class PlotCanvas;
}

class QCustomCanvas : public QWidget
{
    Q_OBJECT
private:
    Ui::PlotCanvas* ui;
    int id;
public:
    explicit QCustomCanvas(QWidget *parent = nullptr);
    void setID(int id_) {id = id_;}
    ~QCustomCanvas();

    void setCountsLabel(int counts);
    CustomPlotZoom* getCanvas(){return ui->canvas;}

    bool getLogScaled();

    int getID();

    void hideLogButton();
signals:
    void canvasCleared(int id_);
    void canvasSaved2Image(int id_);
    void canvasSaved2Txt(int id_);
    void canvasZoomedout(int id_);
    void canvasLogscaled(int id_);

public slots:
    void handleSave2Txt();
    void handleClear();
    void handleZoomout();
    void handleScreenshot();
    void handleLogscaled();
};
#endif // QCUSTOMCANVAS_H
