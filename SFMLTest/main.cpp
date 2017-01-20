#include <SFML/Graphics.hpp>
#include <iostream>

#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\VC\\lib\\sfml-graphics-d.lib")
#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\VC\\lib\\sfml-window-d.lib")
#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\VC\\lib\\sfml-system-d.lib")

bool p_jumping = false;
bool p_pushDown = false;
bool p_first = false;
int p_fpsToWait = 25;
int p_fpsPassed = 0;
int p_ballSpeed = 30;
int p_ballFramesPassed = 0;
int p_loses = 0;
int p_textFpsWait = 0;
enum jumpDirection
{
	up,
	down,
	none
};
enum ballDirection
{
	left,
	right
};
ballDirection p_ballDirection = right;
jumpDirection p_jumpDirection = none;
const int JUMPHEIGHT= 50;
sf::Vector2<float> p_playerPosition;
sf::CircleShape p_ball(5.f);
sf::Text p_text;
sf::RectangleShape p_player(sf::Vector2<float>(10, 20));
sf::RectangleShape p_wall1(sf::Vector2<float>(20, 40));
sf::RectangleShape p_wall2(sf::Vector2<float>(20, 40));
void update();

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 200), "Jumpy, Jumpy, Jump!");

	sf::Font font;
	font.loadFromFile("arial.ttf");
	p_text.setFont(font);
	p_text.setCharacterSize(24);
	p_text.setFillColor(sf::Color::Red);
	p_text.setStyle(sf::Text::Bold);

	sf::Vector2<float> pos1;
	pos1.x = window.getSize().x / 4 - p_wall1.getSize().x/2;
	pos1.y = window.getSize().y - p_wall1.getSize().y;
	p_wall1.setPosition(pos1);
	sf::Vector2<float> pos2;
	pos2.x = window.getSize().x / 4 * 3 - p_wall2.getSize().x / 2;
	pos2.y = window.getSize().y - p_wall2.getSize().y;
	p_wall2.setPosition(pos2);
	p_ball.setFillColor(sf::Color::Green);
	p_ball.setPosition(p_wall1.getPosition().x+p_wall1.getSize().x/2 + p_ball.getRadius()*2, window.getSize().y - (p_ball.getRadius()*2) - 10);
	p_player.setFillColor(sf::Color::Blue);
	p_playerPosition.x = window.getSize().x / 2 - p_player.getSize().x/2-1;
	p_playerPosition.y = window.getSize().y - p_player.getSize().y;
	p_player.setPosition(p_playerPosition);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Key::Space)
				{
					if(!p_pushDown && p_jumping)
					{
						p_pushDown = true;
					}
					else if(!p_jumping)
					{
						p_jumping = true;
						p_first = true;
						p_fpsPassed = 0;
						p_jumpDirection = up;
						std::cout << "jumping" << std::endl;
					}
				}
		}

		update();
		window.clear();
		window.draw(p_ball);
		window.draw(p_player);
		window.draw(p_wall1);
		window.draw(p_wall2);
		window.draw(p_text);
		window.display();
	}

	return 0;
}

void update()
{
	if(p_pushDown && p_jumpDirection == down)
	{
		p_fpsToWait = 10;
	}
	if(p_jumping && p_fpsPassed == p_fpsToWait)
	{
		if(p_player.getPosition().y == p_playerPosition.y-JUMPHEIGHT)
		{
			p_jumpDirection = down;
		}
		if(p_player.getPosition().y == p_playerPosition.y && !p_first)
		{
			p_jumpDirection = none;
			p_jumping = false;
			p_fpsToWait = 25;
			p_pushDown = false;
		}
		if(p_jumpDirection == up)
		{
			sf::Vector2<float> pos = p_player.getPosition();
			pos.y--;
			p_player.setPosition(pos);
		}
		if(p_jumpDirection == down)
		{
			sf::Vector2<float> pos = p_player.getPosition();
			pos.y++;
			p_player.setPosition(pos);
		}
		p_first = false;
		p_fpsPassed = 0;
	}
	if (p_ballFramesPassed == p_ballSpeed)
	{
		if (p_ballDirection == right)
		{
			sf::Vector2<float> pos = p_ball.getPosition();
			pos.x++;
			p_ball.setPosition(pos);
		}
		if (p_ballDirection == left)
		{
			sf::Vector2<float> pos = p_ball.getPosition();
			pos.x--;
			p_ball.setPosition(pos);
		}
		if (p_ball.getPosition().x == p_wall1.getPosition().x + p_wall1.getSize().x)
		{
			p_ballDirection = right;
			srand(time(NULL));
			auto randomNumber = rand() % 41 + 10;
			p_ballSpeed = randomNumber;
		}
		if (p_ball.getPosition().x + p_ball.getRadius() * 2 == p_wall2.getPosition().x)
		{
			p_ballDirection = left;
			srand(time(NULL));
			auto randomNumber = rand() % 41 + 10;
			p_ballSpeed = randomNumber;
		}
		if (p_player.getGlobalBounds().intersects(p_ball.getGlobalBounds()))
		{
			if (p_textFpsWait == 20)
			{
				p_loses++;
				p_textFpsWait = 0;
			}
			p_textFpsWait++;
		}
		p_ballFramesPassed = 0;
		p_text.setString("Loses: " + std::to_string(p_loses));
	}
	p_ballFramesPassed++;
	p_fpsPassed++;
}