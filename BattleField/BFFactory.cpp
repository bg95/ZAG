#include "BFObject/BFOCircle.h"
#include "BFController/BFController.h"

#include "BFFactory.h"

BFFactory::BFFactory()
{
}

void BFFactory::encodeObject(BFObject *o, QIODevice *device)
{
    BFObjectType type;
    type = o->getType();
    device->write((const char *)&type, sizeof(type));
    o->encode(device);
}

BFObject *BFFactory::decodeNewObject(QIODevice *device)
{
    BFObjectType type;
    BFObject *o;
    device->read((char *)&type, sizeof(type));
    switch (type)
    {
    case BFO_CIRCLE:
        o = new BFOCircle;
        break;
    }
    o->decode(device);
    return o;
}
