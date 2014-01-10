#ifndef BFOBJECT_H
#define BFOBJECT_H

#include <QGLWidget>
#include <QIODevice>
#include <QVariant>
#include <string>
#include <typeinfo>
#include <map>
#include "../Vector2d.h"
//#include "BFManager.h"

const double PI = 3.14159265358979323846264338327950288419716939937510;

class BFManager;
class BFController;

typedef size_t BFObjectType;
#define typehash(TYPE) (typeid(TYPE).hash_code())
/*
enum BFObjectType
{
    BFO_CIRCLE = 2, BFO_COLORED_CIRCLE = 3
};*/
enum BFObjectShape
{
    BFO_CIRCULAR = 0
};

class BFObject
{
public:
    static const std::string empty_string;
    static long count;
    static void readStdString(QIODevice *device, std::string &str); //read a string from device
    static void writeStdString(QIODevice *device, const std::string &str); //write a string to device
    static void readQVariant(QIODevice *device, QVariant &var); //read a QVariant
    static void writeQVariant(QIODevice *device, const QVariant &var); //write a QVariant
    static void readVector2d(QIODevice *device, Vector2d &vec); //read a Vector2d
    static void writeVector2d(QIODevice *device, const Vector2d &vec); //write a Vector2d

    BFObject();
    //BFObject(BFManager *_manager = 0);
    virtual ~BFObject();
    virtual BFObject *newObject() = 0;
    virtual BFObject *duplicate();

    virtual void draw(QGLWidget *) = 0;
    virtual BFObjectType getType() const = 0;
    virtual BFObjectShape getShape() const = 0;
    virtual double getRoughRadius() const = 0; //The radius of a circle centered at the object's center, large enough to cover the whole object, used for Quadtree
    virtual Vector2d getPosition() const = 0;
    virtual Vector2d getVelocity() const = 0;

    virtual void move(double time) = 0;

    //encode, decode not tested
    virtual void encode(QIODevice *device); //must be called if overwritten by subclasses
    virtual void decode(QIODevice *device); //must be called if overwritten by subclasses

    friend class BFController;
    BFController *getController();

    long getID();
    bool operator <(BFObject &b)
    {
        return id < b.id;
    }

    void setProperty(const std::string &prop, const QVariant &val);
    const QVariant &getProperty(const std::string &prop);
    //void setProperty(const std::string &prop, const std::string &val);
    QVariant &operator[](const std::string &prop);

private:
    long id;
    //BFManager *manager;
    std::map<std::string, QVariant> properties;
    BFController *controller;

    friend class BFFactory;
    bool created_from_factory;
    bool deleting_from_factory;

};

#include "../BFManager.h"
#include "../BFController/BFController.h"

#endif // BFOBJECT_H
