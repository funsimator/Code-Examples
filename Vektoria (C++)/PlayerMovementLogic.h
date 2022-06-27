#pragma once
#define AnzNachfolger 3
class Griff;
class GameCamera;
class Level;
enum class dir;
class HUD;
class Player;

class PlayerMovementLogic
{
private:
	int maximalerAbstandZwischenGriffenF�rOffsetOne = 2;
	// Index f�r den Linken, Mittleren und Rechten Griff in dieser Reihenfolge
	Griff* nachfolger[AnzNachfolger]{ nullptr };


	   //Enth�lt die Richtungsvektoren der aktuellen Nachfolger von Spielerposition aus. Sollte nur aufgerufen werden, wenn nachfolger != nullptr
	   CHVector nachfolgerVec[AnzNachfolger]{};


	//Pointer auf den Griff an dem der Spieler grade h�ngt 
	Griff* aktGriff;

	//Anzahl an Griffen, die der Spieler bei einem regul�ren Move() weiterbewegt wird
	int griffWeite = 1;


	//Hud des Players wird ben�tigt f�r die Progress-Bar Updates
	HUD& hud;


	//Enth�lt alle aktuell leuchtenden Griffe. Diese m�ssen beim weiterbewegen wieder auf die regul�re Griffe gesetzt werden.
	Griff* glowing[AnzNachfolger]{}; 


	//Speichert die Zeit seit dem ein Sensor einen Peak in X Richtung aufgenommen hat. [0] ist ein negativer Peak, [1] ein Positiver. Damit wird festgestellt ob sich der Sensor von Links nach Rechts oder von Rechts nach Links bewegt hat.
	float xPeaks[2]{};


	//Speichert die Zeit seit dem ein Sensor einen Peak in Y Richtung aufgenommen hat. [0] ist ein negativer Peak, [1] ein Positiver. Damit wird festgestellt ob sich der Sensor von Oben nach Unten oder von Unten nach Oben bewegt hat.
	float yPeaks[2]{};


	//Speichert die Zeit seit dem eine Armbewegung (also die richtige zeitliche Abfolge in xPeaks oder yPeaks vorliegt) erfolgt ist. [0]=Arm nach links, [1]=Arm nach Oben,[2]=Arm nach Rechts
	float lastMovment[3]{};


	//Setzt die Griffe in nachfolger[] auf leuchtend
	void SetNextGlowing();


	//Executes the Move Command during Tick()
	void Move(Player& pl);


	//Hier wird in SetMove() die Richtung gesetzt, die dann in Tick ausgef�hrt wird durch Move()
	dir moveDir;


	//Falls Wert > 0 kann sich der Spieler nicht bewegen. 
	float moveCoolDown = 0.f;

	//Der Cooldown f�r einen regul�ren Move()
	float mcd = 0.0f; //Eigentlich 0.5


	//Schreibt die entsprechenden Richtungsvektoren f�r die nachfolger[] in nachfolgerVec[]
	void CalculateNachfolgerPos();


	//Setzt all nachfolger[] auf nullptr
	void ResetNachfolger() {
		for (int i = 0; i < 3; i++) {
			nachfolger[i] = nullptr; nachfolgerVec[i].SetW(-1);
		}
	}

//konvertiert eine richtung aus dem enum dir in ein int, mit dem dann das entsprechende array angesprochen werden kann
//darf nur mit directions != dir::none aufgerufen werden
	int convertDirToNachfolger(dir direction);
	


	//Nur hier solange es noch keine vollst�ndige Kletter Animation gibt
	CPlacement* mp_player;
	float scale = 0.8;




public:
	const float playerOffsetUnderHandle = -3.0;

	PlayerMovementLogic(GameCamera* cam, CPlacement& mp_player, HUD& hud);
	
	//Die Kamera muss Updates f�r die neue Position des Spielers bekommen
	GameCamera* cam;

	GameCamera* getCam() { return cam; }
	//mp_player nur tempor�r enthalten
	//PlayerMovementLogic(GameCamera& cam, CPlacement* mp_player, HUD& hud);


	//Wird von Player.Init() aufgerufen um den Start Griff zu setzen
	void Init(Griff* start);


	//Richtungsvergleich f�r den Sensoren Input
	bool CheckDirection(CHVector vecMin, CHVector vecMax, float fTimeDelta);


	//Gibt die aktuell gesetzte Bewegunsrichtung aus. 
	dir getDir() { return moveDir; }


	//wird einmal pro frame aufgerufen
	void Tick(float fTimeDelta,Player& pl);

	
	//Setzt den Spieler an aktGriff 
	void SetToGriff();


	//Gibt den movecooldown aus. Falls dieser < 0 kann sich der Spieler bewegen. Bei > 0 gibt movecooldown die zeit in Sekunden an, bis er sich wieder bewegen kann 
	float GetCoolDown() { return moveCoolDown; }


	//Setzt Griffweite auf den gew�nschten Wert. Die Griffweite legt fest, wieviel Griffe der Spieler bei einem Standard Move() vorankommt.
	void SetGriffWeite(unsigned int anz) { griffWeite = anz; CalculateNachfolgerPos(); }




	/// <summary>
/// L�sst den Spieler um eine Anzahl an Griffen nach unten fallen
/// </summary>
/// <param name="amount">Anzahl an Griffen, die der Spieler zur�ckgesetzt wird</param>
	void FallDown(int amount);


	/// <summary>
/// Setzt falls m�glich die Bewegungsrichtung, die dann w�hrend Tick() durch Move() ausgef�hrt wird
/// </summary>
/// <param name="dir">Richtung, in die bewegt werden soll</param>
/// <returns>True falls bewegung m�glich, False falls kein Nachfolger in diser Richtung vorhanden ist.</returns>
	bool SetMove(dir dir);


	//Setzt die Kamera genau auf die Position des Aktuellen Griffs
	void CenterCam();


	/// <summary>
	/// Gibt die X Koordinate des Placment von aktGriff
	/// </summary>
	/// <returns>Y Koord</returns>
	float GetPosX();


	/// <summary>
/// Gibt die Y Koordinate des Placment von aktGriff
/// </summary>
/// <returns>Y Koord</returns>
	float GetPosY();

	//gibt den aktuellen Griff aus
	Griff* getAktGriff() { return aktGriff; }

	//gibt den NachfolgerGridd aus
	Griff** getNachfolger() { return nachfolger; }

	//gibt den NachfolgerVector aus
	CHVector* getNachfolgerVec() { return nachfolgerVec; }


	//Den Wert auf den der Cooldown nach einem Move(), setzen
	void set_mcd(float m) { mcd = m; }

	void setAktGriff(Griff* newAktGriff) {
		this->aktGriff = newAktGriff;
	}

	bool isOffsetOne();
};

