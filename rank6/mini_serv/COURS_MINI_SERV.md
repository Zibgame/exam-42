# MINI_SERV — cours de survie pour l'examen Rank 6

## 1. Le but du programme en une phrase

`mini_serv` est un petit serveur de discussion local : plusieurs clients se
connectent sur `127.0.0.1`, et chaque ligne envoyée par un client est transmise
à tous les autres clients avec son identifiant devant.

Le sujet paraît impressionnant parce qu'il utilise des sockets, mais la logique
réelle tient en quatre actions :

1. créer le serveur ;
2. attendre une activité avec `select` ;
3. accepter les nouveaux clients ;
4. recevoir, découper et diffuser leurs messages.

Le fichier `mini_serv.c` présent dans ce dossier sert de solution courte de
référence. Le but de ce cours est de comprendre son squelette, pas de le rendre
plus compliqué.

### Attention sur la copie actuellement présente

Le test de compilation effectué sans modifier `mini_serv.c` a trouvé deux
petites différences à corriger sur une copie d'examen :

```c
/* ligne actuelle : une accolade fermante est en trop */
a.sin_port=htons(atoi(av[1]))};

/* forme correcte */
a.sin_port = htons(atoi(av[1]));
```

Et, pour compiler avec `-Wall -Wextra -Werror`, utilise le type attendu par
`bind` :

```c
bind(sfd, (struct sockaddr *)&a, sizeof(a))
```

Avec ces deux corrections injectées uniquement dans une compilation temporaire,
les tests fonctionnels de connexion, message fragmenté, plusieurs lignes,
déconnexion et nouvel identifiant passent. GCC émet encore un avertissement
`format-overflow` sous `-Wall -Wextra` parce que `sb` et un `buf[fd]` ont presque
la même taille : au maximum théorique, le préfixe ajouté par `sprintf` pourrait
dépasser `sb`. La logique attendue fonctionne sur les cas normaux, mais il faut
absolument vérifier les flags réellement employés le jour de l'examen.

---

## 2. Ce que le sujet exige exactement

Le programme reçoit un seul argument : le port.

```bash
./mini_serv 8080
```

Il doit :

- écouter uniquement sur `127.0.0.1` ;
- accepter plusieurs clients ;
- attribuer les identifiants `0`, `1`, `2`, etc. dans l'ordre des connexions ;
- annoncer une connexion aux anciens clients ;
- annoncer une déconnexion aux clients encore présents ;
- ajouter `client ID: ` devant chaque ligne reçue ;
- ne jamais renvoyer un message à son auteur ;
- conserver un morceau de ligne reçu sans `\n` jusqu'à la réception suivante ;
- gérer plusieurs lignes reçues en un seul `recv` ;
- ne pas avoir de fuite de descripteur ni de mémoire.

Messages exacts à connaître :

```text
Wrong number of arguments
Fatal error
server: client %d just arrived
server: client %d just left
client %d: 
```

Les majuscules, espaces, deux-points et retours à la ligne comptent.

---

## 3. Le vocabulaire minimal

### Un socket

Un socket est un descripteur de fichier utilisé pour communiquer sur le réseau.
Comme un fichier, il est représenté par un entier appelé `fd`.

Dans ce programme :

- `sfd` est le socket du serveur ;
- chaque client accepté possède son propre `fd` ;
- `close(fd)` ferme un client ;
- le numéro du `fd` n'est pas l'identifiant du client.

Cette dernière distinction est importante. Un `fd` peut être réutilisé par le
système après un `close`, alors que les identifiants des clients continuent à
augmenter.

### Une adresse IPv4

La structure `sockaddr_in` décrit où le serveur écoute :

```c
struct sockaddr_in a;
a.sin_family = AF_INET;
a.sin_addr.s_addr = htonl(2130706433);
a.sin_port = htons(atoi(av[1]));
```

- `AF_INET` signifie IPv4 ;
- `2130706433` correspond à `127.0.0.1` ;
- `atoi(av[1])` transforme le port en entier ;
- `htonl` et `htons` placent les nombres dans l'ordre d'octets du réseau.

Le début fourni le jour de l'examen contient normalement cette partie. Il faut
surtout savoir la reconnaître et la recopier proprement.

### Un ensemble de descripteurs

