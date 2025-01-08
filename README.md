# Projet TCP/UDP : Client et Serveur

Ce projet met en œuvre des fichiers de sockets en C pour les systèmes serveur et client. Elles sont conçues pour les protocoles de transport TCP et UDP, permettant de simuler des interactions avec un système bancaire via des commandes simples.

## Fonctionnalités

- **AJOUT** _<id_client id_compte password somme>_    : Ajout d'un montant au compte.
- **RETRAIT** _<id_client id_compte password somme>_ : Retrait d'un montant du compte.
- **SOLDE** _<id_client id_compte password>_         : Consultation du solde du compte.
- **OPERATIONS** _<id_client id_compte password>_    : Liste des dernières opérations 



## Pré-requis

- **Compilateur C** : `gcc`, `g++` ou équivalent.
- **Système Unix/Linux** : Recommandé pour l'exécution du Makefile.

## Installation

1. Clonez ce dépôt :
   ```bash
   git clone https://github.com/dan-lara/bank-account-project.git
   cd bank-account-project
   ```

### Utilisation TCP

1. **Compiler** :
    ```bash
   cd TCP/
   make #or make redo
   ```
2. **Démarrez le serveur** :
   ```bash
   ./serveur
   ```
3. **Exécutez le client** et entrez vos commandes :
   ```bash
   ./client 127.0.0.1 "USERNAME" #ex : "DanielLara"
   # le système accepte plusiers clients
   ```

### Utilisation UDP

1. **Compiler** :
    ```bash
   cd UDP/
   make #or make redo
   ```
2. **Démarrez le serveur** :
   ```bash
   ./serveur
   ```
3. **Exécutez le client** et entrez vos commandes :
   ```bash
   ./client 127.0.0.1 "USERNAME" #ex : "DanielLara"
   # le système accepte plusiers clients
   ```
   
### Commandes exemples pour les clients
   ```bash
# Voir tous les exemples sur le fichier Samples.txt
AJOUT DanielLara 00556789 mdpDaniel 15320
RETRAIT DanielLara 00556789 mdpDaniel 3900
SOLDE DanielLara 00556789 mdpDaniel
OPERATIONS DanielLara 00556789 mdpDaniel
   ```

### Commandes Makefile

- `make` ou `make tout` : Compile tout le projet.
- `make client` : Compile uniquement le client.
- `make serveur` : Compile uniquement le serveur.
- `make nettoyer` : Supprime les fichiers objets.
- `make purger` : Supprime les fichiers objets et les exécutables.
- `make redo` : Recompile tout depuis zéro.
- `make help` : Affiche les commandes disponibles.

## Contribuer

Les contributions sont les bienvenues ! Suivez ces étapes :

1. **Forkez** le projet.
2. Créez une branche pour vos modifications :
   ```bash
   git checkout -b feature/nouvelle-fonctionnalite
   ```
3. Commitez vos changements :
   ```bash
   git commit -m "Ajout d'une nouvelle fonctionnalité"
   ```
4. Poussez vos modifications :
   ```bash
   git push origin feature/nouvelle-fonctionnalite
   ```
5. Ouvrez une **Pull Request**.