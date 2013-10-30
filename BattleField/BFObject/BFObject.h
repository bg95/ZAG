#ifndef BFOBJECT_H
#define BFOBJECT_H

#include <QGLWidget>
//#include "BFManager.h"

class BFManager;

enum BFObjectType
{
    BFO_CIRCLE
};

class BFObject
{
public:
    static long count;

    BFObject(BFManager *_manager);
    virtual ~BFObject();
    virtual void draw(QGLWidget *) = 0;
    virtual BFObjectType getType() const = 0;
    virtual double getRoughRadius() const = 0;
    virtual double getPosition() const = 0;

    bool intersectWith(BFObject *);
    bool operator <(BFObject &b)
    {
        return id < b.id;
    }

private:
    long id;
    BFManager *manager;

};

#include "../BFManager.h"

#endif // BFOBJECT_H
