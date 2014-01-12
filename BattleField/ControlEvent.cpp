#include "ControlEvent.h"

ControlEvent::ControlEvent(BFObjectID _objid) :
    objid(_objid), acc(0, 0)
{
}

ControlEvent::ControlEvent(QIODevice *device)
{
    decode(device);
}

ControlEvent::ControlEvent(const ControlEvent &another)
{
    objid = another.objid;
    acc = another.acc;
    difference = another.difference;
}

void ControlEvent::encode(QIODevice *device)
{
    device->write((const char *)&objid, sizeof(objid));
    BFObject::writeVector2d(device, acc);
    auto size = difference.size();
    device->write((const char *)&size, sizeof(size));
    for (auto iter = difference.begin(); iter != difference.end(); iter++)
    {
        BFObject::writeStdString(device, (*iter).first);
        BFObject::writeQVariant(device, (*iter).second);
    }
}

void ControlEvent::decode(QIODevice *device)
{
    device->read((char *)&objid, sizeof(objid));
    BFObject::readVector2d(device, acc);
    difference.clear();
    auto size = difference.size();
    device->read((char *)&size, sizeof(size));
    std::pair<std::string, QVariant> pair;
    for (size_t i = 0; i < size; i++)
    {
        BFObject::readStdString(device, pair.first);
        BFObject::readQVariant(device, pair.second);
        difference.push_back(pair);
    }
}

void ControlEvent::addPropertyChange(const std::string &prop, const QVariant &val)
{
    difference.push_back(std::pair<std::string, QVariant>(prop, val));
}

void ControlEvent::encodeControlEventList(std::vector<ControlEvent> &list, QIODevice *device)
{
    auto size = list.size();
    device->write((const char *)&size, sizeof(size));
    for (auto iter = list.begin(); iter != list.end(); iter++)
        (*iter).encode(device);
}

void ControlEvent::decodeAppendControlEventList(std::vector<ControlEvent> &list, QIODevice *device)
{
    size_t i;
    std::vector<ControlEvent>::size_type size;
    device->read((char *)&size, sizeof(size));
    //qDebug("Decode control size = %d", size);
    for (i = 0; i < size; i++)
    {
        list.push_back(ControlEvent(device));
    }
}
