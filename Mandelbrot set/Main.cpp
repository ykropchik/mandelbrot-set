#include <SFML/Graphics.hpp>
#include "wtypes.h"
#include <iostream>
#include "cmath"

float map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

sf::Vector2f getCenter(sf::Vector2f currentCenter, sf::Vector2f boundaries, float mouseX, float mouseY) {
	return sf::Vector2f(map(mouseX, -1.0, 1.0, currentCenter.x - boundaries.x, currentCenter.x + boundaries.x), map(mouseY, -1.0, 1.0, currentCenter.y - boundaries.y, currentCenter.y + boundaries.y));
}

int main() {
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	int w = desktop.right;
	int h = desktop.bottom;
	int mouseX;
	int mouseY;
	float zoomSpeed = 0.8f;
	int zoom = 1;
	sf::Vector2f center = sf::Vector2f(-0.75, 0.0);
	sf::Vector2f boundaries = sf::Vector2f(1.75, 2.5);
	bool mousePressed[2] = { false, false };

	sf::RenderWindow window(sf::VideoMode(w, h), "Mandelbrot set", sf::Style::Fullscreen);
	window.setFramerateLimit(60);

	sf::Texture colorPalette;
	colorPalette.loadFromFile("colorPalette.jpg");

	sf::RenderTexture emptyTexture;
	emptyTexture.create(w, h);
	sf::Sprite emptySprite = sf::Sprite(emptyTexture.getTexture());
	sf::Shader shader;

	shader.loadFromFile("shader.frag", sf::Shader::Fragment);
	shader.setUniform("u_resolution", sf::Vector2f(w, h));
	shader.setUniform("u_colorPalette", colorPalette);
	shader.setUniform("u_center", sf::Vector2f(-0.75, 0.0));

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == 36) {
					window.close();
				}
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					mousePressed[0] = true;
					mousePressed[1] = false;

					mouseX = event.mouseButton.x - w / 2;
					mouseY = event.mouseButton.y - h / 2;

					float mx = ((float)mouseX / w * 2);
					float my = ((float)mouseY / h * 2);


					center = getCenter(center, boundaries, mx, my);
					std::cout << center.x << " : " << center.y << std::endl;
					shader.setUniform("u_center", center);
				}

				if (event.mouseButton.button == sf::Mouse::Right) {
					mousePressed[0] = false;
					mousePressed[1] = true;
				}
			}

			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					mousePressed[0] = false;
					mousePressed[1] = false;
				}

				if (event.mouseButton.button == sf::Mouse::Right) {
					mousePressed[0] = false;
					mousePressed[1] = false;
				}
			}

			if (event.type == sf::Event::MouseWheelScrolled) {
				if (event.mouseWheelScroll.delta < 0 && boundaries.x < 1.75) {
					boundaries /= zoomSpeed;
					zoom--;
				}
				
				if (event.mouseWheelScroll.delta > 0 && boundaries.x > 0.00001) {
					boundaries *= zoomSpeed;
					zoom++;
				}
			}

			
		}

		shader.setUniform("u_zoom", zoom);
		shader.setUniform("u_boundaries", boundaries);

		window.draw(emptySprite, &shader);
		window.display();
	}
	return 0;
}