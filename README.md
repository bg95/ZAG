ZAG
===

ZAG's A Game。

To-do list:
1. Use quadtrees to reduce the running time of intersection testing and collision processing
	Intersection testing: Query each object from the root of the quadtree, only consider the tree nodes that intersect with the object
	Collision processing: Traverse from the root with the information of all intersections. For the four subregions one node's children divides the region into, determine the first several collisions that does not involve more than one subregion. Dispatch these collisions to the children according to the subregion where a collision occurs. At some node, if there is only one collision, process it.
	After this process, redo intersection testing and collision processing over the time from the last processed collision to the end of the frame.
