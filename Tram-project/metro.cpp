#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"


//On d�finit deux constantes globales:

const double EPSILON = 1e-5; //On utilise cette constante pour les comparaisons car c'est une toute petite valeur


//Constructeur de la classe Metro
Metro::Metro() //On initialise les membres de la classe incluant la position, les vitesses, la capacit� maximale, ...
    : position(0.0), distanceEntreStations(0.0), vitesse(0.0),
    vitesseCroisiere(2.0), vitesseArret(0.0), dureeArret(5.0),
    capaciteMaximale(200), nombrePassagers(0), enArret(false),
    directionPositive(true), facteurVariabilite(static_cast<double>(rand()) / RAND_MAX),
    positionRamePrecedente(0.0), activerDistanceSecurite(false), id(rand()) {
}

//On impl�mente une m�thode pour r�cup�rer l'ID du m�tro
int Metro::getID() const {
    return id;
}

//On cherche � v�rifier si le m�tro est � une station
bool Metro::estALaStation() const {
    return enArret;
}

//Cette m�thode initialise la distance entre les stations pour le m�tro
void Metro::initialiser(double distanceEntreStations) {
    this->distanceEntreStations = distanceEntreStations;
}

//On initialise cette m�thode pour faire partie le m�tro de la station actuelle en acc�l�rant � la vitesse de croisi�re
void Metro::quitterStation() {
    accelerer(vitesseCroisiere);
    enArret = false;
}

//Cette m�thode nous permet de parcourir la ligne de m�tro
void Metro::parcourirLigne(sf::Time elapsedTime, const std::vector<Station>& stations, double positionPrecedente, Superviseur& superviseur)
{
    this->positionRamePrecedente = positionPrecedente; //On initialise la variable 'positionRamePrecedente' avec la position pr�c�dente du m�tro sur la ligne

    double distanceSecurite = 50.0; //On d�finit une distance de s�curit�
    double distanceAvecPrecedente = std::abs(position - positionRamePrecedente); //On calcule la distance entre la position actuelle du m�tro et sa position pr�c�dente.

    if (!enArret) { //Si le m�tro n'est pas actuellement � l'arret 
        double deplacement = (vitesse * elapsedTime.asSeconds()) / 4.0; //On calcume la distance parcourue pendant le temps 'elapsedTime'
        bool doitSArreter = false; //On initialise une variable pour indiquer si le m�tro doit s'arreter

        double prochaineStationPos = getProchaineStationPosition(*this, stations); //On obtient la position de la prochaine station
        double distanceProchaineStation = std::abs(prochaineStationPos - position); //On calcule la distance entre la position actuelle du m�tro et la prochaine station

        //On g�re ici la d�c�l�ration avant d'arriver � une station en ajustant la vitesse en fonction de la distance restante jusqu'� la prochaine station
        if (distanceProchaineStation > 120 && !estEnProcessusDeDeceleration) {
            vitesse = 1.4;
            estEnProcessusDeDeceleration = true;
            if (distanceProchaineStation > 140) {
                vitesse = 0.9;
                if (distanceProchaineStation > 170) {
                    vitesse = 0.5;
                }
            }
        }
        estEnProcessusDeDeceleration = false;

        //Si la distance de s�curit� est activ�e et que la distance avec la position pr�c�dente est inf�rieure � la distance de s�curit� 
        if (activerDistanceSecurite && distanceAvecPrecedente < distanceSecurite) {
            vitesse = std::max(0.0, vitesse - 0.5); //On r�duit la vitesse pour maintenir la distance de s�curit�.
        }

        if (activerDistanceSecurite && distanceAvecPrecedente < distanceSecurite) {
            vitesse = std::max(0.0, vitesse - 0.5);
        }

        //On effectue le d�placement normal du m�tro
        if (directionPositive) { //Le d�placement se fait en fonction de la direction du m�tro (positive ou n�gative)
            position += deplacement;
            if (position >= distanceEntreStations) { //Et on d�termine si le m�tro doit s'arr�ter � la station suivante
                position = distanceEntreStations;
                doitSArreter = true;
            }
        }
        else {
            position -= deplacement;
            if (position <= 0.0) {
                position = 0.0;
                doitSArreter = true;
            }
        }

        //Si on d�termine que le m�tro doit s'arreter et qu'il n'est pas d�ja � l'arret, on apelle alors la fonction 'arreterAStation' pour effectuer l'arret
        if (doitSArreter && !enArret) {
            arreterAStation();
        }
    }
}




