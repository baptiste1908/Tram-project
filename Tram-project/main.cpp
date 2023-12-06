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
    int passagersEntrent;
    int passagersSortent;

public:
    Rame(int numero) : numero(numero), position(0.0), vitesse(0.0), nombrePassagers(0), passagersEntrent(0), passagersSortent(0) {}

    void quitterStation() {
        accelerer(5.0);
        std::cout << "La rame " << numero << " quitte la station. Nouvelle vitesse : " << vitesse << std::endl;
    }

    void parcourirLigne() {
        maintenirVitesseCroisiere();
        position += vitesse;  // Mettez à jour la position en fonction de la vitesse
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
        passagersSortent = rand() % (nombrePassagers + 1);

        if (passagersSortent > nombrePassagers) {
            passagersSortent = nombrePassagers;
        }

        nombrePassagers -= passagersSortent;

        passagersEntrent = std::min(capaciteMaximale - nombrePassagers, passagersQuai);

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

    int getPassagersEntrent() const {
        return passagersEntrent;
    }
    int getPassagersSortent() const {
        return passagersSortent;
    }

    int getNombrePassagers() const {
        return nombrePassagers;
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
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Erreur lors du chargement de la police" << std::endl; // Pour voir si on a une erreur lors du chargement
        return EXIT_FAILURE;
    }

    // Texte pour afficher les informations de la rame
    sf::Text positionText, vitesseText, capaciteText, disponibiliteText, passagersText;
    positionText.setFont(font);
    vitesseText.setFont(font);
    capaciteText.setFont(font);
    disponibiliteText.setFont(font);
    passagersText.setFont(font);

    positionText.setCharacterSize(20);
    vitesseText.setCharacterSize(20);
    capaciteText.setCharacterSize(20);
    disponibiliteText.setCharacterSize(20);
    passagersText.setCharacterSize(20);

    positionText.setPosition(10, 10);
    vitesseText.setPosition(10, 40);
    capaciteText.setPosition(10, 70);
    disponibiliteText.setPosition(10, 100);
    passagersText.setPosition(10, 130);

    // La rame effectue un trajet entre les stations pendant 10 itérations
    for (size_t i = 0; i < stations.size(); ++i) {
        int passagersQuai = stations[i].gererRame();
        std::cout << "Station " << stations[i].nom << " - Passagers sur le quai : " << passagersQuai << std::endl;

        rame.gererPassagers(passagersQuai);
        rame.quitterStation();

        // Ajoutez cet appel pour mettre à jour la position après avoir quitté la station
        rame.parcourirLigne();

        rame.arreterAStation();

        // Mettez à jour les textes avec les informations de la rame
        positionText.setString("Station : " + stations[i].nom);
        vitesseText.setString("Vitesse : " + std::to_string(rame.getVitesse()));
        capaciteText.setString("Capacite maximale : " + std::to_string(rame.getCapaciteMaximale()));
        disponibiliteText.setString("Disponibilite : " + std::to_string(rame.getDisponibilite()));
        passagersText.setString(
            "Passagers entrant : " + std::to_string(rame.getPassagersEntrent()) +
            "\nPassagers sortant : " + std::to_string(rame.getPassagersSortent()) +
            "\nNombre de passagers a bord : " + std::to_string(rame.getNombrePassagers()));

        // Effacez le contenu de la fenêtre
        window.clear();

        // Dessinez les textes sur la fenêtre
        window.draw(positionText);
        window.draw(vitesseText);
        window.draw(capaciteText);
        window.draw(disponibiliteText);
        window.draw(passagersText);

        // Affichez le contenu de la fenêtre
        window.display();

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
