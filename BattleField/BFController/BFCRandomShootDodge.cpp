#include "BFCRandomShootDodge.h"

BFCRandomShootDodge::BFCRandomShootDodge(BFManager *_manager, BFObjectID _obj) :
    BFController(_manager, _obj), count(0)
{
    init(_obj);
}

BFCRandomShootDodge::BFCRandomShootDodge(BFManager *_manager, std::vector<BFObjectID> _obj) :
    BFController(_manager, _obj), count(0)
{
    if (_obj.size() == 0)
    {
        bulletv = -1.0;
        return;
    }
    init(_obj[0]);
}

BFCRandomShootDodge::~BFCRandomShootDodge()
{
}

std::vector<ControlEvent> &BFCRandomShootDodge::getControl()
{
    applyControl();
    return controlevents;
}

void BFCRandomShootDodge::applyControl()
{
    controlevents.clear();
    obj = getPrincipalObjectPointer();
    if (!obj)
        return;
    ControlEvent event(obj->getID());
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
        shoot(event, aim);

    randomWalk(event);

    controlevents.push_back(event);
}

void BFCRandomShootDodge::shoot(ControlEvent &event, BFObject *aim)
{
    double t = (aim->getPosition() - obj->getPosition()).abs() / bulletv;
    double theta = (aim->getPosition() + aim->getVelocity() * t - obj->getPosition()).arg();
    shoot(event, theta);
}

void BFCRandomShootDodge::shoot(ControlEvent &event, double theta)
{
    Vector2d r(cos(theta), sin(theta));
    Vector2d v0 = obj->getVelocity();
    double insqrt = (r & v0) * (r & v0) - (r & r) * ((v0 & v0) - bulletv * bulletv);
    if (insqrt >= 0.0)
    {
        double k = (-(r & v0) + sqrt(insqrt)) / (r & r);
        theta = (k * r - v0).arg();
    }

    //obj->setProperty("shoot", theta);
    event.addPropertyChange("shoot", theta);
}

void BFCRandomShootDodge::dodge(ControlEvent &event, BFObject *bullet)
{
}

void BFCRandomShootDodge::randomWalk(ControlEvent &event)
{
    //obj = getObjectPointer();
    if (count <= 0)
    {
        theta = rand() * 2 * PI / (double)RAND_MAX;
        a = rand() * obj->getMaxAcceleration() / (double)RAND_MAX;
        count = rand() & 255;
    }
    count--;
    event.acc = a * Vector2d(cos(theta), sin(theta));
}

void BFCRandomShootDodge::init(BFObjectID _obj)
{
     obj = getObjectPointer(_obj);
     if (!obj)
         return;
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
