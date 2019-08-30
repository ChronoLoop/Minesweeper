#include "Board.h"

void Board::testBoard(const char *testName)
{
    resetBoard();
    std::ifstream file(testName);
    char testBoard[16][25];
    if (file.is_open())
    {
        for (unsigned int rowNum = 0; rowNum < boardRow; rowNum++)
        {
            for (unsigned int columnNum = 0; columnNum < boardColumn; columnNum++)
            {
                file >> testBoard[rowNum][columnNum];
                if (testBoard[rowNum][columnNum] == '1')
                {
                    mines[rowNum][columnNum].activateMine();
                    totalBombs++;
                }
            }
        }
        setAdjacentTiles();
        setNumberTiles();
    }
}
void Board::drawDigits(RenderWindow &window)
{
    int score = 0;
    int digit1 = 0;
    int digit2 = 0;
    if (numberOfFlags == totalBombs)
    {
        window.draw(digitArray[1][digit2]);
        window.draw(digitArray[0][digit1]);
        window.draw(digitSign[0]);
    }
    else if (numberOfFlags < totalBombs)
    {
        score = totalBombs - numberOfFlags;
        digit1 = score%10;
        score = score/10;
        digit2 = score%10;
        window.draw(digitArray[1][digit2]);
        window.draw(digitArray[0][digit1]);
        window.draw(digitSign[0]);
    }
    else if (numberOfFlags > totalBombs)
    {
        score = numberOfFlags-totalBombs;
        while (score > 99)
        {
            score = score - 100;
        }
        digit1 = score%10;
        score = score/10;
        digit2 = score%10;
        score = score/10;
        window.draw(digitArray[1][digit2]);
        window.draw(digitArray[0][digit1]);
        window.draw(digitSign[1]);
    }

}
void Board::setDigits()
{
    const int width = 21;
    const int height = 32;
    digitTexture.loadFromFile("images/digits.png");

    for (unsigned int i = 0; i < 2; i++)
    {
        digitSign[i].setTexture(digitTexture);
        int textureWidth = 0;
        for (unsigned int j = 0; j < 10; ++j)
        {
            digitArray[i][j].setTexture(digitTexture);
            digitArray[i][j].setTextureRect(IntRect(textureWidth, 0, width, height));
            textureWidth = textureWidth + 21;
            if (i == 0)
            {
                digitArray[0][j].setPosition(width*2,height*16);
            }
            else if (i == 1)
            {
                digitArray[1][j].setPosition(width*1,height*16);
            }
        }
    }
    // index 0 is zero and index 1 is negative sign
    digitSign[0].setTextureRect(IntRect(0, 0, width, height));
    digitSign[0].setPosition(0,height*16);
    digitSign[1].setTextureRect(IntRect(width*10, 0, width, height));
    digitSign[1].setPosition(0,height*16);
}

Board::Board()
{
    totalBombs = 0;
    numberOfFlags = 0;
    numberOfRevealedTiles = 0;
    playerHitMine = false;
    revealAllMines = false;
    playerWin = false;
    generatedButtons = false;
}
void Board::resetBoard()
{
    totalBombs = 0;
    numberOfRevealedTiles = 0;
    numberOfFlags = 0;
    playerHitMine = false;
    revealAllMines = false;
    playerWin = false;
    loadBoard();
}

