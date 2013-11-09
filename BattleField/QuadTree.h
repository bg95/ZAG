#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include "Vector2d.h"

class BFObject;
class BFManager;
class IntersectionEvent;

class QuadTreeNode
{
public:
    QuadTreeNode(BFObject **_objs, int _objn, double l, double r, double b, double t);
    bool intersectWith(BFObject *obj);

    BFObject **objs;
    int objn;
    Vector2d bl, tr; //bottom-left and top-right corners
    double rad; //rough radius of the largest object
    QuadTreeNode *c[4]; //0-3: x-,y-; x+,y-; x-,y+; x+,y+
};

class QuadTree
{
public:
    QuadTree(BFManager *_manager);
    void clear();
    void insertObject(BFObject *obj);
    void build();
    void setOutput(std::vector<IntersectionEvent> &intersections);
    void queryAll();

private:
    BFObject **findkth(BFObject **a, int n, int k, int dim); //k starts from 0; dim=0 : x, dim=1 : y
    void buildRecursive(QuadTreeNode *r);
    void clearRecursive(QuadTreeNode *r);
    void queryRecursive(QuadTreeNode *r, BFObject *obj);

    QuadTreeNode *root;
    std::vector<BFObject *> objs;
    std::vector<IntersectionEvent> *intersect;
    BFManager *manager;

};

#include "BFObject/BFObject.h"
#include "BFManager.h"
#include "IntersectionEvent.h"

#endif // QUADTREE_H
