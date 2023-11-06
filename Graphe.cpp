/**
* \file Graphe.cpp
* \brief Implémentation d'un graphe orienté.
* \author Étudiant(e)
* \version 0.5
* \date novembre 2023
*
*  Travail pratique numéro 2
*
*/

#include "Graphe.h"

//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{

	Graphe::Graphe(size_t nbSommets){
        this->sommets.resize(nbSommets);
        this->listesAdj.resize(nbSommets);
        this->nbSommets = nbSommets;
	}

	Graphe::~Graphe(){

	}

	void Graphe::resize(size_t nouvelleTaille){
        this->sommets.resize(nouvelleTaille);
        this->listesAdj.resize(nouvelleTaille);
        this->nbSommets = nouvelleTaille;
	}

	void Graphe::nommer(size_t sommet, const std::string& nom) {
        if (sommet >= this->nbSommets) {
            throw std::logic_error("nommer: numero de sommet invalide");
        }
        this->sommets[sommet] = nom;
	}

    const std::vector<std::string> Graphe::getSommets() const {
        return this->sommets;
    }

	void Graphe::ajouterArc(size_t source, size_t destination, float duree, float cout, int ns) {
        if (source >= nbSommets || destination >= nbSommets) {
            throw std::logic_error("ajouterArc: source ou destination trop grande");
        }
        if (!arcExiste(source, destination)){
            this->listesAdj[source].push_back(Arc(destination, Ponderations(duree, cout, ns)));
            this->nbArcs++;
        }
        else {
            throw std::logic_error("ajouterArc: l'arc existe deja");
        }

	}

	void Graphe::enleverArc(size_t source, size_t destination) {
        int nbArcsAvant = nbArcs;
        if (source >= nbSommets || destination >= nbSommets) {
            throw std::logic_error("enleverArc: source ou destination trop grande");
        }
        for (std::list<Arc>::const_iterator it = listesAdj[source].begin(); it != listesAdj[source].end(); ++it) {
            if (it->destination == destination) {
                listesAdj[source].erase(it);
                nbArcs--;
                break;
            }
        }
        if (nbArcsAvant == nbArcs)
            throw std::logic_error("enleverArc: Arc inexistant");
	}

	bool Graphe::arcExiste(size_t source, size_t destination) const {
        if (source >= nbSommets || destination >= nbSommets) {
            throw std::logic_error("arcExiste: source ou destination trop grande");
        }
        for (const auto & it : listesAdj[source]) {
            if (it.destination == destination) {
                return true;
            }
        }
        return false;
	}

	std::vector<size_t> Graphe::listerSommetsAdjacents(size_t sommet) const {
        if (sommet >= nbSommets) {
            throw std::logic_error("listerSommetsAdjacents: sommet trop grand");
        }
		std::vector<size_t> v;
        for (std::list<Arc>::const_iterator it = listesAdj[sommet].begin(); it != listesAdj[sommet].end(); ++it) {
            v.push_back(it->destination);
        }
		return v;
	}

	std::string Graphe::getNomSommet(size_t sommet) const {
        if (sommet >= nbSommets || sommet < 0) {
            throw std::logic_error("getNomSommet: sommet trop grand ou sous 0");
        }
		return this->sommets[sommet];
	}

	size_t Graphe::getNumeroSommet(const std::string& nom) const {
        for (size_t i = 0; i < nbSommets; ++i) {
            if (this->sommets[i] == nom) {
                return i;
            }
        }
        throw std::logic_error("getNumeroSommet: Nom introuvable");
	}

	int Graphe::getNombreSommets() const {
		return nbSommets;
	}

	int Graphe::getNombreArcs() const {
        return nbArcs;
	}

	Ponderations Graphe::getPonderationsArc(size_t source, size_t destination) const {
        if (source >= nbSommets || destination >= nbSommets) {
            throw std::logic_error("getPonderationsArc: source ou destination trop grande");
        }
        for (auto it = listesAdj[source].begin(); it !=listesAdj[source].end(); ++it) {
            if (it->destination == destination) {
                return it->poids;
            }
        }
        throw std::logic_error("getPonderationsArc: l'arc n'existe pas");
	}

}//Fin du namespace