//mine functions//
bool& Board::checkTileForMine(int rowNum, int columnNum)
{
    return mines[rowNum][columnNum].isMineSet();
}
void Board::showAllMines()
{
    for (unsigned int rowNum = 0; rowNum < boardRow; rowNum++)
    {
        for (unsigned int columnNum = 0; columnNum < boardColumn; columnNum++)
        {
            if (checkTileForMine(rowNum, columnNum))
            {
                mines[rowNum][columnNum].revealTile(true);
            }
        }
    }
}
void Board::showAllMinesForLose()
{
    for (unsigned int rowNum = 0; rowNum < boardRow; rowNum++)
    {
        for (unsigned int columnNum = 0; columnNum < boardColumn; columnNum++)
        {
            if (checkTileForMine(rowNum, columnNum))
            {
                mines[rowNum][columnNum].revealTile(true);
                hiddenTiles[rowNum][columnNum].revealTile(false);
                revealedTiles[rowNum][columnNum].revealTile(true);
            }
        }
    }

}
void Board::hideAllMines()
{
    for (unsigned int rowNum = 0; rowNum < boardRow; rowNum++)
    {
        for (unsigned int columnNum = 0; columnNum < boardColumn; columnNum++)
        {
            mines[rowNum][columnNum].revealTile(false);
        }
    }

}
void Board::showAllFlags()
{
    for (unsigned int rowNum = 0; rowNum < boardRow; rowNum++)
    {
        for (unsigned int columnNum = 0; columnNum < boardColumn; columnNum++)
        {
            if (checkTileForMine(rowNum, columnNum) && !flags[rowNum][columnNum].isTileRevealed())
            {
                flags[rowNum][columnNum].revealTile(true);
                numberOfFlags++;
            }
        }
    }
}
void Board::hitDebugButton()
{
    if (!revealAllMines)
    {
        showAllMines();
        revealAllMines = true;
    }
    else if (revealAllMines)
    {
        hideAllMines();
        revealAllMines = false;
    }
}
void Board::generateMines()
{
    while (totalBombs != 50)
    {
        int random_col = rand() % 25 + 0;
        int random_row = rand() % 16 + 0;
        if (!checkTileForMine(random_row, random_col)) //checks if the mine is activated at this location
        {
            mines[random_row][random_col].activateMine();
            totalBombs++;
        }
    }

    setAdjacentTiles();
    setNumberTiles();
}
void Board::drawTileArray(TileObject (&tileArray)[boardRow][boardColumn], RenderWindow &window)
{
    float y = 0.0f;
    for (unsigned int rowNum = 0; rowNum < boardRow; rowNum++)
    {
        float x = 0.0f;
        for (unsigned int columnNum = 0; columnNum < boardColumn; columnNum++)
        {
            tileArray[rowNum][columnNum].setPosition(x, y);
            if (tileArray[rowNum][columnNum].isTileRevealed()) //check if this tile should be shown
            {
                window.draw(tileArray[rowNum][columnNum].getSprite());
            }
            x = x + tileArray[0][0].getWidth();
        }
        y = y + tileArray[0][0].getHeight();
    }
}
void Board::drawMineField(MineObject (&mineArray)[boardRow][boardColumn], RenderWindow &window)
{
    float y = 0.0f;
    for (unsigned int rowNum = 0; rowNum < boardRow; rowNum++)
    {
        float x = 0.0f;
        for (unsigned int columnNum = 0; columnNum < boardColumn; columnNum++)
        {
            mineArray[rowNum][columnNum].setPosition(x, y);
            if (!flags[rowNum][columnNum].isTileRevealed())
            {
                if (mineArray[rowNum][columnNum].isTileRevealed()) //check if this tile should be shown
                {
                    window.draw(mineArray[rowNum][columnNum].getSprite());
                }
            }
            x = x + mineArray[0][0].getWidth();
        }
        y = y + mineArray[0][0].getHeight();
    }
}