//Cette m�thode est initialiser pour faire arreter le m�tro � une station
void Metro::arreterAStation() {
    if (!enArret) { //On v�rifie si le m�tro n'est pas d�ja � l'arret
        vitesse = 0; //On met la vitesse � 0
        enArret = true;     //On indique que le m�tro est � l'arret
        stationClock.restart();    //On red�marre l'horloge de la station pour mesurer la dur�e de l'arret

        //On calcule la dur�e d'arr�t ajust�e en fonction des passagers qui montent et qui descendent
        double tempsParPassager = 0.1;
        double tempsArretAjuste = dureeArret + (passagersEntrants + passagersSortants) * tempsParPassager;

        //On attent le temps d'arr�t qui a �t� ajust� (simulation)
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(tempsArretAjuste * 1000)));

        //On affiche le temps pass� � la station
        std::cout << "Temps pass� � la station : " << tempsArretAjuste << " secondes" << std::endl;
    }
}





//Cette m�thode permet de g�rer les passagers qui montent et qui descendent
void Metro::gererPassagers(int passagersQuai) {

    //On initialise un facteur de variabilit� qui est bas� sur l'ID du m�tro et une valeur al�atoire
    int variabilite = (rand() % 20 + 1) * (id % 15);

    //On calcule le nombre de passagers sortants avec variabilit�
    passagersSortants = std::min(nombrePassagers, rand() % (nombrePassagers + 1) + variabilite / 20);

    //On calcule le de passagers entrants avec variabilit�
    passagersEntrants = std::min(capaciteMaximale - nombrePassagers, passagersQuai) + variabilite / 25;

    //On met � jour le nombre de passagers � bord
    nombrePassagers += passagersEntrants - passagersSortants;

    //On v�rifie que le nombre de passagers reste dans les limites
    if (nombrePassagers > capaciteMaximale) nombrePassagers = capaciteMaximale;
    if (nombrePassagers < 0) nombrePassagers = 0;
}



//M�thode pour l'arret total du m�tro
void Metro::arretTotal() {
    //On met le m�tro totalement � l'arret
    enArret = true;
    vitesse = vitesseArret;
    arretUrgence = true;
}

//M�thode qui permet au m�tro de reprendre apr�s un arr�t d'urgence
void Metro::reprendreApr�sArretUrgence() {
    //On v�rifie si le m�tro est en arret d'urgence
    if (arretUrgence) {
        //On remet en marche le m�tro apr�s l'arret d'urgence
        enArret = false;
        accelerer(vitesseCroisiere);
        arretUrgence = false;
    }
}


//Les m�thodes qui suivent sont impl�ment�s pour nous permettre d'obtenir toutes les informations sur le m�tro et les passagers
int Metro::getPassagersEntrants() const {
    return passagersEntrants;
}

int Metro::getPassagersSortants() const {
    return passagersSortants;
}

double Metro::getVitesse() const {
    return vitesse;
}

double Metro::getPosition() const {
    return position;
}

int Metro::getCapaciteMaximale() const {
    return capaciteMaximale;
}

int Metro::getNombrePassagers() const {
    return nombrePassagers;
}

double Metro::getDureeArret() const {
    return dureeArret;
}


//Cette m�thode nous permet de faire acc�l�rer le m�tro avec une vitesse qui ne d�passe pas la vitesse de croisi�re
void Metro::accelerer(double acceleration) {
    vitesse += acceleration;
    if (vitesse > vitesseCroisiere) {
        vitesse = vitesseCroisiere;
    }
}


//Cette fonction de transition est utilis� dans le d�placement du m�tro
double Metro::fonctionTransition(double t) const {
    return 2 * t * t * t - 3 * t * t + 1;
}
