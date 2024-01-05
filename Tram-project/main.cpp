#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <mutex>
#include <sstream>
#include <map>
#include <random>

//constexpr const char* _PATH_IMG_ = "../../../../../../../../images/";
constexpr const char* _PATH_IMG_ = "./images/";

const double EPSILON = 1e-5;
const double SEUIL_DEC�L�RATION = 50.0;

class Metro {
    friend class Superviseur; // Permet � Superviseur d'acc�der aux membres priv�s de Metro
    friend class Station;
    friend void simulerMetro(Metro& metro, const std::vector<Station>& stations, bool avecDelai, Superviseur& superviseur, sf::Time& tempsTotalArrete);

    friend size_t getProchaineStationIndex(const Metro& metro, const std::vector<Station>& stations);
    friend void afficherMetro(sf::RenderWindow& window, const Metro& metro, const Metro& deuxiemeMetro, const std::vector<Station>& stations, bool afficherDeuxiemeMetro, Superviseur& superviseur);
    friend double getProchaineStationPosition(const Metro& metro, const std::vector<Station>& stations);

private:
    double position;
    double distanceEntreStations;
    double vitesse = 0;
    const double vitesseCroisiere = 2.0;
    const double vitesseArret = 0.0;
    const double dureeArret = 5.0;
    const int capaciteMaximale = 200;
    int nombrePassagers;
    sf::Clock stationClock;
    bool enArret;
    int passagersEntrants = 0;
    int passagersSortants = 0;

    bool directionPositive;
    double facteurVariabilite;
    bool arretUrgence = false;
    bool estEnProcessusDeDeceleration = false;
    double positionRamePrecedente;

    bool activerDistanceSecurite = false;
    int id = rand(); // Membre pour stocker l'ID

public:
    Metro() : position(0.0), vitesse(0.0), nombrePassagers(0), enArret(false), distanceEntreStations(0.0), directionPositive(true), facteurVariabilite(static_cast<double>(rand()) / RAND_MAX), positionRamePrecedente(0.0) {
        // Reste du constructeur
    }


    // M�thode pour r�cup�rer l'ID du m�tro
    int getID() const {
        return id;
    }


    bool estALaStation() const {
        // Un m�tro est consid�r� comme �tant � une station si sa vitesse est nulle (arr�t�)
        return enArret;
    }


    void initialiser(double distanceEntreStations) {
        this->distanceEntreStations = distanceEntreStations;
    }

    void quitterStation() {
        accelerer(vitesseCroisiere);
        enArret = false;
    }

    void parcourirLigne(sf::Time elapsedTime, const std::vector<Station>& stations, double positionPrecedente, Superviseur& superviseur)
    {
        this->positionRamePrecedente = positionPrecedente;

        double distanceSecurite = 50.0; // Distance de s�curit� en m�tres
        double distanceAvecPrecedente = std::abs(position - positionRamePrecedente);

        if (!enArret) {
            double deplacement = (vitesse * elapsedTime.asSeconds()) / 4.0;
            bool doitSArreter = false;

            double prochaineStationPos = getProchaineStationPosition(*this, stations);
            double distanceProchaineStation = std::abs(prochaineStationPos - position);

            // G�rer la d�c�l�ration avant d'arriver � la station
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

            // Activer la distance de s�curit� apr�s 20 secondes
            if (activerDistanceSecurite && distanceAvecPrecedente < distanceSecurite) {
                vitesse = std::max(0.0, vitesse - 0.5); // Ralentir si n�cessaire
            }

            if (activerDistanceSecurite && distanceAvecPrecedente < distanceSecurite) {
                // Ralentir si n�cessaire
                vitesse = std::max(0.0, vitesse - 0.5);
            }

            // Continuer avec la logique de d�placement normale
            if (directionPositive) {
                position += deplacement;
                if (position >= distanceEntreStations) {
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

            // Gestion de l'arr�t � la station
            if (doitSArreter && !enArret) {
                arreterAStation();
            }
        }
    }





    void arreterAStation() {
        if (!enArret) {
            vitesse = 0;
            enArret = true;
            stationClock.restart();

            // Calcul de la dur�e d'arr�t ajust�e
            double tempsParPassager = 0.1;
            double tempsArretAjuste = dureeArret + (passagersEntrants + passagersSortants) * tempsParPassager;

            // Attendre le temps d'arr�t ajust� (simulation)
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(tempsArretAjuste * 1000)));

            std::cout << "Temps pass� � la station : " << tempsArretAjuste << " secondes" << std::endl;
        }
    }









