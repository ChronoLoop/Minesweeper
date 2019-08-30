#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"
using namespace sf;


int main()
{
    RenderWindow window(VideoMode(winWidth, winHeight), "Minesweeper");
    Board board;
    board.loadBoard();
    board.generateMines();

    while (window.isOpen())
    {
        Vector2i mousePos = Mouse::getPosition(window);
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    board.leftClickTile(mousePos);
                }
                else if (event.mouseButton.button == Mouse::Right)
                {
                    board.flagTile(mousePos);
                }
            }
        }
        window.clear();
        board.drawBoard(window);
        window.display();
    }

    return 0;
}
