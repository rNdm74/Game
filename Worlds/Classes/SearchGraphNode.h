#ifndef __com_dotdat_World__SEARCH_GRAPH_NODE_H__
#define __com_dotdat_World__SEARCH_GRAPH_NODE_H__

#include "cocos2d.h"

using namespace cocos2d;

class ISearchGraphNode
{
public:
	ISearchGraphNode(){};
	virtual ~ISearchGraphNode(){};

	virtual int setParent(ISearchGraphNode* parent) = 0;
	virtual int compareTo(ISearchGraphNode* searchGraphNode) = 0;

	/**
	* Variables
	*/
	ISearchGraphNode* parent;
	Vec2 coordinate;
	float heuristic;
	float cost;
	int depth;
};


/**
* A single node in the search graph
*/
class SearchGraphNode : public ISearchGraphNode
{
public:    
	SearchGraphNode(Vec2 coordinate);
	virtual ~SearchGraphNode(){};

	virtual int setParent(ISearchGraphNode* parent) override;
	virtual int compareTo(ISearchGraphNode* searchGraphNode) override;
};

#endif /* defined(__com_dotdat_World__SEARCH_GRAPH_NODE_H__) */
