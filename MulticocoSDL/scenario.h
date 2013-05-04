#ifndef SCENARIO_H
#define SCENARIO_H

#include <QString>
#include "vector2D.h"

#define SCENARIO_CORRIDOR               0
#define SCENARIO_WALL                   1
#define SCENARIO_ENEMY_RESPAWN_POINT    8
#define SCENARIO_PLAYER_START_POINT     9

class Scenario
{
public:
								Scenario(int w, int h);
								Scenario(QString str);
								~Scenario();

	bool						saveBoard(QString fileName);
	bool**						board(){return _scenario;}
	unsigned int				width(){return _width;}
	unsigned int				height(){return _height;}
	bool						isWall(int x, int y);
	bool						isCorridor(int x, int y);

	void						setEnemySpawningCell(int x, int y);
	void						setPlayerSpawningCell(int x, int y);

	Vector2D					enemySpawningCell();
	Vector2D					playerSpawningCell();

	void						render(int w, int h);

private:
	void						createScenario();

private:
	bool**						_scenario;
	unsigned int				_width;
	unsigned int				_height;
	Vector2D					_enemy_spawning_cell;
	Vector2D					_player_spawning_cell;
};

#endif // SCENARIO_H
