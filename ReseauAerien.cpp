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
        Chemin resultat;
        std::map<std::string, float> longueur;
        for (int i = 0; i < unReseau.getNombreSommets(); ++i) {
            longueur.insert({unReseau.getNomSommet(i), 12345});
        }
        longueur[origine] = 0;
        std::vector<std::string> solutionnes;
        std::vector<std::string> nonSolutionne = unReseau.getSommets();
        for (int i = 0; i < unReseau.getNombreSommets(); ++i) {
            std::map<std::string, float>::iterator minimal = trouverMinimal(longueur);
            nonSolutionne.erase(minimal->first, );
        }
        return resultat;
    }

    Chemin ReseauAerien::rechercheCheminBellManFord(const std::string &origine, const std::string &destination, int dureeCoutNiveau) const
    {
        Chemin resultat;
        return resultat;
    }

    std::map<std::string, float>::iterator ReseauAerien::trouverMinimal(std::map<std::string, float> map) const {
        std::map<std::string, float>::iterator itMinimal = map.begin();
        for (std::map<std::string, float>::iterator it = map.begin(); it != map.end(); ++it) {
            if (it->second < itMinimal->second) {
                itMinimal = it;
            }
        }
        return itMinimal;
    }


}//Fin du namespace
