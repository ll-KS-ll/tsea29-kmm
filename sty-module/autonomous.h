/************************************************************************
 *																		*
 * Author: GünTECH							                            *
 * Purpose: Logic used by robot to drive through labyrinth				*
 * Language: C															*
 * File type: Header													*
 *																		*
/************************************************************************/

#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_

void exploreLabyrinth();

void rotateOnSpot(int startAngle, int endAngle);
void moveOneNode();
void autoAdjustPosition();


#endif /* AUTONOMOUS_H_ */