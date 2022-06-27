#include "pch.h"
#include "PlayerMovementLogic.h"
#include "Player.h"
#include "Game.h"

//Class that handles the movment of the player in the climbing game X-Climb

void PlayerMovementLogic::SetNextGlowing(){
	for (int i = 0; i < AnzNachfolger; i++)
	{
		if (glowing[i])
			glowing[i]->SetGlowing(false);
		glowing[i] = nullptr;
	}

	for (int i = 0; i < AnzNachfolger; i++)
	{
		if (nachfolger[i]) {
			glowing[i] = nachfolger[i];
			glowing[i]->SetGlowing(true);
		}

	}
}


void PlayerMovementLogic::Move(Player& pl){

	switch (moveDir)
	{
	case dir::links:
		aktGriff = nachfolger[0];
		cam->MoveTo(nachfolgerVec[0]);
		break;
	case dir::rechts:
		aktGriff = nachfolger[2];
		cam->MoveTo(nachfolgerVec[2]);
		break;
	case dir::mitte:
		aktGriff = nachfolger[1];
		cam->MoveTo(nachfolgerVec[1]);
		break;
	case dir::none:
		return;
	default:
		return;
	}
	if (moveDir != dir::none)
		aktGriff->aktionGreifen(pl);
	moveDir = dir::none;
	SetToGriff();

}


void PlayerMovementLogic::SetToGriff()
{
	moveCoolDown = mcd;

	CalculateNachfolgerPos();
	mp_player->Scale(scale);
	mp_player->RotateDelta(0, 1, 0, -90 * PI / 180);
	mp_player->TranslateDelta(aktGriff->getX(), aktGriff->getY() + playerOffsetUnderHandle, 0);
}


void PlayerMovementLogic::CalculateNachfolgerPos(){
	ResetNachfolger();
	if (aktGriff->getAnzahlNachfolger() > 3)
		throw std::logic_error("Mehr als 3 Nachfolger aktuell Nicht möglich.");
	for (auto i = 0; i < aktGriff->getAnzahlNachfolger(); i++) {
		Griff* nextGriff = aktGriff->getNachfolger(i); //Adjusting to Griffweite
		int iter = 1;
		while (iter < griffWeite) {
			if (nextGriff->getAnzahlNachfolger() != 1)
				break;
			nextGriff = nextGriff->getNachfolger(0);
			iter++;
		}
		if (nextGriff->getX() > aktGriff->getX()) {
			nachfolgerVec[2] = CHVector(nextGriff->getX(), nextGriff->getY(), 0, 0) - CHVector(aktGriff->getX(), aktGriff->getY(), 0, 0);

			nachfolger[2] = nextGriff;
		}
		else if (nextGriff->getX() < aktGriff->getX()) {
			nachfolgerVec[0] = CHVector(nextGriff->getX(), nextGriff->getY(), 0, 0) - CHVector(aktGriff->getX(), aktGriff->getY(), 0, 0);

			nachfolger[0] = nextGriff;
		}
		else {
			nachfolgerVec[1] = CHVector(nextGriff->getX(), nextGriff->getY(), 0, 0) - CHVector(aktGriff->getX(), aktGriff->getY(), 0, 0);


			nachfolger[1] = nextGriff;
		}




		ULInfo("X: %f | Y: %f", nachfolgerVec[i].GetX(), nachfolgerVec[i].GetY());
	}
}


void PlayerMovementLogic::Tick(float fTimeDelta,Player& pl){
	SetNextGlowing();
	moveCoolDown -= fTimeDelta;
	cam->Tick(fTimeDelta);
	if (aktGriff)
		hud.Tick(aktGriff->getY(), fTimeDelta);
	else
		hud.Tick(0, fTimeDelta);

#ifdef testing
	//Move(pl);
#endif

}


PlayerMovementLogic::PlayerMovementLogic(GameCamera* cam, CPlacement& mp_player,HUD& hud)
	: cam(cam), mp_player(&mp_player),hud(hud),moveDir(dir::none){

}


float PlayerMovementLogic::GetPosY(){
		if (aktGriff)return aktGriff->getY(); return 0;
}


float PlayerMovementLogic::GetPosX(){
	if (aktGriff)return aktGriff->getX(); return 0;
}


void PlayerMovementLogic::Init(Griff* start)
{
	aktGriff = start;

	for (int i = 0; i < 3; i++)
	{
		nachfolgerVec[i].SetW(-1);
	}

	SetToGriff();

	cam->SetTo(GetPosX(), GetPosY());

	CalculateNachfolgerPos();
}


void PlayerMovementLogic::FallDown(int amount){
	if (aktGriff) {
		aktGriff = aktGriff->GetNachfolgerX(amount); SetToGriff(); cam->SetTo(aktGriff->getX(), aktGriff->getY());
	}
}


void PlayerMovementLogic::CenterCam(){ 
	cam->SetTo(aktGriff->getX(), aktGriff->getY()); 
}


