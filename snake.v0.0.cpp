// Версия игры Snake: Eat or Die 0.0(Test)
// на базе библиотеки SFML

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

#define Width_field 733  //Ширина поля
#define Height_field 500 //Высота поля

const int width = Width_field;   //Ширина поля
const int height = Height_field; //Высота поля 

bool First_do = false;
bool addition = false; //Доворот змейки в повороте 
bool Menu = false;
enum Rotation {UP = 0, DOWN, LEFT, RIGHT};
short Closed_u_turn = DOWN;


	float Snake_position_x = (width - 54) / 2,  //Стартовые координаты главного героя по ширине
	      Snake_position_y = (height - 54) / 2; //Стартовые координаты главного героя по высоте
	float SpeedX = 0,
	      SpeedY = 0;
	short texture_snake_width, texture_snake_height, texture_snake_position = 0; //Выбор текстуры главного героя при повороте


	float New_end_tail_x, New_end_tail_y,
		   Old_end_tail_x, Old_end_tail_y,
		   Position_tail_x[271], Position_tail_y[271];
	short texture_end_tail_width, texture_end_tail_height, texture_end_tail_position = 0;//Выбор текстуры хвоста при повороте


	int Food_x = rand() % width;  //Стартовые координаты еды по ширине
	int Food_y = rand() % height; //Стартовые координаты еды по высоте


	struct direction {
		short Snake_direction[4][2] = {
		{ 0 , 0 },
		{ 54 , 0 },
		{ 0 , 54 },
		{ 54 , 54 } };

		short End_tail_direction[4][2] = {
		{ 0 , 0 },
		{ 54 , 0 },
		{ 0 , 54 },
		{ 54 , 54 } };
	};

