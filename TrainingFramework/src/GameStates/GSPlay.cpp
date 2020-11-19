#include "GSPlay.h"
#include "Shaders.h"
#include "Texture.h"
#include "Models.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite2D.h"
#include "Sprite3D.h"
#include "Text.h"
#include "iostream"
#include "SpriteAnimation.h"
using namespace std;
extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine
int GSPlay::GetLevel() {
	return this->level;
}
GSPlay::GSPlay()
{
}


GSPlay::~GSPlay()
{

}


void GSPlay::Init()
{
	SetObstacles(level);	//Ban đầu là level 1
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("lv1");
	t = 0;	//thời gian chuyển động của quả bóng
	point = 0;	//ban đầu, số su ăn đc = 0
	//BackGround
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_BackGround->SetSize(screenWidth, screenHeight);

	//nút back
	texture = ResourceManagers::GetInstance()->GetTexture("button_back");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth-75, 30);
	button->SetSize(150, 50);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->PopState();
		ResourceManagers::GetInstance()->PlaySound("introduction", true);
	});
	m_listButton.push_back(button);

	//drag drop button
	shader = ResourceManagers::GetInstance()->GetShader("Animation");
	texture = ResourceManagers::GetInstance()->GetTexture("drag");
	std::shared_ptr<SpriteAnimation> objDrag = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1);
	objDrag->Set2DPosition(50, 400);
	objDrag->SetSize(80, 80);
	buttonDragDrop.push_back(objDrag);	//drag là 0

	texture = ResourceManagers::GetInstance()->GetTexture("drop");
	std::shared_ptr<SpriteAnimation> objDrop = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1);
	objDrop->Set2DPosition(350, 100);
	objDrop->SetSize(80, 80);
	buttonDragDrop.push_back(objDrop);	//drop là 1
	//drag drop button

	//Vận tốc và góc ban đầu
	Get_v0_and_alpha();
	cout << "v0 = " << v0<<" alpha = "<<alpha * 180 / Pi<<"\n";
	//Vận tốc và góc ban đầu

	//score
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_score = std::make_shared< Text>(shader, font, "Score: 0", TEXT_COLOR::RED, 1.0);	//so diem
	m_score->Set2DPosition(Vector2(5, 25));

	//level
	m_level = std::make_shared< Text>(shader, font, "Level: 1", TEXT_COLOR::YELLOW, 1.0);	//level
	m_level->Set2DPosition(Vector2(225, 25));

	//coins total quantity
	m_coins = std::make_shared< Text>(shader, font, "x: 0", TEXT_COLOR::RED, 1.0);	//so xu da an duoc
	m_coins->Set2DPosition(screenWidth - 225, 37);

	// Animation đồng xu
	shader = ResourceManagers::GetInstance()->GetShader("Animation");
	texture = ResourceManagers::GetInstance()->GetTexture("coin");
	std::shared_ptr<SpriteAnimation> objCoin = std::make_shared<SpriteAnimation>(model, shader, texture, 18, 1.0f/30);
	objCoin->Set2DPosition(screenWidth - 240, 30);
	objCoin->SetSize(22, 22);
	m_listSpriteAnimations.push_back(objCoin);
	
	//3 đồng xu di chuyển
	//Đồng xu thứ 0;
	std::shared_ptr<SpriteAnimation> objCoin1 = std::make_shared<SpriteAnimation>(model, shader, texture, 18, 1.0f / 30);
	objCoin1->Set2DPosition(400, 50);
	objCoin1->SetSize(52, 52);
	m_3Coins.push_back(objCoin1);
	//Đồng xu thứ 1;
	std::shared_ptr<SpriteAnimation> objCoin2 = std::make_shared<SpriteAnimation>(model, shader, texture, 18, 1.0f / 30);
	objCoin2->Set2DPosition(700, 50);
	objCoin2->SetSize(52, 52);
	m_3Coins.push_back(objCoin2);
	//Đồng xu thứ 2;
	std::shared_ptr<SpriteAnimation> objCoin3 = std::make_shared<SpriteAnimation>(model, shader, texture, 18, 1.0f / 30);
	objCoin3->Set2DPosition(1000, 50);
	objCoin3->SetSize(52, 52);
	m_3Coins.push_back(objCoin3);
	//3 đồng xu di chuyển

	//Bóng
	shader = ResourceManagers::GetInstance()->GetShader("Animation");
	texture = ResourceManagers::GetInstance()->GetTexture("ball");
	std::shared_ptr<SpriteAnimation> objBall = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1.0);
	objBall->Set2DPosition(30, screenHeight * 0.8);
	objBall->SetSize(50, 50);
	m_listSpriteAnimations.push_back(objBall);
	x0 = (objBall->Get2DPosition()).x*1.0;	//30, screenHeight * 0.8
	y0 = (objBall->Get2DPosition()).y*1.0;	//30, screenHeight * 0.8

	//Mục tiêu
	shader = ResourceManagers::GetInstance()->GetShader("Animation");
	texture = ResourceManagers::GetInstance()->GetTexture("Grass");
	std::shared_ptr<SpriteAnimation> objTarget = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1.0);
	objTarget->Set2DPosition(1410, screenHeight * 0.8);
	objTarget->SetSize(30, 80);
	m_listSpriteAnimations.push_back(objTarget);

	//base đỡ bóng
	texture = ResourceManagers::GetInstance()->GetTexture("Rock");
	std::shared_ptr<SpriteAnimation> objRock = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1.0);
	objRock->Set2DPosition(30, screenHeight * 0.8 + 100);
	objRock->SetSize(50, 150);
	m_listSpriteAnimations.push_back(objRock);

	ResourceManagers::GetInstance()->PlaySound("level" + to_string(level) , true);	//true là có lặp lại
}