bool PlayerMovementLogic::SetMove(dir dir)
{


	if (dir == dir::none) { 
		moveDir = dir::none;
		return false; 
	}

	//if (!moveCoolDown > 0){
		if (nachfolger[convertDirToNachfolger(dir)]) {
			moveDir = dir;
			
			return true;
		}
	//}


}
bool PlayerMovementLogic::CheckDirection(CHVector vecMin, CHVector vecMax, float fTimeDelta)
{

#ifdef SensorTesting
	lastMovment[0] += fTimeDelta;
	lastMovment[1] += fTimeDelta;
	lastMovment[2] += fTimeDelta;

	for (int i = 0; i < AnzNachfolger; i++)
	{
		//Bewegung nach Rechts Oben
		if (nachfolgerVec[i].GetW() > -0.5)
		{
			CHVector dirVec = nachfolgerVec[i];
			dirVec.Norm();
			//Ckeck for a Negative Y Peak
			if (vecMin.GetY() < 0.5 && yPeaks[0] > 0.3) {
				yPeaks[0] = fTimeDelta;
			}
			//Ckeck for a Positive Y Peak
			if (vecMax.GetY() > 1.3 && yPeaks[1] > 0.3) {
				yPeaks[1] = fTimeDelta;
			}
			//Ckeck for a Positive X Peak
			if (vecMin.GetX() < -0.2 && xPeaks[0] > 0.3) {
				xPeaks[0] = fTimeDelta;
			}
			//Ckeck for a Negative X Peak
			if (vecMax.GetX() > 0.2 && xPeaks[1] > 0.3) {
				xPeaks[1] = fTimeDelta;
			}

		}
		else {

		}
	}

	int moveDir = 0;

	if (yPeaks[0] > yPeaks[1] && yPeaks[0] - yPeaks[1] < 0.5f) {
		//Bewegung von Oben nach Unten
		yPeaks[0] = 1;
		yPeaks[1] = 2;
		CGame::SensY.PrintF("Oben nach Unten");

		if (lastMovment[0] < 1 && lastMovment[1] < 1 && lastMovment[2] >  1) {
			lastMovment[0] += 1;
			lastMovment[1] += 1;
			lastMovment[2] += 1;
			return SetMove(dir::links);
		}

		else if (lastMovment[1] < 1 && lastMovment[2] < 1 && lastMovment[1] < 1) {
			lastMovment[0] += 1;
			lastMovment[1] += 1;
			lastMovment[2] += 1;
			return SetMove(dir::rechts);
		}
		else if (lastMovment[1] < 1) {
			lastMovment[0] += 1;
			lastMovment[1] += 1;
			lastMovment[2] += 1;
			return SetMove(dir::mitte);
		}
		return true;
	}
	else if (yPeaks[0] < yPeaks[1] && yPeaks[1] - yPeaks[0] < 0.5f) {
		//Bewegung von Unten nach Oben
		yPeaks[0] = 1;
		yPeaks[1] = 2;
		lastMovment[1] = fTimeDelta;
		CGame::SensY.PrintF("Unten nach Oben");
		return true;
	}

	if (xPeaks[0] > xPeaks[1] && xPeaks[0] - xPeaks[1] < 0.5f) {
		//Links Nach Rechts
		xPeaks[0] = 1;
		xPeaks[1] = 2;
		CGame::SensZ.PrintF("Links Nach Rechts");
		lastMovment[2] = fTimeDelta;
		if (lastMovment[1] < 1 && lastMovment[0] < 1) {
			return SetMove(dir::links);
		}
		return true;
	}
	else if (xPeaks[0] < xPeaks[1] && xPeaks[1] - xPeaks[0] < 0.5f) {
		//Rechts nach Links
		xPeaks[0] = 1;
		xPeaks[1] = 2;
		lastMovment[0] = fTimeDelta;
		CGame::SensZ.PrintF("Rechts nach Links");


		if (lastMovment[1] < 1 && lastMovment[2] < 1) {
			lastMovment[0] += 1;
			lastMovment[1] += 1;
			lastMovment[2] += 1;
			return SetMove(dir::rechts);
		}

		return true;
	}

	for (int i = 0; i < 2; i++) {
		xPeaks[i] += fTimeDelta;
		yPeaks[i] += fTimeDelta;

	}
#endif // SensorTesting

	return false;
}



int PlayerMovementLogic::convertDirToNachfolger(dir direction) {
	if (direction == dir::none) throw std::logic_error("die Methode convertDirToNachfolger(dir) darf nur mit Richtungen ungleich dir::none aufgerufen werden!");

	switch (direction) {
	case dir::links: return 0;
		break;
	case dir::mitte: return 1;
		break;
	case dir::rechts: return 2;
		break;
	case dir::none: return -1;
		break;
	default: return -1;
	}
}



bool PlayerMovementLogic::isOffsetOne() {
	switch (moveDir) {
	case dir::links:
		if (nachfolgerVec[0].x > -maximalerAbstandZwischenGriffenFürOffsetOne) return true; //wenn vektor zum nachfolger kleineren Abstand als 2 hat
		else return false;
		
	case dir::mitte:
		return true;

	case dir::rechts:
		if (nachfolgerVec[2].x < maximalerAbstandZwischenGriffenFürOffsetOne) return true;
		else return false; 
		
	case dir::none:
		break;
	default:
		break;
	}
}