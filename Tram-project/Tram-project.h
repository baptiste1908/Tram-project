// Tram-project.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets.

#pragma once

#include <iostream>
#include <iostream>
#include <vector>

class Rame {
public:
    int numero;
    double position;
    double vitesse;
    int nombrePassagers;

    // Autres attributs et méthodes nécessaires pour la gestion des rames

    Rame(int num, double pos, double vit, int passagers)
        : numero(num), position(pos), vitesse(vit), nombrePassagers(passagers) {}

    // Méthode pour déplacer la rame
    void deplacer() {
        // Logique pour déplacement de la rame
    }

    // Autres méthodes de la rame
};

class Station {
public:
    int nombrePassagersQuai;
    bool ramePresente;

    // Autres attributs et méthodes nécessaires pour la gestion des stations

    Station() : nombrePassagersQuai(0), ramePresente(false) {}

    // Méthode pour signaler à la rame qu'elle peut quitter la station
    void signalerDepartRame() {
        // Logique pour signaler le départ
    }

    // Autres méthodes de la station
};

class Superviseur {
public:
    std::vector<Rame> rames;

    // Autres attributs et méthodes nécessaires pour la gestion du superviseur

    // Méthode pour récupérer la position de toutes les rames
    void recupererPositionsRames() {
        for (const auto& rame : rames) {
            std::cout << "Rame " << rame.numero << " - Position : " << rame.position << std::endl;
        }
    }

    // Autres méthodes du superviseur
};