void Board::loadBoard() //load everything we need for the minesweeper board
{
    for (unsigned int rowNum = 0; rowNum < boardRow; rowNum++)
    {
        for (unsigned int columnNum = 0; columnNum < boardColumn; columnNum++)
        {
            hiddenTiles[rowNum][columnNum].initializeTile("images/tile_hidden.png",true); //initializeTile(image name, bool showtile)
            revealedTiles[rowNum][columnNum].initializeTile("images/tile_revealed.png", false);
            flags[rowNum][columnNum].initializeTile("images/flag.png", false);
            mines[rowNum][columnNum].initializeTile("images/mine.png", false);
            number1[rowNum][columnNum].initializeTile("images/number_1.png", false);
            number2[rowNum][columnNum].initializeTile("images/number_2.png", false);
            number3[rowNum][columnNum].initializeTile("images/number_3.png", false);
            number4[rowNum][columnNum].initializeTile("images/number_4.png", false);
            number5[rowNum][columnNum].initializeTile("images/number_5.png", false);
            number6[rowNum][columnNum].initializeTile("images/number_6.png", false);
            number7[rowNum][columnNum].initializeTile("images/number_7.png", false);
            number8[rowNum][columnNum].initializeTile("images/number_8.png", false);
        }
    }

    if (!generatedButtons)
    {
        //buttons//
        happyFace.initializeTile("images/face_happy.png", true);
        loseFace.initializeTile("images/face_lose.png", false);
        winFace.initializeTile("images/face_win.png", false);
        debugButton.initializeTile("images/debug.png", true);
        test1Button.initializeTile("images/test_1.png", true);
        test2Button.initializeTile("images/test_2.png", true);
        test3Button.initializeTile("images/test_3.png", true);
        //setting up buttons
        happyFace.setPosition(happyFace.getWidth() * 5.75f, happyFace.getHeight() * 8);
        loseFace.setPosition(loseFace.getWidth() * 5.75f, happyFace.getHeight() * 8);
        winFace.setPosition(winFace.getWidth() * 5.75f, happyFace.getHeight() * 8);
        debugButton.setPosition(happyFace.getPosition().x + debugButton.getWidth() * 2.0f, debugButton.getHeight() * 8);
        test1Button.setPosition(debugButton.getPosition().x + test1Button.getWidth(), test1Button.getHeight() * 8);
        test2Button.setPosition(test1Button.getPosition().x + test2Button.getWidth(), test2Button.getHeight() * 8);
        test3Button.setPosition(test2Button.getPosition().x + test3Button.getWidth(), test3Button.getHeight() * 8);
        generatedButtons = true;
    }
    setDigits();
}

void Board::drawBoard(RenderWindow &window)
{
    //draw tile arrays
    if (revealAllMines) //debug mode
    {
        drawTileArray(revealedTiles, window);
        drawTileArray(number1, window);
        drawTileArray(number2, window);
        drawTileArray(number3, window);
        drawTileArray(number4, window);
        drawTileArray(number5, window);
        drawTileArray(number6, window);
        drawTileArray(number7, window);
        drawTileArray(number8, window);
        drawTileArray(hiddenTiles, window);
        drawMineField(mines, window);
        drawTileArray(flags, window);
    }
    else
    {
        drawTileArray(revealedTiles, window);
        drawTileArray(number1, window);
        drawTileArray(number2, window);
        drawTileArray(number3, window);
        drawTileArray(number4, window);
        drawTileArray(number5, window);
        drawTileArray(number6, window);
        drawTileArray(number7, window);
        drawTileArray(number8, window);
        drawTileArray(hiddenTiles, window);
        drawTileArray(flags, window);
        drawMineField(mines, window);
    }

    //draw buttons
    if (playerHitMine)
    {
        loseFace.revealTile(true);
        window.draw(loseFace.getSprite());
    }
    else if (!playerHitMine)
    {
        window.draw(happyFace.getSprite());
        if (playerWin)
        {
            window.draw(winFace.getSprite());
        }
    }
    window.draw(debugButton.getSprite());
    window.draw(test1Button.getSprite());
    window.draw(test2Button.getSprite());
    window.draw(test3Button.getSprite());
    drawDigits(window);
}