`fd_set` est un ensemble de `fd` surveillés par `select`.

Les quatre outils à connaître :

```c
FD_ZERO(&fds);       // vide l'ensemble
FD_SET(fd, &fds);    // ajoute fd
FD_CLR(fd, &fds);    // retire fd
FD_ISSET(fd, &fds);  // teste si fd est présent
```

---

## 4. Les variables globales du code

```c
int ids[1024], id, sfd, mx;
char sb[600000], mb[600000], buf[1024][600000];
fd_set fds;
```

### `ids[fd]`

Associe un descripteur à l'identifiant logique du client.

Exemple : le client d'identifiant `0` peut avoir le `fd` 4. On stocke alors :

```c
ids[4] = 0;
```

### `id`

Contient le prochain identifiant disponible. Les variables globales commencent
à zéro, donc le premier client reçoit naturellement l'identifiant `0`.

Après :

```c
ids[c] = id++;
```

le client reçoit la valeur actuelle, puis `id` augmente.

### `sfd`

Le descripteur du socket serveur. Quand `select` indique que `sfd` est prêt,
cela signifie qu'une nouvelle connexion attend un `accept`.

### `mx`

`select` attend le plus grand descripteur surveillé **plus un**.

```c
mx = sfd + 1;
if (c >= mx)
    mx = c + 1;
```

On parcourt donc les descripteurs avec `fd < mx`.

### `sb`

`send buffer` : message complet qui va être diffusé aux clients.

### `mb`

`message buffer` : données tout juste reçues par le dernier appel à `recv`.
Elles ne forment pas forcément une ligne complète et ne se terminent pas
forcément par `\0`. Voilà pourquoi le code utilise exactement les `r` octets
retournés par `recv`.

### `buf[fd]`

Tampon personnel de chaque client. Il conserve sa ligne en construction entre
plusieurs appels à `recv`.

Sans un tampon par client, les morceaux de messages de deux clients pourraient
se mélanger.

### `fds`

La liste permanente des sockets actuellement surveillés : le serveur et tous
les clients connectés.

---

## 5. Les deux petites fonctions

### `err`

```c
void err(char *s) {
    write(2, s, strlen(s));
    exit(1);
}
```

Elle écrit sur le descripteur `2`, donc sur `stderr`, puis quitte avec le statut
`1`.

Utilisations :

- mauvais nombre d'arguments : `Wrong number of arguments\n` ;
- erreur fatale avant la boucle : `Fatal error\n` ;
- erreur de `select` : `Fatal error\n`.

### `cast`

```c
void cast(int ex) {
    for (int i = 0; i < mx; i++)
        if (i != ex && i != sfd && FD_ISSET(i, &fds))
            send(i, sb, strlen(sb), 0);
    bzero(sb, sizeof(sb));
}
```

`cast` signifie ici « diffuser ».

Elle envoie `sb` à tous les clients valides sauf :

- `ex`, le client à exclure ;
- `sfd`, car on n'envoie pas un message au socket d'écoute ;
- les nombres qui ne correspondent pas à un client présent dans `fds`.

Après l'envoi, `sb` est vidé pour préparer le prochain message.

Cas importants :

- arrivée : `cast(c)` exclut le nouveau client ; seuls les anciens sont avertis ;
- message : `cast(fd)` exclut l'auteur ;
- départ : après `FD_CLR(fd, &fds)`, `cast(fd)` avertit les autres clients.

---

## 6. La construction du serveur

### Étape 1 — vérifier les arguments

```c
if (ac != 2)
    err("Wrong number of arguments\n");
```

`ac` doit valoir 2 : le nom du programme et le port.

### Étape 2 — créer le socket

```c
sfd = socket(AF_INET, SOCK_STREAM, 0);
```

- `AF_INET` : IPv4 ;
- `SOCK_STREAM` : TCP ;
- `0` : protocole choisi automatiquement.

Une valeur négative indique une erreur.

### Étape 3 — préparer l'adresse

Le serveur écoute sur `127.0.0.1` et sur le port donné en argument.

### Étape 4 — attacher puis écouter

```c
if (bind(sfd, &a, sizeof(a)) < 0 || listen(sfd, 10) < 0)
    err("Fatal error\n");
```

- `bind` attache le socket à l'adresse et au port ;
- `listen` transforme le socket en socket d'écoute ;
- `10` est la taille de la file d'attente des connexions.

