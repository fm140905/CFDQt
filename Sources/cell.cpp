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
    a = QRandomGenerator::global()->generateDouble();
    for (std::size_t i = 0; i < energyCDF.getNBins(); i++)
    {
        if( a < energyCDF.getBinContent(i))
        {
            initE = energyCDF.getBinCenter(i);
            break;
        }
    }
    return Particle(initPos, initDir, initE, 1.0);
}

void Particle::scatter(const double cosAng)
{
    double alpha = 2 * M_PI * QRandomGenerator::global()->generateDouble(); // angel phi
    double R1 = dir.x();
    double R2 = dir.y();
    double R3 = dir.z();
    double sinAng = std::sqrt(1 - cosAng * cosAng);
    if (std::abs(std::abs(R3) - 1) > 1e-8)
    {
        double eta = 1.0 / std::sqrt(1 - R3 * R3);
        dir.setX(cosAng * R1 + sinAng * (std::cos(alpha) * R3 * R1 - std::sin(alpha) * R2) * eta);
        dir.setY(cosAng * R2 + sinAng * (std::cos(alpha) * R3 * R2 + std::sin(alpha) * R1) * eta);
        dir.setZ(cosAng * R3 - sinAng * std::cos(alpha) / eta);
    }
    else
    {
        dir.setX(sinAng * std::cos(alpha));
        dir.setY(sinAng * std::sin(alpha));
        dir.setZ(cosAng * R3);
    }
}