#include <sstream>

#include "BFCHuman.h"

Qt::Key BFCHuman::XNegKey = Qt::Key_A, BFCHuman::XPosKey = Qt::Key_D, BFCHuman::YNegKey = Qt::Key_S, BFCHuman::YPosKey = Qt::Key_W;

BFCHuman::BFCHuman(BFObject *_obj) :
    BFController(_obj)
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
    if (obj->getType() == BFO_CIRCLE)
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
            cir->setProperty("shoot", theta);

            //qDebug("mouse left button pressed. %lf %s", theta);
        }

    }

    //...
    //not completed
}
