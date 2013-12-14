#include "BFObject/BFOCircle.h"
#include "BFController/BFController.h"

#include "BFCodec.h"

BFCodec::BFCodec()
{
}

void BFCodec::encodeObject(BFObject *o, QIODevice *device)
{
    BFObjectType type;
    type = o->getType();
    device->write((const char *)&type, sizeof(type));
    o->decode(device);
}

BFObject *BFCodec::decodeNewObject(QIODevice *device)
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