void GSPlay::Exit()
{

}


void GSPlay::Pause()
{

}

void GSPlay::Resume()
{

}


void GSPlay::HandleEvents()
{

}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{
	static int MINPOINT = 30;
	static int delta_pixel = 10;
	double momentY = (buttonDragDrop[0]->Get2DPosition()).y;
	double momentX = (buttonDragDrop[0]->Get2DPosition()).x;
	//
	if (key == KEY_UP && bIsPressed == true && isShoted == false) {
		//0: đồng xu
		//1: nhân vật
		//2: Mục tiêu
		if (momentY > 100) {	//tung độ ko đc thấp hơn 100
			buttonDragDrop[0]->Set2DPosition(momentX, momentY - delta_pixel);
			//x0 = (buttonDragDrop[0]->Get2DPosition()).x*1.0;
			//y0 = (buttonDragDrop[0]->Get2DPosition()).y*1.0;
			Get_v0_and_alpha();
		}
	}
	else if (key == KEY_DOWN && bIsPressed == true && isShoted == false) {
		//0: đồng xu
		//1: nhân vật
		//2: Mục tiêu
		if (momentY < 400) {	//Tung độ ko đc nhiều hơn 400
			buttonDragDrop[0]->Set2DPosition(momentX, momentY + delta_pixel);
			//x0 = (buttonDragDrop[0]->Get2DPosition()).x*1.0;
			//y0 = (buttonDragDrop[0]->Get2DPosition()).y*1.0;
			Get_v0_and_alpha();
		}
	}
	else if (key == KEY_LEFT && bIsPressed == true && isShoted == false) {
		//0: đồng xu
		//1: nhân vật
		//2: Mục tiêu
		if (momentX > 50) {	//hoành độ ko đc thấp hơn 50
			buttonDragDrop[0]->Set2DPosition(momentX - delta_pixel, momentY);
			//x0 = (buttonDragDrop[0]->Get2DPosition()).x*1.0;
			//y0 = (buttonDragDrop[0]->Get2DPosition()).y*1.0;
			Get_v0_and_alpha();
		}
	}
	else if (key == KEY_RIGHT && bIsPressed == true && isShoted == false) {
		//0: đồng xu
		//1: nhân vật
		//2: Mục tiêu
		if (momentX < 350) {	//hoành độ ko đc nhiều hơn 350
			buttonDragDrop[0]->Set2DPosition(momentX + delta_pixel, momentY);
			//x0 = (buttonDragDrop[0]->Get2DPosition()).x*1.0;
			//y0 = (buttonDragDrop[0]->Get2DPosition()).y*1.0;
			Get_v0_and_alpha();
		}
	}

	else if (key == VK_SPACE && bIsPressed == true && isShoted == false) {
		Get_v0_and_alpha();
		isShoted = 1;
		//cout << "isShoted = " << isShoted << "\n";
	}
	//
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)	//ấn nút
{
	for (auto it : m_listButton)
	{
		(it)->HandleTouchEvents(x, y, bIsPressed);
		if ((it)->IsHandle()) break;
	}

	if (bIsPressed == true && x >= screenWidth - 150 && x <= screenWidth && y >= 5 && y <= 55) {
		ResourceManagers::GetInstance()->PauseSound("level" + to_string(level));	//ấn nút back thì dừng nhạc
	}
}

