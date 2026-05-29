#ifndef MOTIONBLUR_H
#define MOTIONBLUR_H

#include "Blur.h"
#include <QVector>

class MotionBlur : public Blur
{
public:
    double angle;
    QVector< QVector <double> > vectorXYb;
    const QString getName() const{
        return "MotionBlur";
    }
};

#endif // MOTIONBLUR_H
