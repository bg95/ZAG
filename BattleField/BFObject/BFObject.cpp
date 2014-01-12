#include "BFObject.h"

const std::string BFObject::empty_string("");
long BFObject::count = 0;
const QVariant BFObject::QVariant_Invalid = QVariant(QVariant::Invalid);

BFObject::BFObject()//BFManager *_manager)
    :created_from_factory(false), deleting_from_factory(false)
{
    id = count;
    count++;
    //controller = 0;
}

BFObject::~BFObject()
{
    //if (created_from_factory && !deleting_from_factory)
    //    qCritical("Deleting Object %lX (id=%ld) outside BFFactory!", this, id);
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
    std::string prop;
    device->write((const char *)&size, sizeof(size));
    for (iter = properties.begin(); iter != properties.end(); iter++)
    {
        QVariant var = (*iter).second;
        //var.convert(QVariant::String);
        writeStdString(device, (*iter).first);
        //writeStdString(device, (*iter).second);
        //writeStdString(device, var.toString().toStdString());
        writeQVariant(device, var);
    }
}

void BFObject::decode(QIODevice *device)
{
    int i;
    int size;
    std::string prop;
    QVariant val;
    properties.clear();
    device->read((char *)&size, sizeof(size));
    for (i = 0; i < size; i++)
    {
        readStdString(device, prop);
        readQVariant(device, val);
        //qDebug("read property %s:%s", prop.c_str(), val.c_str());
        setProperty(prop, val);
    }
}
/*
BFController *BFObject::getController()
{
    return controller;
}
*/
BFObjectID BFObject::getID()
{
    return id;
}

void BFObject::setProperty(const std::string &prop, const QVariant &val)
{
    properties[prop] = val;
}

const QVariant &BFObject::getProperty(const std::string &prop)
{
    //std::map<std::string, QVariant>::iterator iter;
    auto iter = properties.find(prop);
    if (iter == properties.end())
        return QVariant_Invalid;
    //qDebug("property %s found: %s", prop.c_str(), (*iter).second.c_str());
    return (*iter).second;
}

QVariant &BFObject::operator [](const std::string &prop)
{
    /*
    auto iter = properties.find(prop);
    if (iter == properties.end())
        return properties[prop];//return QVariant(QVariant::Invalid); //don't know what to do
    return (*iter).second;
    */
    return properties[prop];
}

/*
void BFObject::setProperty(const std::string &prop, const std::string &val)
{
    QVariant var(QString(val.c_str()));
    setProperty(prop, var);
}
*/
//static

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

void BFObject::readQVariant(QIODevice *device, QVariant &var)
{
    QVariant::Type type;
    int size;
    QByteArray ba;
    device->read((char *)&type, sizeof(type));
    device->read((char *)&size, sizeof(size));
    ba.resize(size);
    device->read(ba.data(), size);
    var.setValue(ba);
    bool ok = var.convert(type);
    //qDebug("read  from type %s to %s, %d", QVariant::typeToName(QVariant::ByteArray), QVariant::typeToName(type), ok);
}

void BFObject::writeQVariant(QIODevice *device, const QVariant &var)
{
    QVariant::Type type = var.type();
    QVariant bavar(var);
    bool ok = bavar.convert(QVariant::ByteArray);
    QByteArray ba = bavar.toByteArray();
    int size = ba.size();
    //qDebug("write from type %s to %s, %d", QVariant::typeToName(type), QVariant::typeToName(QVariant::ByteArray), ok);
    device->write((const char *)&type, sizeof(type));
    device->write((const char *)&size, sizeof(size));
    device->write(ba.data(), ba.size());
}

void BFObject::readVector2d(QIODevice *device, Vector2d &vec)
{
    device->read((char *)vec.c, sizeof(vec.c));
}

void BFObject::writeVector2d(QIODevice *device, const Vector2d &vec)
{
    device->write((const char *)vec.c, sizeof(vec.c));
}

void BFObject::applyControlEvent(ControlEvent &ce)
{
    setAcceleration(ce.acc);
    for (auto iter = ce.difference.begin(); iter != ce.difference.end(); iter++)
        setProperty((*iter).first, (*iter).second);
}
