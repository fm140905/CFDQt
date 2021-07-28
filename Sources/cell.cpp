#include "cell.h"

Particle Source::createParticle() const
{
    // uniform, [0. 1)
    double a = QRandomGenerator::global()->generateDouble();
    double b = QRandomGenerator::global()->generateDouble();
    if (b < a)
        std::swap(a, b);
    
    double initX = b * cylinder.getRadius() * qCos(2 * M_PI * a /b);
    double initY = b * cylinder.getRadius() * qSin(2 * M_PI * a /b);
    double initZ = QRandomGenerator::global()->generateDouble() * cylinder.getHeight();
    QVector3D initPos = QVector3D(initX, initY, initZ) + cylinder.getBaseCenter();
    // QVector3D initPos = cylinder.getBaseCenter();
    // initPos.setZ(10);

    double phi= 2 * M_PI * QRandomGenerator::global()->generateDouble();
    double costheta = 1 - 2 * QRandomGenerator::global()->generateDouble();
    double sintheta = qSqrt(1-costheta*costheta);
    QVector3D initDir = QVector3D(sintheta * qCos(phi), sintheta * qSin(phi), costheta);

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