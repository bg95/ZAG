#include "BFCRandomShootDodge.h"

BFCRandomShootDodge::BFCRandomShootDodge(BFManager *_manager, BFObject *_obj) :
    BFController(_manager, _obj)
{
    if (!(*obj)["bullet prototype"].isValid())
        bulletv = -1.0;
    else
    {
        QByteArray ba = (*obj)["bullet prototype"].toByteArray();
        BFObject *bullet = manager->getFactory()->decodeNewObject(ba);
        bulletv = bullet->getVelocity().abs();
        bulletv = 6.0;
        manager->getFactory()->deleteObject(bullet);
        //qDebug("bulletv = %lf", bulletv);
    }
}

BFCRandomShootDodge::~BFCRandomShootDodge()
{
}

void BFCRandomShootDodge::applyControl()
{
    BFObject *aim;
    double dist = 1.0 / 0.0;
    for (auto iter = manager->getObjects().begin(); iter != manager->getObjects().end(); iter++)
    {
        if ((**iter)["isBullet"] != "Yes" && (**iter)["fraction"] != (*obj)["fraction"])
        {
            double d = ((*iter)->getPosition() - obj->getPosition()).abs();
            if (d < dist)
            {
                dist = d;
                aim = *iter;
            }
        }
    }
    if (dist != 1.0 / 0.0)
        shoot(aim);
}

void BFCRandomShootDodge::shoot(BFObject *aim)
{
    double t = (aim->getPosition() - obj->getPosition()).abs() / bulletv;
    double theta = (aim->getPosition() + aim->getVelocity() * t - obj->getPosition()).arg();
    shoot(theta);
}

void BFCRandomShootDodge::shoot(double theta)
{
    Vector2d r(cos(theta), sin(theta));
    Vector2d v0 = obj->getVelocity();
    double insqrt = (r & v0) * (r & v0) - (r & r) * ((v0 & v0) - bulletv * bulletv);
    if (insqrt >= 0.0)
    {
        double k = (-(r & v0) + sqrt(insqrt)) / (r & r);
        theta = (k * r - v0).arg();
    }

    obj->setProperty("shoot", theta);
}

void BFCRandomShootDodge::dodge(BFObject *bullet)
{
}
