//// Tram-project.h : fichier Include pour les fichiers Include système standard,
//// ou les fichiers Include spécifiques aux projets.
//
//#pragma once
//
//#include <iostream>
//#include <vector>
//#include <thread>
//#include <mutex>
//#include <chrono>
//std::mutex mtx; // Mutex pour la synchronisation
//
//class Rame {
//public:
//    int numero;
//    double position;
//    double vitesse;
//    int nombrePassagers;
//    bool arretUrgence;
//
//    Rame(int num, double pos, double vit, int passagers)
//        : numero(num), position(pos), vitesse(vit), nombrePassagers(passagers), arretUrgence(false) {}
//
//    void deplacer() {
//        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulation du déplacement
//        position += vitesse;
//    }
//
//    void gestionRame() {
//        while (true) {
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Simulation de la gestion de la rame
//            deplacer();
//
//            // Logique pour la gestion des passagers, accélération, décélération, etc.
//            // ...
//
//            // Utilisez le mutex pour synchroniser l'accès aux données partagées
//            std::lock_guard<std::mutex> lock(mtx);
//            if (arretUrgence) {
//                std::cout << "Rame " << numero << " - Arrêt d'urgence !" << std::endl;
//                arretUrgence = false; // Réinitialiser l'arrêt d'urgence
//            }
//            else {
//                std::cout << "Rame " << numero << " - Position : " << position << std::endl;
//            }
//        }
//    }
//};
//
//class Station {
//public:
//    int nombrePassagersQuai;
//    bool ramePresente;
//
//    Station() : nombrePassagersQuai(0), ramePresente(false) {}
//
//    void signalerDepartRame() {
//        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Simulation du signal de départ
//        ramePresente = false;
//    }
//
//    void gestionStation(Rame& rame) {
//        while (true) {
//            std::this_thread::sleep_for(std::chrono::milliseconds(1500)); // Simulation de la gestion de la station
//            // Logique pour la gestion des passagers, signalisation à la rame, etc.
//            // ...
//
//            // Utilisez le mutex pour synchroniser l'accès aux données partagées
//            std::lock_guard<std::mutex> lock(mtx);
//            if (ramePresente) {
//                if (rame.nombrePassagers < 50) {
//                    int passagersEntrants = rand() % (50 - rame.nombrePassagers + 1);
//                    rame.nombrePassagers += passagersEntrants;
//                    nombrePassagersQuai -= passagersEntrants;
//                    std::cout << passagersEntrants << " passagers entrent dans la rame." << std::endl;
//                }
//                if (nombrePassagersQuai > 0) {
//                    int passagersSortants = rand() % (rame.nombrePassagers + 1);
//                    rame.nombrePassagers -= passagersSortants;
//                    nombrePassagersQuai += passagersSortants;
//                    std::cout << passagersSortants << " passagers sortent de la rame." << std::endl;
//                }
//            }
//            else {
//                std::cout << "Pas de rame à la station." << std::endl;
//            }
//        }
//    }
//};
//
//class Superviseur {
//public:
//    std::vector<Rame> rames;
//
//    void recupererPositionsRames() {
//        while (true) {
//            std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Simulation de la récupération des positions
//            // Logique pour récupérer les positions des rames
//            // ...
//
//            // Utilisez le mutex pour synchroniser l'accès aux données partagées
//            std::lock_guard<std::mutex> lock(mtx);
//            for (const auto& rame : rames) {
//                std::cout << "Rame " << rame.numero << " - Position : " << rame.position << std::endl;
//            }
//        }
//    }
//};            std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Simulation de la récupération des positions
//            // Ajoutez la logique pour récupérer les positions des rames
//            // Utilisez le mutex pour synchroniser l'accès aux données partagées
//            std::lock_guard<std::mutex> lock(mtx);
//            for (const auto& rame : rames) {
//                std::cout << "Rame " << rame.numero << " - Position : " << rame.position << std::endl;
//            }
//        }
//    }
//};