#include "Utils.h"


Vec2 Utils::tileCoordForPosition(Vec2 position, Size mapSize, Size tileSize)
{
	float x = floor(position.x / tileSize.width);

	float levelHeightInPixels = mapSize.height * tileSize.height;

	float y = floor((levelHeightInPixels - position.y) / tileSize.height);

	return Vec2(x, y);
}

Rect Utils::tileRectFromTileCoords(Vec2 tileCoords, Size mapSize, Size tileSize)
{	
	float levelHeightInPixels = mapSize.height * tileSize.height;

	Vec2 origin = Vec2(tileCoords.x * tileSize.width, levelHeightInPixels - ((tileCoords.y + 1) * tileSize.height));

	return Rect(origin.x, origin.y, tileSize.width, tileSize.height);
}

Rect Utils::RectIntersection(Rect r1, Rect r2)
{
	Rect intersection;

	intersection = Rect(std::max(r1.getMinX(), r2.getMinX()), std::max(r1.getMinY(), r2.getMinY()), 0, 0);
	intersection.size.width = std::min(r1.getMaxX(), r2.getMaxX()) - intersection.getMinX();
	intersection.size.height = std::min(r1.getMaxY(), r2.getMaxY()) - intersection.getMinY();

	return intersection;
}

bool Utils::RectIntersectsRect(Rect r1, Rect r2)
{
	return !
	(
		r1.getMaxX() < r2.getMinX() ||
		r2.getMaxX() < r1.getMinX() ||
		r1.getMaxY() < r2.getMinY() ||
		r2.getMaxY() < r1.getMinY()
	);
}

TileDataArray Utils::getSurroundingTilesAtPosition(Vec2 position, TMXLayer& layer, Size mapSize, Size tileSize)
{
	Vec2 gameObjectPosition = tileCoordForPosition(position, mapSize, tileSize);

	TileDataArray gids;

	int count = 0;

	for (int i = 0; i < 9; i++)
	{
		int column = i % 3;
		int row = static_cast<int>(i / 3);

		// 0,0 | 0,1 | 0,2
		// 1,0 | 1,1 | 1,2
		// 2,0 | 2,1 | 2,2
		if (column == 1 && row == 1)
			continue;

		Vec2 tileCoordinates = Vec2
		(
			gameObjectPosition.x + (column - 1),
			gameObjectPosition.y + (row - 1)
		);

		int tileGid = 0;

		// if its a valid tilepos for layer
		if (tileCoordinates.x >= 0 && tileCoordinates.x < mapSize.width &&
			tileCoordinates.y >= 0 && tileCoordinates.y < mapSize.height)
		{
			tileGid = layer.getTileGIDAt(tileCoordinates);
		}

		if (tileGid)
		{
			Rect tileRect = tileRectFromTileCoords(tileCoordinates, mapSize, tileSize);

			gids[count].gid = tileGid;
			gids[count].tile = tileRect;
			gids[count].coordinates = tileCoordinates;
		}

		count++;
	}

	// top left and bottom
	std::swap(gids[0], gids[6]); // bottom now in position 0 
	std::swap(gids[2], gids[3]); // left now in position 2
	std::swap(gids[3], gids[4]); // right now in position 3

	std::swap(gids[4], gids[6]); // top left now in position 4
	std::swap(gids[5], gids[6]); // top right now in position 5

	/*
	* OLD | NEW
	* --- + ---
	* 012 | 415
	* 3 4 | 2 3
	* 567 | 607
	*/

	return gids;
}