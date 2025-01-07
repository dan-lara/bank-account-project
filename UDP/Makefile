# Configuration du compilateur et des options
COMPILATEUR = gcc
OPTIONS = -I.

# Définition des programmes
PROGRAMMES = client serveur

# Fichiers source
SOURCES_CLIENT = client.c
SOURCES_SERVEUR = serveur.c

# Génération automatique des noms de fichiers objets
OBJETS_CLIENT = $(SOURCES_CLIENT:.c=.o)
OBJETS_SERVEUR = $(SOURCES_SERVEUR:.c=.o)

# Tous les fichiers objets combinés
TOUS_OBJETS = $(OBJETS_CLIENT) $(OBJETS_SERVEUR)

# Règle par défaut
.DEFAULT_GOAL := tout

# Compilation complète
tout: message_debut $(PROGRAMMES) message_fin

# Message de début de compilation
message_debut:
	@echo "🚀 Démarrage de la compilation du UDP..."

# Message de fin de compilation
message_fin:
	@echo "✨ Compilation terminée avec succès!"

# Règle pour le client
client: $(OBJETS_CLIENT)
	@echo "🔨 Assemblage du client..."
	@$(COMPILATEUR) $(OPTIONS) -o $@ $^
	@echo "✅ Client prêt!"

# Règle pour le serveur
serveur: $(OBJETS_SERVEUR)
	@echo "🔨 Assemblage du serveur..."
	@$(COMPILATEUR) $(OPTIONS) -o $@ $^
	@echo "✅ Serveur prêt!"

# Compilation des fichiers source
%.o: %.c
	@echo "📝 Compilation de $<..."
	@$(COMPILATEUR) $(OPTIONS) -c $< -o $@

# Nettoyage simple
nettoyer:
	@echo "🧹 Nettoyage des fichiers temporaires..."
	@rm -f $(TOUS_OBJETS)
	@echo "✨ Nettoyage terminé"

# Nettoyage complet
purger: nettoyer
	@echo "🗑️  Suppression des exécutables..."
	@rm -f $(PROGRAMMES)
	@echo "✨ Purge terminée"

# Recompilation complète
redo: purger tout

# Affichage de l'help
help:
	@echo "📚 Guide d'utilisation du Makefile:"
	@echo "  make ou make tout    → Compile tout le projet"
	@echo "  make client         → Compile uniquement le client"
	@echo "  make serveur        → Compile uniquement le serveur"
	@echo "  make nettoyer       → Supprime les fichiers objets"
	@echo "  make purger         → Supprime tout (objets et programmes)"
	@echo "  make redo    → Recompile tout depuis zéro"
	@echo "  make help           → Affiche ce message"

# Déclarer les cibles qui ne sont pas des fichiers
.PHONY: tout message_debut message_fin nettoyer purger redo help