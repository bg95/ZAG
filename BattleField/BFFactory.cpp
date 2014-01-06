#include "BFObject/BFOCircle.h"
#include "BFObject/BFOColoredCircle.h"
#include "BFController/BFController.h"

#include "BFFactory.h"

BFFactory::BFFactory()
{
    prototypes[BFO_CIRCLE] = new BFOCircle;
    prototypes[BFO_COLORED_CIRCLE] = new BFOColoredCircle;
}

BFFactory::~BFFactory()
{
    clear();
}

BFObject *BFFactory::newObject(BFObjectType type)
{
    std::map<BFObjectType, BFObject *>::iterator iter = prototypes.find(type);
    if (iter == prototypes.end())
        return 0;
    BFObject *p = (*iter).second->newObject();
    objects[p->getID()] = p;
    qDebug("new object of type %d id=%ld", (int)type, p->getID());
    return p;
}

void BFFactory::deleteObject(BFObject *o)
{
    std::map<long, BFObject *>::iterator iter = objects.find(o->getID());
    if (iter == objects.end())
        return;
    objects.erase(iter);
    delete o;
}

void BFFactory::encodeObject(BFObject *o, QIODevice *device)
{
    BFObjectType type;
    type = o->getType();
    qDebug("encode object type %d", (int)type);
    device->write((const char *)&type, sizeof(type)); //change to a string??
    o->encode(device);
}

BFObject *BFFactory::decodeNewObject(QIODevice *device)
{
    BFObjectType type;
    BFObject *o;
    device->read((char *)&type, sizeof(type));
    o = newObject(type);
    o->decode(device);
    return o;
}

void BFFactory::clear()
{
    std::map<long, BFObject *>::iterator oiter;
    for (oiter = objects.begin(); oiter != objects.end(); oiter++)
        delete (*oiter).second;
    objects.clear();
    std::map<BFObjectType, BFObject *>::iterator piter;
    for (piter = prototypes.begin(); piter != prototypes.end(); piter++)
        delete (*piter).second;
    prototypes.clear();
}
