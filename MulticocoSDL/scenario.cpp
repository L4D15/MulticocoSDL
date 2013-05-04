#include "scenario.h"
#include "labyrithGenerator.h"

#include <QFile>
#include <QTextStream>
#include <QVector>

/**
 * @brief		Constructor del escenario que crea un laberinto dividido en el
 *				numero de celdas indicado. El tamaño  en pixeles de cada celda
 *				se ajusta a la resolución de representación automáticamente.
 *				Si se crea el escenario mediante este constructor hace falta
 *				indicar manualmente los puntos de aparición tanto del jugador
 *				como de los enemigos mediante setEnemyRespawningPoint(x,y) y
 *				setPlayerRespawningPoint(x,y).
 *
 * @param w		Numero de casillas de ancho.
 * @param h		Numero de casillas de alto.
 */
Scenario::Scenario(int w, int h)
{
	_width = w;
	_height = h;
	createScenario();
}

/**
 * @brief		Crea el laberinto usando un fichero de texto externo.
 *				Este fichero de texto debe contener la estructura del laberinto
 *				indicando en la primera linea el tamaño del laberinto y en las
 *				siguientes lineas la estructura del laberinto indicando para cada
 *				celda del mismo el tipo de entre los siguientes.
 *				0 = Pasillo, se puede recorrer sin problemas.
 *				1 = Pared, no se puede atravesar.
 *				8 = Punto de creación de los enemigos. Se puede atravesar.
 *				9 = Punto de creación del jugador. Se puede atravesar.
 * @param file	Ruta donde se encuentra el fichero con la estructura del laberinto.
 */
Scenario::Scenario(QString str)
{
	QFile file(str);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::exception();
    }
	QVector<QByteArray> vector;
	while (!file.atEnd()) {
			QByteArray line = file.readLine();
			vector.append(line);
		}
	file.close();

    // Determinamos el tamaño del escenario
	_height = vector.size();
	_width = vector[0].size();
	_scenario = new bool*[_height];

    // Inicilizamos el escenario
	for(unsigned int i=0; i<_height; i++)
		_scenario[i] = new bool[_width];

    // Construimos el escenario mediante la información del fichero
	for(unsigned int i=0; _height; i++) {
		QByteArray* array = &vector[i];
        for(unsigned int j=0; j<_height; j++) {
            int cell = QString(array->at(j)).toInt();
            switch(cell){
            case SCENARIO_WALL:
                // No se puede pasar, pared
                _scenario[i][j] = WALL;
				break;
            case SCENARIO_CORRIDOR:
                // Pasillo
                _scenario[i][j] = CORRIDOR;
				break;
            case SCENARIO_PLAYER_START_POINT:
                // Punto de creación del jugador
                _scenario[i][j] = CORRIDOR;
				setPlayerSpawningCell(i,j);
				break;
            case SCENARIO_ENEMY_RESPAWN_POINT:
                // Punto de creación de enemigos
                _scenario[i][j] = CORRIDOR;
				setEnemySpawningCell(i,j);
				break;
			default:
				//carácter incorrecto.
				throw std::exception();
			}
		}
	}
}

/**
 * @brief Scenario::~Scenario
 */
Scenario::~Scenario()
{
    for(unsigned int i=0; i<_height; i++)
            delete[] _scenario[i];
    delete[] _scenario;
}

bool Scenario::saveBoard(QString fileName)
{
    QFile file(fileName);
    if(file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&file);
        for(unsigned int i=0; i<_height; i++){
            for(unsigned int j=0; j<_width; j++){
                char toWrite = _scenario[i][j] ? '1' : '0';
                out << toWrite;
            }
            out << '\n';
        }
        file.close();
        return true;
    }
    return false;
}

/**
 * @brief		Indica al escenario desde qué celda se generaran los enemigos.
 *				Si el escenario ha sido cargado desde archivo este método no
 *				es necesario a menos que se quiera indicar una celda distinta
 *				a la indicada en el archivo.
 * @param x		Posición X de la celda.
 * @param y		Posición Y de la celda.
 */
void Scenario::setEnemySpawningCell(int x, int y)
{
	_enemy_spawning_cell.setX(x);
	_enemy_spawning_cell.setY(y);
}

/**
 * @brief		Indica al escenario desde qué celda aparece el jugador.
 *				Si el escenario ha sido cargado desde archivo este método no
 *				es necesario a menos que se quiera indicar una celda distinta
 *				a la indicada en el archivo.
 * @param x		Posición X de la celda.
 * @param y		Posición Y de la celda.
 */
void Scenario::setPlayerSpawningCell(int x, int y)
{
	_player_spawning_cell.setX(x);
	_player_spawning_cell.setY(y);
}

/**
 * @brief		Posicion en la que se generan los enemigos.
 * @return		Vector de dos dimensiones con la posicion X e Y de la casilla
 *				donde se generan los enemigos. Estas posiciones son relativas a
 *				la matriz que representa el escenario, no son coordenadas cartesianas
 *				del plano de representación.
 */
Vector2D Scenario::enemySpawningCell()
{
	return _enemy_spawning_cell;
}

/**
 * @brief		Posición en la que aparece el jugador.
 * @return		Vector de dos dimensiones con la posicion X e Y de la casilla
 *				donde aparece el jugador. Estas posiciones son relativas a
 *				la matriz que representa el escenario, no son coordenadas cartesianas
 *				del plano de representación.
 */
Vector2D Scenario::playerSpawningCell()
{
	return _player_spawning_cell;
}

/**
 * @brief		Muestra el escenario por pantalla. Para ello necesita del tamaño
 *				del display para calcular la cantidad de pixeles que le corresponde
 *				a cada celda del escenario de ancho y de alto.
 * @param w		Ancho del display en el que se mostrará el escenario.
 * @param h		Alto del display en el que se motrará el escenario.
 */
void Scenario::render(int w, int h)
{
	int cell_width = (int)(w / _width);	//TODO: Posiblemente haga falta ponerlo como atributo
	int cell_height = (int)(h / _height);
	//TODO: Ladis
}

//----------------------------------------------------------------------------//

/**
 * @brief		Genera un escenario aleatorio para el juego, creando pasillos
 *				ciclicos e indicando las posiciones de generación de enemigos y
 *				de aparición del jugador.
 */
void Scenario::createScenario()
{
    // ¿De verdad hace falta crear otra clase para generar el escenario?
    // Ya estamos en la clase Scenario, podría haberse creado mediante métodos
    // privados en esta misma clase, sin necesidad de otra.
	LabyrithGenerator lg(_height, _width);
	_scenario = lg.getGeneratedLabyrith();
	setEnemySpawningCell(_width/2, _height/2);
	setPlayerSpawningCell(1, 1);
}

/**
 * @brief
 * @param x
 * @param y
 * @return
 */
bool Scenario::isWall(int x, int y)
{
    return _scenario[x][y] == WALL;
}

/**
 * @brief
 * @param x
 * @param y
 * @return
 */
bool Scenario::isCorridor(int x, int y)
{
    return _scenario[x][y] == CORRIDOR;
}
