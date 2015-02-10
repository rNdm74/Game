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
	//log(" Started loading assets: %s", timestamp());
	textureCache->addImageAsync(HUD_PNG, CC_CALLBACK_1(AppResources::loadHudResources, this));

	textureCache->addImageAsync(BACKGROUND_PNG, CC_CALLBACK_1(AppResources::loadBackgroundResources, this));
	
	textureCache->addImageAsync(CHARACTERS_PNG, CC_CALLBACK_1(AppResources::loadCharacterResources, this));				
			    
    return true;
}

bool AppResources::mainLoad()
{
	//textureCache->addImageAsync(CONTROLS_PNG, CC_CALLBACK_1(AppResources::loadControlResources, this));

	textureCache->addImageAsync(ITEMS_PNG, CC_CALLBACK_1(AppResources::loadItemResources, this));

	textureCache->addImageAsync(PARTICLES_PNG, CC_CALLBACK_1(AppResources::loadParticleResources, this));

	//textureCache->addImageAsync(TILES_PNG, CC_CALLBACK_1(AppResources::loadTileResources, this));

	return true;
}

void AppResources::loadBackgroundResources(Texture2D* texture)
{
   // CCASSERT(texture == NULL, "No texture found, check if file exists");
	cache->addSpriteFramesWithFile(BACKGROUND_PLIST, texture);
}

void AppResources::loadCharacterResources(Texture2D* texture)
{
	//CCASSERT(texture == nullptr, "No texture found, check if file exists");

	cache->addSpriteFramesWithFile(CHARACTERS_PLIST, texture);
}

void AppResources::loadControlResources(Texture2D* texture)
{
	//CCASSERT(texture == nullptr, "No texture found, check if file exists");
    cache->addSpriteFramesWithFile(CONTROLS_PLIST, texture);
}

void AppResources::loadHudResources(Texture2D* texture)
{
	//CCASSERT(texture == nullptr, "No texture found, check if file exists");
	cache->addSpriteFramesWithFile(HUD_PLIST, texture);
}

void AppResources::loadItemResources(Texture2D* texture)
{
	//CCASSERT(texture == nullptr, "No texture found, check if file exists");
	cache->addSpriteFramesWithFile(ITEMS_PLIST, texture);
}

void AppResources::loadParticleResources(Texture2D* texture)
{
	//CCASSERT(texture == nullptr, "No texture found, check if file exists");
	cache->addSpriteFramesWithFile(PARTICLES_PLIST, texture);
}

void AppResources::loadTileResources(Texture2D* texture)
{
	//CCASSERT(texture == nullptr, "No texture found, check if file exists");
	cache->addSpriteFramesWithFile(TILES_PLIST, texture);
}