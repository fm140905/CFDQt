#include "cell.h"

Particle Source::createParticle() const
{
    // uniform, [0. 1)
    double initZ = QRandomGenerator::global()->generateDouble() * cylinder.getHeight();
    double initX, initY;
    do
    {
        initX = cylinder.getRadius() * QRandomGenerator::global()->generateDouble();
        initY = cylinder.getRadius() * QRandomGenerator::global()->generateDouble();
    } while (initX * initX + initY * initY >= qPow(cylinder.getRadius(), 2));
    QVector3D initPos = QVector3D(initX, initY, initZ) + cylinder.getBaseCenter();

    double phi= 2 * M_PI * QRandomGenerator::global()->generateDouble();
    double theta = qAcos(1 - 2 * QRandomGenerator::global()->generateDouble());
    QVector3D initDir = QVector3D(qCos(phi) * qSin(theta), qSin(phi) * qSin(theta), qCos(theta));

    double initE = 0;
    for (std::size_t i = 0; i < energyCDF.getNBins(); i++)
    {
        if( QRandomGenerator::global()->generateDouble() < energyCDF.getBinContent(i))
        {
            initE = energyCDF.getBinCenter(i);
            break;
        }
    }
    return Particle(initPos, initDir, initE, 1.0);
}