    void gererPassagers(int passagersQuai) {
        // Facteur de variabilit� bas� sur l'ID du m�tro et une valeur al�atoire
        int variabilite = (rand() % 20 + 1) * (id % 15);

        // Calcul du nombre de passagers sortants avec variabilit�
        passagersSortants = std::min(nombrePassagers, rand() % (nombrePassagers + 1) + variabilite / 20);

        // Calcul du nombre de passagers entrants avec variabilit�
        passagersEntrants = std::min(capaciteMaximale - nombrePassagers, passagersQuai) + variabilite / 25;

        // Mise � jour du nombre de passagers � bord
        nombrePassagers += passagersEntrants - passagersSortants;

        // Assurer que le nombre de passagers reste dans les limites
        if (nombrePassagers > capaciteMaximale) nombrePassagers = capaciteMaximale;
        if (nombrePassagers < 0) nombrePassagers = 0;
    }




    void arretTotal() {
        enArret = true;
        vitesse = 0.0;
        arretUrgence = true; // Indicateur d'arr�t d'urgence activ�
    }

    void reprendreApr�sArretUrgence() {
        if (arretUrgence) {
            enArret = false;
            accelerer(vitesseCroisiere);
            arretUrgence = false;  // R�initialisation de l'�tat d'urgence
        }
    }

    int getPassagersEntrants() const {
        return passagersEntrants;
    }

    int getPassagersSortants() const {
        return passagersSortants;
    }

    double getVitesse() const {
        return vitesse;
    }

    double getPosition() const {
        return position;
    }

    int getCapaciteMaximale() const {
        return capaciteMaximale;
    }

    int getNombrePassagers() const {
        return nombrePassagers;
    }

    double getDureeArret() const {
        return dureeArret;
    }

    void accelerer(double acceleration) {
        vitesse += acceleration;
    }





    double fonctionTransition(double t) const {
        return 2 * t * t * t - 3 * t * t + 1;
    }
};

class Station {
    friend class Metro;
    friend class Superviseur;
    friend size_t getProchaineStationIndex(const Metro& metro, const std::vector<Station>& stations);
    friend void afficherMetro(sf::RenderWindow& window, const Metro& metro, const Metro& deuxiemeMetro, const std::vector<Station>& stations, bool afficherDeuxiemeMetro, Superviseur& superviseur);
    friend void simulerMetro(Metro& metro, const std::vector<Station>& stations, bool avecDelai, Superviseur& superviseur, sf::Time& tempsTotalArrete);
    friend double getProchaineStationPosition(const Metro& metro, const std::vector<Station>& stations);

private:
    std::string nom;
    double position;
    bool metroPresent;

public:
    Station(const std::string& nom, double position) : nom(nom), position(position) {}

    void signalerArriveeMetro() {
        metroPresent = true;
    }

    // M�thode pour signaler le d�part d'un m�tro
    void signalerDepartMetro() {
        metroPresent = false;
    }

    bool estMetroPresent() const {
        return metroPresent;
    }
    int gererPassagers() const {
        return rand() % 150;
    }


};


size_t getProchaineStationIndex(const Metro& metro, const std::vector<Station>& stations) {
    size_t stationIndex = 0;

    if (metro.directionPositive) {
        // Lorsque le m�tro se d�place vers la droite, trouvez la prochaine station devant lui.
        // Commencez par la premi�re station et d�placez-vous vers la droite.
        for (size_t i = 0; i < stations.size() - 1; ++i) {
            if (metro.getPosition() >= stations[i].position && metro.getPosition() < stations[i + 1].position) {
                stationIndex = i;
                break;
            }
        }
    }
    else {
        // Lorsque le m�tro se d�place vers la gauche, trouvez la prochaine station derri�re lui.
        // Commencez par la derni�re station et d�placez-vous vers la gauche.
        for (size_t i = stations.size() - 1; i > 0; --i) {
            if (metro.getPosition() <= stations[i].position && metro.getPosition() > stations[i - 1].position) {
                stationIndex = i;
                break;
            }
        }
    }

    // Si le m�tro est � la derni�re station et se d�place vers la droite, il est en train de tourner.
    if (metro.directionPositive && metro.getPosition() >= stations.back().position) {
        stationIndex = stations.size() - 1;
    }

    // Si le m�tro est � la premi�re station et se d�place vers la gauche, il est en train de tourner.
    if (!metro.directionPositive && metro.getPosition() <= stations.front().position) {
        stationIndex = 0;
    }

    return stationIndex;
}