void GSPlay::Update(float deltaTime)	//hoạt ảnh chuyển động
{
	for (auto SpriteAnimations : m_listSpriteAnimations)
	{
		SpriteAnimations->Update(deltaTime);
	}

	for (auto _3Coins : m_3Coins)
	{
		_3Coins->Update(deltaTime);
	}

	for (auto obstacle : obstacles)
	{
		obstacle->Update(deltaTime);
	}
	ObstaclesTwirl(deltaTime);	//Chướng ngại vật quay đều
	Dropping(deltaTime);
//		EvenlyStraightMovement(deltaTime);
		SteadilyFastMovement(deltaTime);
//		ConditioningOscillation(deltaTime);
	Coin1Moving(deltaTime);
	Coin2Moving(deltaTime);
	Coin3Moving(deltaTime);
	//Kiểm tra xem chạm đích chưa
	double x1 = m_listSpriteAnimations[1]->Get2DPosition().x;
	double y1 = m_listSpriteAnimations[1]->Get2DPosition().y;
	double x2 = m_listSpriteAnimations[2]->Get2DPosition().x;
	double y2 = m_listSpriteAnimations[2]->Get2DPosition().y;
	//cout << "Distance = " << Distance(x1, y1, x2, y2)<< "\n";
	if (Distance(x1, y1, x2, y2) < 100) {	//Nếu chạm đích
		//text game title
		ResourceManagers::GetInstance()->PlaySound("victory", false);
		ResourceManagers::GetInstance()->PauseSound("level" + to_string(level));

		//Sleep(1000);
		static int point = 0;

		auto shader = ResourceManagers::GetInstance()->GetShader("TextShader");
		std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
		point += 1000; level++;
		ResourceManagers::GetInstance()->PlaySound("level" + to_string(level), true);
		m_score = std::make_shared< Text>(shader, font, "Score: "+std::to_string(point) , TEXT_COLOR::RED, 1.0);	//so diem
		m_score->Set2DPosition(Vector2(5, 25));

		m_level = std::make_shared< Text>(shader, font, "Level: " + std::to_string(level), TEXT_COLOR::YELLOW, 1.0);	//so diem
		m_level->Set2DPosition(Vector2(225, 25));

		RecoverState();

		hasBeenEattenCoin1 = 0;
		hasBeenEattenCoin2 = 0;
		hasBeenEattenCoin3 = 0;
		CheckPlayerEatCoin1();
		CheckPlayerEatCoin2();
		CheckPlayerEatCoin3();
			//chạm đích thì tăng level
		//obstacles.clear();	//Xóa hết chướng ngại vật cũ
		SetObstacles(level);
		tObstacle = 0;

		auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
		auto texture = ResourceManagers::GetInstance()->GetTexture("lv" + to_string((level % 5) + 1));
		
		//BackGround
		shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
		m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
		m_BackGround->Set2DPosition(screenWidth / 2, screenHeight / 2);
		m_BackGround->SetSize(screenWidth, screenHeight);
	}
	//Kiểm tra xem chạm đích chưa

	//Kiểm tra xem đã ăn đồng xu 1 chưa
	x2 = m_3Coins[0]->Get2DPosition().x;
	y2 = m_3Coins[0]->Get2DPosition().y;
	if (Distance(x1, y1, x2, y2) < 100 && hasBeenEattenCoin1 == false) {	//Nếu ăn xu 1
		ResourceManagers::GetInstance()->PlaySound("eat_coin", false);
		auto shader = ResourceManagers::GetInstance()->GetShader("TextShader");
		std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
		point += 1;
		m_coins = std::make_shared< Text>(shader, font, "x: "+to_string(point), TEXT_COLOR::RED, 1.0);	//so xu da an duoc
		m_coins->Set2DPosition(screenWidth - 225, 37);
		
		hasBeenEattenCoin1 = 1;	//Đánh dấu đồng xu 1 đã bị ăn
		CheckPlayerEatCoin1();
	}
	//Kiểm tra xem đã ăn đồng xu 1 chưa

	//Kiểm tra xem đã ăn đồng xu 2 chưa
	x2 = m_3Coins[1]->Get2DPosition().x;
	y2 = m_3Coins[1]->Get2DPosition().y;
	if (Distance(x1, y1, x2, y2) < 100 && hasBeenEattenCoin2 == false) {	//Nếu ăn xu 2
		ResourceManagers::GetInstance()->PlaySound("eat_coin", false);
		auto shader = ResourceManagers::GetInstance()->GetShader("TextShader");
		std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
		point += 1;
		m_coins = std::make_shared< Text>(shader, font, "x: " + to_string(point), TEXT_COLOR::RED, 1.0);	//so xu da an duoc
		m_coins->Set2DPosition(screenWidth - 225, 37);

		hasBeenEattenCoin2 = 1;	//Đánh dấu đồng xu 1 đã bị ăn
		CheckPlayerEatCoin2();
	}
	//Kiểm tra xem đã ăn đồng xu 2 chưa

	//Kiểm tra xem đã ăn đồng xu 3 chưa
	x2 = m_3Coins[2]->Get2DPosition().x;
	y2 = m_3Coins[2]->Get2DPosition().y;
	if (Distance(x1, y1, x2, y2) < 100 && hasBeenEattenCoin3 == false) {	//Nếu ăn xu 3
		ResourceManagers::GetInstance()->PlaySound("eat_coin", false);
		auto shader = ResourceManagers::GetInstance()->GetShader("TextShader");
		std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
		point += 1;
		m_coins = std::make_shared< Text>(shader, font, "x: " + to_string(point), TEXT_COLOR::RED, 1.0);	//so xu da an duoc
		m_coins->Set2DPosition(screenWidth - 225, 37);

		hasBeenEattenCoin3 = 1;	//Đánh dấu đồng xu 1 đã bị ăn
		CheckPlayerEatCoin3();
	}
	//Kiểm tra xem đã ăn đồng xu 3 chưa
}

