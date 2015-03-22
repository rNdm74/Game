#include "AppResources.h"
#include "Constants.h"


AppResources* AppResources::m_pInstance = NULL;

AppResources* AppResources::getInstance()
{
	return m_pInstance ? m_pInstance : m_pInstance = new AppResources();
}

AppResources::AppResources()
{
    cache = SpriteFrameCache::getInstance();		
	textureCache = Director::getInstance()->getTextureCache();		
}

AppResources::~AppResources()
{
}

bool AppResources::initLoad()
{		
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tibbetts.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hero.plist");

	textureCache->addImageAsync(HUD_PNG, CC_CALLBACK_1(AppResources::loadHudResources, this));

	textureCache->addImageAsync(BACKGROUND_PNG, CC_CALLBACK_1(AppResources::loadBackgroundResources, this));
	
	textureCache->addImageAsync(CHARACTERS_PNG, CC_CALLBACK_1(AppResources::loadCharacterResources, this));				
			    
    return true;
}

bool AppResources::mainLoad()
{	
	textureCache->addImageAsync(ITEMS_PNG, CC_CALLBACK_1(AppResources::loadItemResources, this));

	textureCache->addImageAsync(PARTICLES_PNG, CC_CALLBACK_1(AppResources::loadParticleResources, this));	

	return true;
}

void AppResources::loadGrassResources(Texture2D* texture)
{	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(GRASS_PLIST, texture);
}

void AppResources::loadSnowResources(Texture2D* texture)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(SNOW_PLIST, texture);
}

/** PRIVATE **/

void AppResources::loadBackgroundResources(Texture2D* texture)
{
   	cache->addSpriteFramesWithFile(BACKGROUND_PLIST, texture);
}

void AppResources::loadCharacterResources(Texture2D* texture)
{
	cache->addSpriteFramesWithFile(CHARACTERS_PLIST, texture);
}

void AppResources::loadControlResources(Texture2D* texture)
{
    cache->addSpriteFramesWithFile(CONTROLS_PLIST, texture);
}

void AppResources::loadHudResources(Texture2D* texture)
{
	cache->addSpriteFramesWithFile(HUD_PLIST, texture);
}

void AppResources::loadItemResources(Texture2D* texture)
{
	cache->addSpriteFramesWithFile(ITEMS_PLIST, texture);
}

void AppResources::loadParticleResources(Texture2D* texture)
{
	cache->addSpriteFramesWithFile(PARTICLES_PLIST, texture);
}

void AppResources::loadTileResources(Texture2D* texture)
{
}