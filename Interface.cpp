#include "Interface.h"
#include <ftxui/component/screen_interactive.hpp>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace ftxui;
using namespace std;

// constructeur
Ui::Ui() : client_("invité") {

   // couleurs utilisées dans l’interface 
    rose_ = Color::RGB(245, 194, 231);
    mauve_ = Color::RGB(203, 166, 247);
    bleu_doux_ = Color::RGB(137, 180, 250);
    vert_clair_ = Color::RGB(166, 227, 161);
    fond_ = Color::RGB(30, 30, 46);
    surface_ = Color::RGB(49, 50, 68);
    alerte_ = Color::RGB(243, 139, 168);
    accent_ = Color::RGB(250, 179, 135);
}

//fonction pour afficher l'historique 
Element Ui::render_historique() {
    Elements lignes;
    lignes.push_back(text(" Historique des transactions") | bold | color(mauve_));
    lignes.push_back(separator());

    const auto& j = magasin_.journal();
    if (j.empty()) {
        lignes.push_back(text("Aucune commande enregistrée") | dim);
    } else {
        // afficher du plus récent au plus ancien
        for (auto it = j.rbegin(); it != j.rend(); ++it) {
            std::istringstream iss(*it);
            std::string ligne;
            while (std::getline(iss, ligne)) {
                lignes.push_back(text(ligne));
            }
            lignes.push_back(separator());
        }
    }

    return window(text(" Historique "), vbox(move(lignes))) | bgcolor(surface_);
}



