Interface et fonctionnalité:

Belle interface! Bien de pouvoir placer les pièces arbitrairement mais la fonctionnalité doit être complétée (pas de vérification sur la cohérence du jeu).

Le programme s'interrompt de manière inattendu pendant l'exécution.

Je ne suis pas sûre de la fonctionnalité du bouton pour la rotation?

~~Reste à implémenter la logique du jeu.~~

Implémentation:

Ne pas oublier pas les tests.

Bonne utilisation des struct et enum class!

Les connexions à l'interface Qt devraient être abstraitent dans le code du controleur.

~~Le Piece::getType() peut être implémenter dans la Piece, si on ajoute un attribut pour sauvegarder le type d'une piece.~~

Quelques répétitions de code, pensez à utiliser des structures de données comme des tableaux, et des sous-fonctions.

Pratique de programmation/lisibilité/propreté:

Attention aux chiffres magiques.

Certains const et passage par référence sont à ajouter dans les paramètres.

~~Mettre à jour les en-têtes de fichier..? King.cpp~~

Mettre les conditions booléennes dans des variables pour améliorer la lisibilité du code (le nom de la variable va apporter des informations sur le but de la condition).

~~Enlevez la macro #pragma once de Piece.h.~~