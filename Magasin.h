#ifndef MAGASIN_H
#define MAGASIN_H

#include <string>
#include <vector>
#include <ctime>

// contient les informations de calcul du panier
struct DetailPrix {
    double sous_total = 0.0;
    double remise = 0.0;
    double total_ht = 0.0;
    double tva = 0.0;
    double total_ttc = 0.0;
};

// représente un produit du magasin
class Produit {
public:
    Produit(int id, std::string nom, std::string categorie, double prix, int stock)
        : id_(id), nom_(nom), categorie_(categorie), prix_(prix), stock_(stock) {}
        
    virtual ~Produit() = default;

    int id() const { return id_; }
    const std::string& nom() const { return nom_; }
    std::string categorie() const { return categorie_; }
    double prix() const { return prix_; }
    int stock() const { return stock_; }
    void diminuer_stock(int q) { stock_ -= q; }
    void augmenter_stock(int q) { stock_ += q; }
    virtual std::string get_description_complete() const {
        return nom_ + " (" + categorie_ + ")";
    }

protected: 
    int id_; 
    std::string nom_, categorie_; 
    double prix_; 
    int stock_;
};
class ProduitElectronique : public Produit {
public:
    ProduitElectronique(int id, std::string nom, std::string categorie, double prix, int stock, int garantie_mois = 24)
        : Produit(id, nom, categorie, prix, stock), garantie_mois_(garantie_mois) {}
    // Redéfinition polymorphique
    std::string get_description_complete() const override {
        return nom_ + " (Garantie : " + std::to_string(garantie_mois_) + " mois)";
    }
private:
    int garantie_mois_;
};
// Deuxième classe dérivée : Produit Alimentaire
class ProduitAlimentaire : public Produit {
public:
    ProduitAlimentaire(int id, std::string nom, std::string categorie, double prix, int stock, std::string date_peremption)
        : Produit(id, nom, categorie, prix, stock), date_peremption_(date_peremption) {}

    // Redéfinition polymorphique
    std::string get_description_complete() const override {
        return nom_ + " (À consommer avant le : " + date_peremption_ + ")";
    }
private:
    std::string date_peremption_;
};

// Troisième classe dérivée : Produit en Promotion
class ProduitEnPromotion : public Produit {
public:
    ProduitEnPromotion(int id, std::string nom, std::string categorie, double prix, int stock, int remise_pourcent)
        : Produit(id, nom, categorie, prix, stock), remise_pourcent_(remise_pourcent) {}

    // Redéfinition polymorphique
    std::string get_description_complete() const override {
        return nom_ + " [PROMO -" + std::to_string(remise_pourcent_) + "% !]";
    }
private:
    int remise_pourcent_;
};

// gère les produits ajoutés par le client
class Panier {
public:
    struct Ligne { int produit_id; int quantite; };
    void ajouter(int id, int q);
    void vider() { lignes_.clear(); }
    bool vide() const { return lignes_.empty(); }
    const std::vector<Ligne>& lignes() const { return lignes_; }
    friend std::ostream& operator<<(std::ostream& os, const Panier& panier);

private:
    std::vector<Ligne> lignes_;
};

// Classe de base pour tous les utilisateurs du système
class Utilisateur {
public:
    Utilisateur(std::string nom, std::string role) : nom_(nom), role_(role) {}
    virtual ~Utilisateur() = default;

    std::string nom() const { return nom_; }
    std::string role() const { return role_; }
    void set_nom(std::string n) { nom_ = n; }

    // Méthode virtuelle polymorphique
    virtual std::string get_permissions() const {
        return "Accès de base au catalogue.";
    }

protected:
    std::string nom_;
    std::string role_;
};

// La classe Client hérite publiquement de Utilisateur
class Client : public Utilisateur {
public:
    explicit Client(std::string nom) : Utilisateur(nom, "Client") {}

    Panier& panier() { return panier_; }

    // Redéfinition polymorphique des permissions
    std::string get_permissions() const override {
        return "Boutique : Consulter, Ajouter au panier, Passer commande.";
    }

private:
    Panier panier_;
};

// Nouvelle classe Administrateur qui hérite aussi de Utilisateur
class Administrateur : public Utilisateur {
public:
    explicit Administrateur(std::string nom) : Utilisateur(nom, "Administrateur") {}

    // Redéfinition polymorphique des permissions
    std::string get_permissions() const override {
        return "Back-Office : Gestion totale des stocks et modification des prix.";
    }
}; 

// logique principale du magasin
class Magasin {
public:
    Magasin();
    std::vector<Produit>& produits() { return produits_; }
    Produit* trouver(int id);
    const Produit* trouver(int id) const;
    bool ajouter_au_panier(Panier& p, int id, int q, std::string& msg);
    DetailPrix calculer_prix(const Panier& p) const;
    void sauvegarder_journal(const std::string& log);
    const std::vector<std::string>& journal() const { return journal_; }
    void ajouter_nouveau_produit(const Produit& p) {
    produits_.push_back(p);
    }
private:
    std::vector<Produit> produits_;
    std::vector<std::string> journal_;
};

#endif
