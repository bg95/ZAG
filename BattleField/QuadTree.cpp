#include "QuadTree.h"

#include "main.h"

#include "BFObject/BFOCircle.h"

QuadTreeNode::QuadTreeNode(BFObject **_objs, int _objn, double l, double r, double b, double t) :
    objs(_objs), objn(_objn), bl(l, b), tr(r, t)
{
    c[0] = 0;
    c[1] = 0;
    c[2] = 0;
    c[3] = 0;
    rad = 0.0;/*
    int i;
    for (i = 0; i < objn; i++)
        rad = std::max(rad, objs[i]->getRoughRadius());*/
}

bool QuadTreeNode::intersectWith(BFObject *obj)
{
    double r = obj->getRoughRadius() + rad;
    Vector2d pos = obj->getPosition();
    //return obj->getPosition() >= bl - Vector2d(r, r) && obj->getPosition() < tr + Vector2d(r, r);
    //return pos.x >= bl.x - r && pos.y >= bl.y - r && pos.x < tr.x + r && pos.y < tr.y + r;
    return pos.x >= bl.x - r && pos.y >= bl.y - r && pos.x < tr.x + r && pos.y < tr.y + r;
    //return 1;
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
    //qDebug("insert (%lf,%lf)", obj->getPosition().x, obj->getPosition().y);
    objs.push_back(obj);
}

int depth;
void QuadTree::build()
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
    //qDebug("(%lf,%lf) (%lf,%lf)", l, b, r, t);
    for (iter = objs.begin(); iter != objs.end(); iter++)
    {
        v = (*iter)->getPosition();
        l = std::min(l, v.x);
        r = std::max(r, v.x);
        b = std::min(b, v.y);
        t = std::max(t, v.y);
        //rad = std::max(rad, (*iter)->getRoughRadius());
        //qDebug("(%lf,%lf)", v.x, v.y);
    }
    //qDebug("(%lf,%lf) (%lf,%lf)", l, b, r, t);
    /*
    l = manager->getLeft();
    r = manager->getRight();
    b = manager->getBottom();
    t = manager->getTop();*/
    a = objs.data();
    n = objs.size();
    root = new QuadTreeNode(a, n, l, r + margin * (r - l), b, t + margin * (t - b));
    depth = 0;//for debugging
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

void QuadTree::query(BFObject *obj)
{
    queryRecursive(root, obj);
}

