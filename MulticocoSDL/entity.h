#ifndef ENTITY_H
#define ENTITY_H

#include "vector2d.h"
#include "sprite.h"
#include <SDL.h>

class Entity
{
public:
							Entity(){}
	virtual					~Entity(){}

	void					setPosition(float x, float y){ _position.setX(x);_position.setY(y); }
	void					setPosition(Vector2D& v){ _position = v; }
	void					setDirection(float x, float y){ _direction.setX(x);_direction.setY(y); }
	void					setDirection(Vector2D& v){ _direction = v; }
    inline void				setVisible(bool v){ _visible = v; }
	inline void				setMoving(bool m){ _moving = m; }
	void					setSpriteSheet(const char* file, int w, int h, int* animations, int nAnimations)
                                        { _sprite = new SpriteSheet(file,w,h,animations,nAnimations); }
    SpriteSheet&            spriteSheet(){ return *_sprite; }
    void                    setAnimation(const char* name){ _sprite->setAnimation(name); }

	inline bool				isVisible(){ return _visible; }
	inline bool				isMoving(){ return _moving; }

	inline Vector2D&		position(){ return _position; }
	inline Vector2D&		direction(){ return _direction; }

    virtual inline void		update(){ _sprite->nextFrame(); }

	void					move(){ _position = _position + _direction; }
    void                    render(SDL_Surface* screen){ if (_visible) _sprite->render(screen, this->_position); }

protected:
	Vector2D				_position;
	Vector2D				_direction;
	bool					_visible;
    bool					_moving;
	SpriteSheet*			_sprite;
};

#endif // ENTITY_H
