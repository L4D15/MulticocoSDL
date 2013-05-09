#ifndef __MulticocoSDL__enemy__
#define __MulticocoSDL__enemy__

#include <iostream>
#include "entity.h"
#include "scenario.h"
#include <list>
#include <vector>

class Enemy : public Entity
{
public:
    typedef enum
    {
        FAST, NORMAL, PREDICTION, RANDOM
    }Type;
    
                        Enemy(Type type, Scenario* scenario);
                        ~Enemy();
    
    void                update();
    
    bool				isAlive();
	bool				isVulnerable();
    
	void				recordPlayerPosition(Vector2D pos);
    Vector2D            destinationCell(){ return this->_destinationCell; }
    
private:
	void				fastIA();
	void				normalIA();
	void				predictionIA();
	void				randomIA();
	void				avaiableDirections(std::vector<Vector2D>& avaiableDirections);
	Vector2D			getInverseDirection(){return _direction * (-1.0f);}
    
private:
    Type                _type;
    bool                _alive;
    bool                _vulnerable;
    std::list<Vector2D> _movements;
    Scenario*           _scenario;
    
    Vector2D            _destinationCell;
    bool                _arrivedDestination;
};


#endif /* defined(__MulticocoSDL__enemy__) */