void Ui::lancer() {

    // lancement de l’interface
    auto screen = ScreenInteractive::FitComponent();
    
    // menu principal 
    vector<string> menu_entries = {"💖 Client", "📦 Produits", "🛒 Panier", "📜 Historique","🛠 Admin"};
    int selected_tab = 0;
    auto menu_principal = Menu(&menu_entries, &selected_tab);

    vector<string> produits_liste;
    int produit_selectionne = 0;

       // quantité choisie
    string saisie_qte = "1";
    auto menu_panier = Menu(&produits_liste, &produit_selectionne);
    auto input_qte = Input(&saisie_qte, "Qté");

    // vider le panier
    auto btn_vider = Button("Vider Panier", [&] {
        client_.panier().vider();
        message_ = "🗑 Panier vidé avec succès";
    });

   // ajout d'un produit
    auto btn_ajouter = Button("Ajouter au Panier", [&] {
        try {
            int qte = stoi(saisie_qte);
            int id_prod = magasin_.produits()[produit_selectionne].id();
            if(magasin_.ajouter_au_panier(client_.panier(), id_prod, qte, message_)) {
                saisie_qte = "1"; 
            }
        } catch (...) { message_ = "⚠ Quantité invalide"; }
    });

    // validation de la commande
    auto btn_valider = Button("Valider Commande", [&] { action_valider_commande(); }
);

    // informations du client + saisie du nom
    // input initialisé avec le nom courant pour faciliter l'édition
    std::string input_nom_client = client_.nom();
    auto input_client = Input(&input_nom_client, "Votre nom");
    auto btn_confirmer = Button("Confirmer", [&] {
        if (!input_nom_client.empty()) {
            client_.set_nom(input_nom_client);
            message_ = "✅ Nom mis à jour";
        } else {
            message_ = "⚠ Nom vide";
        }
        screen.RequestAnimationFrame();
    });

    auto conteneur_client = Container::Vertical({input_client, btn_confirmer});

    auto vue_client = Renderer(conteneur_client, [&] {
        return window(text(" Informations Client "),
                      vbox({
                          text("Connecté en tant que : " + client_.nom()) | color(bleu_doux_),
                          separator(),
                          hbox({text(" Nom : ") | size(WIDTH, EQUAL, 8), input_client->Render() | border, btn_confirmer->Render() | border}),
                          separator(),
                          text("Statut : Prêt à commander") | dim
                      })) | bgcolor(surface_);
    });
    
     // affichage des produits 
    auto vue_produits = Renderer([&] { return render_produits(); });

    auto conteneur_panier = Container::Vertical({menu_panier, input_qte, btn_ajouter, btn_vider, btn_valider});

 // partie panier  
auto vue_panier = Renderer(conteneur_panier, [&] {
        produits_liste.clear();

         // mise à jour de la liste des produits
        for (auto& p : magasin_.produits()) {
            std::string item = p.nom() + " (" + to_string(p.stock()) + " en stock)";
            if (p.stock() < 5) item += " - stock faible";
            produits_liste.push_back(item);
        }
        
        return vbox({
            window(text(" Sélectionner un produit "), menu_panier->Render() | vscroll_indicator | frame | size(HEIGHT, EQUAL, 6)),
            hbox({text(" Quantité : "), input_qte->Render() | border}),
            hbox({
                btn_ajouter->Render() | border, 
                btn_vider->Render() | border, 
                filler(), 
                btn_valider->Render() | border
            }),
            separator(),

            // résumé du panier
            render_panier()
        }) | bgcolor(surface_);
    });

    auto vue_historique = Renderer([&] { return render_historique(); });
    // Variables pour les nouveaux produits (à déclarer au début de lancer())
string nouv_nom = "";
string nouv_prix = "0.0";
string nouv_stock = "0";
string admin_mdp = "";
    bool admin_authenticated = false;

    auto input_nom = Input(&nouv_nom, "Nom du produit");
    auto input_prix = Input(&nouv_prix, "Prix");
    auto input_stock = Input(&nouv_stock, "Stock");
    auto input_mdp = Input(&admin_mdp, "Mot de passe");

    // bouton d'ajout (sera utilisé uniquement si admin_authenticated == true)
    auto btn_ajouter_admin = Button("Ajouter au Catalogue", [&] {
    try {
        double p = stod(nouv_prix);
        int s = stoi(nouv_stock);
            if (!admin_authenticated) {
                message_ = "⚠ Accès refusé : veuillez vous connecter en tant qu'administrateur";
            } else if (!nouv_nom.empty() && p > 0 && s >= 0) {
                // Création et tentative d'ajout via notre méthode sécurisée
                ProduitElectronique prod(magasin_.produits().size() + 1, nouv_nom, "informatique", p, s);
                verifier_et_ajouter(prod);

                // Réinitialisation des champs après succès
                nouv_nom = "";
                nouv_prix = "0.0";
                nouv_stock = "0";
            } else {
                message_ = "⚠ Données invalides (Le prix doit être supérieur à 0)";
            }
    } catch (...) {
        message_ = "⚠ Erreur de saisie dans les champs numériques";
    }
        // FORCE LE RAFRAÎCHISSEMENT DE L'ÉCRAN APRÈS LE CLIC
        screen.RequestAnimationFrame();
    });

    // Login button (shown when not authenticated)
    auto btn_login = Button("Se connecter", [&] {
        if (admin_mdp == "admin123") {
            admin_authenticated = true;
            message_ = "✅ Administrateur authentifié";
        } else {
            message_ = "⚠ Mot de passe administrateur incorrect";
        }
        screen.RequestAnimationFrame();
    });

    // Logout button (optional)
    auto btn_logout = Button("Se déconnecter", [&] {
        admin_authenticated = false;
        admin_mdp = "";
        message_ = "🔒 Administrateur déconnecté";
        screen.RequestAnimationFrame();
    });

    // Layouts
    auto conteneur_admin_auth = Container::Vertical({input_mdp, btn_login});
    auto conteneur_admin_form = Container::Vertical({input_nom, input_prix, input_stock, btn_ajouter_admin, btn_logout});

// Admin component to hold both auth and form for focus management
auto admin_component = Container::Vertical(Components{conteneur_admin_auth, conteneur_admin_form});

// Rendu graphique de la fenêtre Admin : affiche d'abord l'authentification, puis le formulaire
auto vue_admin = Renderer(admin_component, [&] {
    if (!admin_authenticated) {
        return window(text(" [Zone Sécurisée] Authentification ") | bold | color(accent_), 
            vbox({
                hbox(text(" Mot de passe : ") | size(WIDTH, EQUAL, 14), input_mdp->Render() | border),
                separator(),
                hbox({ btn_login->Render() | center }),
                separator(),
                text(" Entrez le mot de passe administrateur pour accéder au formulaire.") | dim | italic
            })
        ) | bgcolor(surface_);
    } else {
        return window(text(" [Zone Sécurisée] Ajouter un Produit au Catalogue ") | bold | color(accent_), 
            vbox({
                hbox(text(" Nom : ") | size(WIDTH, EQUAL, 10), input_nom->Render() | border),
                hbox(text(" Prix : ") | size(WIDTH, EQUAL, 10), input_prix->Render() | border),
                hbox(text(" Stock : ") | size(WIDTH, EQUAL, 10), input_stock->Render() | border),
                separator(),
                hbox({ btn_ajouter_admin->Render() | center, btn_logout->Render() | center }),
                separator(),
                text(" Note : Vous êtes connecté en tant qu'administrateur.") | dim | italic
            })
        ) | bgcolor(surface_);
    }
});

    // navigation entre les pages
    auto main_container = Container::Tab(Components{vue_client, vue_produits, vue_panier, vue_historique, vue_admin}, &selected_tab);

     // affichage principal
    auto main_renderer = Renderer(Container::Horizontal(Components{menu_principal, main_container}), [&] {
        return vbox(Elements{
            text(" boutique en ligne ") | bold | center | color(rose_),
            hbox(Elements{

                  // menu de navigation 
                window(text("navigation") | color(bleu_doux_), menu_principal->Render()) | size(WIDTH, EQUAL, 25),

                // contenu principal
                window(text(menu_entries[selected_tab]) | color(mauve_), main_container->Render() | flex)
            }) | flex,

            // messages affichés à l'utilisateur 
            window(text("messages") | color(vert_clair_),
                   text(message_.empty() ? "pret" : message_) 
                   | color(message_.find("⚠") != std::string::npos ? alerte_ : vert_clair_)
            ) | size(HEIGHT, EQUAL, 5),
            text("realise par MDL") | center | dim
        }) | bgcolor(fond_) | border;
    });

      // boucle principale
    screen.Loop(main_renderer);
}


  // boucle principale 