class Superviseur {
    friend class Metro;
    friend class Station;
    friend size_t getProchaineStationIndex(const Metro& metro, const std::vector<Station>& stations);
    friend void afficherMetro(sf::RenderWindow& window, const Metro& metro, const Metro& deuxiemeMetro, const std::vector<Station>& stations, bool afficherDeuxiemeMetro, Superviseur& superviseur);
    friend void simulerMetro(Metro& metro, const std::vector<Station>& stations, bool avecDelai, Superviseur& superviseur, sf::Time& tempsTotalArrete);

public:
    Superviseur(Metro& metro1, Metro& metro2, sf::RenderWindow& window)
        : m_metro1(metro1), m_metro2(metro2), m_window(window), arretUrgence(false) {
        demarrageTime = std::chrono::steady_clock::now();
    }

    void verifierEtReagir(sf::Event event, sf::Clock& clock, sf::Time& tempsTotalArrete) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
                if (boutonArret.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    std::cout << "Bouton Stop cliqu�" << std::endl;

                    toggleArretUrgence(clock, tempsTotalArrete);
                }
            }
        }
    }

    void toggleArretUrgence(sf::Clock& clock, sf::Time& tempsTotalArrete) {
        arretUrgence = !arretUrgence;
        if (arretUrgence) {
            tempsTotalArrete += clock.getElapsedTime();
            m_metro1.arretTotal();
            m_metro2.arretTotal();
        }
        else {
            m_metro1.quitterStation();
            m_metro2.quitterStation();
        }
    }

    void Superviseur::mettreAJourLesPositions(Metro& metro1, Metro& metro2, sf::Time elapsedTime, const std::vector<Station>& stations, bool simulationDebut) {
        double positionMetro1 = metro1.getPosition();
        double positionMetro2 = metro2.getPosition();

        metro1.parcourirLigne(elapsedTime, stations, positionMetro2, *this); // Passer une r�f�rence � ce superviseur
        metro2.parcourirLigne(elapsedTime, stations, positionMetro1, *this); // Idem ici
    }


    bool getArretUrgence() const { return arretUrgence; }

    // M�thode pour obtenir l'ID du premier m�tro
    int getIdMetro1() const {
        return m_metro1.getID();
    }

    // M�thode pour obtenir l'ID du deuxi�me m�tro
    int getIdMetro2() const {
        return m_metro2.getID();
    }

    // M�thode pour obtenir la position du premier m�tro
    double getPositionMetro1() const {
        return m_metro1.getPosition();
    }

    // M�thode pour obtenir la position du deuxi�me m�tro
    double getPositionMetro2() const {
        return m_metro2.getPosition();
    }
    void arretDurgence() {
        m_metro1.arretTotal();
        m_metro2.arretTotal();
        arretUrgence = true;
        // Ajoutez ici d'autres actions pour l'arr�t d'urgence
    }

    void verifierDistanceSecurite(Metro& metro1, Metro& metro2) {
        auto elapsed = demarrageClock.getElapsedTime().asSeconds();

        // Activer la distance de s�curit� apr�s 20 secondes
        if (elapsed > 20 && !activerDistanceSecurite) {
            activerDistanceSecurite = true;
            std::cout << "Activation de la distance de s�curit� apr�s 20 secondes." << std::endl;
        }

        if (!activerDistanceSecurite) {
            return; // Si la distance de s�curit� n'est pas activ�e, rien de plus � faire
        }

        double distanceSecurite = 50.0; // Distance de s�curit� en m�tres
        double distanceActuelle = std::abs(metro1.getPosition() - metro2.getPosition());

        // V�rifier si les deux m�tros se d�placent dans la m�me direction
        bool memeDirection = metro1.directionPositive == metro2.directionPositive;

        // V�rifier la distance et d�clencher un arr�t d'urgence si n�cessaire
        if (memeDirection && distanceActuelle < distanceSecurite && !arretUrgence) {
            std::cout << "Arr�t d'urgence d�clench� en raison d'une proximit� trop proche." << std::endl;

            arretDurgence();
        }

    }



    // Assurez-vous que ces membres sont accessibles l� o� ils sont n�cessaires
    sf::RectangleShape boutonArret;
    sf::Text texteBouton;
    sf::Time tempsArrete;
    sf::Clock demarrageClock;




