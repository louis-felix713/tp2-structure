/**
 * \file ReseauAerien.cpp
 * \brief Implémentattion de la classe ReseauAerien.
 * \author Étudiant(e)
 * \version 0.1
 * \date novembre 2023
 *
 *  Travail pratique numéro 2
 *
 */

#include "ReseauAerien.h"
#include <sstream>
#include <fstream>
#include <stack>
#include <algorithm>
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{
    ReseauAerien::ReseauAerien(std::string nomReseau, size_t nbVilles) {
        this->nomReseau = nomReseau;
        this->unReseau = Graphe(nbVilles);
    }

    ReseauAerien::~ReseauAerien() {
    }


    void ReseauAerien::resize(size_t nouvelleTaille) {
        this->unReseau.resize(nouvelleTaille);
    }

    // Méthode fournie
    /**
     * \fn void ReseauAerien::chargerReseau(std::ifstream & fichierEntree)
     * \brief Permet de charger le réseau à partir d'un fichier texte
     * \param[in] le fichier contenant l'information sur le réseau
     * \pre fichierEntree a déjà été ouvert
     * \post Le réseau a été chargé
     * \exception logic_error si le fichier n'est pas déjà ouvert 
     */
    void ReseauAerien::chargerReseau(std::ifstream & fichierEntree)
    {
        if (!fichierEntree.is_open())
            throw std::logic_error("ReseauAerien::chargerReseau: Le fichier n'est pas ouvert !");

        std::string buff;
        getline(fichierEntree, nomReseau);
        nomReseau.erase(0, 15);

        int nbVilles;

        fichierEntree >> nbVilles;
        getline(fichierEntree, buff); //villes

        unReseau.resize(nbVilles);

        getline(fichierEntree, buff); //Liste des villes

        size_t i = 0;

        getline(fichierEntree, buff); //Premiere ville

        while(buff != "Liste des trajets:")
        {
            std::string ville = buff;

            unReseau.nommer(i, ville);

            getline(fichierEntree, buff);
            i++;
        }

        while(!fichierEntree.eof())
        {
            getline(fichierEntree, buff);
            std::string source = buff;

            getline(fichierEntree, buff);
            std::string destination = buff;

            getline(fichierEntree, buff);
            std::istringstream iss(buff);

            float duree;
            iss >> duree;

            float cout;
            iss >> cout;

            int ns;
            iss >> ns;

            unReseau.ajouterArc(unReseau.getNumeroSommet(source), unReseau.getNumeroSommet(destination), duree, cout, ns);
        }
    }

    Chemin ReseauAerien::rechercheCheminDijkstra(const std::string &origine, const std::string &destination, bool dureeCout) const
    {
        std::vector<Distance> longueur;
        std::vector<Predecesseur> predecesseur;
        for (int i = 0; i < unReseau.getNombreSommets(); ++i) {
            if (unReseau.getNomSommet(i) == origine) {
                longueur.push_back({unReseau.getNomSommet(i), 0});
            }
            else {
                longueur.push_back({unReseau.getNomSommet(i), 12345});
            }

            predecesseur.push_back({unReseau.getNomSommet(i), ""});
        }

        std::vector<std::string> solutionnes;
        std::vector<std::string> nonSolutionne = unReseau.getSommets();

        while(nonSolutionne.size()) {
            Distance minimal = trouverMinimal(longueur, nonSolutionne);
            nonSolutionne = removeNonSolutionne(minimal.nom, nonSolutionne);
            solutionnes.push_back(minimal.nom);
            size_t sommet = unReseau.getNumeroSommet(minimal.nom);
            std::vector<size_t> sommetsAdjacents = unReseau.listerSommetsAdjacents(sommet);

            for (size_t sommetAdjacent : sommetsAdjacents) {
                Ponderations arc = unReseau.getPonderationsArc(sommet, sommetAdjacent);
                float temp;
                if (dureeCout) {
                    temp = minimal.distance + arc.duree;
                }
                else {
                    temp = minimal.distance + arc.cout;
                }
                if (temp < longueur[sommetAdjacent].distance) {
                    longueur[sommetAdjacent].distance = temp;
                    predecesseur[sommetAdjacent].predecesseur = minimal.nom;
                }
            }
        }
        Chemin resultat = calculerChemin(origine, destination, longueur, predecesseur);
        if (dureeCout) {
            resultat.dureeTotale = longueur[unReseau.getNumeroSommet(destination)].distance;
        }
        else {
            resultat.coutTotal = longueur[unReseau.getNumeroSommet(destination)].distance;
        }
        return resultat;
    }

    Chemin ReseauAerien::rechercheCheminBellManFord(const std::string &origine, const std::string &destination, int dureeCoutNiveau) const
    {
        Chemin resultat;
        std::vector<Distance> longueur;
        std::vector<Predecesseur> predecesseur;
        std::vector<Trajet> arcs;
        for (size_t i = 0; i < unReseau.getNombreSommets(); ++i) {
            if (unReseau.getNomSommet(i) == origine) {
                longueur.push_back({unReseau.getNomSommet(i), 0});
            } else {
                longueur.push_back({unReseau.getNomSommet(i), 12345});
            }
            predecesseur.push_back({unReseau.getNomSommet(i), ""});
            for(size_t sommet : unReseau.listerSommetsAdjacents(i)) {
                arcs.push_back({i, sommet});
            }
        }
        for (int i = 0; i < unReseau.getNombreSommets(); ++i) {
            for (Trajet arc: arcs) {
                Ponderations cout = unReseau.getPonderationsArc(arc.depart, arc.arrivee);
                float temp;
                switch(dureeCoutNiveau) {
                    case 1:
                        temp = longueur[arc.depart].distance + cout.duree;
                        break;
                    case 2:
                        temp = longueur[arc.depart].distance + cout.cout;
                        break;
                    case 3:
                        temp = longueur[arc.depart].distance + cout.ns;
                        break;
                }
                if (temp < longueur[arc.arrivee].distance) {
                    longueur[arc.arrivee].distance = temp;
                    predecesseur[arc.arrivee].predecesseur = unReseau.getNomSommet(arc.depart);
                }
            }

        }
        resultat = calculerChemin(origine, destination, longueur, predecesseur);
        switch(dureeCoutNiveau) {
            case 1:
                resultat.dureeTotale = longueur[unReseau.getNumeroSommet(destination)].distance;
                break;
            case 2:
                resultat.coutTotal = longueur[unReseau.getNumeroSommet(destination)].distance;
                break;
            case 3:
                resultat.nsTotal = (int)longueur[unReseau.getNumeroSommet(destination)].distance;
                break;
        }
        if (dureeCoutNiveau == 3) {
            for (Trajet arc: arcs) {
                Ponderations cout = unReseau.getPonderationsArc(arc.depart, arc.arrivee);
                if (longueur[arc.arrivee].distance > longueur[arc.depart].distance + cout.ns) {
                    resultat.reussi = false;
                    return resultat;
                }
            }
        }

        return resultat;
    }

    std::vector<std::string> ReseauAerien::removeNonSolutionne(const std::basic_string<char> string, std::vector<std::string> vector) const {
        std::vector<std::string> newNonSolutionne = std::vector<std::string>();
        for(auto noeud : vector) {
            if (string != noeud) {
                newNonSolutionne.push_back(noeud);
            }
        }
        return newNonSolutionne;
    }

    Chemin ReseauAerien::calculerChemin(const std::string &origine, const std::string &destination,
                                        std::vector<Distance> longueur,
                                        std::vector<Predecesseur> predecesseur) const {
        Chemin resultat;
        std::stack<std::string> cheminInverse;
        if (predecesseur[unReseau.getNumeroSommet(destination)].predecesseur == "") {
            resultat.reussi = false;
            return resultat;
        }
        std::string sommetPrecedent = predecesseur[unReseau.getNumeroSommet(destination)].predecesseur;
        cheminInverse.push(destination);
        while(sommetPrecedent != "") {
            cheminInverse.push(sommetPrecedent);
            sommetPrecedent = predecesseur[unReseau.getNumeroSommet(sommetPrecedent)].predecesseur;
        }
        while(cheminInverse.size()) {
            resultat.listeVilles.push_back(cheminInverse.top());
            cheminInverse.pop();
        }
        resultat.reussi = true;
        return resultat;
    }

    Distance
    ReseauAerien::trouverMinimal(std::vector<Distance> longueur, std::vector<std::string> nonSolutionne) const {
        Distance minimal = {"", 12345.0};
        for (Distance noeud : longueur) {
            if(std::find(nonSolutionne.begin(), nonSolutionne.end(), noeud.nom) != nonSolutionne.end() && minimal.nom == "") {
                minimal = noeud;
                continue;
            }
            if (noeud.distance < minimal.distance && std::find(nonSolutionne.begin(), nonSolutionne.end(), noeud.nom) != nonSolutionne.end()) {
                minimal = noeud;
            }
        }
        return minimal;
    }


}//Fin du namespace
