# Introduction #

Cette page présente les différentes méthode d'installer FreetuxTV.

# Via les dépôts #

Le plus simple pour installer FreetuxTV est de l'installer via un dépôt de logiciel. De ce fait, vous n'aurez pas à vous préoccuper des dépendances.

## Via les dépôts officiels ##

Le plus simple pour installer FreetuxTV est d'utiliser le paquet inclus par défaut dans votre distribution. Cependant, seule une petite parti des distributions ont inclut FreetuxTV dans leur dépôts.

Voici la liste des distributions ayant inclus le paquet dans leur dépôt :

| **Distribution** | **Inclusion** | **Installation** |
|:-----------------|:--------------|:-----------------|
| Ubuntu           | -             |                  |
| Debian (sid)     | Oui           | aptitude install freetuxtv |
| Mandriva         | Oui           |                  |
| openSUSE         | -             |                  |
| Pardus           | Oui           |                  |
| Frugalware       | Oui           |                  |

## Via les dépôts non officiels ##

Des dépôts non officiel existe pour certaines distributions. Ceci permet aussi d'avoir des versions plus à jour que celle inclus dans les dépôts officiel.

### Ubuntu ###

Vous pouvez utiliser les PPA (Personal Package Archive) de l'équipe de FreetuxTV :

**PPA Stable (recommandé)**

Ce PPA contient les paquets des dernières versions stable de FreetuxTV.

  * https://launchpad.net/~freetuxtv/+archive/freetuxtv

```
sudo add-apt-repository ppa:freetuxtv/freetuxtv
sudo aptitude update && sudo aptitude install freetuxtv
```

**PPA Non-stable**

Ce PPA contient des paquets mis à jour quotidiennement avec les dernières sources du logiciel. Attention! Ces versions peuvent ne pas être complètement fonctionnelles.

  * https://launchpad.net/~freetuxtv/+archive/freetuxtv-dev

```
sudo add-apt-repository ppa:freetuxtv/freetuxtv-dev 
sudo aptitude update && sudo aptitude install freetuxtv
```

### openSUSE ###

Exécuter les commandes ci-dessous, pour installer les dépôts openSUSE.

**12.1**
```
zypper ar -n packman-multimedia http://packman.inode.at/suse/openSUSE_12.1/Multimedia packman-multimedia
```

**11.4**
```
zypper ar -n packman-multimedia http://packman.inode.at/suse/openSUSE_11.4/Multimedia packman-multimedia
```

Puis pour installer freetuxtv:

```
zypper refresh
zypper install freetuxtv
```

# Via les paquets logiciel #

Vous pouvez télécharger directement des paquets sans passez par les dépots.

Les paquets sont disponible à l'adresse suivante : http://freetuxtv.eric-beuque.com/packages/.

A l'heure actuelle, l'équipe de FreetuxTV fourni des paquets pour les distributions et architectures suivante :

| **Distribution** | **32bits** | **64bits** |
|:-----------------|:-----------|:-----------|
| Ubuntu           | Oui        | Oui        |
| Debian           | Oui        | Oui        |
| Mandriva         | Oui        | -          |
| openSUSE         | Oui        | Oui        |
| Fedora           | Oui        | -          |

Nous sommes à la recherche de personnes pouvant empaqueter les paquets pour les autres distributions Linux.

_**Note :**_

Les paquets de FreetuxTV 0.4.0 pour la distribution Linux "Arch Linux" sont disponibles ici en 32bits : http://repo.x-demon.org/archlinux/os/i686/ et là en 64 bits : http://repo.x-demon.org/archlinux/os/x86_64/

Les paquets de FreetuxTV 0.4.1 pour la distribution Linux "Pardus" sont disponibles ici en 32bits : http://miroir.pardus-fr.org/2009-depot-fr-test/ et là en 64 bits : http://repo.x-demon.org/archlinux/os/x86_64/

# Via les sources #

## Version stable ##

Télécharger et compiler les sources de l'archive disponible ici : http://code.google.com/p/freetuxtv/downloads/list.

## Version de développement ##

FreetuxTV est aussi disponible en version de développement. Vous pouvez l'installer et l'utiliser sans aucun problèmes pour regarder la télévision ou écouter la radio avec les dernières fonctionnalités. Regardez le fichier [README](http://code.google.com/p/freetuxtv/source/browse/trunk/README) pour savoir comment l'installer.