private:
    Metro& m_metro1;
    Metro& m_metro2;
    sf::RenderWindow& m_window;
    bool enArret = false;
    bool arretUrgence;
    bool activerDistanceSecurite = false;
    std::chrono::steady_clock::time_point demarrageTime;



};


void drawTextWithBackground(sf::RenderWindow& window, sf::Text& text, sf::FloatRect& bounds, sf::Color topColor, sf::Color bottomColor) {
    sf::RectangleShape backgroundRect;
    backgroundRect.setSize(sf::Vector2f(bounds.width, bounds.height + 10)); // +10 pour un peu d'espace suppl�mentaire
    backgroundRect.setPosition(text.getPosition().x, text.getPosition().y);

    // Cr�ez le d�grad� en utilisant sf::VertexArray
    sf::VertexArray gradient(sf::Quads, 4);

    // Configurez le d�grad� pour qu'il corresponde � la taille et � la position de backgroundRect
    gradient[0].position = backgroundRect.getPosition();
    gradient[1].position = sf::Vector2f(backgroundRect.getPosition().x + backgroundRect.getSize().x, backgroundRect.getPosition().y);
    gradient[2].position = sf::Vector2f(backgroundRect.getPosition().x + backgroundRect.getSize().x, backgroundRect.getPosition().y + backgroundRect.getSize().y);
    gradient[3].position = sf::Vector2f(backgroundRect.getPosition().x, backgroundRect.getPosition().y + backgroundRect.getSize().y);



    // D�finissez les couleurs des coins pour le d�grad�
    gradient[0].color = topColor;
    gradient[1].color = topColor;
    gradient[2].color = bottomColor;
    gradient[3].color = bottomColor;

    // Dessinez le d�grad�
    window.draw(gradient);

    // Dessinez le texte
    window.draw(text);
}

