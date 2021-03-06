#include "SearchGraphNode.h"


/**
* Create a new SearchGraphNode
*
* @param coordinate The Vec2 coordinate of the SearchGraphNode
*/
SearchGraphNode::SearchGraphNode(Vec2 coordinate)
{
	this->coordinate = coordinate;
}


/**
* Set the parent of this SearchGraphNode
*
* @param parent The parent SearchGraphNode which lead us to this SearchGraphNode
* @return The depth we have no reached in searching
*/
int SearchGraphNode::setParent(ISearchGraphNode* parent)
{
	this->depth = parent->depth + 1;
	this->parent = parent;

	return depth;
}


/**
* Compare the heuristic cost between two SearchGraphNodes
*
* @param searchGraphNode The SearchGraphNode which is being compared
* @return The -1 if cost is less else 1 if costs is greater else cost is the same 0
*/
int SearchGraphNode::compareTo(ISearchGraphNode* searchGraphNode)
{
	ISearchGraphNode* o = searchGraphNode;

	float f = this->heuristic + this->cost;
	float of = o->heuristic + o->cost;

	if (f < of)
	{
		return -1;
	}
	else if (f > of)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}