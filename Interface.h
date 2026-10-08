#ifndef INTERFACE_H
#define INTERFACE_H

// Inclusion de la classe Magasin (logique métier du projet)
#include "Magasin.h"

// Bibliothèques FTXUI pour créer une interface en terminal (TUI)
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

// Classe principale qui gère l'interface utilisateur
class Ui {
public:
    Ui();    // Constructeur : initialise l'interface
    void lancer();   // Lance l'application (boucle principale) 

private:

    // Méthodes de rendu
    ftxui::Element render_produits();
    ftxui::Element render_panier();
    ftxui::Element render_historique();
    void action_valider_commande();
void verifier_et_ajouter(const ProduitElectronique& nouveau_produit);

    // Données du magasin
  Magasin magasin_;
    Client client_;
    std::string message_;

    // Palette de couleurs complète
    ftxui::Color rose_, mauve_, bleu_doux_, vert_clair_;
    ftxui::Color fond_, surface_, alerte_, accent_, texte_gris_;
};

#endif
