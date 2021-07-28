#include "customplotzoom.h"

CustomPlotZoom::CustomPlotZoom(QWidget * parent)
    : QCustomPlot(parent)
    , mZoomMode(false)
    , mRubberBand(new QRubberBand(QRubberBand::Rectangle, this))
    , autoScaled(true)
{}

CustomPlotZoom::~CustomPlotZoom()
{
    delete mRubberBand;
}

void CustomPlotZoom::setZoomMode(bool mode)
{
    mZoomMode = mode;
}

void CustomPlotZoom::setAutoScale(bool mode)
{
    autoScaled=mode;
}

bool CustomPlotZoom::isAutoScaled()
{
    return autoScaled;
}

void CustomPlotZoom::mousePressEvent(QMouseEvent * event)
{
    if (mZoomMode)
    {
        if (event->button() == Qt::RightButton)
        {
            mOrigin = event->pos();
            mRubberBand->setGeometry(QRect(mOrigin, QSize()));
            mRubberBand->show();
        }
    }
    pressed=true;
    QCustomPlot::mousePressEvent(event);
}

void CustomPlotZoom::mouseMoveEvent(QMouseEvent * event)
{
    if (mRubberBand->isVisible())
    {
        mRubberBand->setGeometry(QRect(mOrigin, event->pos()).normalized());
    }
    if(pressed)
        autoScaled=false;
    QCustomPlot::mouseMoveEvent(event);
}

void CustomPlotZoom::mouseReleaseEvent(QMouseEvent * event)
{
    if (mRubberBand->isVisible())
    {
        const QRect & zoomRect = mRubberBand->geometry();
        int xp1, yp1, xp2, yp2;
        zoomRect.getCoords(&xp1, &yp1, &xp2, &yp2);
        auto x1 = xAxis->pixelToCoord(xp1);
        auto x2 = xAxis->pixelToCoord(xp2);
        auto y1 = yAxis->pixelToCoord(yp1);
        auto y2 = yAxis->pixelToCoord(yp2);

        xAxis->setRange(x1, x2);
        yAxis->setRange(y1, y2);

        mRubberBand->hide();
        replot();
    }
    pressed=false;
    QCustomPlot::mouseReleaseEvent(event);
}

void CustomPlotZoom::wheelEvent(QWheelEvent *event)
{
    autoScaled=false;
    QCustomPlot::wheelEvent(event);
}
