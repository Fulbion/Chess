// Include SFML library
#include <SFML/Graphics.hpp>
// Include `array` header
#include <array>

// Some `typedef` to shorten keywords
typedef unsigned char byte;
typedef unsigned int uint;

// Define the size of one piece
const byte SIZE = 56;

// Create an array of sprites
std::array<sf::Sprite, 32> figures;

// Create an array for the board
// // NOTE: I planned to use `std::array<std::array<char, 8>, 8>`, but for some reason,
// //       Visual Studio told me that was incorrect. Please help.
// I just included `array` and now it works... Does that mean I'm stupid?
std::array<std::array<char, 8>, 8> board =
{
	-1,-2,-3,-4,-5,-3,-2,-1,
	-6,-6,-6,-6,-6,-6,-6,-6,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0,
	 6, 6, 6, 6, 6, 6, 6, 6,
	 1, 2, 3, 4, 5, 3, 2, 1
};

// Load the pieces in the screen with a function
void loadPosition()
{
	char k = 0;
	
	// Make a two-dimensional `for` loop
	for (byte i = 0; i < 8; i++)
	{
		for (byte j = 0; j < 8; j++)
		{
			// Store the index of the piece in a variable
			char index = board[i][j];

			// If the variable is null, then don't execute the rest of the function
			if (index == NULL)
				continue;

			// The X coordinate in the texture is equal to the positive part of
			// the index (also named "absolute value") multiplied by the size of
			// 1 piece (I'm not talking about the anime, abruti)
			byte textureX = abs(index) - 1;
			// If the index is positive, the Y coordinate is 56 (the size in pixels
			// of 1 piece), otherwise it's 0
			byte textureY = index > 0 ? 1 : 0;

			// Resize and move the sprite's texture
			figures[k].setTextureRect(sf::IntRect(SIZE * textureX, SIZE * textureY, SIZE, SIZE));
			figures[k].setPosition(SIZE * j, SIZE * i);

			// Increment the `k` index (dude I just made 2 `for` loops, adding one
			// more would be a crime)
			k++;
		}
	}
}

// Create the main function
int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(453, 453), "Chess", sf::Style::Close);

	// Import the textures
	sf::Texture t1, t2;
	t1.loadFromFile("resources/images/figures.png");
	t2.loadFromFile("resources/images/board0.png");

	// Assign textures to their sprites
	for (byte i = 0; i < 32; i++)
		figures[i].setTexture(t1);
	sf::Sprite chessboard(t2);

	// Use the `loadPosition` function (I explained it earlier)
	loadPosition();

	// Declare some variable to move the pieces
	bool isMoving = false;
	float dx = 0.0F, dy = 0.0F;
	byte n = 0;

	// Make the game loop
	while (window.isOpen())
	{
		// While the window can receive events...
		sf::Event e;
		while (window.pollEvent(e))
		{
			// If the 'X' button is clicked, then close the window
			if (e.type == sf::Event::Closed)
				window.close();

			// Drag and drop the pieces
			if (e.type == sf::Event::MouseButtonPressed)
			{
				if (e.key.code == sf::Mouse::Left)
				{
					for (byte i = 0; i < 32; i++)
					{
						if (figures[i].getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
						{
							n = i;
							isMoving = true;
							dx = sf::Mouse::getPosition(window).x - figures[i].getPosition().x;
							dy = sf::Mouse::getPosition(window).y - figures[i].getPosition().y;
						}
					}
				}
			}

			if (e.type == sf::Event::MouseButtonReleased)
			{
				if (e.key.code == sf::Mouse::Left)
				{
					isMoving = false;
					sf::Vector2f currentPosition = figures[n].getPosition() + sf::Vector2f(SIZE / 2, SIZE / 2);
					sf::Vector2f newPosition = sf::Vector2f(SIZE * static_cast<char>(currentPosition.x / SIZE), SIZE * static_cast<char>(currentPosition.y / SIZE));
					figures[n].setPosition(newPosition);
				}
			}
		}

		if (isMoving)
			figures[n].setPosition(sf::Mouse::getPosition(window).x - dx, sf::Mouse::getPosition(window).y - dy);

		// Clear the window
		window.clear();

		// Draw the chessboard
		window.draw(chessboard);
		// Draw the figures
		for (byte i = 0; i < 32; i++)
			window.draw(figures[i]);

		// Update the window
		window.display();
	}

	// End the program
	return EXIT_SUCCESS;
}