void afficherMetro(sf::RenderWindow& window, const Metro& metro, const Metro& deuxiemeMetro, const std::vector<Station>& stations, bool afficherDeuxiemeMetro, Superviseur& superviseur) {
    window.clear(sf::Color(28, 48, 68));

    sf::RectangleShape outerTrackRect;
    outerTrackRect.setSize(sf::Vector2f(static_cast<float>(stations.back().position + 20), 40));
    outerTrackRect.setFillColor(sf::Color::Transparent);
    outerTrackRect.setOutlineColor(sf::Color(170, 140, 100));
    outerTrackRect.setOutlineThickness(20);
    outerTrackRect.setPosition(0.0f, static_cast<float>(window.getSize().y) / 2.0f - 20.0f);
    window.draw(outerTrackRect);


    sf::Font fontStation;
    if (!fontStation.loadFromFile("C:/Windows/Fonts/BAUHS93.TTF")) {
        std::cerr << "Erreur lors du chargement de la police" << std::endl;
        return;
    }

    sf::Texture stationTexture;
    if (!stationTexture.loadFromFile(std::string(_PATH_IMG_) + "Logo_Metro.png")) {
        std::cerr << "Erreur lors du chargement de l'image de la station" << std::endl;
        return;
    }

    // Pour chaque station, dessinez la station et son nom
    for (const auto& station : stations) {
        sf::Sprite stationSprite;
        stationSprite.setTexture(stationTexture);

        // Ajustez la taille si n�cessaire
        stationSprite.setScale(0.06f, 0.06f); // Ajustez cette �chelle selon la taille de votre image et la taille souhait�e sur l'�cran

        // Centrez le sprite sur l'emplacement de la station
        sf::Vector2u textureSize = stationTexture.getSize();
        stationSprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
        stationSprite.setPosition(static_cast<float>(station.position), static_cast<float>(window.getSize().y) / 2.0f);

        window.draw(stationSprite);

        // Cr�er un objet Text pour le nom de la station
        sf::Text stationText;
        stationText.setFont(fontStation);
        stationText.setString(station.nom);
        stationText.setCharacterSize(16);
        stationText.setFillColor(sf::Color::White);

        // Positionner le texte au-dessus de la station
        sf::FloatRect textRect = stationText.getLocalBounds();
        stationText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        stationText.setPosition(static_cast<float>(station.position), static_cast<float>(window.getSize().y) / 2.0f - 60.0f);

        window.draw(stationText);
    }

    // Position verticale pour la ligne du haut et du bas
    const float ligneHaut = static_cast<float>(window.getSize().y) / 2.0f - 40.0f;
    const float ligneBas = static_cast<float>(window.getSize().y) / 2 + 20;






    sf::Texture metroTexture;
    if (!metroTexture.loadFromFile(std::string(_PATH_IMG_) + "metro-test.png")) {
        std::cerr << "Erreur lors du chargement de l'image du m�tro" << std::endl;
        // G�rez l'erreur comme il se doit - peut-�tre fermer le programme ou utiliser une image par d�faut
    }

    // Utilisation de la texture pour cr�er un sprite
    sf::Sprite metroSprite;
    metroSprite.setTexture(metroTexture);

    // Ajustez la taille si n�cessaire
    // Par exemple, si l'image est trop grande, vous pouvez la r�duire
    metroSprite.setScale(0.1f, 0.1f); // Ajustez cette �chelle selon la taille de votre image et la taille souhait�e sur l'�cran

    // Positionnez le sprite du m�tro selon la position du m�tro dans le jeu
    float metroPosY = metro.directionPositive ? ligneHaut : ligneBas; // Assurez-vous que ligneHaut et ligneBas sont d�finis
    metroSprite.setPosition(static_cast<float>(metro.getPosition() - 20), metroPosY);

    // Dessinez le sprite du m�tro
    window.draw(metroSprite);



    sf::Texture metroTexture2;
    if (!metroTexture2.loadFromFile(std::string(_PATH_IMG_) + "metro1.png")) {
        std::cerr << "Erreur lors du chargement de l'image du deuxi�me m�tro" << std::endl;
        // G�rez l'erreur comme il se doit - peut-�tre fermer le programme ou utiliser une image par d�faut
    }


    if (afficherDeuxiemeMetro) {
        sf::Sprite deuxiemeMetroSprite;
        deuxiemeMetroSprite.setTexture(metroTexture2);
        // Ajustez la taille si n�cessaire
        deuxiemeMetroSprite.setScale(0.2f, 0.3f); // Ajustez cette �chelle selon la taille de votre image et la taille souhait�e sur l'�cran

        // Positionnez le sprite du deuxi�me m�tro selon la position du m�tro dans le jeu
        float deuxiemeMetroPosY = deuxiemeMetro.directionPositive ? ligneHaut : ligneBas;
        deuxiemeMetroSprite.setPosition(static_cast<float>(deuxiemeMetro.getPosition() - 20), deuxiemeMetroPosY);

        // Dessinez le sprite du deuxi�me m�tro
        window.draw(deuxiemeMetroSprite);
    }

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/ALGER.ttf")) {
        std::cerr << "Erreur lors du chargement de la police" << std::endl;
        return;
    }


    // Configurez le premier bloc de texte
    sf::Text textMetro1;
    textMetro1.setFont(font);
    textMetro1.setCharacterSize(20);
    textMetro1.setFillColor(sf::Color::White);
    textMetro1.setString(std::string("               M�tro 1 (ID: ") + std::to_string(metro.getID()) + ")" +
        "\n\n --> Capacit� maximale : " + std::to_string(metro.getCapaciteMaximale()) +
        "\n\n | Entrants : " + std::to_string(metro.getPassagersEntrants()) +
        "\n | Sortants : " + std::to_string(metro.getPassagersSortants()) +
        "\n | Passagers � bord : " + std::to_string(metro.getNombrePassagers()) +
        "\n | Places disponibles : " + std::to_string(metro.getCapaciteMaximale() - metro.getNombrePassagers()) +
        "\n | Vitesse : " + std::to_string(metro.getVitesse()) + " m/s" +
        "\n\n | Station actuelle : " + (metro.estALaStation() ? stations[getProchaineStationIndex(metro, stations)].nom : "En d�placement"));

    // Ajoutez le nombre de passagers sur le quai si le nombre de passagers � bord est sup�rieur � 200
    if (metro.getNombrePassagers() > 200) {
        textMetro1.setString(textMetro1.getString() +
            "\n | Passagers sur le quai : " + std::to_string(stations[getProchaineStationIndex(metro, stations)].gererPassagers()));
    }

    textMetro1.setPosition(350, 50); // Position de d�part du premier bloc de texte


    // Configuration du deuxi�me bloc de texte pour le M�tro 2
    sf::Text textMetro2;
    textMetro2.setFont(font);
    textMetro2.setCharacterSize(20);
    textMetro2.setFillColor(sf::Color::White);
    textMetro2.setString(std::string("               M�tro 2 (ID: ") + std::to_string(deuxiemeMetro.getID()) + ")" +
        "\n\n --> Capacit� maximale : " + std::to_string(deuxiemeMetro.getCapaciteMaximale()) +
        "\n\n | Entrants : " + std::to_string(deuxiemeMetro.getPassagersEntrants()) +
        "\n | Sortants : " + std::to_string(deuxiemeMetro.getPassagersSortants()) +
        "\n | Passagers � bord : " + std::to_string(deuxiemeMetro.getNombrePassagers()) +
        "\n | Places disponibles : " + std::to_string(deuxiemeMetro.getCapaciteMaximale() - deuxiemeMetro.getNombrePassagers()) +
        "\n | Vitesse : " + std::to_string(deuxiemeMetro.getVitesse()) + " m/s" +
        "\n\n | Station actuelle : " + (deuxiemeMetro.estALaStation() ? stations[getProchaineStationIndex(deuxiemeMetro, stations)].nom : "En d�placement"));

    // Ajoutez le nombre de passagers sur le quai si le nombre de passagers � bord est sup�rieur � 200
    if (deuxiemeMetro.getNombrePassagers() > 200) {
        textMetro1.setString(textMetro1.getString() +
            "\n | Passagers sur le quai : " + std::to_string(stations[getProchaineStationIndex(deuxiemeMetro, stations)].gererPassagers()));
    }

    textMetro2.setPosition(1050, 50);

    // Dessinez les d�grad�s et les textes
    sf::FloatRect boundsMetro1 = textMetro1.getLocalBounds();
    sf::FloatRect boundsMetro2 = textMetro2.getLocalBounds();

    drawTextWithBackground(window, textMetro1, boundsMetro1, sf::Color(255, 50, 60, 200), sf::Color(40, 50, 100, 200));
    drawTextWithBackground(window, textMetro2, boundsMetro2, sf::Color(255, 50, 60, 200), sf::Color(40, 50, 100, 200));

    superviseur.boutonArret.setSize(sf::Vector2f(100, 50));
    superviseur.boutonArret.setPosition(850, 800); // Position du bouton
    superviseur.boutonArret.setFillColor(sf::Color::Red);

    sf::Font font_stop;
    if (!font_stop.loadFromFile("C:/Windows/Fonts/ALGER.ttf")) {
        std::cerr << "Erreur lors du chargement de la police" << std::endl;
        return;
    }

    superviseur.texteBouton.setFont(font_stop); // Assurez-vous que la police est charg�e
    superviseur.texteBouton.setString("STOP");
    superviseur.texteBouton.setCharacterSize(24);
    superviseur.texteBouton.setFillColor(sf::Color::White);

    // Calculer la position pour centrer le texte sur le bouton
    sf::FloatRect textRect = superviseur.texteBouton.getLocalBounds();
    superviseur.texteBouton.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    superviseur.texteBouton.setPosition(850 + 50, 800 + 25); // Ajustez en fonction de la taille du bouton


    window.draw(superviseur.boutonArret);
    window.draw(superviseur.texteBouton);

    window.display();
}



