#include "Utils.h"


Vec2 Utils::getTileCoordForPosition(Vec2 position, Size mapSize, Size tileSize)
{
	float x = floor(position.x / tileSize.width);

	float levelHeightInPixels = mapSize.height * tileSize.height;

	float y = floor((levelHeightInPixels - position.y) / tileSize.height);

	return Vec2(x, y);
}

Rect Utils::getTileRectFromTileCoords(Vec2 tileCoords, Size mapSize, Size tileSize)
{	
	float levelHeightInPixels = mapSize.height * tileSize.height;

	Vec2 origin = Vec2(tileCoords.x * tileSize.width, levelHeightInPixels - ((tileCoords.y + 1) * tileSize.height));

	return Rect(origin.x, origin.y, tileSize.width, tileSize.height);
}

Rect Utils::getRectIntersection(Rect r1, Rect r2)
{
	Rect intersection;

	intersection = Rect(std::max(r1.getMinX(), r2.getMinX()), std::max(r1.getMinY(), r2.getMinY()), 0, 0);
	intersection.size.width = std::min(r1.getMaxX(), r2.getMaxX()) - intersection.getMinX();
	intersection.size.height = std::min(r1.getMaxY(), r2.getMaxY()) - intersection.getMinY();

	return intersection;
}

bool Utils::isRectIntersectsRect(Rect r1, Rect r2)
{
	return !
	(
		r1.getMaxX() < r2.getMinX() ||
		r2.getMaxX() < r1.getMinX() ||
		r1.getMaxY() < r2.getMinY() ||
		r2.getMaxY() < r1.getMinY()
	);
}

bool Utils::isRectContainsRect(Rect r1, Rect r2)
{
	return true;
}

int Utils::getTilesetMaxGID(TMXLayer& layer)
{
	TMXTilesetInfo* t = layer.getTileSet();		
	return (t->_imageSize.width / t->_tileSize.width) * (t->_imageSize.height / t->_tileSize.height);
}


bool Utils::isPixelCollision(Sprite* s1, Sprite* s2, Rect intersection, bool pixelCollision)
{
	bool isColliding = false;

	// If not checking for pixel perfect collisions, return true
	if (pixelCollision == false)
		return true;

	unsigned int x = intersection.origin.x;
	unsigned int y = intersection.origin.y;
	unsigned int w = intersection.size.width;
	unsigned int h = intersection.size.height;

	unsigned int numPixels = w * h;

	// If no intersection, return false
	if (numPixels <= 0) 
		return false;

	// Draw into the RenderTexture
	Size size = Director::getInstance()->getWinSize();
	RenderTexture *rt = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
	rt->beginWithClear(0, 0, 0, 0);

	// Render both sprites: first one in RED and second one in GREEN
	glColorMask(1, 0, 0, 1);
	s1->visit();
	glColorMask(0, 1, 0, 1);
	s2->visit();
	glColorMask(1, 1, 1, 1);

	// Get color values of intersection area
	Color4B* buffer = static_cast<Color4B*>(malloc(sizeof(Color4B) * numPixels));
	glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	rt->end();

	// Read buffer
	unsigned int step = 1;

	for (unsigned int i = 0; i < numPixels; i += step) 
	{
		Color4B color = buffer[i];
		
		log("Pixel color: %d, %d, %d", color.r, color.g, color.b);

		if (color.r > 0 && color.g > 0) 
		{
			isColliding = true;

			log("Colliding");
			
			break;
		}
	}

	// Free buffer memory
	free(buffer);

	return isColliding;
}