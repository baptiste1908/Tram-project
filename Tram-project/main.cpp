#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

class Rame {
private:
    int numero;
    double position;
    double vitesse;
    const double vitesseCroisiere = 30.0;
    const int capaciteMaximale = 300;  // Capacité maximale de la rame
    int nombrePassagers;

public:
    Rame(int numero) : numero(numero), position(0.0), vitesse(0.0), nombrePassagers(0) {}

    void quitterStation() {
        accelerer(5.0);
        std::cout << "La rame " << numero << " quitte la station. Nouvelle vitesse : " << vitesse << std::endl;
    }

    void parcourirLigne() {
        maintenirVitesseCroisiere();
        std::cout << "La rame " << numero << " parcourt la ligne a vitesse de croisiere : " << vitesse << std::endl;
    }

    void arreterAStation() {
        decelerer(5.0);
        std::cout << "La rame " << numero << " ralentit en approchant de la station : " << vitesse << std::endl;
        vitesse = 0.0;
        std::cout << "La rame " << numero << " est a l'arret a la station." << std::endl;
        std::cout << "Capacite maximale de la rame : " << capaciteMaximale << std::endl;
        std::cout << "Disponibilite dans la rame : " << getDisponibilite() << std::endl;
        std::cout << std::endl;
    }

    void gererPassagers(int passagersQuai) {
        int passagersSortent = rand() % (nombrePassagers + 1);

        if (passagersSortent > nombrePassagers) {
            passagersSortent = nombrePassagers;
        }

        nombrePassagers -= passagersSortent;

        int passagersEntrent = std::min(capaciteMaximale - nombrePassagers, passagersQuai);

        nombrePassagers += passagersEntrent;

        std::cout << "Passagers entrant : " << passagersEntrent << ", Passagers sortant : " << passagersSortent << std::endl;
        std::cout << "La rame " << numero << " - Nombre de passagers a bord : " << nombrePassagers << std::endl;
    }

    double getPosition() const {
        return position;
    }

    int getCapaciteMaximale() const {
        return capaciteMaximale;
    }

    int getDisponibilite() const {
        return capaciteMaximale - nombrePassagers;
    }

    double getVitesse() const {
        return vitesse;
    }

private:
    void accelerer(double acceleration) {
        vitesse += acceleration;
    }

    void decelerer(double deceleration) {
        vitesse -= deceleration;
    }

    void maintenirVitesseCroisiere() {
        vitesse = vitesseCroisiere;
    }
};

class Station {
public:
    std::string nom;

    Station(const std::string& nom) : nom(nom) {}

    int gererRame() {
        return rand() % 160; // Retourne un nombre aléatoire de passagers sur le quai
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::vector<Station> stations = {
        Station("Saint-Philibert"),
        Station("Bourg"),
        Station("Maison des Enfants"),
        Station("Mitterie"),
        Station("Pont Supérieur"),
        Station("Lomme - Lambersart - Arthur-Notebart"),
        Station("Canteleu"),
        Station("Bois Blancs"),
        Station("Port de Lille"),
        Station("Cormontaigne"),
        Station("Montebello"),
        Station("Porte des Postes"),
        Station("Porte d'Arras"),
        Station("Porte de Douai"),
        Station("Porte de Valenciennes"),
        Station("Lille Grand Palais"),
        Station("Mairie de Lille"),
        Station("Gare Lille Flandres")
    };

    Rame rame(1);

    // Fenêtre SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // Police pour le texte

    sf::Font font;
	if (!font.loadFromFile("C:/Program Files/SFML/font/arial.ttf")) {
        std::cerr << "Erreur lors du chargement de la police" << std::endl;
        return EXIT_FAILURE;
    }


    // Texte pour afficher les informations de la rame
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setPosition(10, 10);

    // La rame effectue un trajet entre les stations pendant 10 itérations
    for (size_t i = 0; i < stations.size(); ++i) {
        int passagersQuai = stations[i].gererRame();
        std::cout << "Station " << stations[i].nom << " - Passagers sur le quai : " << passagersQuai << std::endl;

        rame.gererPassagers(passagersQuai);
        rame.quitterStation();
        rame.parcourirLigne();
        rame.arreterAStation();

        // Mettez à jour le texte avec les informations de la rame
        text.setString(
            "\nPosition : " + std::to_string(rame.getPosition()) +
            "\nVitesse : " + std::to_string(rame.getVitesse()) +
            "\nCapacite maximale : " + std::to_string(rame.getCapaciteMaximale()) +
            "\nDisponibilite : " + std::to_string(rame.getDisponibilite()));

        // Effacez le contenu de la fenêtre
        window.clear();

        // Dessinez le texte sur la fenêtre
        window.draw(text);

        // Affichez le contenu de la fenêtre
        window.display();

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
