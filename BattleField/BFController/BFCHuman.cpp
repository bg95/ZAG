#include <sstream>
#include <cmath>

#include "BFCHuman.h"

Qt::Key BFCHuman::XNegKey = Qt::Key_A, BFCHuman::XPosKey = Qt::Key_D, BFCHuman::YNegKey = Qt::Key_S, BFCHuman::YPosKey = Qt::Key_W;

BFCHuman::BFCHuman(BFManager *_manager, BFObjectID _obj) :
    BFCRandomShootDodge(_manager, _obj)
{
}

BFCHuman::BFCHuman(BFManager *_manager, std::vector<BFObjectID> _obj) :
    BFCRandomShootDodge(_manager, _obj)
{
}

BFCHuman::~BFCHuman()
{
}
/*
BFControllerType BFCHuman::getType() const
{
    return BFC_HUMAN;
}
*/
std::vector<ControlEvent> &BFCHuman::getControl()
{
    applyControl();
    qDebug("getHumanControl %lf,%lf", controlevents[0].acc.x, controlevents[0].acc.y);
    return controlevents;
}

void BFCHuman::applyControl()
{
    controlevents.clear();
    obj = getPrincipalObjectPointer();
    if (!obj)
        return;
    ControlEvent event(obj->getID());
    Vector2d a = Vector2d(0, 0);
    if (keyPressed(XNegKey))
        a = a - Vector2d(1, 0);
    if (keyPressed(XPosKey))
        a = a + Vector2d(1, 0);
    if (keyPressed(YNegKey))
        a = a - Vector2d(0, 1);
    if (keyPressed(YPosKey))
        a = a + Vector2d(0, 1);
    double bfrot = manager->getBattleField()->getRotation();
    a = a.rotate(-bfrot);
    if (a.abs() > 0)
        a = a / a.abs();
    //cir->a = a * cir->maxa;
    event.acc = a * obj->getMaxAcceleration();
    //qDebug("appling to object %lX: %lf,%lf", (long)obj, cir->a.x, cir->a.y);

    if (mousebut & Qt::LeftButton)
    {
        BFObject *faim = 0;
        double theta = (mousepos - obj->getPosition()).arg();
        if (keyPressed(Qt::Key_Shift))
        {
            //qDebug("shift pressed");
            double closestcos = -2;
            std::set<BFObject *>::iterator iter;
            for (iter = manager->getObjects().begin(); iter != manager->getObjects().end(); iter++)
            {
                //qDebug("object");
                if ((*iter)->getID() != obj->getID() &&
                        (*iter)->getProperty("isBullet") != "Yes" &&
                        (*iter)->getProperty("fraction") != (*obj)["fraction"])
                {
                    BFObject *aim = (*iter);
                    double t = (aim->getPosition() - obj->getPosition()).abs() / bulletv;
                    double ttheta = (aim->getPosition() + aim->getVelocity() * t - obj->getPosition()).arg();

                    //qDebug("ttheta = %lf (%lf deg)", ttheta, ttheta / PI * 180.0);
                    if (cos(ttheta - theta) > closestcos)
                    {
                        closestcos = cos(ttheta - theta);
                        faim = aim;
                    }
                }
            }
            if (!faim)
            {
                //closesttheta = theta;
                shoot(event, theta);
            }
            else
            {
                //theta = closesttheta;
                shoot(event, faim);
            }
        }
        else
            shoot(event, theta);
        //qDebug("mouse left button pressed. %lf %s", theta);

    }
    //...
    //not completed
    controlevents.push_back(event);
}