BFObject **QuadTree::findkth(BFObject **a, int n, int k, int dim)
{
    //qDebug("findkth %lX, %d, %d, %d", (long)a, n, k, dim);
    Vector2d v;
    int i, j, p;
    while (1)
    {
        //qDebug("findkth %lX, %d, %d, %d", (long)a, n, k, dim);
        if (n == 1)
        {
            //qDebug("n=1");
            return a;
        }
        p = rand() % n;
        std::swap(a[0], a[p]);
        v = a[0]->getPosition();
        i = 1, j = n - 1;
        while (i <= j)
        {
            while (i <= j && a[i]->getPosition().c[dim] < v.c[dim])
                i++;
            while (i <= j && a[j]->getPosition().c[dim] > v.c[dim])
                j--;
            if (i <= j)
            {
                std::swap(a[i], a[j]);
                i++;
                j--;
            }
        }
        i = (i + j) / 2;
        std::swap(a[0], a[i]);
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
    depth++;
    //qDebug("%*cNode %lX, %d, range (%lf,%lf) (%lf,%lf) rad=%lf", depth, ' ', (long)r->objs, r->objn, r->bl.x, r->bl.y, r->tr.x, r->tr.y, r->rad);
    //qDebug("%*cNode %lX, objs=%lX, %d", depth, ' ', (long)r, (long)r->objs, r->objn);
    if (r->objn == 0)
    {
        depth--;
        return;
    }/*
    if (r->objn == 1) //for debugging
    {
        r->rad = r->objs[0]->getRoughRadius();
        mw->bf->makeCurrent();
        glColor3d(0.0, 1.0, 0.0);
        glBegin(GL_LINE_LOOP);
        glVertex2d(r->bl.x - r->rad, r->bl.y - r->rad);
        glVertex2d(r->bl.x - r->rad, r->tr.y + r->rad);
        glVertex2d(r->tr.x + r->rad, r->tr.y + r->rad);
        glVertex2d(r->tr.x + r->rad, r->bl.y - r->rad);
        glEnd();
    }
*/
    if (r->objn == 1)
    {
        r->rad = r->objs[0]->getRoughRadius();
        r->bl = r->objs[0]->getPosition();
        r->tr = r->bl;
        BFObject **&a = r->objs;
        int &n = r->objn;/*
        for (int i = 0; i < n; i++)
        {
            qDebug("%*cObject %lX (%lf,%lf), %lf", depth + 4, ' ', (unsigned long long)a[i], a[i]->getPosition().x,  a[i]->getPosition().y, a[i]->getRoughRadius());
        }
        qDebug("%*cNode %lX, objs=%lX, %d, range (%lf,%lf) (%lf,%lf)", depth, ' ', (long)r, (long)r->objs, r->objn, r->bl.x, r->bl.y, r->tr.x, r->tr.y);
*/
        depth--;
        return;
    }
    BFObject **midx, **midyl, **midyr;
    BFObject **&a = r->objs;
    double midxx;
    int &n = r->objn;
    midx = findkth(a, n, n / 2, 0);
    midxx = (*midx)->getPosition().x;
    //qDebug("findkth %lX, %d, %d, %d -> %d,%lf", (long)a, n, n / 2, 0, (midx - n), (*midx)->getPosition().x);
    midyl = findkth(a, midx - a, (midx - a) / 2, 1);
    midyr = findkth(midx, a + n - midx, (a + n - midx) / 2, 1);
    int i;/*
    for (i = 0; i < n; i++)
    {
        qDebug("%*cObject %lX (%lf,%lf), %lf", depth + 4, ' ', (unsigned long long)a[i], a[i]->getPosition().x,  a[i]->getPosition().y, a[i]->getRoughRadius());
    }
    i = midx - a;
    qDebug("%*cObject midx  %lX (%lf,%lf), %lf", depth + 4, ' ', (unsigned long long)a[i], a[i]->getPosition().x,  a[i]->getPosition().y, a[i]->getRoughRadius());
    i = midyl - a;
    qDebug("%*cObject midyl %lX (%lf,%lf), %lf", depth + 4, ' ', (unsigned long long)a[i], a[i]->getPosition().x,  a[i]->getPosition().y, a[i]->getRoughRadius());
    i = midyr - a;
    qDebug("%*cObject midyr %lX (%lf,%lf), %lf", depth + 4, ' ', (unsigned long long)a[i], a[i]->getPosition().x,  a[i]->getPosition().y, a[i]->getRoughRadius());
*/
    r->c[0] = new QuadTreeNode(a, midyl - a, r->bl.x, midxx, r->bl.y, (*midyl)->getPosition().y);
    r->c[1] = new QuadTreeNode(midyl, midx - midyl, r->bl.x, midxx, (*midyl)->getPosition().y, r->tr.y);
    r->c[2] = new QuadTreeNode(midx, midyr - midx, midxx, r->tr.x, r->bl.y, (*midyr)->getPosition().y);
    r->c[3] = new QuadTreeNode(midyr, a + n - midyr, midxx, r->tr.x, (*midyr)->getPosition().y, r->tr.y);
    buildRecursive(r->c[0]);
    buildRecursive(r->c[1]);
    buildRecursive(r->c[2]);
    buildRecursive(r->c[3]);
    r->rad = std::max(std::max(std::max(r->c[0]->rad, r->c[1]->rad), r->c[2]->rad), r->c[3]->rad);
    r->bl.x = std::min(r->c[0]->bl.x, r->c[1]->bl.x);
    r->bl.y = std::min(r->c[0]->bl.y, r->c[2]->bl.y);
    r->tr.x = std::max(r->c[2]->tr.x, r->c[3]->tr.x);
    r->tr.y = std::max(r->c[1]->tr.y, r->c[3]->tr.y);

    //qDebug("%*cNode %lX, objs=%lX, %d, range (%lf,%lf) (%lf,%lf)", depth, ' ', (long)r, (long)r->objs, r->objn, r->bl.x, r->bl.y, r->tr.x, r->tr.y);
    /*
    {//for debugging
        r->rad = r->objs[0]->getRoughRadius();
        mw->bf->makeCurrent();
        glColor3d(0.0, 1.0, 0.0);
        glBegin(GL_LINE_LOOP);
        glVertex2d(r->bl.x - r->rad, r->bl.y - r->rad);
        glVertex2d(r->bl.x - r->rad, r->tr.y + r->rad);
        glVertex2d(r->tr.x + r->rad, r->tr.y + r->rad);
        glVertex2d(r->tr.x + r->rad, r->bl.y - r->rad);
        glEnd();
    }*/
    depth--;
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
    //qDebug("Query object %lX (%lf,%lf) on node %lX", (long)obj, obj->getPosition().x, obj->getPosition().y, (long)r);
    //qDebug("on node %lX", (long)r);
    if (r == 0 || r->objn == 0 || !r->intersectWith(obj))
    {
        //qDebug("ommiting node %lX", (long)r);
        return;
    }
    if (r->objn == 1)
    {
        if (r->objs[0] <= obj)
            return;
        //if (*(r->objs[0]) < (*obj))
        //    return;
        if (manager->intersecting(r->objs[0], obj))
            intersect->push_back(IntersectionEvent(r->objs[0], obj, manager->intersectingTime(r->objs[0], obj)));
        return;
    }
    if (r->c[0])
    {
        queryRecursive(r->c[0], obj);
        queryRecursive(r->c[1], obj);
        queryRecursive(r->c[2], obj);
        queryRecursive(r->c[3], obj);
    }
}
