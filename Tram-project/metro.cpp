#include "metro.hpp"
#include "superviseur.hpp"
#include "station.hpp"

#include "bibliotheque.hpp"


//On définit deux constantes globales:

const double EPSILON = 1e-5; //On utilise cette constante pour les comparaisons car c'est une toute petite valeur


//Constructeur de la classe Metro
Metro::Metro() //On initialise les membres de la classe incluant la position, les vitesses, la capacité maximale, ...
    : position(0.0), distanceEntreStations(0.0), vitesse(0.0),
    vitesseCroisiere(2.0), vitesseArret(0.0), dureeArret(5.0),
    capaciteMaximale(200), nombrePassagers(0), enArret(false),
    directionPositive(true), facteurVariabilite(static_cast<double>(rand()) / RAND_MAX),
    positionRamePrecedente(0.0), activerDistanceSecurite(false), id(rand()) {
}

//On implémente une méthode pour récupérer l'ID du métro
int Metro::getID() const {
    return id;
}

//On cherche à vérifier si le métro est à une station
bool Metro::estALaStation() const {
    return enArret;
}

//Cette méthode initialise la distance entre les stations pour le métro
void Metro::initialiser(double distanceEntreStations) {
    this->distanceEntreStations = distanceEntreStations;
}

//On initialise cette méthode pour faire partie le métro de la station actuelle en accélérant à la vitesse de croisière
void Metro::quitterStation() {
    accelerer(vitesseCroisiere);
    enArret = false;
}

//Cette méthode nous permet de parcourir la ligne de métro
void Metro::parcourirLigne(sf::Time elapsedTime, const std::vector<Station>& stations, double positionPrecedente, Superviseur& superviseur)
{
    this->positionRamePrecedente = positionPrecedente; //On initialise la variable 'positionRamePrecedente' avec la position précédente du métro sur la ligne

    double distanceSecurite = 50.0; //On définit une distance de sécurité
    double distanceAvecPrecedente = std::abs(position - positionRamePrecedente); //On calcule la distance entre la position actuelle du métro et sa position précédente.

    if (!enArret) { //Si le métro n'est pas actuellement à l'arret 
        double deplacement = (vitesse * elapsedTime.asSeconds()) / 4.0; //On calcume la distance parcourue pendant le temps 'elapsedTime'
        bool doitSArreter = false; //On initialise une variable pour indiquer si le métro doit s'arreter

        double prochaineStationPos = getProchaineStationPosition(*this, stations); //On obtient la position de la prochaine station
        double distanceProchaineStation = std::abs(prochaineStationPos - position); //On calcule la distance entre la position actuelle du métro et la prochaine station

        //On gère ici la décélération avant d'arriver à une station en ajustant la vitesse en fonction de la distance restante jusqu'à la prochaine station
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

        //Si la distance de sécurité est activée et que la distance avec la position précédente est inférieure à la distance de sécurité 
        if (activerDistanceSecurite && distanceAvecPrecedente < distanceSecurite) {
            vitesse = std::max(0.0, vitesse - 0.5); //On réduit la vitesse pour maintenir la distance de sécurité.
        }

        if (activerDistanceSecurite && distanceAvecPrecedente < distanceSecurite) {
            vitesse = std::max(0.0, vitesse - 0.5);
        }

        //On effectue le déplacement normal du métro
        if (directionPositive) { //Le déplacement se fait en fonction de la direction du métro (positive ou négative)
            position += deplacement;
            if (position >= distanceEntreStations) { //Et on détermine si le métro doit s'arrêter à la station suivante
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

        //Si on détermine que le métro doit s'arreter et qu'il n'est pas déja à l'arret, on apelle alors la fonction 'arreterAStation' pour effectuer l'arret
        if (doitSArreter && !enArret) {
            arreterAStation();
        }
    }
}




//Cette méthode est initialiser pour faire arreter le métro à une station
void Metro::arreterAStation() {
    if (!enArret) { //On vérifie si le métro n'est pas déja à l'arret
        vitesse = 0; //On met la vitesse à 0
        enArret = true;     //On indique que le métro est à l'arret
        stationClock.restart();    //On redémarre l'horloge de la station pour mesurer la durée de l'arret

        //On calcule la durée d'arrêt ajustée en fonction des passagers qui montent et qui descendent
        double tempsParPassager = 0.1;
        double tempsArretAjuste = dureeArret + (passagersEntrants + passagersSortants) * tempsParPassager;

        //On attent le temps d'arrêt qui a été ajusté (simulation)
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(tempsArretAjuste * 1000)));

        //On affiche le temps passé à la station
        std::cout << "Temps passé à la station : " << tempsArretAjuste << " secondes" << std::endl;
    }
}





//Cette méthode permet de gérer les passagers qui montent et qui descendent
void Metro::gererPassagers(int passagersQuai) {

    //On initialise un facteur de variabilité qui est basé sur l'ID du métro et une valeur aléatoire
    int variabilite = (rand() % 20 + 1) * (id % 15);

    //On calcule le nombre de passagers sortants avec variabilité
    passagersSortants = std::min(nombrePassagers, rand() % (nombrePassagers + 1) + variabilite / 20);

    //On calcule le de passagers entrants avec variabilité
    passagersEntrants = std::min(capaciteMaximale - nombrePassagers, passagersQuai) + variabilite / 25;

    //On met à jour le nombre de passagers à bord
    nombrePassagers += passagersEntrants - passagersSortants;

    //On vérifie que le nombre de passagers reste dans les limites
    if (nombrePassagers > capaciteMaximale) nombrePassagers = capaciteMaximale;
    if (nombrePassagers < 0) nombrePassagers = 0;
}



//Méthode pour l'arret total du métro
void Metro::arretTotal() {
    //On met le métro totalement à l'arret
    enArret = true;
    vitesse = vitesseArret;
    arretUrgence = true;
}

//Méthode qui permet au métro de reprendre après un arrêt d'urgence
void Metro::reprendreAprèsArretUrgence() {
    //On vérifie si le métro est en arret d'urgence
    if (arretUrgence) {
        //On remet en marche le métro après l'arret d'urgence
        enArret = false;
        accelerer(vitesseCroisiere);
        arretUrgence = false;
    }
}


//Les méthodes qui suivent sont implémentés pour nous permettre d'obtenir toutes les informations sur le métro et les passagers
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


//Cette méthode nous permet de faire accélérer le métro avec une vitesse qui ne dépasse pas la vitesse de croisière
void Metro::accelerer(double acceleration) {
    vitesse += acceleration;
    if (vitesse > vitesseCroisiere) {
        vitesse = vitesseCroisiere;
    }
}


//Cette fonction de transition est utilisé dans le déplacement du métro
double Metro::fonctionTransition(double t) const {
    return 2 * t * t * t - 3 * t * t + 1;
}