void simulerMetro(Metro& metro, const std::vector<Station>& stations, bool avecDelai, Superviseur& superviseur, sf::Time& tempsTotalArrete) {
    if (avecDelai) {
        std::this_thread::sleep_for(std::chrono::seconds(45));
    }

    sf::Clock clock;
    sf::Time tempsArrete;
    bool estArrete = false;
    size_t stationIndex = 0;
    bool aller = true;
    bool simulationDebut = true;

    while (true) {
        //superviseur.verifierDistanceSecurite(metro1, metro2);
        if (!superviseur.getArretUrgence()) {
            sf::Time tempsTotalEcoul� = clock.getElapsedTime() - tempsTotalArrete;

            if (estArrete) {
                tempsArrete += clock.restart();
                estArrete = false;
            }

            if (!metro.enArret) {
                sf::Time tempsEcoul� = clock.getElapsedTime();


                metro.parcourirLigne(tempsEcoul�, stations, stationIndex, superviseur);

                double nextStationPos = stations[stationIndex].position;
                if ((aller && metro.getPosition() >= nextStationPos) ||
                    (!aller && metro.getPosition() <= nextStationPos)) {
                    metro.arreterAStation();
                    if ((stationIndex == stations.size() - 1 && aller) ||
                        (stationIndex == 0 && !aller)) {
                        metro.passagersSortants = metro.nombrePassagers;
                        metro.nombrePassagers = 0;
                        metro.passagersEntrants = 0;
                        std::cout << "Terminus, tout le monde descend !\n";

                        aller = !aller;
                        stationIndex = aller ? 0 : stations.size() - 1;
                        clock.restart();
                        tempsArrete = sf::Time::Zero;
                    }
                    else {
                        metro.gererPassagers(stations[stationIndex].gererPassagers());
                        stationIndex += (aller) ? 1 : -1;

                    }
                    metro.directionPositive = aller;
                }
            }

            if (metro.enArret && tempsTotalEcoul�.asSeconds() - tempsArrete.asSeconds() >= metro.getDureeArret()) {
                metro.quitterStation();
                clock.restart();
                tempsArrete = sf::Time::Zero;
            }
        }
        else {
            if (!estArrete) {
                tempsArrete = clock.getElapsedTime();
                estArrete = true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}



double getProchaineStationPosition(const Metro& metro, const std::vector<Station>& stations) {
    size_t stationIndex = getProchaineStationIndex(metro, stations);
    return stations[stationIndex].position;
}









int main() {
    srand(static_cast<unsigned int>(time(0)));

    Metro metro;
    metro.initialiser(1700);

    Metro deuxiemeMetro;
    deuxiemeMetro.initialiser(1700);

    bool arretUrgence = false;


    std::vector<Station> stations = {
        Station("Saint-Philibert", 100),
        Station("Bourg", 300),
        Station("Maison des Enfants", 500),
        Station("Mitterie", 700),
        Station("Pont Sup�rieur", 900),
        Station("Lomme - Lambersart", 1100),
        Station("Canteleu", 1300),
        Station("Bois Blancs", 1500),
        Station("Port de Lille", 1700),
    };



    // Cr�ation de l'horloge et du temps total d'arr�t
    sf::Clock clock;
    sf::Time tempsTotalArrete;



    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Metro Lille");
    Superviseur superviseur(metro, deuxiemeMetro, window);

    superviseur.demarrageClock.restart();




    std::thread metroThread1(simulerMetro, std::ref(metro), std::cref(stations), false, std::ref(superviseur), std::ref(tempsTotalArrete));
    std::thread metroThread2(simulerMetro, std::ref(deuxiemeMetro), std::cref(stations), true, std::ref(superviseur), std::ref(tempsTotalArrete));



    metro.quitterStation();
    deuxiemeMetro.quitterStation();

    bool simulationDebut = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            superviseur.verifierEtReagir(event, clock, tempsTotalArrete);


        }
        sf::Time elapsedTime = clock.restart(); // R�cup�re le temps �coul� depuis le dernier appel

        for (auto& station : stations) {
            
            size_t stationIndexMetro = getProchaineStationIndex(metro, stations);
            size_t stationIndexDeuxiemeMetro = getProchaineStationIndex(deuxiemeMetro, stations);

            // V�rifier si le m�tro est � une station
            if (metro.estALaStation()) {
                stations[stationIndexMetro].signalerArriveeMetro();
            }
            else {
                stations[stationIndexMetro].signalerDepartMetro();
            }

            // V�rifier si le deuxi�me m�tro est � une station
            if (deuxiemeMetro.estALaStation()) {
                stations[stationIndexDeuxiemeMetro].signalerArriveeMetro();
            }
            else {
                stations[stationIndexDeuxiemeMetro].signalerDepartMetro();
            }
        }

        superviseur.verifierDistanceSecurite(metro, deuxiemeMetro);
        superviseur.mettreAJourLesPositions(metro, deuxiemeMetro, elapsedTime, stations, simulationDebut);



        if (!superviseur.getArretUrgence()) {
            afficherMetro(window, metro, deuxiemeMetro, stations, true, superviseur);
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    metroThread1.join();
    metroThread2.join();

    return 0;
}