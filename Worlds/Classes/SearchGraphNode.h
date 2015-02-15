#ifndef __com_dotdat_World__SEARCH_GRAPH_NODE_H__
#define __com_dotdat_World__SEARCH_GRAPH_NODE_H__

#include "cocos2d.h"

using namespace cocos2d;


/**
* A single node in the search graph
*/
class SearchGraphNode : public Ref
{
public:
	static SearchGraphNode* create(Vec2 coordinate);

	SearchGraphNode(Vec2 coordinate);
	virtual ~SearchGraphNode(){}

	int setParent(SearchGraphNode* parent);
	int compareTo(SearchGraphNode* searchGraphNode);

	/**
	* Variables
	*/
	Vec2 coordinate;	
	SearchGraphNode* parent;
	float heuristic;
	float cost;
	int depth;
};

#endif /* defined(__com_dotdat_World__SEARCH_GRAPH_NODE_H__) */
