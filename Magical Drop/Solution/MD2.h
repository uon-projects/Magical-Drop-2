//In The "Header" File Go All The Declarations As Well As All The Implementations of The Class' Methods...
#include <SFML/Graphics.hpp>
#include <string>
#include <list>

namespace std
{
	class MD2
	{
		sf::Text btnTextString;
		sf::Font font;
		int x, y;
		sf::RectangleShape btnL1, btnL2, btnL3;
		bool over;
		bool unlocked;

	public:
		sf::Text getBtnTextString();

	public:
		sf::RectangleShape getBtn1();

	public:
		sf::RectangleShape getBtn2();

	public:
		sf::RectangleShape getBtn3();

	public:
		void setLocation(int, int);

	public:
		void drawBtn(sf::RenderWindow&, std::string, int, sf::Font);

	public:
		bool btnClicked(sf::RenderWindow&);

	public:
		bool imgCLicked(sf::RenderWindow&, sf::Sprite);

	public:
		void setUnlocked(bool);

	};

	void MD2::setUnlocked(bool unlockedValue) {
		unlocked = unlockedValue;
	}

	sf::Text MD2::getBtnTextString() {
		return btnTextString;
	}

	sf::RectangleShape MD2::getBtn1() {
		return btnL1;
	}

	sf::RectangleShape MD2::getBtn2() {
		return btnL2;
	}

	sf::RectangleShape MD2::getBtn3() {
		return btnL3;
	}

	void MD2::setLocation(int newX, int newY) {
		x = newX;
		y = newY;
	}

	void MD2::drawBtn(sf::RenderWindow& window, std::string title, int characterSize, sf::Font font)
	{
		btnTextString.setString(title);
		btnTextString.setPosition(x, y);
		btnTextString.setCharacterSize(characterSize);
		btnTextString.setFillColor(sf::Color::White);
		btnTextString.setFont(font);
		btnTextString.setOrigin(btnTextString.getLocalBounds().width/2, btnTextString.getLocalBounds().height);///10*12);

		btnL1.setSize(sf::Vector2f(btnTextString.getLocalBounds().width + 30, btnTextString.getLocalBounds().height  + 20));
		btnL2.setSize(sf::Vector2f(btnTextString.getLocalBounds().width + 30, btnTextString.getLocalBounds().height  + 20));
		btnL3.setSize(sf::Vector2f(btnTextString.getLocalBounds().width + 30, btnTextString.getLocalBounds().height  + 20));

		btnL1.setOrigin(btnL1.getGlobalBounds().width/2, btnL1.getGlobalBounds().height/2);
		btnL2.setOrigin(btnL2.getGlobalBounds().width/2, btnL2.getGlobalBounds().height/2);
		btnL3.setOrigin(btnL3.getGlobalBounds().width/2, btnL3.getGlobalBounds().height/2);
	
		if(unlocked) {
			btnL1.setFillColor(sf::Color(76,175,80));
			btnL2.setFillColor(sf::Color(27,94,32));
			btnL3.setFillColor(sf::Color(27,94,32));
		} else {
			btnL1.setFillColor(sf::Color(96,125,139));
			btnL2.setFillColor(sf::Color(69,90,100));
			btnL3.setFillColor(sf::Color(69,90,100));
		}
		
		btnL1.setPosition(x, y);

		sf::IntRect btnCharactersRect(btnL1.getPosition().x - btnL1.getGlobalBounds().width / 2, btnL1.getPosition().y - btnL1.getGlobalBounds().height / 2,
		btnL1.getGlobalBounds().width, btnL1.getGlobalBounds().height);

		if (btnCharactersRect.contains(sf::Mouse::getPosition(window))) {
			btnL2.setPosition(x + 5, y - 5);
			btnL3.setPosition(x - 5, y + 5);
			over = false;
		} else {
			btnL2.setPosition(x + 15, y - 10);
			btnL3.setPosition(x - 15, y + 10);
			over = true;
		}

		window.draw(btnL2);
		window.draw(btnL3);
		window.draw(btnL1);
		window.draw(btnTextString);

	}

	bool MD2::btnClicked(sf::RenderWindow& window)
	{
		sf::IntRect btnCharactersRect(btnL1.getPosition().x - btnL1.getGlobalBounds().width / 2, btnL1.getPosition().y - btnL1.getGlobalBounds().height / 2,
		btnL1.getGlobalBounds().width, btnL1.getGlobalBounds().height);
		bool clicked = false;
		if (btnCharactersRect.contains(sf::Mouse::getPosition(window))) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && unlocked) {
				clicked = true;
			}
		}
		return clicked;
	}

	bool MD2::imgCLicked(sf::RenderWindow& window, sf::Sprite character)
	{
		sf::IntRect btnCharactersRect(character.getPosition().x - character.getGlobalBounds().width / 2,
			character.getPosition().y, character.getGlobalBounds().width, character.getGlobalBounds().height*2);
		bool clicked = false;
		if (btnCharactersRect.contains(sf::Mouse::getPosition(window))) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				clicked = true;
			}
		}
		return clicked;
	}

}