void Board::leftClickTile(const Vector2i &mousePos)
{
    if (playerHitMine || playerWin)
    {
        if (happyFace.getSprite().getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            resetBoard();
            generateMines();
        }
    }
    else if (!playerHitMine && !playerWin)
    {
        if (happyFace.getSprite().getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            resetBoard();
            generateMines();
        }
        if (debugButton.getSprite().getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            hitDebugButton();
        }

        else if (test1Button.getSprite().getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            testBoard("boards/testboard1.brd");
        }
        else if (test2Button.getSprite().getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            testBoard("boards/testboard2.brd");
        }
        else if (test3Button.getSprite().getGlobalBounds().contains(mousePos.x, mousePos.y))
        {
            testBoard("boards/testboard3.brd");
        }
        else
        {
            for (unsigned int rowNum = 0; rowNum < boardRow; rowNum++)
            {
                for (unsigned int columnNum = 0; columnNum < boardColumn; columnNum++)
                {
                    checkHiddenTile(rowNum, columnNum, mousePos);
                }
            }
        }
    }


}

void Board::checkHiddenTile(const int &rowNum, const int &columnNum, const Vector2i &mousePos)
{
    if (hiddenTiles[rowNum][columnNum].isTileRevealed()) //if this location has a hidden tile
    {
        FloatRect hiddenTileRect = hiddenTiles[rowNum][columnNum].getSprite().getGlobalBounds();
        if (hiddenTileRect.contains(mousePos.x, mousePos.y)) //check if mouse is inside hidden tile
        {
            if (!flags[rowNum][columnNum].isTileRevealed()) //dont do anything if player left clicks a flag tile
            {
                recursiveReveal(rowNum,columnNum);
                hitMine(rowNum, columnNum);
                if (!playerHitMine)
                {
                    checkForWin();
                }

            }
        }
    }
}