### Étape 5 — initialiser `select`

```c
mx = sfd + 1;
FD_ZERO(&fds);
FD_SET(sfd, &fds);
fd_set rf;
```

Au début, seul le serveur doit être surveillé.

---

## 7. Comprendre `select`, le cœur de l'exercice

`select` attend qu'au moins un descripteur soit prêt à être lu.

```c
rf = fds;
if (select(mx, &rf, 0, 0, 0) < 0)
    err("Fatal error\n");
```

Pourquoi deux ensembles ?

- `fds` est la liste permanente ;
- `rf` est une copie temporaire donnée à `select`.

`select` modifie l'ensemble reçu : après son retour, `rf` ne contient plus que
les descripteurs prêts. Si on lui donnait directement `fds`, on perdrait les
autres clients à chaque tour.

Les arguments sont :

```c
select(nombre_de_fd, lecture, écriture, exception, timeout);
```

Ici, on surveille uniquement la lecture et on met les autres arguments à zéro.
Un timeout nul sous cette forme signifie attendre sans limite jusqu'à une
activité.

Puis on cherche les descripteurs prêts :

```c
for (int fd = 0; fd < mx; fd++) {
    if (!FD_ISSET(fd, &rf))
        continue;
    /* traiter fd */
}
```

Il existe deux possibilités :

- `fd == sfd` : une nouvelle connexion attend ;
- sinon : un client a envoyé des données ou s'est déconnecté.

---

## 8. Cas 1 : un client arrive

```c
int c = accept(sfd, 0, 0);
```

`accept` crée un nouveau socket connecté au client. `sfd` reste disponible pour
accepter les clients suivants.

Ensuite :

```c
if (c < 0)
    continue;
if (c >= mx)
    mx = c + 1;
FD_SET(c, &fds);
ids[c] = id++;
sprintf(sb, "server: client %d just arrived\n", ids[c]);
cast(c);
bzero(buf[c], sizeof(buf[c]));
continue;
```

Ordre à mémoriser :

1. accepter ;
2. mettre à jour `mx` ;
3. ajouter le client à `fds` ;
4. attribuer son identifiant ;
5. préparer et diffuser le message d'arrivée ;
6. vider son tampon personnel.

Le `continue` est utile : après avoir traité le socket serveur, on ne doit pas
essayer de faire `recv` dessus comme s'il s'agissait d'un client.

---

## 9. Cas 2 : un client part

```c
int r = recv(fd, mb, sizeof(mb), 0);
if (r <= 0) {
    FD_CLR(fd, &fds);
    sprintf(sb, "server: client %d just left\n", ids[fd]);
    cast(fd);
    close(fd);
    bzero(buf[fd], sizeof(buf[fd]));
    continue;
}
```

`recv` retourne :

- un nombre positif : nombre d'octets reçus ;
- `0` : le client a fermé la connexion ;
- une valeur négative : erreur.

Dans cette solution courte, `r <= 0` est traité comme un départ.

Ordre à retenir : retirer, annoncer, fermer, nettoyer.

Le point essentiel pour les fuites de descripteurs est `close(fd)`.

---

## 10. Cas 3 : un client envoie du texte

Le réseau ne respecte pas forcément les lignes. Un appel à `recv` peut recevoir :

- une demi-ligne : `bonj` ;
- la suite : `our\n` ;
- plusieurs lignes : `un\ndeux\ntrois\n` ;
- des lignes complètes plus le début de la suivante : `un\ndeux\ntro`.

Il faut donc lire caractère par caractère et attendre `\n`.

```c
int x = strlen(buf[fd]);
for (int i = 0; i < r; i++) {
    if (x < 599999)
        buf[fd][x++] = mb[i];
    if (mb[i] == '\n') {
        buf[fd][x] = 0;
        sprintf(sb, "client %d: %s", ids[fd], buf[fd]);
        cast(fd);
        bzero(buf[fd], sizeof(buf[fd]));
        x = 0;
    }
}
```

Déroulement :

