# 🛒 Mini E-Commerce en C++

## 📌 Description

Ce projet est une application e-commerce développée en C++ moderne.
L’application permet de gérer un catalogue de produits, un panier d’achat,
les commandes des clients ainsi qu’un historique des achats.

Le projet utilise la programmation orientée objet (POO),
la bibliothèque STL et une interface utilisateur terminal moderne avec FTXUI.

---

# 🚀 Fonctionnalités

✅ Gestion du catalogue produits  
✅ Gestion du stock  
✅ Ajout au panier  
✅ Calcul automatique des prix  
✅ Calcul TVA et remises  
✅ Validation des commandes  
✅ Historique des commandes  
✅ Interface utilisateur interactive  
✅ Gestion des erreurs utilisateur  

---

# 🧱 Architecture du Projet

```text
.
├── CMakeLists.txt
├── main.cpp
├── Interface.cpp
├── Interface.h
├── Magasin.cpp
├── Magasin.h
├── README.md
└── .gitignore
```

---

# 🛠️ Technologies utilisées

- C++17
- STL
- Programmation Orientée Objet
- FTXUI
- CMake
- Git / GitHub

---

# ⚙️ Compilation
###.l'organisation et la vérification de ton environnement:
Commencez par télécharger le code.
<img width="520" height="400" alt="image" src="https://github.com/user-attachments/assets/512880e2-9fc4-4bf7-9b89-e8df00f75e41" />

Aprés :
 Se placer à la racine du projetC'est l'étape de base. Tu dois être dans le dossier qui contient ton fichier main.cpp et ton CMakeLists.txt.
par exemple :  
```bash
 cd /c/Users/HP/Downloads/MINIboutique
```

Pour ceux qui possèdent MSYS2 MINGW64 Il est bon de l'utiliser parce que c' est l'environnement idéal pour compiler un projet C++ sur Windows car il offre une expérience proche de Linux tout en utilisant le compilateur GCC de manière native
## 1. Créer le dossier build

```bash
mkdir build
cd build
```
 ❗pour MSYS2 MINGW64 utilisateur
   ```bash
    cmake -G "MinGW Makefiles" ..
   ```
   (Ignorez la deuxième étape et passez à la troisième.)
## 2. Générer le projet avec CMake

```bash
cmake ..
```

## 3. Compiler

```bash
make
```

---

# ▶️ Exécution

```bash
./boutique_app.exe
```
# 📖 Utilisation du programme
## 📷 Figure 1 — Menu principal

<img width="448" height="335" alt="image" src="https://github.com/user-attachments/assets/735dd981-9989-48ee-896b-e90ba966096a" />




Cette interface représente le menu principal de l’application .
La barre de navigation située à gauche permet d’accéder aux différentes sections du programme :
- Client
- Produits
- Panier
- Historique

L’utilisateur peut naviguer facilement entre les différentes fonctionnalités grâce à l’interface interactive développée avec FTXUI.
---


## 📦 Figure 2 — Catalogue des produits

<img width="584" height="384" alt="image" src="https://github.com/user-attachments/assets/493f8862-4479-496d-ae24-e18efd955bd8" />


Cette interface affiche le catalogue des produits disponibles dans le magasin.

Chaque produit possède :
- un identifiant,
- un nom,
- un prix,
- une quantité en stock.

Le système signale automatiquement les produits ayant un stock faible afin d’améliorer la gestion des articles.

---
## 🛒 Figure 3 — Gestion du panier

<img width="651" height="521" alt="image" src="https://github.com/user-attachments/assets/22e7ddfe-f549-45bf-8532-e459f0e0a87a" />


Cette interface permet à l’utilisateur de gérer son panier d’achat.

L’utilisateur peut :
- sélectionner un produit,
- saisir une quantité,
- ajouter un produit au panier,
- vider le panier,
- valider une commande.

Le programme calcule automatiquement :
- le sous-total,
- les remises,
- la TVA,
- le total TTC.
---
## 🔐Figure 4 — Section Administration

<img width="737" height="323" alt="image" src="https://github.com/user-attachments/assets/79da7d2a-34fe-47be-9247-3fe61913c8f3" />


Aprés ocnnecter a le mode L’administrateur par code :admin123  

<img width="622" height="407" alt="image" src="https://github.com/user-attachments/assets/1dd3f39d-1601-4898-bc58-5ec5205f216d" />


vous pouvez ajouter dynamiquement de nouveaux produits au catalogue grâce à un formulaire interactif développé avec FTXUI.

L’administrateur peut saisir :
- le nom du produit,
- le prix,
- la quantité disponible en stock.

Cette fonctionnalité améliore la gestion du catalogue et introduit une séparation entre les fonctionnalités client et administrateur.

---

## 📜 Figure 5 — Historique des commandes

<img width="639" height="491" alt="image" src="https://github.com/user-attachments/assets/49f7414d-ea41-4b05-97dd-bd091195455e" />


Cette interface affiche l’historique des commandes effectuées par le client.

Après la validation d’une commande, les informations suivantes sont enregistrées :
- l’identifiant de la commande,
- la date et l’heure,
- les produits achetés,
- les quantités,
- le montant total TTC.
  
Vous trouverez tout cela dans le fichier Journal de votre fichier builld.

Cette fonctionnalité permet au client de consulter les anciennes commandes et améliore le suivi des achats réalisés dans l’application.



# 📚 Concepts POO utilisés

- Encapsulation
- Abstraction
- Composition
- Modularité
- Gestion des classes

---


un Rapport sur le projet disponible sur le page  suivant :https://statuesque-madeleine-c942b3.netlify.app/


---

# 🎨 Interface

L’interface utilisateur est réalisée avec la bibliothèque FTXUI afin de proposer une expérience moderne directement dans le terminal.

---

# 👨‍💻 Auteur

Projet réalisé par MDL.
Maryem, Doaa & Laila






































































