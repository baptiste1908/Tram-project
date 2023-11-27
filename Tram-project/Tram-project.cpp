#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

/*class Rame;
class Station;

class Superviseur {
private:
    std::vector<Rame*> rames;

public:
    void ajouterRame(Rame* rame) {
        rames.push_back(rame);
    }

    void arretUrgenceAleatoire() {
        if (!rames.empty()) {
            int indiceRameArretUrgence = rand() % rames.size();
            Rame* rameArretUrgence = rames[indiceRameArretUrgence];
            std::cout << "Arrêt d'urgence pour la rame " << rameArretUrgence->getNumero() << std::endl;
            rameArretUrgence->arretUrgence();
        }
    }

    void getPositionRames() const {
        std::cout << "Position des rames : " << std::endl;
        for (const auto& rame : rames) {
            std::cout << "Rame " << rame->getNumero() << " - Position : " << rame->getPosition() << std::endl;
        }
    }
};

class Rame {
private:
    int numero;
    double position;
    double vitesse;
    const double vitesseCroisiere = 30.0;
    int nombrePassagers;

public:
    Rame(int numero) : numero(numero), position(0.0), vitesse(0.0), nombrePassagers(0) {}

    void quitterStation() {
        accelerer(5.0);
        std::cout << "La rame " << numero << " quitte la station. Nouvelle vitesse : " << vitesse << std::endl;
    }

    void parcourirLigne() {
        maintenirVitesseCroisiere();
        std::cout << "La rame " << numero << " parcourt la ligne à vitesse de croisière : " << vitesse << std::endl;
    }

    void arreterAStation() {
        decelerer(5.0);
        std::cout << "La rame " << numero << " ralentit en approchant de la station : " << vitesse << std::endl;
        vitesse = 0.0;
        std::cout << "La rame " << numero << " est à l'arrêt à la station." << std::endl;
    }

    void gererPassagers(Station& station) {
        station.rameArrive();

        int passagersEntrent = rand() % 21;
        int passagersSortent = rand() % (nombrePassagers + 1);

        if (passagersSortent > nombrePassagers) {
            passagersSortent = nombrePassagers;
        }

        nombrePassagers -= passagersSortent;

        int capaciteMax = 100;
        if (nombrePassagers + passagersEntrent <= capaciteMax) {
            nombrePassagers += passagersEntrent;
        }
        else {
            std::cout << "La rame est pleine. Certains passagers restent sur le quai." << std::endl;
        }

        std::cout << "La rame " << numero << " - Nombre de passagers à bord : " << nombrePassagers << std::endl;
    }

    void arretUrgence() {
        vitesse = 0.0;
        std::cout << "Arrêt d'urgence effectué pour la rame " << numero << "." << std::endl;
    }

    double getPosition() const {
        return position;
    }

    int getNumero() const {
        return numero;
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
private:
    int nombrePassagersQuai;

public:
    Station() : nombrePassagersQuai(0) {}

    void rameArrive() {
        int passagersEntrent = rand() % 21;
        int passagersSortent = rand() % (nombrePassagersQuai + 1);

        if (passagersSortent > nombrePassagersQuai) {
            passagersSortent = nombrePassagersQuai;
        }

        nombrePassagersQuai -= passagersSortent;
        nombrePassagersQuai += passagersEntrent;

        std::cout << "Nombre de passagers sur le quai : " << nombrePassagersQuai << std::endl;
    }

    int getNombrePassagersQuai() const {
        return nombrePassagersQuai;
    }

    bool autoriserDepartRame() const {
        return nombrePassagersQuai < 10;
    }
};
*/
int main() {
   /* srand(static_cast<unsigned int>(time(0)));

    Rame rame1(1);
    Rame rame2(2);

    Station station;

    Superviseur superviseur;
    superviseur.ajouterRame(&rame1);
    superviseur.ajouterRame(&rame2);

    for (int i = 0; i < 5; ++i) {
        superviseur.getPositionRames();

        rame1.quitterStation();
        rame1.parcourirLigne();
        rame1.gererPassagers(station);
        rame1.arreterAStation();
        rame1.gererPassagers(station);

        rame2.quitterStation();
        rame2.parcourirLigne();
        rame2.gererPassagers(station);
        rame2.arreterAStation();
        rame2.gererPassagers(station);

        if (rand() % 10 == 0) {
            superviseur.arretUrgenceAleatoire();
        }
    }

    return 0;*/



        sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
        sf::CircleShape shape(100.f);
        shape.setFillColor(sf::Color::Green);

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
                window.draw(shape);
                window.display();
        }

        return 0;
    
}
