#include <sstream>
#include <cmath>

#include "BFCHuman.h"

Qt::Key BFCHuman::XNegKey = Qt::Key_A, BFCHuman::XPosKey = Qt::Key_D, BFCHuman::YNegKey = Qt::Key_S, BFCHuman::YPosKey = Qt::Key_W;

BFCHuman::BFCHuman(BFManager *_manager, BFObject *_obj) :
    BFController(_manager, _obj)
{
}

BFCHuman::~BFCHuman()
{
}

BFControllerType BFCHuman::getType() const
{
    return BFC_HUMAN;
}

void BFCHuman::applyControl()
{
    if (obj->getShape() == BFO_CIRCULAR)
    {
        BFOCircle *cir = (BFOCircle *)obj;
        cir->a = Vector2d(0, 0);
        if (keyPressed(XNegKey))
            cir->a = cir->a - Vector2d(cir->maxa, 0);
        if (keyPressed(XPosKey))
            cir->a = cir->a + Vector2d(cir->maxa, 0);
        if (keyPressed(YNegKey))
            cir->a = cir->a - Vector2d(0, cir->maxa);
        if (keyPressed(YPosKey))
            cir->a = cir->a + Vector2d(0, cir->maxa);
        //qDebug("appling to object %lX: %lf,%lf", (long)obj, cir->a.x, cir->a.y);

        if (mousebut & Qt::LeftButton)
        {
            double theta = (mousepos - cir->p).arg();
            if (keyPressed(Qt::Key_Shift))
            {
                double closesttheta;
                double closestcos = -2;
                std::set<BFObject *>::iterator iter;
                for (iter = manager->getObjects().begin(); iter != manager->getObjects().end(); iter++)
                    if ((*iter) != cir && (*iter)->getProperty("isBullet") != "Yes")
                    {
                        BFOCircle *aim = (BFOCircle *)(*iter);
                        double t = ((*iter)->getPosition() - cir->p).abs() / 6.0;
                        double ttheta = (aim->p + aim->v * t - cir->p).arg();
                        if (cos(ttheta - theta) > closestcos)
                        {
                            closestcos = cos(ttheta - theta);
                            closesttheta = ttheta;
                        }
                    }
                if (closestcos == -2)
                    closesttheta = theta;
                cir->setProperty("shoot", closesttheta);
            }
            else
            {
                cir->setProperty("shoot", theta);
            }

            //qDebug("mouse left button pressed. %lf %s", theta);
        }

    }

    //...
    //not completed
}