void Board::flagTile(const Vector2i &mousePos)
{
    if (!playerHitMine && !playerWin)
    {
        for (unsigned int rowNum = 0; rowNum < boardRow; rowNum++)
        {
            for (unsigned int columnNum = 0; columnNum < boardColumn; columnNum++)
            {
                if (hiddenTiles[rowNum][columnNum].isTileRevealed()) //if this location has a hidden tile
                {
                    FloatRect hiddenTileRect = hiddenTiles[rowNum][columnNum].getSprite().getGlobalBounds();
                    if (hiddenTileRect.contains(mousePos.x, mousePos.y)) //check if mouse is inside hidden tile
                    {
                        if (flags[rowNum][columnNum].isTileRevealed())
                        {
                            flags[rowNum][columnNum].revealTile(false);
                            numberOfFlags--;
                        }
                        else
                        {
                            flags[rowNum][columnNum].revealTile(true);
                            numberOfFlags++;
                        }
                    }
                }
            }
        }
    }
}
void Board::setAdjacentTiles()
{
    for (unsigned int rowNum = 0; rowNum < boardRow; rowNum++)
    {
        for (unsigned int columnNum = 0; columnNum < boardColumn; columnNum++)
        {
            ///////////corners///////////
            if (rowNum == 0 && columnNum == 0) //upper left corner
            {
                if (checkTileForMine(rowNum, columnNum))
                {
                    MineObject *temp = &mines[rowNum][columnNum];
                    hiddenTiles[rowNum+1][columnNum].addOneNeighbor(temp);
                    hiddenTiles[rowNum][columnNum+1].addOneNeighbor(temp);
                    hiddenTiles[rowNum+1][columnNum+1].addOneNeighbor(temp);
                }
            }
            else if (1 <= columnNum && columnNum <= 23 && rowNum == 0) //top edge
            {
                if (checkTileForMine(rowNum, columnNum))
                {
                    MineObject *temp = &mines[rowNum][columnNum];
                    hiddenTiles[rowNum][columnNum-1].addOneNeighbor(temp);
                    hiddenTiles[rowNum][columnNum+1].addOneNeighbor(temp);
                    hiddenTiles[rowNum+1][columnNum].addOneNeighbor(temp);
                    hiddenTiles[rowNum+1][columnNum+1].addOneNeighbor(temp);
                    hiddenTiles[rowNum+1][columnNum-1].addOneNeighbor(temp);

                }
            }
            else if (1 <= columnNum && columnNum <= 23 && rowNum == 15) //bottom edge
            {
                if (checkTileForMine(rowNum, columnNum))
                {
                    MineObject *temp = &mines[rowNum][columnNum];
                    hiddenTiles[rowNum][columnNum - 1].addOneNeighbor(temp);
                    hiddenTiles[rowNum][columnNum + 1].addOneNeighbor(temp);
                    hiddenTiles[rowNum - 1][columnNum].addOneNeighbor(temp);
                    hiddenTiles[rowNum - 1][columnNum + 1].addOneNeighbor(temp);
                    hiddenTiles[rowNum - 1][columnNum - 1].addOneNeighbor(temp);
                }
            }
            else if (1 <= rowNum && rowNum <= 14 && columnNum == 0) //left edge
            {
                if (checkTileForMine(rowNum, columnNum))
                {
                    MineObject *temp = &mines[rowNum][columnNum];
                    hiddenTiles[rowNum - 1][columnNum].addOneNeighbor(temp);
                    hiddenTiles[rowNum - 1][columnNum + 1].addOneNeighbor(temp);
                    hiddenTiles[rowNum][columnNum + 1].addOneNeighbor(temp);
                    hiddenTiles[rowNum + 1][columnNum + 1].addOneNeighbor(temp);
                    hiddenTiles[rowNum + 1][columnNum].addOneNeighbor(temp);
                }
            }
            else if (1 <= rowNum && rowNum <= 14 && columnNum == 24) //right edge
            {
                if (checkTileForMine(rowNum, columnNum))
                {
                    MineObject *temp = &mines[rowNum][columnNum];
                    hiddenTiles[rowNum - 1][columnNum].addOneNeighbor(temp);
                    hiddenTiles[rowNum - 1][columnNum -1].addOneNeighbor(temp);
                    hiddenTiles[rowNum][columnNum - 1].addOneNeighbor(temp);
                    hiddenTiles[rowNum + 1][columnNum -1].addOneNeighbor(temp);
                    hiddenTiles[rowNum + 1][columnNum].addOneNeighbor(temp);
                }
            }
            else if (rowNum == 0 && columnNum == 24) //upper right corner
            {
                if (checkTileForMine(rowNum, columnNum))
                {
                    MineObject *temp = &mines[rowNum][columnNum];
                    hiddenTiles[rowNum+1][columnNum].addOneNeighbor(temp);
                    hiddenTiles[rowNum+1][columnNum-1].addOneNeighbor(temp);
                    hiddenTiles[rowNum][columnNum-1].addOneNeighbor(temp);
                }
            }
            else if (rowNum == 15 && columnNum == 0) //lower left corner
            {
                if (checkTileForMine(rowNum, columnNum))
                {
                    MineObject *temp = &mines[rowNum][columnNum];
                    hiddenTiles[rowNum - 1][columnNum].addOneNeighbor(temp);
                    hiddenTiles[rowNum - 1][columnNum + 1].addOneNeighbor(temp);
                    hiddenTiles[rowNum][columnNum + 1].addOneNeighbor(temp);
                }
            }
            else if (rowNum == 15 && columnNum == 24)//lower right corner
            {
                if (checkTileForMine(rowNum, columnNum))
                {
                    MineObject *temp = &mines[rowNum][columnNum];
                    hiddenTiles[rowNum - 1][columnNum].addOneNeighbor(temp);
                    hiddenTiles[rowNum - 1][columnNum - 1].addOneNeighbor(temp);
                    hiddenTiles[rowNum][columnNum - 1].addOneNeighbor(temp);
                }
            }
            else
            {
                if (checkTileForMine(rowNum, columnNum))
                {
                    MineObject *temp = &mines[rowNum][columnNum];
                    hiddenTiles[rowNum-1][columnNum-1].addOneNeighbor(temp);
                    hiddenTiles[rowNum-1][columnNum].addOneNeighbor(temp);
                    hiddenTiles[rowNum-1][columnNum+1].addOneNeighbor(temp);
                    hiddenTiles[rowNum][columnNum-1].addOneNeighbor(temp);
                    hiddenTiles[rowNum][columnNum+1].addOneNeighbor(temp);
                    hiddenTiles[rowNum+1][columnNum-1].addOneNeighbor(temp);
                    hiddenTiles[rowNum+1][columnNum].addOneNeighbor(temp);
                    hiddenTiles[rowNum+1][columnNum+1].addOneNeighbor(temp);
                }
            }
        }
    }

}
void Board::setNumberTiles()
{
    for (unsigned int rowNum = 0; rowNum < boardRow; rowNum++)
    {
        for (unsigned int columnNum = 0; columnNum < boardColumn; columnNum++)
        {
            int numOfNeighbors = hiddenTiles[rowNum][columnNum].getNumOfNeighbors();
            if (!checkTileForMine(rowNum, columnNum))
            {
                if (numOfNeighbors  == 1)
                {
                    number1[rowNum][columnNum].revealTile(true);
                }
                else if (numOfNeighbors  == 2)
                {
                    number2[rowNum][columnNum].revealTile(true);
                }
                else if (numOfNeighbors  == 3)
                {
                    number3[rowNum][columnNum].revealTile(true);
                }
                else if (numOfNeighbors  == 4)
                {
                    number4[rowNum][columnNum].revealTile(true);
                }
                else if (numOfNeighbors  == 5)
                {
                    number5[rowNum][columnNum].revealTile(true);
                }
                else if (numOfNeighbors  == 6)
                {
                    number6[rowNum][columnNum].revealTile(true);
                }
                else if (numOfNeighbors  == 7)
                {
                    number7[rowNum][columnNum].revealTile(true);
                }
                else if (numOfNeighbors  == 8)
                {
                    number8[rowNum][columnNum].revealTile(true);
                }
            }
        }
    }
}