1. `x` reprend la longueur déjà conservée pour ce client ;
2. chaque nouvel octet est ajouté à son tampon ;
3. tant qu'il n'y a pas `\n`, rien n'est envoyé ;
4. sur `\n`, le tampon devient une chaîne terminée par `\0` ;
5. le préfixe et la ligne sont placés dans `sb` ;
6. la ligne est diffusée ;
7. le tampon client est vidé et `x` repart à zéro ;
8. la boucle continue, car le même `recv` peut contenir une autre ligne.

Pourquoi tester `mb[i] == '\n'` et non chercher `\n` avec `strlen(mb)` ? Parce
que `recv` ne garantit pas que `mb` se termine par `\0`.

---

## 11. Simulation complète

### Connexions

- A se connecte : il reçoit l'id `0`. Personne d'autre n'est averti.
- B se connecte : il reçoit l'id `1`. A reçoit :

```text
server: client 1 just arrived
```

### Ligne fragmentée

A envoie d'abord `sal`, sans retour à la ligne.

- `buf[A]` devient `sal` ;
- aucun message n'est diffusé.

A envoie ensuite `ut\n`.

- `buf[A]` devient `salut\n` ;
- B reçoit :

```text
client 0: salut
```

### Plusieurs lignes d'un coup

B envoie `oui\nnon\n` dans un seul `recv`.

A reçoit deux diffusions :

```text
client 1: oui
client 1: non
```

Le préfixe est bien répété devant chaque ligne.

### Déconnexion

B ferme sa connexion. A reçoit :

```text
server: client 1 just left
```

Si un nouveau client C arrive ensuite, il obtient l'id `2`, pas `1`.

---

## 12. Les mini-cas que le correcteur peut tester

### Aucun argument ou trop d'arguments

Résultat exact sur `stderr`, puis statut `1` :

```text
Wrong number of arguments
```

### Premier client

Il reçoit l'id `0`. Il ne reçoit pas sa propre annonce d'arrivée.

### Nouveau client

Les anciens clients reçoivent l'annonce. Le nouveau ne la reçoit pas.

### Message sans retour à la ligne

Il est conservé, mais pas encore diffusé.

### Message coupé entre plusieurs `recv`

Les morceaux sont rassemblés grâce à `buf[fd]`.

### Plusieurs `\n` dans un même `recv`

Chaque ligne est envoyée séparément et reçoit son propre préfixe.

### Ligne vide

Si un client envoie seulement `\n`, les autres reçoivent :

```text
client ID: 
```

### Deux clients écrivent des morceaux incomplets

Chaque client possède son tampon, donc les morceaux ne se mélangent pas.

### Déconnexion avec une ligne incomplète

La ligne incomplète est abandonnée et le tampon du client est vidé.

### Réutilisation d'un `fd`

Un futur client peut récupérer le même `fd`, mais `ids[fd]` est remplacé par un
nouvel identifiant et `buf[fd]` est nettoyé.

### Auteur du message

Il ne reçoit jamais son propre message, car son `fd` est passé comme exclusion à
`cast`.

---

## 13. Les erreurs classiques qui font perdre l'examen

1. Donner `fds` directement à `select` au lieu d'en faire une copie.
2. Oublier d'ajouter `sfd` dans `fds`.
3. Utiliser `mx` au lieu de `mx + 1`, ou oublier que `mx` contient déjà le plus
   grand `fd` plus un.
4. Confondre le `fd` et l'identifiant du client.
5. Réattribuer un ancien identifiant après une déconnexion.
6. Envoyer l'annonce d'arrivée au nouveau client.
7. Renvoyer un message à son auteur.
8. Supposer qu'un `recv` correspond à une ligne.
9. Supposer que le tampon de `recv` se termine par `\0`.
10. Mettre le préfixe une seule fois devant plusieurs lignes.
11. Oublier de garder une ligne incomplète pour le prochain `recv`.
12. Oublier `FD_CLR` lors d'un départ.
13. Oublier `close(fd)` et créer une fuite de descripteur.
14. Écrire les messages d'erreur sur la sortie standard au lieu de `stderr`.
15. Modifier les textes imposés par le sujet.
16. Utiliser une fonction interdite ou ajouter un `#define`.
17. Complexifier la solution avec des structures ou allocations inutiles.

---

## 14. La recette à mémoriser

Apprends cette histoire, pas seulement les lignes de code :