void GSPlay::Draw()	//render lên màn hình
{
	m_BackGround->Draw();
	m_score->Draw();
	m_coins->Draw();
	m_level->Draw();
	for (auto it : m_listButton)
	{
		it->Draw();
	}
	for (auto it : buttonDragDrop)
	{
		it->Draw();
	}

	for (auto SpriteAnimations : m_listSpriteAnimations)
	{
		SpriteAnimations->Draw();
	}

	for (auto _3Coins : m_3Coins)
	{
		_3Coins->Draw();
	}

	for (auto it : obstacles) {
		it->Draw();
	}
}

void GSPlay::SetNewPostionForBullet()
{
}

double GSPlay::Distance(double x1, double y1, double x2, double y2) {
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

void GSPlay::Dropping(double deltaTime) {	//Bóng chuyển động
	if (isShoted == 1) {
		//Tọa độ ban đầu

		double x = v0 * cos(alpha) * t + x0;
		double y = ((g * t * t) / 2) - v0 * (sin(alpha)) * t + y0;
		double vx = v0 * cos(alpha);
		double vy = g * t - v0 * sin(alpha);
		if (atan(vy / vx) * 180 / Pi > 0) cout << "Peak! ";
		cout << "t = " << t << " x = " << x << " y = " << y << " vx = "<<vx<<" vy = "<<vy<<" angle = "<< atan(vy / vx)* 180 / Pi <<"\n";
		m_listSpriteAnimations[1]->Set2DPosition(x, y);
		//Phương trình Chuyển động của đạn
		t = t + 7 * deltaTime;


		if (y - 0.0000000000001 > screenHeight) {	//Vật chạm đất
			//Tính thời gian bóng chạm đất
			cout << "Because y > screenHeight, which means y = " << y << " > screenHeight = " << screenHeight << "\n";
			double tToPeak = (v0 * sin(alpha)) / g;
			double tFromPeakToGround = sqrt((2 / g) * (screenHeight - y0 + (v0 * v0 * sin(alpha) * sin(alpha)) / (2 * g)));
			double tToGround = tToPeak + tFromPeakToGround;
			cout << "x = " << v0 * cos(alpha) * tToGround + x0 << " y = "<< ((g * tToGround * tToGround) / 2) - v0 * (sin(alpha)) * tToGround + y0 << "\n";
			cout << "tToPeak = " << tToPeak << " tFromPeakToGround = " << tFromPeakToGround << " tToGround = " << tToGround << " vSum = " << sqrt(vx*vx + vy*vy) << " vGround = " << sqrt(2 * g*(screenHeight - y0 + (v0 * v0 * sin(alpha) * sin(alpha)) / (2 * g))) << "\n";
			//cout << "vGround = " << sqrt(2 * g*(screenHeight - y0 + (v0 * v0 * sin(alpha) * sin(alpha)) / (2 * g))) << "\n";
			//Bóng nảy lên khi rơi xuống đất
			x0 = v0 * cos(alpha) * tToGround + x0;
			y0 = ((g * tToGround * tToGround) / 2) - (v0 * (sin(alpha)) * tToGround) + y0;
			alpha = atan(vy / vx);	//hệ số góc của tiếp tuyến của quỹ đạo tại điểm bóng chạm đất
			v0 = sqrt(vx*vx + vy*vy);
			cout << "x0 = " << x0 << " y0 = " << y0 << " alpha = " << alpha * 180 / Pi << " v0 = " << v0 << " hmax = "<< screenHeight - y0 + (v0 * v0 * sin(alpha) * sin(alpha)) / (2 * g) <<"\n";
			t = 0;
		}

		if (x > screenWidth) {	//vật chạm 
			ResourceManagers::GetInstance()->PlaySound("out_screen", false);
			Sleep(3000);
			RecoverState();
		}

		for (int i = 0; i < obstacles.size(); i++) {	//chướng ngại vật chuyển động tròn đều
			double x1 = obstacles[i]->Get2DPosition().x;
			double y1 = obstacles[i]->Get2DPosition().y;
			if (Distance(x1, y1, x, y) < 30) {
				ResourceManagers::GetInstance()->PlaySound("out_screen",false);
				Sleep(3000);
				RecoverState();
			}
		}
	}
}

void GSPlay::EvenlyStraightMovement(double deltaTime) //mục tiêu chuyển động thẳng đều
{
	double y = y0Target + vTarget * tTarget;	//y = y0 + vt
	//cout << "y0Target = " << y0Target << " vTarget = " << vTarget << " y = " << y << "\n";

	tTarget += 7 * deltaTime;
	m_listSpriteAnimations[2]->Set2DPosition(1410, y);
	if (y > screenHeight) {	//mục tiêu quay ngược trở lại
		double xTarget = m_listSpriteAnimations[2]->Get2DPosition().x;
		m_listSpriteAnimations[2]->Set2DPosition(xTarget, y);	//bắn ra ngoài thì set lại vị trí ban đầu trước khi bắn
		tTarget = 0;	//thời gian tính lại từ đầu
		y0Target = screenHeight;	//
		vTarget = -vTarget;
	} if (y < 0) {	//mục tiêu quay ngược trở lại
		double xTarget = m_listSpriteAnimations[2]->Get2DPosition().x;
		m_listSpriteAnimations[2]->Set2DPosition(xTarget, y);	//bắn ra ngoài thì set lại vị trí ban đầu trước khi bắn
		tTarget = 0;	//thời gian tính lại từ đầu
		y0Target = 0;
		vTarget = -vTarget;
	}
};
void GSPlay::SteadilyFastMovement(double deltaTime)	//mục tiêu chuyển động nhanh dần đều
{
	double y = y0Target + v0Target * tTarget + 0.5 * aTarget * tTarget * tTarget;	//y = y0 + v0t + 1/2 a t^2
	vTarget = v0Target + aTarget * tTarget;	//v = dy/dt = v0 + at;
	//cout <<"tTarget = "<<tTarget<<" y0Target = " << y0Target << " v0Target = " << v0Target << " y = " << y << " vTarget = " << vTarget <<" aTarget = "<<aTarget<< "\n";
	tTarget += 7 * deltaTime;
	m_listSpriteAnimations[2]->Set2DPosition(1410, y);
	if (y > screenHeight) {	//nảy lên
		double xTarget = m_listSpriteAnimations[2]->Get2DPosition().x;
		m_listSpriteAnimations[2]->Set2DPosition(xTarget, y);	//bắn ra ngoài thì set lại vị trí ban đầu trước khi bắn
		tTarget = 0;	//thời gian tính lại từ đầu
		y0Target = screenHeight;	//
		v0Target = -sqrt(2 * aTarget * screenHeight);
	} 
	else if (y < 0) {	//rơi xuống
		double xTarget = m_listSpriteAnimations[2]->Get2DPosition().x;
		m_listSpriteAnimations[2]->Set2DPosition(xTarget, y);	//bắn ra ngoài thì set lại vị trí ban đầu trước khi bắn
		tTarget = 0;	//thời gian tính lại từ đầu
		y0Target = 0;
		v0Target = 0;
	}
};
void GSPlay::ConditioningOscillation(double deltaTime) //mục tiêu dao động điều hòa
{
	double x = ATarget * cos(wTarget * tTarget + phi) + 400;	//x = A cos(wt + phi)
	vTarget = ATarget * wTarget * cos(wTarget * tTarget + phi + Pi/2);	//v = Aw cos(wt + phi + pi/2)
	tTarget +=  7 * deltaTime;
	//cout << "x = " << x << " vTarget = "<<vTarget<<" tTarget = "<<tTarget<<"\n";
	m_listSpriteAnimations[2]->Set2DPosition(1410, x);
};

void GSPlay::Coin1Moving(double deltaTime) {	//Đồng xu 1 di chuyển
	double y = y0Coin1 + vCoin1 * tCoin1;	//y = y0 + vt

	tCoin1 += 7 * deltaTime;
	if(hasBeenEattenCoin1 == 0) m_3Coins[0]->Set2DPosition(400, y);
	if (y > screenHeight) {	//mục tiêu quay ngược trở lại
		double xCoin1 = m_3Coins[0]->Get2DPosition().x;
		m_3Coins[0]->Set2DPosition(xCoin1, y);	//bắn ra ngoài thì set lại vị trí ban đầu trước khi bắn
		tCoin1 = 0;	//thời gian tính lại từ đầu
		y0Coin1 = screenHeight;	//
		vCoin1 = -vCoin1;
	} if (y < 0) {	//mục tiêu quay ngược trở lại
		double xCoin1 = m_3Coins[0]->Get2DPosition().x;
		m_3Coins[0]->Set2DPosition(xCoin1, y);	//bắn ra ngoài thì set lại vị trí ban đầu trước khi bắn
		tCoin1 = 0;	//thời gian tính lại từ đầu
		y0Coin1 = 0;
		vCoin1 = -vCoin1;
	}
}

void GSPlay::Coin2Moving(double deltaTime) {	//Đồng xu 2 di chuyển
	double y = y0Coin2 + vCoin2 * tCoin2;	//y = y0 + vt
	//cout << "vCoin2 = " << vCoin2 << "\n";
	tCoin2 += 7 * deltaTime;
	if (hasBeenEattenCoin2 == 0) m_3Coins[1]->Set2DPosition(700, y);
	if (y > screenHeight) {	//mục tiêu quay ngược trở lại
		double xCoin2 = m_3Coins[1]->Get2DPosition().x;
		m_3Coins[1]->Set2DPosition(xCoin2, y);	//bắn ra ngoài thì set lại vị trí ban đầu trước khi bắn
		tCoin2 = 0;	//thời gian tính lại từ đầu
		y0Coin2 = screenHeight;	//
		vCoin2 = -vCoin2;
	} if (y < 0) {	//mục tiêu quay ngược trở lại
		double xCoin2 = m_3Coins[1]->Get2DPosition().x;
		m_3Coins[1]->Set2DPosition(xCoin2, y);	//bắn ra ngoài thì set lại vị trí ban đầu trước khi bắn
		tCoin2 = 0;	//thời gian tính lại từ đầu
		y0Coin2 = 0;
		vCoin2 = -vCoin2;
	}
}

void GSPlay::Coin3Moving(double deltaTime) {	//Đồng xu 3 di chuyển
	double y = y0Coin3 + vCoin3 * tCoin3;	//y = y0 + vt
	//cout << "vCoin3 = " << vCoin3 << "\n";
	tCoin3 += 7 * deltaTime;
	if (hasBeenEattenCoin3 == 0) m_3Coins[2]->Set2DPosition(1000, y);
	if (y > screenHeight) {	//mục tiêu quay ngược trở lại
		double xCoin3 = m_3Coins[2]->Get2DPosition().x;
		m_3Coins[2]->Set2DPosition(xCoin3, y);	//bắn ra ngoài thì set lại vị trí ban đầu trước khi bắn
		tCoin3 = 0;	//thời gian tính lại từ đầu
		y0Coin3 = screenHeight;	//
		vCoin3 = -vCoin3;
	} if (y < 0) {	//mục tiêu quay ngược trở lại
		double xCoin3 = m_3Coins[2]->Get2DPosition().x;
		m_3Coins[2]->Set2DPosition(xCoin3, y);	//bắn ra ngoài thì set lại vị trí ban đầu trước khi bắn
		tCoin3 = 0;	//thời gian tính lại từ đầu
		y0Coin3 = 0;
		vCoin3 = -vCoin3;
	}
}

//1
void GSPlay::CheckPlayerEatCoin1() {
	if (hasBeenEattenCoin1 == 1) {
		vCoin1 = 0;
		tCoin1 = 0;
		m_3Coins[0]->Set2DPosition(screenWidth - 300, 30);	//Set lại vị trí của đồng xu
	}
	else if (hasBeenEattenCoin1 == 0) {
		vCoin1 = 10;
		tCoin1 = 0;
		m_3Coins[0]->Set2DPosition(400, 50);	//Set lại vị trí của đồng xu
	}
}
	//2
void GSPlay::CheckPlayerEatCoin2() {
	if (hasBeenEattenCoin2 == 1) {
		vCoin2 = 0;
		tCoin2 = 0;
		m_3Coins[1]->Set2DPosition(screenWidth - 350, 30);	//Set lại vị trí của đồng xu
	}
	else if (hasBeenEattenCoin2 == 0) {
		vCoin2 = 20;
		tCoin2 = 0;
		m_3Coins[1]->Set2DPosition(700, 50);	//Set lại vị trí của đồng xu
	}
}
	//3
void GSPlay::CheckPlayerEatCoin3() {
	if (hasBeenEattenCoin3 == 1) {
		vCoin3 = 0;
		tCoin3 = 0;
		m_3Coins[2]->Set2DPosition(screenWidth - 400, 30);	//Set lại vị trí của đồng xu
	}
	else if (hasBeenEattenCoin3 == 0) {
		vCoin3 = 40;
		tCoin3 = 0;
		m_3Coins[2]->Set2DPosition(1000, 50);	//Set lại vị trí của đồng xu
	}
}

void GSPlay::RecoverState() {
	x0 = 30;	//chạm tường hay chạm đất thì quay về vị trí ban đầu
	y0 = screenHeight * 0.8;	//chạm tường hay chạm đất thì quay về vị trí ban đầu
	m_listSpriteAnimations[1]->Set2DPosition(x0, y0);	//bắn ra ngoài thì set lại vị trí ban đầu trước khi bắn
	isShoted = 0;
	t = 0;
	//v0 = 100;
	//alpha = Pi / 2.5;
	
	
}

void GSPlay::Get_v0_and_alpha() {
	std::shared_ptr<SpriteAnimation> objDrag = buttonDragDrop[0];
	std::shared_ptr<SpriteAnimation> objDrop = buttonDragDrop[1];
	v0 = 0.65*sqrt(
		((objDrag->Get2DPosition().x) - (objDrop->Get2DPosition().x)) * ((objDrag->Get2DPosition().x) - (objDrop->Get2DPosition().x))
		+
		((objDrag->Get2DPosition().y) - (objDrop->Get2DPosition().y)) * ((objDrag->Get2DPosition().y) - (objDrop->Get2DPosition().y))
	);
	alpha = abs(atan(
		((objDrag->Get2DPosition().y) - (objDrop->Get2DPosition().y))
		/
		((objDrag->Get2DPosition().x) - (objDrop->Get2DPosition().x))
	));
};



//
void GSPlay::SetObstacles(int level) {	//Khởi tạo chướng ngại vật
	if (level == 1) {	//level 1
		auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
		auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
		auto texture = ResourceManagers::GetInstance()->GetTexture("fireball");
		std::shared_ptr<SpriteAnimation> fireball = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1);
		fireball->Set2DPosition(screenWidth * 4 / 5, screenHeight / 2);
		fireball->SetSize(40,40);
		obstacles.push_back(fireball);	//level 2
	} else if(level == 2) {
		auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
		auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
		auto texture = ResourceManagers::GetInstance()->GetTexture("fireball");
		std::shared_ptr<SpriteAnimation> fireball = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1);
		fireball->Set2DPosition(screenWidth * 4 / 5 - 40, screenHeight / 2);
		fireball->SetSize(40, 40);
		obstacles.push_back(fireball);
	}
	else if (level == 3) {	//level 3
		auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
		auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
		auto texture = ResourceManagers::GetInstance()->GetTexture("fireball");
		std::shared_ptr<SpriteAnimation> fireball1 = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1);
		fireball1->Set2DPosition(screenWidth * 4 / 5 - 80, screenHeight / 2);
		fireball1->SetSize(40, 40);
		obstacles.push_back(fireball1);

		std::shared_ptr<SpriteAnimation> fireball2 = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1);
		fireball2->Set2DPosition(screenWidth * 4 / 5 - 120, screenHeight / 2);
		fireball2->SetSize(40, 40);
		obstacles.push_back(fireball2);
	}
	else if (level == 5) {	//level 5	//vòng lửa dài 6
		obstacles[0]->Set2DPosition(screenWidth * 4 / 5, screenHeight / 2);
		auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
		auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
		auto texture = ResourceManagers::GetInstance()->GetTexture("fireball");
		std::shared_ptr<SpriteAnimation> fireball1 = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1);
		fireball1->Set2DPosition(screenWidth * 4 / 5 - 160, screenHeight / 2);
		fireball1->SetSize(40, 40);
		obstacles.push_back(fireball1);

		std::shared_ptr<SpriteAnimation> fireball2 = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1);
		fireball2->Set2DPosition(screenWidth * 4 / 5 - 200, screenHeight / 2);
		fireball2->SetSize(40, 40);
		obstacles.push_back(fireball2);

	}
	else if (level == 6) {	//level 6	//vòng lửa dài 9
		obstacles[0]->Set2DPosition(screenWidth * 4 / 5, screenHeight / 2);
		auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
		auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
		auto texture = ResourceManagers::GetInstance()->GetTexture("fireball");
		std::shared_ptr<SpriteAnimation> fireball1 = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1);
		fireball1->Set2DPosition(screenWidth * 4 / 5 - 240, screenHeight / 2);
		fireball1->SetSize(40, 40);
		obstacles.push_back(fireball1);

		std::shared_ptr<SpriteAnimation> fireball2 = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1);
		fireball2->Set2DPosition(screenWidth * 4 / 5 - 280, screenHeight / 2);
		fireball2->SetSize(40, 40);
		obstacles.push_back(fireball2);

		std::shared_ptr<SpriteAnimation> fireball3 = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1);
		fireball3->Set2DPosition(screenWidth * 4 / 5 - 320, screenHeight / 2);
		fireball3->SetSize(40, 40);
		obstacles.push_back(fireball3);
	}
	else if (level == 8) {	//level 8	//vòng lửa dài 12
		obstacles[0]->Set2DPosition(screenWidth * 3 / 5, screenHeight / 2);
		auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
		auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
		auto texture = ResourceManagers::GetInstance()->GetTexture("fireball");
		std::shared_ptr<SpriteAnimation> fireball1 = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1);
		fireball1->Set2DPosition(screenWidth * 3 / 5 - 360, screenHeight / 2);
		fireball1->SetSize(40, 40);
		obstacles.push_back(fireball1);

		std::shared_ptr<SpriteAnimation> fireball2 = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1);
		fireball2->Set2DPosition(screenWidth * 3 / 5 - 400, screenHeight / 2);
		fireball2->SetSize(40, 40);
		obstacles.push_back(fireball2);

		std::shared_ptr<SpriteAnimation> fireball3 = std::make_shared<SpriteAnimation>(model, shader, texture, 1, 1);
		fireball3->Set2DPosition(screenWidth * 3 / 5 - 440, screenHeight / 2);
		fireball3->SetSize(40, 40);
		obstacles.push_back(fireball3);
	}

	else if (level == 9) {
		wObstacle *= 1.5;
	}
}

