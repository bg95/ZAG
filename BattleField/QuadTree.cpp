#include "QuadTree.h"

QuadTreeNode::QuadTreeNode(BFObject **_objs, int _objn, double l, double r, double b, double t) :
    objs(_objs), objn(_objn), bl(l, b), tr(r, t)
{
    int i;
    c[0] = 0;
    c[1] = 0;
    c[2] = 0;
    c[3] = 0;
    rad = 0.0;
    for (i = 0; i < objn; i++)
        rad = std::max(rad, objs[i]->getRoughRadius());
}

bool QuadTreeNode::intersectWith(BFObject *obj)
{
    double r = obj->getRoughRadius() + rad;
    Vector2d pos = obj->getPosition();
    //return obj->getPosition() >= bl - Vector2d(r, r) && obj->getPosition() < tr + Vector2d(r, r);
    return pos.x >= bl.x - r && pos.y >= bl.y - r && pos.x < tr.x + r && pos.y < tr.y + r;
}

QuadTree::QuadTree(BFManager *_manager)
{
    root = 0;
    manager = _manager;
}

void QuadTree::clear()
{
    clearRecursive(root);
    objs.clear();
}

void QuadTree::insertObject(BFObject *obj)
{
    objs.push_back(obj);
}

void QuadTree::build() //exists bug
{
    static const double margin = 0.01;
    double l, r, b, t;
    //double rad; //rad unused
    Vector2d v;
    std::vector<BFObject *>::iterator iter;
    BFObject **a;
    int n;
    l = objs[0]->getPosition().x;
    r = l + margin;
    b = objs[0]->getPosition().y;
    t = b + margin;
    //rad = 0.0;
    for (iter = objs.begin(); iter != objs.end(); iter++)
    {
        v = (*iter)->getPosition();
        l = std::min(l, v.x);
        r = std::max(r, v.x);
        b = std::min(b, v.y);
        t = std::max(t, v.y);
        //rad = std::max(rad, (*iter)->getRoughRadius());
    }
    a = objs.data();
    n = objs.size();
    root = new QuadTreeNode(a, n, l, r + margin * (r - l), b, t + margin * (t - b));
    buildRecursive(root);
}

void QuadTree::setOutput(std::vector<IntersectionEvent> &intersections)
{
    intersect = &intersections;
}

void QuadTree::queryAll()
{
    std::vector<BFObject *>::iterator iter;
    for (iter = objs.begin(); iter != objs.end(); iter++)
        queryRecursive(root, *iter);
}

BFObject **QuadTree::findkth(BFObject **a, int n, int k, int dim) //exists bug
{
    //qDebug("findkth %lX, %d, %d, %d", (long)a, n, k, dim);
    Vector2d v;
    int i, j;
    while (1)
    {
        //qDebug("findkth %lX, %d, %d, %d", (long)a, n, k, dim);
        if (n == 1)
        {
            //qDebug("n=1");
            return a;
            //qDebug("n=1");
        }
        v = a[rand() % n]->getPosition();
        i = 0, j = n - 1;
        while (i <= j)
        {
            while (a[i]->getPosition().c[dim] < v.c[dim])
                i++;
            while (a[j]->getPosition().c[dim] > v.c[dim])
                j--;
            if (i <= j)
            {
                std::swap(a[i], a[j]);
                i++;
                j--;
            }
        }
        i = (i + j) / 2;
        if (k == i)
            return a + i;
        if (k > i)
        {
            a += i + 1;
            n -= i + 1;
            k -= i + 1;
        }
        else
        {
            n = i;
        }
    }
}

void QuadTree::buildRecursive(QuadTreeNode *r)
{
    if (r->objn <= 1)
        return;
    BFObject **midx, **midyl, **midyr;
    BFObject **&a = r->objs;
    int &n = r->objn;
    //qDebug("findkth %lX, %d, %d, %d", (long)a, n, n / 2, 0);
    //qDebug("Node %lX, %d", (long)a, n);
    midx = findkth(a, n, n / 2, 0);
    midyl = findkth(a, midx - a, (midx - a) / 2, 1);
    midyr = findkth(midx, a + n - midx, (a + n - midx) / 2, 1);
    r->c[0] = new QuadTreeNode(a, midyl - a, r->bl.x, (*midx)->getPosition().x, r->bl.y, (*midyl)->getPosition().y);
    r->c[2] = new QuadTreeNode(midyl, midx - midyl, r->bl.x, (*midx)->getPosition().x, (*midyl)->getPosition().y, r->tr.y);
    r->c[1] = new QuadTreeNode(midx, midyr - midx, (*midx)->getPosition().x, r->tr.x, r->bl.y, (*midyr)->getPosition().y);
    r->c[3] = new QuadTreeNode(midyr, a + n - midyr, (*midx)->getPosition().x, r->tr.x, (*midyr)->getPosition().y, r->tr.y);
    buildRecursive(r->c[0]);
    buildRecursive(r->c[1]);
    buildRecursive(r->c[2]);
    buildRecursive(r->c[3]);
}

void QuadTree::clearRecursive(QuadTreeNode *r)
{
    if (!r)
        return;
    clearRecursive(r->c[0]);
    clearRecursive(r->c[1]);
    clearRecursive(r->c[2]);
    clearRecursive(r->c[3]);
    delete r;
}

void QuadTree::queryRecursive(QuadTreeNode *r, BFObject *obj)
{
    if (r == 0 || r->objn == 0 || !r->intersectWith(obj))
        return;
    if (r->objn == 1)
    {
        if (r->objs[0] == obj)
            return;
        if (*(r->objs[0]) < (*obj))
            return;
        if (manager->intersecting(r->objs[0], obj))
            intersect->push_back(IntersectionEvent(r->objs[0], obj, manager->intersectingTime(r->objs[0], obj)));
        return;
    }
    queryRecursive(r->c[0], obj);
    queryRecursive(r->c[1], obj);
    queryRecursive(r->c[2], obj);
    queryRecursive(r->c[3], obj);
}
