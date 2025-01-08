// Fonctions réseau
socket : Crée un point d'accès réseau (socket) pour la communication.
close : Ferme un descripteur de fichier (ex. socket) pour libérer des ressources.
setsockopt : Configure des options pour un socket (ex. réutilisation d'adresse).
getsockname : Récupère l'adresse locale d'un socket.
getprotobyname : Récupère les informations sur un protocole réseau (ex. TCP).
gethostbyname : Traduit un nom de domaine en adresse IP (obsolète, préférer getaddrinfo).
getaddrinfo : Résout un nom de domaine ou une adresse texte en informations réseau.
freeaddrinfo : Libère la mémoire allouée par getaddrinfo.
bind : Associe un socket à une adresse et un port spécifiques.
connect : Établit une connexion entre un socket client et un serveur.
listen : Configure un socket pour écouter les connexions entrantes.
accept : Accepte une connexion entrante et crée un nouveau socket pour cette connexion.

// Fonctions de manipulation des données
htons : Convertit un entier 16 bits de l'ordre hôte à l'ordre réseau.
htonl : Convertit un entier 32 bits de l'ordre hôte à l'ordre réseau.
ntohs : Convertit un entier 16 bits de l'ordre réseau à l'ordre hôte.
ntohl : Convertit un entier 32 bits de l'ordre réseau à l'ordre hôte.
inet_addr : Convertit une adresse IPv4 sous forme de chaîne en entier binaire.
inet_ntoa : Convertit une adresse IPv4 binaire en chaîne lisible.

// Fonctions d'envoi et réception
send : Envoie des données via un socket.
recv : Reçoit des données sur un socket.

// Gestion des signaux
signal : Définit un gestionnaire pour un signal spécifique (ex. SIGINT).
sigaction : Configure une action avancée pour un signal (remplace signal).

// Gestion des fichiers et des sockets
lseek : Modifie la position actuelle dans un fichier ouvert.
fstat : Récupère des informations sur un fichier ouvert (ex. taille).
fcntl : Modifie les attributs d'un descripteur de fichier (ex. rendre un socket non bloquant).

// Multiplexage d'E/S
poll : Surveille plusieurs descripteurs de fichier pour détecter leur état (lecture, écriture, erreurs).

