#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
using namespace sf;
using std::vector;
using std::cout;
using std::endl;

class TileObject{
    float width;
    float height;
    bool showTile;
    Texture tileTexture;
    Sprite tileSprite;
    TileObject *neighbors[8];
    int numOfNeighbors;
public:
    TileObject();
    virtual void initializeTile(const char* name, bool reveal);
    void setPosition(float x, float y);
    void revealTile(bool reveal);
    bool& isTileRevealed();
    TileObject* getNeighbors();
    void addOneNeighbor(TileObject* tile);
    //accessors//
    const float &getHeight() const;
    const float &getWidth() const;
    const Sprite &getSprite() const;
    const Texture &getTexture() const;
    const Vector2f &getPosition() const;
    const int &getNumOfNeighbors() const;
};

class MineObject : public TileObject{
    bool isMineActivated; //used to check if this mine is activated
public:
    void initializeTile(const char* name, bool reveal);
    bool& isMineSet();
    void activateMine();
    MineObject();
};
