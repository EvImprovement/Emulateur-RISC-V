# Projet RISC-V IN330

Auteurs : GRABE Evan et EYMERY Léa

Planifiez-vous de sauter l'étape 5 (oui/non) ? : Non

## Étape 2 (réponses aux exercices préliminaires)

[1 - 101101100000011000110011, le bit #0 est à droite, sa valeur est 51
 2 - Sa valeur est de 12s
 5- On répète le bit de signe sur les 4 nouveaux bits.
 ]

## Étapes 3 et 4 (émulateur de base)

* Avez-vous compris le fonctionnement de chaque instruction à partir de la documentation fournie ? Si non, quels sont les points obscurs ?

[Nous avons bien compris le fonctionnement des insctructions]

* Quels exemples de programmes avez-vous choisi pour tester le calcul ? Les comparaisons et sauts ? La mémoire ?

[Nous avons écris des tests pour toutes les insctructions du set d'insctruction donc des opérations arithmétiques avec add,sub addi, des comparaisons avec ble,bge,blt,bne, des sauts avec jal et des manipulations de mémoire avec des ld et des sw]

* Reste-t-il des bugs que vous avez découverts et pas corrigés ?

[Pour ld et sd il reste à implémenter la gestion des cas où la valeur est signée]

* D'autres remarques sur votre programme ?

[Non]

* Cochez (en remplaçant `[ ]` par `[x]`) si vous avez :**
  - [x] Implémenté l'émulation de toutes les instructions de `doc_riscv_projetC.pdf`
  - [x] Tous vos tests qui passent.
  - [x] Vérifié que vous tests couvrent toutes les instructions émulées.
  - [x] Testé les cas particuliers : valeurs négatives, overflows...
  - [ ] Un port fonctionnel de DOOM pour votre émulateur.

* Des retours sur le projet en général ?

[COMPLÉTER ICI]

## Étape 5 (extension de l'émulateur)

* Racontez ici ce que vous avez fait pour cette étape et/ou toutes informations utiles pour qu'on comprenne ce qui marche ou pas.

[Le but est d'éxécuter ligne par ligne le code de blob.hex et d'implémenter les instructions manquantes. Voici comment nous avons procédé :
  - On a rajouté une condition d'arrêt sur la lecture des instructions pour les isoler
  - On affiche ensuite leur opcode,funct3,funct7 et on regarde dans le jeu d'instruction RISC-V l'instruction à implémenter
  - On rajoute un case à notre switch et on crée une fonction do pour implémenter l'instruction
  - On répéte jusqu'à ce que la dernière instruction soit lue ie la fin.
  - On utilise la commande : ./riscv-emulator blob.hex blob.state >/dev/null pour isoler les ECALL
  - On admire le magnifique logo RISC-V affiché en couleur sur notre terminal]