> Je vérifie l'argument. Je crée, attache et mets le serveur en écoute. Je crée
> mon ensemble permanent et j'y place le serveur. Dans la boucle, je copie cet
> ensemble puis j'appelle `select`. Je parcours les `fd` prêts. Si c'est le
> serveur, j'accepte, j'enregistre et j'annonce le client. Sinon je fais
> `recv` : zéro ou moins signifie départ ; sinon j'ajoute les octets au tampon
> du client et je diffuse chaque fois que je rencontre un retour à la ligne.

Version ultra-courte à écrire au brouillon :

```text
ARG
SOCKET -> ADDRESS -> BIND -> LISTEN
ZERO -> SET(server)
LOOP:
    copy -> SELECT
    scan fd
        server: ACCEPT -> max -> SET -> id -> arrive
        client: RECV
            <= 0: CLR -> left -> CLOSE -> clear
            > 0: append -> on \n: prefix -> cast -> clear
```

---

## 15. Ordre conseillé pour réécrire le programme en examen

Ne cherche pas à tout écrire parfaitement d'un seul coup.

### Passage 1 — le squelette

Écris :

- les includes ;
- les variables globales ;
- `err` ;
- `cast` ;
- la vérification de `ac` ;
- le début fourni : `socket`, adresse, `bind`, `listen`.

### Passage 2 — la boucle réseau

Écris :

- initialisation de `fds` et `mx` ;
- copie `rf = fds` ;
- `select` ;
- boucle `fd < mx` ;
- test `FD_ISSET`.

### Passage 3 — les trois branches

Ajoute dans cet ordre :

1. arrivée ;
2. départ ;
3. réception et découpage des lignes.

### Passage 4 — audit de cinq minutes

Vérifie :

- tous les textes exacts ;
- tous les `\n` ;
- copie de `fds` avant chaque `select` ;
- `FD_SET` à l'arrivée ;
- `FD_CLR` et `close` au départ ;
- exclusion de l'auteur ;
- `x = 0` après une ligne ;
- aucune fonction interdite ;
- aucune accolade ou parenthèse en trop.

---

## 16. Tests simples avec `nc`

Compilation typique :

```bash
cc -Wall -Wextra -Werror mini_serv.c -o mini_serv
```

Lancer le serveur :

```bash
./mini_serv 8080
```

Dans deux autres terminaux :

```bash
nc 127.0.0.1 8080
```

Tests manuels à faire :

1. connecter A, puis B, et vérifier l'annonce reçue par A ;
2. écrire une ligne depuis A et vérifier que seule B la reçoit ;
3. écrire plusieurs lignes rapidement ;
4. envoyer une ligne vide ;
5. fermer B avec `Ctrl-D` et vérifier l'annonce chez A ;
6. reconnecter C et vérifier que son id continue d'augmenter.

Pour tester précisément plusieurs lignes en une écriture :

```bash
printf 'un\ndeux\ntrois\n' | nc 127.0.0.1 8080
```

---

## 17. Stratégie pour les 3 heures d'examen

Une répartition raisonnable :

### 0:00 à 0:20 — lire et annoter

- comparer le sujet au début de code fourni ;
- repérer les fonctions autorisées ;
- écrire la recette courte au brouillon ;
- identifier les variables nécessaires.

### 0:20 à 1:10 — produire une première version

- recopier le début fourni avec attention ;
- construire le squelette ;
- traiter arrivée, départ et messages.

### 1:10 à 1:40 — compiler et corriger

- corriger d'abord toutes les erreurs de compilation ;
- relire les accolades et les conditions ;
- compiler avec les flags demandés.

### 1:40 à 2:25 — tester avec plusieurs clients

- connexion ;
- message simple ;
- plusieurs lignes ;
- ligne fragmentée ;
- déconnexion ;
- reconnexion.

### 2:25 à 3:00 — audit final

- textes exacts ;
- fonctions autorisées ;
- sorties d'erreur ;
- aucun envoi à l'auteur ;
- nettoyage des clients ;
- dernière compilation dans les conditions réelles.

Ne reste pas bloqué vingt minutes sur une seule faute. Reviens à la recette et
vérifie branche par branche.

---

## 18. Peut-on apprendre cela en 23 heures ?

Oui, c'est réaliste pour cette solution courte, à condition de viser la
reproduction comprise plutôt qu'une récitation fragile. Tu n'as pas besoin de
devenir expert réseau : tu dois maîtriser un seul schéma et ses cas limites.