void Board::recursiveReveal(int rowNum, int columnNum)
{

    int numOfNeighbors = hiddenTiles[rowNum][columnNum].getNumOfNeighbors();
    if (rowNum < 0 || columnNum < 0 || rowNum > 15 || columnNum > 24)
    {
        return;
    }
    else if (numOfNeighbors != 0)
    {
        if (!revealedTiles[rowNum][columnNum].isTileRevealed() && !flags[rowNum][columnNum].isTileRevealed() && !checkTileForMine(rowNum,columnNum))
        {
            hiddenTiles[rowNum][columnNum].revealTile(false);
            revealedTiles[rowNum][columnNum].revealTile(true);
            numberOfRevealedTiles++;
        }
        return;
    }
    else if (numOfNeighbors == 0 && !revealedTiles[rowNum][columnNum].isTileRevealed() && !checkTileForMine(rowNum,columnNum) && !flags[rowNum][columnNum].isTileRevealed())
    {
        revealedTiles[rowNum][columnNum].revealTile(true);
        hiddenTiles[rowNum][columnNum].revealTile(false);
        numberOfRevealedTiles++;
        recursiveReveal(rowNum, columnNum -1);
        recursiveReveal(rowNum,columnNum +1);
        recursiveReveal(rowNum + 1, columnNum +1);
        recursiveReveal(rowNum + 1, columnNum);
        recursiveReveal(rowNum + 1, columnNum -1);
        recursiveReveal(rowNum - 1, columnNum - 1);
        recursiveReveal(rowNum - 1, columnNum);
        recursiveReveal(rowNum - 1, columnNum + 1);
    }
}

///win and loss conditions//
void Board::hitMine(const int &rowNum, const int &columnNum)
{
    if (checkTileForMine(rowNum, columnNum))
    {
        showAllMinesForLose();
        playerHitMine = true;
        revealAllMines = true;
    }
}
void Board::checkForWin()
{
    if (numberOfRevealedTiles == 400 - totalBombs)
    {
        playerWin = true;
        showAllFlags();
        winFace.revealTile(true);
    }
}