void GSPlay::ObstaclesTwirl(double deltaTime) {	//Chướng ngại vật quay vòng
	double centerX = obstacles[0]->Get2DPosition().x;
	double centerY = obstacles[0]->Get2DPosition().y;
	if (!(level == 4 || level == 7 || level >= 10 || level == 5 || level == 2)) {
		obstacles[0]->Set2DPosition(70 * cos(wObstacle * tObstacle + phi) + screenWidth * 4 / 5, 70 * sin(wObstacle * tObstacle + phi) + screenHeight / 2);
	}
	for (int i = 1; i < obstacles.size();i++) {	//chướng ngại vật chuyển động tròn đều
		double radius = 40 * i;//bán kính quay
		obstacles[i]->Set2DPosition(radius*cos(wObstacle * tObstacle + phi) + centerX, radius*sin(wObstacle * tObstacle + phi) + centerY);
	}
	tObstacle += 7*deltaTime;
	cout <<"centerX = "<<centerX<<" centerY = "<<centerY<< " tObstaclesTwirl = " << tObstacle << "\n";
	if (level == 4 || level == 7 || level >= 10 || level == 5 || level == 2) {
		obstacles[0]->Set2DPosition(centerX, vObstacle*tObstacle);	//tâm di chuyển lên xuống
		cout << "vObstacle = " << vObstacle << " tObstacle = " << tObstacle << "\n";
		//quay ngược trở lại
		if (centerY > screenHeight) {	//mục tiêu quay ngược trở lại
			centerY = screenHeight;
			obstacles[0]->Set2DPosition(centerX, centerY);
			vObstacle = -vObstacle;
			tObstacle = 0;
		} if (centerY < 0) {	//mục tiêu quay ngược trở lại
			centerY = 0;
			obstacles[0]->Set2DPosition(centerX, centerY);
			vObstacle = -vObstacle;
			tObstacle = 0;
		}
	}
}

