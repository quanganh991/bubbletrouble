#pragma once
#include "gamestatebase.h"
#include "GameButton.h"
#include "SpriteAnimation.h"
#include <math.h>
#define Pi 3.141592653589793
#include <windows.h>
class Sprite2D;
class Sprite3D;
class Text;

class GSPlay :
	public GameStateBase
{
public:
	GSPlay();
	~GSPlay();

	void Init();
	void Exit();

	void Pause();
	void Resume();

	void HandleEvents();
	void HandleKeyEvents(int key, bool bIsPressed);

	void HandleTouchEvents(int x, int y, bool bIsPressed);
	void Update(float deltaTime);
	void Draw();
	double Distance(double x1, double y1, double x2, double y2);
	
	void SetNewPostionForBullet();
	void Dropping(double deltaTime);
	void EvenlyStraightMovement(double deltaTime);	//mục tiêu chuyển động thẳng đều
	void SteadilyFastMovement(double deltaTime);	//mục tiêu chuyển động nhanh dần đều
	void ConditioningOscillation(double deltaTime);	//mục tiêu dao động điều hòa
													//Đồng xu
	void Coin1Moving(double deltaTime);
	void Coin2Moving(double deltaTime);
	void Coin3Moving(double deltaTime);
	void CheckPlayerEatCoin1();
	void CheckPlayerEatCoin2();
	void CheckPlayerEatCoin3();
	void RecoverState();
	void Get_v0_and_alpha();
	void SetObstacles(int level);
	void ObstaclesTwirl(double deltaTime);
	int GetLevel();
private:

	std::shared_ptr<Sprite2D> m_BackGround;
	std::shared_ptr<Text>  m_score;
	std::shared_ptr<Text>  m_coins;
	std::shared_ptr<Text>  m_level;
	std::vector<std::shared_ptr<GameButton>>	m_listButton;
	std::vector<std::shared_ptr<SpriteAnimation>> m_listSpriteAnimations;
	std::vector<std::shared_ptr<SpriteAnimation>> buttonDragDrop;
	std::vector<std::shared_ptr<SpriteAnimation>> m_3Coins;	//Chứa 3 đồng xu
	std::vector<std::shared_ptr<SpriteAnimation>> obstacles;//Chứa chướng ngại vật
	double x0, y0;
	int point = 0;
	double t = 0;
	int isShoted = 0;
	double v0 = 100; //vận tốc ban đầu, m/s
	double alpha = Pi / 2.5;	//góc bắn ban đầu, radian
	double g = 9.8;	//Gia tốc trọng trường, m/s^2

	double y0Target = 0;
	double vTarget = 30;	//chuyển động thẳng đều thì vTarget luôn ko đổi
	double tTarget = 0;
	double v0Target = 0;	//vận tốc ban đầu của chuyển động thẳng đều
	double aTarget = 20;		//vận tốc ban đầu của chuyển động thẳng đều

	double ATarget = 400;	//Biên độ dao động điều hòa
	double wTarget = Pi/8;	//Tần số góc
	double phi = -Pi / 4;	//Pha ban đầu

	double vCoin1 = 10; double vCoin2 = 20; double vCoin3 = 40;	//Vận tốc của 3 đồng xu
	double tCoin1 = 0; double tCoin2 = 0; double tCoin3 = 0;	//thời gian di chuyển của 3 đồng xu
	double y0Coin1 = 50; double y0Coin2 = 50; double y0Coin3 = 50;	//tọa độ ban đầu của 3 đồng xu
	int hasBeenEattenCoin1 = 0; int hasBeenEattenCoin2 = 0; int hasBeenEattenCoin3 = 0;
	int level = 1;
	double tObstacle = 0;	//thời gian quay của chướng ngại vật
	double wObstacle = Pi / 3;
	double vObstacle = 35;	//tốc độ di chuyển của chướng ngại vật
};