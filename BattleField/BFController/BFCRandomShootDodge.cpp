#include "BFCRandomShootDodge.h"

BFCRandomShootDodge::BFCRandomShootDodge(BFManager *_manager, BFObjectID _obj) :
    BFController(_manager, _obj), count(0)
{
    obj = getObjectPointer();
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
    obj = getObjectPointer();
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

    //randomWalk(event);
    dodge(event);

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

Vector2d BFCRandomShootDodge::intersectionPosition(Vector2d p1, Vector2d p2, Vector2d v1, Vector2d v2) {
    double k1 = v1.y / v1.x, k2 = v2.y / v2.x;
    double x = ( (k1 * p1.x - p1.y) - (k2 * p2.x - p2.y) ) / (k1 - k2);
    double y = k1 * (x - p1.x) + p1.y;
    return Vector2d(x, y);
}

void BFCRandomShootDodge::dodge(ControlEvent &event) {
    std::vector<Vector2d> pIntersections;
    BFObject* self = getObjectPointer();
    Vector2d pOwn = self->getPosition();
    Vector2d vOwn = self->getVelocity();

    for(auto iter = manager->getObjects().begin(); iter != manager->getObjects().end(); iter ++) {
        if((**iter)["isBullet"] == "Yea" && (**iter)["fraction"] != (*obj)["fraction"]) {
            Vector2d pBullet = (*iter)->getPosition();
            Vector2d vBullet = (*iter)->getVelocity();

            Vector2d pIntersection = intersectionPosition(pBullet, pOwn, vBullet, vOwn);

            double t1 = (pBullet - pIntersection).abs();
            double t2 = (pOwn - pIntersection).abs();
            double delta_t = 2 * ((*iter)->getRoughRadius() + self->getRoughRadius()) / (vBullet.abs() + vOwn.abs());
            if(t1 - t2 > delta_t || t2 - t1 > delta_t)//they never intersect
                break;
            pIntersections.push_back(pIntersection);
        }
    }

    Vector2d delta_a(0.0, 0.0);
    for(auto iter = pIntersections.begin(); iter != pIntersections.end(); iter ++) {
        delta_a = delta_a + (pOwn - (*iter)).unit();
    }
    delta_a = delta_a.unit();
    delta_a = delta_a * self->getMaxAcceleration();
    event.acc = delta_a;
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
