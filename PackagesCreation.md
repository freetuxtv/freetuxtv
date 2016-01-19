

# Introduction #

Si vous le souhaitez, vous pouvez aider à développer FreetuxTV en créant des paquets pour votre distribution.

Actuellement, des paquets sont disponible pour Ubuntu, openSuse et Mandriva.

Leurs téléchargements est disponible ici : http://freetuxtv.eric-beuque.com/packages/.


# Empaqueteurs actuels #

| **Distribution** | **Version** | **Architecture** | **Empaqueteurs** |
|:-----------------|:------------|:-----------------|:-----------------|
| Ubuntu           | Jaunty      | i386             | Phillipe C.      |
| Ubuntu           | Jaunty      | amd64            | _N/A_            |
| openSuse         | all         | all              | Gilles S.        |
| Mandriva         | 2009.1      | i586             | _N/A_            |
| Mandriva         | 2009.1      | x86\_64          | _N/A_            |
| Mandriva         | 2009.0      | i586             | _N/A_            |
| Mandriva         | 2009.0      | x86\_64          | _N/A_            |

N'hésitez pas à venir compléter la liste pour les distributions manquantes.

# Transmission des paquets #

Vous pouvez transmettre vos paquets par mail au développeur.

# Consigne pour la création de paquets #

Si vous souhaitez créer un paquet vous pouvez avoir besoin de connaître certaines informations sur le fonctionnement de FreetuxTV pour qu'il s'installe correctement.

## Dépendances ##

FreetuxTV a besoin de plusieurs librairies pour fonctionner. Il est important que votre paquet renseigne les dépendances correctement. (Informations valable pour la dernière version stable : 0.5.0).

### VLC ###

Le point délicat est de satisfaire les dépendances vis à vis de VLC. Pour compiler, FreetuxTV à besoin de la librairie **libvlc** (aussi appelé **libvlc2**) qui permet d'utiliser le moteur de VLC.

De plus, il faut que la machine cible possède la même version de FreetuxTV que la celle où il a été compiler. En effet, la librairie subit régulièrement de gros changement et des fonctions peuvent être supprimé au cours des versions ce qui peut planter FreetuxTV.

C'est pourquoi dans la mesure du possible, il serait judicieux de faire respecter les règles par votre gestionnaire de paquets.

  * Si vous compiler avec **libvlc 0.8.x**, le paquet doit pouvoir s'installer uniquement si la machine de destination possède **libvlc0**.
  * Si vous compiler avec **libvlc 0.9.x**, le paquet doit pouvoir s'installer uniquement si la machine de destination possède **libvlc2**.
  * Si vous compiler avec **libvlc 1.0.x**, le paquet doit pouvoir s'installer uniquement si la machine de destination possède **libvlc2 >= 1.0**.
  * Si vous compiler avec **libvlc 1.1.x**, le paquet doit pouvoir s'installer uniquement si la machine de destination possède **libvlc5**.

Par ailleurs, pour des raisons historique dut à la conception du paquet VLC, il est indispensable d'installer le paquet **vlc** qui contient les librairies permettant d'affichage de la vidéo dans une fenêtre graphique.
Le problème à été signalé est sera sûrement corrigé dans une prochaine version du paquet. La discussion se trouve ici : (http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=513177).
En attendant, il est indispensable de l'installer pour avoir la vidéo dans FreetuxTV.

### Autres ###

Pour les autres dépendances, les contraintes sont moindres. Vous devez juste respecter ceci.

  * gtk+ (>= 2.12.0)
  * glib (>= 2.16.0)
  * gio (>= 2.16.0)
  * gobject (>= 2.16.0)
  * dbus-glib-1 (>= 0.74)
  * libsqlite3 (>= 3.4.0)
  * libcurl (>= 7.18)
  * libnotify (>= 0.4)
  * libvlc (>= 0.8.6)