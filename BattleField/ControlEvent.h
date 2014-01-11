#ifndef CONTROLEVENT_H
#define CONTROLEVENT_H

#include <string>
#include <vector>
#include <QVariant>

#include "BFObject/BFObject.h"

class ControlEvent
{
public:
    ControlEvent();
    ControlEvent(QIODevice *device);
    void encode(QIODevice *device);
    void decode(QIODevice *device);

    BFObjectID objid;
    Vector2d acc;
    std::vector<std::pair<std::string, QVariant>> difference;

};

void encodeControlEventList(std::vector<ControlEvent> &list, QIODevice *device);
void decodeAppendControlEventList(std::vector<ControlEvent> &list, QIODevice *device);

#endif // CONTROLEVENT_H