int main() {
	setlocale(LC_ALL, "rus");

		RenderWindow window(VideoMode(width, height), "Snake: Eat or Die!");
		//Прорисовка текстур

		Texture Menu_window;
		Menu_window.loadFromFile("./Textures.0.0/start.png");
		Sprite Menu_game(Menu_window);

		Texture Textures_Background; //Текстура фона
		Textures_Background.loadFromFile("./Textures.0.0/Wall.png");
		Sprite Background_body(Textures_Background);

		Texture Textures_Snake; //Текстура змейки
		Textures_Snake.loadFromFile("./Textures.0.0/TexturesSnake.png");
		Sprite Snake_body(Textures_Snake);

		Texture Tail;
		Tail.loadFromFile("./Textures.0.0/TexturesTail.png");
		Sprite End_tail(Tail);

		Texture Food;
		Food.loadFromFile("./Textures.0.0/Apple.png");
		Sprite Apple(Food);

		//Конец прорисовки текстур

		while (window.isOpen()) {

		  //Управление игрой

			Event event;
			while (window.pollEvent(event)) {

				if (event.type == Event::Closed) {
					window.close();
				}

				if (event.type == Event::KeyPressed)
					if (Keyboard::isKeyPressed(Keyboard::Enter)) { Menu = true; }
					else if (event.key.code == Keyboard::Up    && Closed_u_turn != UP)   { Closed_u_turn = DOWN; SpeedY = -0.20; SpeedX = 0; texture_snake_position = UP;   texture_end_tail_position = UP;   addition = true;}
					else if (event.key.code == Keyboard::Down  && Closed_u_turn != DOWN) { Closed_u_turn = UP;   SpeedY =  0.20; SpeedX = 0; texture_snake_position = DOWN; texture_end_tail_position = DOWN; addition = true;}
					else if (event.key.code == Keyboard::Left  && Closed_u_turn != LEFT) { Closed_u_turn = RIGHT;SpeedX = -0.20; SpeedY = 0; texture_snake_position = LEFT; texture_end_tail_position = LEFT; addition = true;}
					else if (event.key.code == Keyboard::Right && Closed_u_turn != RIGHT){ Closed_u_turn = LEFT; SpeedX =  0.20; SpeedY = 0; texture_snake_position = RIGHT;texture_end_tail_position = RIGHT;addition = true;}
			}
			//Конец управления игрой

			//Логика игры

			direction direction;

			texture_snake_width = direction.Snake_direction[texture_snake_position][0]; //Выбор текстуры направления
			texture_snake_height = direction.Snake_direction[texture_snake_position][1];
			Snake_body.setTextureRect(IntRect(texture_snake_width, texture_snake_height, 54, 54)); //Границы текстуры змейки на
			                                                                                       //изображении текстур
			Snake_position_x += SpeedX;
			Snake_position_y += SpeedY;

			Snake_body.setPosition(Snake_position_x, Snake_position_y);

			//if (Snake_position_x < 0 - 54) Snake_position_x = width;  //Перенос героя на обратную сторону по ширине
			//else if (Snake_position_x > width) Snake_position_x = 0 - 54;
			//if (Snake_position_y < 0 - 54) Snake_position_y = height; //Перенос героя на обратную сторону по высоте
			//else if (Snake_position_y > height) Snake_position_y = 0 - 54;

			if (Snake_position_x <= 0 || Snake_position_x >= width - 54) {
				SpeedX = 0;
				cout << "GAME OVER!";
				return 0;
			} //Границы движения по полю по ширине
			if (Snake_position_y <= 0 || Snake_position_y >= height - 54){
				SpeedY = 0;
				cout << "GAME OVER!";
				return 0;
			} //Границы движения по полю по высоте
			//SpeedX = 0; //Позволяет делать движения с остоновкой


			 texture_end_tail_width = direction.End_tail_direction[texture_end_tail_position][0];
			 texture_end_tail_height = direction.End_tail_direction[texture_end_tail_position][1];
			 End_tail.setTextureRect(IntRect(texture_end_tail_width, texture_end_tail_height, 54, 54));


			 if (First_do == false) {
				 Position_tail_x[0] = Snake_position_x;
				 Position_tail_y[0] = Snake_position_y + 54;
			 }
			 else if (First_do == true) {
				 Position_tail_x[0] = Snake_position_x;
				 Position_tail_y[0] = Snake_position_y;
			 }
			 New_end_tail_x = Snake_position_x;
			 New_end_tail_y = Snake_position_y;

			 for (short i = 0; i <= 270; ++i) { // 270 * 0.20 = 54 | количество записей смещения героя:
			                                    // 54 / 0.20 = 270 (размер героя / расстояние его шага = количество шагов
			                                    // для смещения на свой размер)

				 if (addition == true) {
					 New_end_tail_x = Snake_position_x;
					 New_end_tail_y = Snake_position_y;
					 First_do = true;
				 }
				 Old_end_tail_x = Position_tail_x[i];
				 Old_end_tail_y = Position_tail_y[i];
				 Position_tail_x[i] = New_end_tail_x;
				 Position_tail_y[i] = New_end_tail_y;
				 New_end_tail_x = Old_end_tail_x;
				 New_end_tail_y = Old_end_tail_y;

				 if (i == 270)addition = false; //180 - 2/3, либо 270  200 | 210
			 }

			 for (short i = 0; i <= 270; ++i) {
				 End_tail.setPosition(Position_tail_x[i], Position_tail_y[i]);
			 }


			 Apple.setTextureRect(IntRect(0, 0, 26, 26));
			 Apple.setPosition((float)Food_x,(float)Food_y);

			 for (short i = 0; i < 100; ++i) {
				 if ((Snake_position_x >= Food_x - 54 && Snake_position_x <= Food_x + 27) && //Границы возможности съесть объект
				     (Snake_position_y >= Food_y - 54 && Snake_position_y <= Food_y + 27)) {
					 Food_x = rand() % (width - 26);
					 Food_y = rand() % (height - 26);}
			 }

			 //Конец логики игры

			 //Вывод на экран

			window.draw(Menu_game);

			if (Menu == true) {
				window.draw(Background_body);

				window.draw(Apple);

				window.draw(End_tail);

				window.draw(Snake_body);

			}
			window.display();

		}

	return 0;
}
