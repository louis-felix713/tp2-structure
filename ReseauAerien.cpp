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
        std::map<std::string, float> longueur;
        std::map<std::string, std::string> predecesseur;
        for (int i = 0; i < unReseau.getNombreSommets(); ++i) {
            longueur.insert({unReseau.getNomSommet(i), 12345});
            predecesseur.insert({unReseau.getNomSommet(i), ""});
        }

        longueur[origine] = 0;
        std::vector<std::string> solutionnes;
        std::vector<std::string> nonSolutionne = unReseau.getSommets();

        for (int i = 0; i < unReseau.getNombreSommets(); ++i) {
            std::map<std::string, float>::iterator minimal = trouverMinimal(longueur);
            nonSolutionne = removeNonSolutionne(minimal->first, nonSolutionne);\
            solutionnes.push_back(minimal->first);
            size_t sommet = unReseau.getNumeroSommet(minimal->first);
            std::vector<size_t> sommetsAdjacents = unReseau.listerSommetsAdjacents(sommet);

            for (size_t sommetAdjacent : sommetsAdjacents) {
                Ponderations arc = unReseau.getPonderationsArc(sommet, sommetAdjacent);
                float temp;
                if (dureeCout) {
                    temp = minimal->second + arc.duree;
                }
                else {
                    temp = minimal->second + arc.cout;
                }
                if (temp < longueur[unReseau.getNomSommet(sommetAdjacent)]) {
                    longueur[unReseau.getNomSommet(sommetAdjacent)] = temp;
                    predecesseur[unReseau.getNomSommet(sommetAdjacent)] = minimal->first;
                }
            }
        }
        return calculerChemin(origine, destination, longueur, predecesseur, dureeCout);
    }

    Chemin ReseauAerien::rechercheCheminBellManFord(const std::string &origine, const std::string &destination, int dureeCoutNiveau) const
    {
        Chemin resultat;
        return resultat;
    }

    std::map<std::string, float>::iterator ReseauAerien::trouverMinimal(std::map<std::string, float> map) const{
        std::map<std::string, float>::iterator itMinimal = map.begin();
        for (std::map<std::string, float>::iterator it = map.begin(); it != map.end(); ++it) {
            if (it->second < itMinimal->second) {
                itMinimal = it;
            }
        }
        return itMinimal;
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
                                        std::map<std::string, float> longueur,
                                        std::map<std::string, std::string> predecesseur, bool dureeCout) const {
        Chemin resultat;
        std::stack<std::string> cheminInverse;
        if (predecesseur[destination] == "") {
            resultat.reussi = false;
            return resultat;
        }
        std::string sommetPrecedent = predecesseur[destination];
        cheminInverse.push(destination);
        while(sommetPrecedent != "") {
            cheminInverse.push(sommetPrecedent);
            sommetPrecedent = predecesseur[sommetPrecedent];
        }
        while(cheminInverse.size()) {
            resultat.listeVilles.push_back(cheminInverse.top());
            cheminInverse.pop();
        }
        resultat.reussi = true;
        if (dureeCout) {
            resultat.dureeTotale = longueur[destination];
        }
        else {
            resultat.coutTotal = longueur[destination];
        }

        return resultat;

    }

}//Fin du namespace
