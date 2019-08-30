#include "TileObject.h"
TileObject::TileObject()
{
    width = 0;
    height = 0;
    showTile = false;
    numOfNeighbors = 0;
    for (unsigned int i = 0; i < 8; ++i)
    {
        neighbors[i] = nullptr;
    }
}
void TileObject::initializeTile(const char *name, bool reveal)
{
    numOfNeighbors = 0;
    for (unsigned int i = 0; i < 8; ++i)
    {
        neighbors[i] = nullptr;
    }
    tileTexture.loadFromFile(name);
    tileSprite.setTexture(tileTexture);
    width = tileSprite.getGlobalBounds().width;
    height = tileSprite.getGlobalBounds().height;
    revealTile(reveal);
}
void TileObject::setPosition(float x, float y)
{
    tileSprite.setPosition(x, y);
}
void TileObject::revealTile(bool reveal)
{
    showTile = reveal;
}

bool& TileObject::isTileRevealed()
{
    return showTile;
}

void TileObject::addOneNeighbor(TileObject* tile)
{
    if (numOfNeighbors != 8)
    {
        neighbors[numOfNeighbors] = tile;
    }
    numOfNeighbors++;

}
///accessors////
const float& TileObject::getHeight() const
{
    return height;
}
const float& TileObject::getWidth() const
{
    return width;
}
const Sprite& TileObject::getSprite() const
{
    return tileSprite;
}
const Texture& TileObject::getTexture() const
{
    return tileTexture;
}
const Vector2f& TileObject::getPosition() const
{
    return getSprite().getPosition();
}
TileObject* TileObject::getNeighbors()
{
    return *neighbors;
}
const int& TileObject::getNumOfNeighbors() const
{
    return numOfNeighbors;
}

//MineObject//
MineObject::MineObject()
{
    isMineActivated = false;
}
bool& MineObject::isMineSet()
{
    return isMineActivated;
}
void MineObject::activateMine()
{
    isMineActivated = true;
}
void MineObject::initializeTile(const char *name, bool reveal)
{
    TileObject::initializeTile(name, reveal);
    isMineActivated = false;
}
