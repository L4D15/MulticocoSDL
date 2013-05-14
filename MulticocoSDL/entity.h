#ifndef ENTITY_H
#define ENTITY_H

#include "vector2d.h"
#include "spritesheet.h"
#include <SDL.h>
#include "collisionbox.h"

#define COLLISION_BOX_RESIZE_FACTOR 0.8

class Entity
{
public:
                            Entity(){ }
	virtual					~Entity(){}

	void					setPosition(float x, float y){ _position.setX(x);_position.setY(y); }
	void					setPosition(Vector2D v){ _position.setX(v.x()); _position.setY(v.y()); this->_collisionBox->updatePosition(this->_position); }
	void					setDirection(float x, float y){ _direction.setX(x);_direction.setY(y); }
	void					setDirection(Vector2D& v){ _direction = v; }
    inline void				setVisible(bool v){ _visible = v; }
	inline void				setMoving(bool m){ _moving = m; }
	void					setSpriteSheet(const char* file, int w, int h, int* animations, int nAnimations)
                                        {   _sprite = new SpriteSheet(file,w,h,animations,nAnimations);
                                            _collisionBox = new CollisionBox(_position,w,h,COLLISION_BOX_RESIZE_FACTOR);
                                        }
    SpriteSheet&            spriteSheet(){ return *_sprite; }
    CollisionBox&           collisionBox(){ return *_collisionBox; }
    
    void                    setAnimation(const char* name){ _sprite->setAnimation(name); }

	inline bool				isVisible(){ return _visible; }
	inline bool				isMoving(){ return _moving; }

	inline Vector2D&		position(){ return _position; }
	inline Vector2D&		direction(){ return _direction; }

    virtual inline void		update(){ _sprite->nextFrame(); }

	void					move(){ _previousPosition = _position; _position = _position + _direction; _collisionBox->updatePosition(this->_position); }
    void                    moveToPreviousPosition(){ _position = _previousPosition; }
    void                    render(SDL_Surface* screen, bool showDebugGraphics = false)
                            {
                                if (_visible) _sprite->render(screen, this->_position);
                                if (showDebugGraphics) _collisionBox->render(screen, 255,0,255);
                            }

protected:
	Vector2D				_position;
    Vector2D                _previousPosition;
	Vector2D				_direction;
	bool					_visible;
    bool					_moving;
	SpriteSheet*			_sprite;
    CollisionBox*           _collisionBox;
};

#endif // ENTITY_H
