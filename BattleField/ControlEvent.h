#ifndef CONTROLEVENT_H
#define CONTROLEVENT_H

#include <string>
#include <vector>
#include <QVariant>

#include "BFObject/BFObject.h"

class ControlEvent
{
public:
    ControlEvent(BFObjectID _objid);
    ControlEvent(QIODevice *device);
    void encode(QIODevice *device);
    void decode(QIODevice *device);

    void addPropertyChange(const std::string &prop, const QVariant &val);

    BFObjectID objid;
    Vector2d acc;
    std::vector<std::pair<std::string, QVariant>> difference;

};

void encodeControlEventList(std::vector<ControlEvent> &list, QIODevice *device);
void decodeAppendControlEventList(std::vector<ControlEvent> &list, QIODevice *device);

#endif // CONTROLEVENT_H
