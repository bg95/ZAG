#include "BFObject.h"

const std::string BFObject::empty_string("");
long BFObject::count = 0;

BFObject::BFObject()//BFManager *_manager)
    :/*manager(_manager), */info()
{
    id = count;
    count++;
}

BFObject::~BFObject()
{
}

BFObject *BFObject::duplicate()
{
    BFObject *p = newObject();
    p->properties = properties;
    return p;
}

void BFObject::encode(QIODevice *device)
{
    std::map<std::string, QVariant>::iterator iter;
    int size = properties.size();
    std::string prop, val;
    device->write((const char *)&size, sizeof(size));
    for (iter = properties.begin(); iter != properties.end(); iter++)
    {
        QVariant var = (*iter).second;
        var.convert(QVariant::String);
        writeStdString(device, (*iter).first);
        //writeStdString(device, (*iter).second);
        writeStdString(device, var.toString().toStdString());
    }
}

void BFObject::decode(QIODevice *device)
{
    int i;
    int size;
    std::string prop, val;
    properties.clear();
    device->read((char *)&size, sizeof(size));
    for (i = 0; i < size; i++)
    {
        readStdString(device, prop);
        readStdString(device, val);
        setProperty(prop, QString(val.c_str()));
    }
}
/*
bool BFObject::intersectWith(BFObject *b)
{
    return manager->intersecting(this, b);
}
*/
void BFObject::setInfo(std::string str)
{
    info = str;
}

const std::string &BFObject::getInfo() const
{
    return info;
}

void BFObject::setProperty(const std::string &prop, const QVariant &val)
{
    properties[prop] = val;
}

const QVariant &BFObject::getProperty(const std::string &prop)
{
    std::map<std::string, QVariant>::iterator iter;
    iter = properties.find(prop);
    if (iter == properties.end())
        return QVariant(QVariant::Invalid);
    //qDebug("property %s found: %s", prop.c_str(), (*iter).second.c_str());
    return (*iter).second;
}

const QVariant &BFObject::operator [](const std::string &prop)
{
    return getProperty(prop);
}

/*
void BFObject::setProperty(const std::string &prop, const std::string &val)
{
    QVariant var(QString(val.c_str()));
    setProperty(prop, var);
}
*/
//private

void BFObject::readStdString(QIODevice *device, std::string &str)
{
    int strsize;
    device->read((char *)&strsize, sizeof(strsize));
    str.resize(strsize);
    str.resize(device->read(&str[0], strsize));
}

void BFObject::writeStdString(QIODevice *device, const std::string &str)
{
    int strsize;
    strsize = str.size();
    device->write((const char *)&strsize, sizeof(strsize));
    device->write(str.data(), strsize);
}
