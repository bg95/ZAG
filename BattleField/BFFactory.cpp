#include "BFObject/BFOCircle.h"
#include "BFObject/BFOColoredCircle.h"
#include "BFController/BFController.h"

#include <QBuffer>

#include "BFFactory.h"

BFFactory::BFFactory()
{
    prototypes[typehash(BFOCircle)] = new BFOCircle;
    prototypes[typehash(BFOColoredCircle)] = new BFOColoredCircle;
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
    p->created_from_factory = true;
    objects[p->getID()] = p;
    //qDebug("new object of type %ld id=%ld", (long)type, p->getID());
    return p;
}

void BFFactory::deleteObject(BFObject *o)
{
    std::map<BFObjectID, BFObject *>::iterator iter = objects.find(o->getID());
    if (iter == objects.end())
        return;
    qDebug("delete object %ld", o->getID());
    objects.erase(iter);
    o->deleting_from_factory = true;
    delete o;
}

BFObject *BFFactory::replaceObject(BFObjectID id, BFObjectType type)
{
    std::map<BFObjectType, BFObject *>::iterator piter = prototypes.find(type);
    if (piter == prototypes.end())
    {
        qDebug("Cannot find prototype");
        return 0;
    }
    std::map<BFObjectID, BFObject *>::iterator iter = objects.find(id);
    BFObject *p = (*piter).second->newObject();
    p->setID(id);
    if (iter == objects.end())
    {
        objects[id] = p;
        qDebug("replace non-existing object %ld->%lX. #obj=%d", id, (unsigned long)p, objects.size());
        return p;
    }
    (*iter).second->deleting_from_factory = true;
    delete (*iter).second;
    (*iter).second = p;
    //qDebug("replace object of type %ld id=%ld", (long)type, p->getID());
    return p;
}

void BFFactory::encodeObject(BFObject *o, QIODevice *device)
{
    BFObjectType type;
    type = o->getType();
    //qDebug("encode object type %ld", (long)type);
    device->write((const char *)&type, sizeof(type)); //Change to a string? No.
    device->write((const char *)&o->id, sizeof(o->id));
    o->encode(device);
}

BFObject *BFFactory::decodeNewObject(QIODevice *device)
{
    BFObjectType type;
    BFObject *o;
    BFObjectID tid;
    device->read((char *)&type, sizeof(type));
    device->read((char *)&tid, sizeof(tid));
    o = newObject(type);
    if (o)
        o->decode(device);
    return o;
}

BFObject *BFFactory::decodeNewObject(QByteArray ba)
{
    QBuffer buf(&ba);
    buf.open(QIODevice::ReadOnly);
    return decodeNewObject(&buf);
}

BFObject *BFFactory::decodeReplaceObject(/*BFObjectID id, */QIODevice *device)
{
    BFObjectType type;
    BFObject *o;
    BFObjectID tid;
    device->read((char *)&type, sizeof(type));
    device->read((char *)&tid, sizeof(tid));
    o = replaceObject(tid, type);
    if (!o)
        return 0;
    o->decode(device);
    return o;
}

void BFFactory::clear()
{
    std::map<BFObjectID, BFObject *>::iterator oiter;
    for (oiter = objects.begin(); oiter != objects.end(); oiter++)
        delete (*oiter).second;
    objects.clear();
    std::map<BFObjectType, BFObject *>::iterator piter;
    for (piter = prototypes.begin(); piter != prototypes.end(); piter++)
        delete (*piter).second;
    prototypes.clear();
}

BFObject *BFFactory::objectByID(BFObjectID id)
{
    auto iter = objects.find(id);
    if (iter == objects.end())
        return 0;
    return (*iter).second;
}
