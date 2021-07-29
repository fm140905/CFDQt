#ifndef CFDWORKER_H
#define CFDWORKER_H

#include <QObject>
#include "geometry.h"
#include "data.h"
#include "cell.h"
#include "tracking.h"
#include "cfd.h"
#include <atomic>

class CFDWorker : public QObject
{
    Q_OBJECT
public:
    explicit CFDWorker(QObject *parent = nullptr);
    ~CFDWorker();
private:
    Tally tally;
    MCSettings* config;
    void initSetup();
    std::atomic<bool> stop;
    std::atomic<bool> changed;
    void getSpectrum();
signals:
    void spectrumChanged(Tally t);
public slots:
    void work();
    void onCenterChanged(QVector3D newCenter);
    void onRadiusChanged(double newradius);
//    void listen(QVector3D newCenter);
    void stopWorker();
};

#endif // CFDWORKER_H
