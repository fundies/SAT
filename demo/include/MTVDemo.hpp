#ifndef MTVDEMO_HPP
#define MTVDEMO_HPP

#include "Demo.hpp"
#include "drawables.hpp"

#include <Crash2D/collision.hpp>

#include <memory>

class MTVDemo : public Demo {
	using Shape_ptr = std::unique_ptr<shape>;

	std::vector<Shape_ptr> vecA;
	std::vector<Shape_ptr> vecB;

	const int ShapeA = 1;
	const int ShapeB = 2;

public:
	MTVDemo(sf::RenderWindow &window) : Demo(window), vecA(3), vecB(3) {
		vecA[0] = Shape_ptr(new circle(Crash2D::Vector2(300, 400), 150));
		vecA[1] = Shape_ptr(new polygon());
		vecA[1]->SetPointCount(4);
		vecA[1]->SetPoint(0, Crash2D::Vector2(250, 250));
		vecA[1]->SetPoint(1, Crash2D::Vector2(350, 250));
		vecA[1]->SetPoint(2, Crash2D::Vector2(350, 350));
		vecA[1]->SetPoint(3, Crash2D::Vector2(250, 350));
		vecA[1]->ReCalc();

		vecA[2] = Shape_ptr(new segment(Crash2D::Vector2(400, 300), Crash2D::Vector2(500, 300)));

		vecB[0] = Shape_ptr(new circle(Crash2D::Vector2(400, 300), 50));

		vecB[1] = Shape_ptr(new polygon());
		vecB[1]->SetPointCount(3);
		vecB[1]->SetPoint(0, Crash2D::Vector2(350, 300));
		vecB[1]->SetPoint(1, Crash2D::Vector2(400, 300));
		vecB[1]->SetPoint(2, Crash2D::Vector2(325, 350));
		vecB[1]->ReCalc();

		vecB[2] = Shape_ptr(new segment(Crash2D::Vector2(450, 250), Crash2D::Vector2(450, 350)));

		vecA[ShapeA]->SetColor(sf::Color::Green);
		vecB[ShapeB]->SetColor(sf::Color::Red);
	}

	void draw() {
		// clear the window with black color
		window.clear(sf::Color::Black);

		// Move
		int hdir = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) - sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		int vdir = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) - sf::Keyboard::isKeyPressed(sf::Keyboard::Up);

		// Scale
		int scl = sf::Keyboard::isKeyPressed(sf::Keyboard::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::A);

		// Rot
		int rot = sf::Keyboard::isKeyPressed(sf::Keyboard::X) - sf::Keyboard::isKeyPressed(sf::Keyboard::Z);

		Crash2D::Transformation t;
		t.Translate(Crash2D::Vector2(hdir * 4, vdir * 4));
		t.SetPivot(vecB[ShapeB]->GetCenter());
		t.Rotate(rot);
		
		if (scl == 1)
			t.SetScale(Crash2D::Vector2(1.1, 1.1));
		else if (scl == -1)
			t.SetScale(Crash2D::Vector2(0.9, 0.9));

		vecB[ShapeB]->Transform(t);

		Crash2D::Collision collision = vecA[ShapeA]->GetCollision(*vecB[ShapeB]);

		if (vecA[ShapeA]->Contains(*vecB[ShapeB]))
		{
			vecB[ShapeB]->SetColor(sf::Color::Blue);
		}
		else if (collision.Overlaps())
		{
			vecB[ShapeB]->SetColor(sf::Color(255, 255, 0, 200));
		}
		else
			vecB[ShapeB]->SetColor(sf::Color::Red);

		window.draw(*vecA[ShapeA]);
		window.draw(*vecB[ShapeB]);

		for (auto && p : collision.GetIntersects())
		{
			circle c(p,  3);
			c.SetColor(sf::Color::Red);
			window.draw(c);
		}

		if (collision.Overlaps() || collision.AcontainsB())
		{
			Crash2D::Transformation t;

			t.Translate(collision.GetDisplacement());
			vecB[ShapeB]->Transform(t);
			vecB[ShapeB]->SetColor(sf::Color(255, 0, 255, 50));

			window.draw(*vecB[ShapeB]);

			t.Translate(-collision.GetDisplacement());
			t.Translate(-collision.GetDisplacement());
			vecB[ShapeB]->Transform(t);
		}

		circle c1(vecA[ShapeA]->GetCenter(), 3);
		c1.SetColor(sf::Color::Magenta);

		circle c2(vecB[ShapeB]->GetCenter(), 3);
		c2.SetColor(sf::Color::Magenta);

		window.draw(c1);
		window.draw(c2);
	}
};

#endif // MTVDEMO_HPP