Element Ui::render_produits() {
    Elements rows;
    for (const auto& p : magasin_.produits()) {
        stringstream ss;
        ss << fixed << setprecision(2) << p.prix() << " DH";
        std::string stock_label = " | stock " + std::to_string(p.stock());
        if (p.stock() < 5) stock_label += " (stock faible)";
        rows.push_back(hbox({
            text("#" + to_string(p.id()) + " " + p.nom()),
            filler(),
            text(ss.str()) | color(vert_clair_),
            text(stock_label) | color(p.stock() < 5 ? alerte_ : bleu_doux_)
        }));
    }
    return window(text(" Catalogue "), vbox(move(rows))) | bgcolor(surface_);
}


// résumé de la facture
Element Ui::render_panier() {
    DetailPrix d = magasin_.calculer_prix(client_.panier());
    auto ligne_style = [](string label, double val, Color col) {
        stringstream ss;
        ss << fixed << setprecision(2) << val << " DH";
        return hbox({ text(label), filler(), text(ss.str()) }) | color(col);
    };

    return window(text(" Résumé Facture "), vbox({
        ligne_style("sous-total : ", d.sous_total, bleu_doux_),
        ligne_style("remise 10% : ", -d.remise, accent_),
        ligne_style("tva 20%    : ", d.tva, mauve_),
        separator(),
        ligne_style("total ttc  : ", d.total_ttc, vert_clair_) | bold
    }));
}


// validation de la commande
  void Ui::action_valider_commande() {
    // 1. Vérifier si le panier est vide
    if (client_.panier().vide()) {
        message_ = "⚠ Le panier est vide !";
        return;
    }

    DetailPrix d = magasin_.calculer_prix(client_.panier());
    stringstream ss;

    // 2. Construction d'un ticket de caisse détaillé
    // ajout de la date et heure
    time_t now = time(nullptr);
    char buf[64];
    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now))) {
        ss << "Date: " << buf << "\n";
    }
    ss << "========================================\n";
    ss << " COMMANDE CLIENT : " << client_.nom() << "\n";
    ss << "========================================\n";
    ss << "Détail des articles :\n";

    // On boucle sur les lignes du panier
    for (const auto& ligne : client_.panier().lignes()) {
        // cherche le produit dans le magasin pour avoir son nom et son prix unitaire
        const Produit* p = magasin_.trouver(ligne.produit_id);
        if (p) {
            ss << "- " << p->nom() 
               << " (x" << ligne.quantite << ")"
               << " : " << fixed << setprecision(2) << (p->prix() * ligne.quantite) << " DH\n";
        }
    }

    ss << "----------------------------------------\n";
    ss << "Sous-total : " << d.sous_total << " DH\n";
    if (d.remise > 0) {
        ss << "Remise 10% : -" << d.remise << " DH\n";
    }
    ss << "TVA 20%    : " << d.tva << " DH\n";
    ss << "TOTAL TTC  : " << d.total_ttc << " DH\n"; 
    ss << "========================================\n\n";

    // 3. Sauvegarde de la chaîne complète  dans le journal
    magasin_.sauvegarder_journal(ss.str());

    // 4. Vider le panier après la validation
    client_.panier().vider();
    message_ = "📦 Commande validée et enregistrée !";
}
void Ui::verifier_et_ajouter(const ProduitElectronique& nouveau_produit) {
    // l'accès est maintenant contrôlé par le mot de passe dans l'UI
    magasin_.ajouter_nouveau_produit(nouveau_produit);
    message_ = "💾 [Admin] Nouveau produit ajouté au catalogue avec succès !";
}
















