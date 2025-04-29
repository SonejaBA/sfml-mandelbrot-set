#include "ComplexPlane.h"

int main() {
	std::srand(std::time(0));
	int screenWidth = sf::VideoMode::getDesktopMode().width;
	int screenHeight = sf::VideoMode::getDesktopMode().height;
	sf::RenderWindow window(sf::VideoMode(screenWidth / 4, screenHeight / 4), "Mandelbrot set"); 
	ComplexPlane complexPlane (screenWidth / 4, screenHeight / 4) ;

	sf::Font font;
	
	if (!font.loadFromFile("fonts/CaviarDreams.ttf")) {
		std::cout << "Error. Font file not found." << std::endl;
		return -1;
	}

	sf::Text text;
	text.setFont(font);
	text.setString("Testing");


	while (window.isOpen()) {
		
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				if(event.key.code == sf::Mouse::Right) {
					complexPlane.zoomOut();
					complexPlane.setCenter(sf::Vector2i(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
				}
				else if (event.key.code == sf::Mouse::Left) {
					complexPlane.zoomIn();
					complexPlane.setCenter(sf::Vector2i(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
				}
			}

			if (event.type == sf::Event::MouseMoved) {
				complexPlane.setMouseLocation(sf::Vector2i(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));
			}
		}
		complexPlane.updateRender();

		window.clear();
		
		
		complexPlane.draw(window, sf::RenderStates::Default);
		window.draw(text);

		
		window.display();

	}

	return 0;
}
