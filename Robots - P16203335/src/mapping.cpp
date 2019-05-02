#pragma once
#include <Aria.h>
#include <math.h>
#include <iostream>
#include "mapping.h"

mapping::mapping() : ArAction("Mapping") {
	window.create(sf::VideoMode(800, 600), "Map - P16203335");
	window.setActive(false);
	robot.setSize(sf::Vector2f(20.f, 20.f));
	robot.setFillColor(sf::Color().Red);
}

ArActionDesired * mapping::fire(ArActionDesired d)
{	
	desiredState.reset();
	for (int i = 0; i < 16; i++)	//For all sonars
	{
		if (myRobot->getSonarRange(i) < 5000)
		{
			Sonar sonar;
			sonar.range = myRobot->getSonarRange(i);
			sonar.number = i;
			sonars.push_back(sonar);
		}
	}
	double robotX = myRobot->getX();
	double robotY = myRobot->getY();
	double robotTh = myRobot->getTh();
	for (Sonar sonar : sonars)
	{
		double xPoint = (cos((sonar.getAngle()*(M_PI/180))) * (sonar.range + ((myRobot->getRobotWidth())/2)));
		double yPoint = (sin((sonar.getAngle()*(M_PI / 180))) * (sonar.range + ((myRobot->getRobotWidth()) / 2)));
		sf::CircleShape circle(3);
		circle.setPosition(sf::Vector2f(-(xPoint/6), (yPoint/6)));
		circle.setFillColor(sf::Color().Cyan);
		circles.push_back(circle);
	}
	sf::View view(sf::Vector2f(robot.getPosition().x, robot.getPosition().y), sf::Vector2f(800, 600));
	window.setView(view);
	window.clear();
	window.draw(robot);
	int count;
	for (int i = 0; i < circles.size(); i++)
	{
		window.draw(circles.at(i));
	}
	count++;

	window.setActive(true);
	window.display();
	if (count > 4)
	{
		circles.clear();
		count = 0;
	}
	return &desiredState;
}