L'objectif utile n'est pas « je reconnais le code quand je le lis », mais :

> À partir d'une feuille vide, je peux expliquer puis reconstruire chaque bloc,
> compiler, trouver mes fautes et prouver le comportement avec deux `nc`.

### Planning possible sur 23 heures avant l'examen

Il faut compter le sommeil dans ces 23 heures. Une nuit blanche diminue fortement
la mémoire de travail, exactement celle dont tu as besoin pour retrouver une
accolade ou raisonner sur `select`.

- **Après la douche, 45 min :** lire entièrement ce cours et suivre le code avec
  le doigt, sans essayer de mémoriser.
- **1 h :** expliquer à voix haute les variables, `select`, arrivée, départ et
  réception sans regarder le cours.
- **1 h 30 :** recopier une première fois le programme depuis une feuille vide,
  puis comparer ligne par ligne.
- **30 min de pause et repas :** vraie coupure.
- **1 h 30 :** refaire le programme sans regarder, compiler et tester avec deux
  clients.
- **45 min :** travailler uniquement les erreurs commises, pas tout recopier.
- **30 min :** réciter la recette et dessiner le flux sur papier.
- **7 h 30 à 8 h 30 :** dormir.
- **Au réveil, 1 h 30 :** simulation complète chronométrée sans aide.
- **45 min :** correction et tests des mini-cas.
- **30 min :** revoir les messages exacts, les fonctions autorisées et la recette.
- **Avant l'épreuve :** manger, boire, préparer le trajet et garder une marge.

Adapte les heures exactes à l'heure de début de ton examen. La priorité est de
faire au moins deux reconstructions sans regarder, dont une après avoir dormi.

---

## 19. Questions de contrôle

Tu dois pouvoir répondre rapidement à chacune :

1. Pourquoi faut-il copier `fds` dans `rf` avant `select` ?
2. Pourquoi `mx` vaut-il le plus grand `fd` plus un ?
3. Quelle différence existe entre `fd` et identifiant client ?
4. Pourquoi faut-il un `buf` séparé par client ?
5. Pourquoi ne peut-on pas faire `strlen(mb)` juste après `recv` ?
6. Que signifie `recv == 0` ?
7. Pourquoi continue-t-on la boucle après avoir rencontré un `\n` ?
8. Comment le code répète-t-il le préfixe devant chaque ligne ?
9. Pourquoi appelle-t-on `FD_CLR` puis `close` au départ ?
10. Qui doit recevoir une annonce d'arrivée ?

Réponses courtes :

1. Parce que `select` modifie l'ensemble reçu.
2. Parce que c'est la convention du premier argument de `select`.
3. Le `fd` appartient au système et peut être réutilisé ; l'id est attribué par
   le serveur et augmente toujours.
4. Chaque client peut avoir une ligne incomplète différente.
5. `recv` ne garantit pas un octet nul final.
6. Le client a fermé proprement sa connexion.
7. Le même `recv` peut contenir plusieurs lignes.
8. Il construit et diffuse un nouveau `sb` à chaque `\n`.
9. Pour ne plus surveiller le client et éviter une fuite de descripteur.
10. Tous les clients déjà connectés, mais pas le nouveau.

---

## 20. La checklist finale à connaître par cœur

```text
[ ] ac == 2, sinon message exact sur stderr
[ ] socket vérifié
[ ] adresse = AF_INET + 127.0.0.1 + port
[ ] bind et listen vérifiés
[ ] mx = sfd + 1
[ ] FD_ZERO puis FD_SET(sfd)
[ ] copie de fds avant chaque select
[ ] parcours fd < mx et FD_ISSET(fd, &rf)
[ ] accept + mx + FD_SET + id + annonce
[ ] recv <= 0 : FD_CLR + annonce + close + nettoyage
[ ] recv > 0 : tampon propre à chaque client
[ ] une diffusion pour chaque \n
[ ] préfixe exact devant chaque ligne
[ ] auteur exclu de la diffusion
[ ] pas de fonction interdite, pas de #define
[ ] compilation et test avec au moins deux nc
```

Si cette checklist et la recette de la section 14 sortent naturellement de ta
tête, tu as déjà l'architecture complète de l'exercice.
