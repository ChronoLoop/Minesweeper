#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "TileObject.h"
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace sf;
using std::cout;
using std::endl;
const int winWidth = 800;
const int winHeight = 600;
const int boardRow = 16;
const int boardColumn = 25;


class Board {
    int totalBombs;
    int numberOfRevealedTiles;
    int numberOfFlags;
    bool playerHitMine;
    bool revealAllMines;
    bool playerWin;
    bool generatedButtons;
    TileObject hiddenTiles[16][25];
    TileObject revealedTiles[16][25];
    MineObject mines[16][25];
    TileObject flags[16][25];
    TileObject number1[16][25];
    TileObject number2[16][25];
    TileObject number3[16][25];
    TileObject number4[16][25];
    TileObject number5[16][25];
    TileObject number6[16][25];
    TileObject number7[16][25];
    TileObject number8[16][25];
    //buttons//
    TileObject happyFace;
    TileObject loseFace;
    TileObject winFace;
    TileObject test1Button;
    TileObject test2Button;
    TileObject test3Button;
    TileObject debugButton;
    Texture digitTexture;
    Sprite digitArray [2][10];
    Sprite digitSign [2]; // index 0 is negative sign and index 1 is negative sign

public:
    //board functions//
    Board();
    void loadBoard(); //load everything we need for the minesweeper board
    void drawBoard(RenderWindow &window);
    void drawTileArray(TileObject (&tileArray)[boardRow][boardColumn], RenderWindow &window);
    void resetBoard();
    void setDigits();
    void drawDigits(RenderWindow &window);


    //show functions//
    void showAllFlags();
    void showAllMinesForLose();
    void showAllMines();

    //check functions//
    bool& checkTileForMine(int rowNum, int columnNum);
    void checkHiddenTile(const int &rowNum, const int &columnNum, const Vector2i &mousePos);

    //mouse functions//
    void leftClickTile(const Vector2i &mousePos);
    void flagTile(const Vector2i &mousePos);

    //win and loss conditions//
    void checkForWin();
    void hitMine(const int &rowNum, const int &columnNum);

    //mines functions//
    void generateMines();
    void drawMineField(MineObject (&tileArray)[boardRow][boardColumn], RenderWindow &window);
    void hideAllMines();

    //check adjacent tiles for mines//
    void setAdjacentTiles();
    void setNumberTiles();
    void recursiveReveal(int rowNum, int columnNum);

    //for buttons//
    void testBoard(const char * testName);
    void hitDebugButton();
};

