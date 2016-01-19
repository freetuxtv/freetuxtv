

# Introduction #

Vous trouverez sur cette page les réponses aux problèmes et aux questions les plus courants sur FreetuxTV.

# FAQ #

## Questions générales ##

### Comment installer FreetuxTV sur ma distribution Linux ###

Rendez-vous sur le wiki de votre distribution :

  * Sur Ubuntu : http://doc.ubuntu-fr.org/freetuxtv
  * Sur Mandriva : http://wiki.mandriva.com/fr/FreetuxTV

### Peut-on enregistrer les chaînes de télévision ? ###

Depuis FreetuxTV 0.3.0 (2009-08-25), il est possible d'enregistrer des chaînes qui sont regardées en direct. Et à partir de FreetuxTV 0.4.2 (2010-03-14), l'utilisateur peut en plus programmer la durée de l'enregistrement des chaines regardées en direct.
Voir [capture écran avec enregistrement en fonctionnement](http://freetuxtv.blogspot.com/2009/08/screenshot-du-26082009.html) et [capture écran Spécifier la durée de l'enregistrement](http://freetuxtv.blogspot.com/2010/03/commit-specifier-la-duree-de.html)

### Peut-on programmer l'enregistrement de chaînes de télévision ? ###

Pour l'instant avec FreetuxTV 0.5.1, il n'est pas possible de programmer l'enregistrement de chaines de télévision ou de radio. Vous pouvez suivre l'état de réalisation de cette fonction [sur cette page](http://code.google.com/p/freetuxtv/wiki/RoadMap).
Par contre, en attendant, vous pouvez programmer l'enregistrement de chaines de télévision ou de radio avec des logiciels comme [Qmagneto](http://code.google.com/p/qmagneto/), [Fricorder](http://manatlan.online.fr/fricorder.php).

### Faut-il être forcément abonné à Free, SFR, Bouygues, Alice et Orange pour avoir accès aux chaines? ###

Non, tous les utilisateurs de FreetuxTV peuvent utiliser les chaines de WebTV et WebRadio partout dans le monde, abonné ou pas à un des FAI (Free, SFR, Bouygues, Alice et Orange).
Par contre la liste des chaines Free ne sera accesible/utilisable que par les abonnés au FAI Free en France et de même pour la liste des chaines d'Orange aux abonnés d'Orange, et c'est la meme chose pour SFR, Bouygues, Alice. 

&lt;BR&gt;


L'objectif est que le maximum de personnes puissent utiliser FreetuxTV gràce aux listes de chaines de WebTV ou WebRadios en plusieurs langues qui sont fournies dans FreeTuxTV mais s'ils ne sont pas chez un FAI français.

&lt;BR&gt;


Presque la totalité des radios disponibles chez Free dans sa liste de chaines radios se retrouve dans les listes des WebRadios avec leur logos.

### Est-il possible d'ajouter ma chaine de TV ou radio favorite dans FreetuxTV ? ###

Il est possible de rajouter votre chaine de TV ou radio favorite dans FreetuxTV sous réserve de compatibilité technique avec FreetuxTV.
Pour proposer votre chaine, veuillez nous fournir l'adresse/URL du site web de votre chaine et l'adresse/URL de la page du site web où se trouve le lien ou le lecteur video (TV) ou audio (radio).
Dans la mesure du possible, veuillez nous indiquer le pays et langue principale utilisée par la chaine. Toutes ces informations sont à nous transmettre en créant un ticket sur [cette page (Issues)](http://code.google.com/p/freetuxtv/issues/entry) ou en créant un article sur le forum de FreetuxTV sur [cette autre page (Forums)](http://groups.google.com/group/freetuxtv).

### Est-il possible d'ajouter une nouvelle fonctionnalité dans FreetuxTV ? ###

Il est possible d'ajouter une nouvelle fonctionnalité dans FreetuxTV sous réserve de compatibilité technique avec FreetuxTV.
Pour proposer nouvelle fonctionnalité, d'abord veuillez vérifier qu'elle n'est pas prise en compte déjà dans la page [Feuille de route](http://code.google.com/p/freetuxtv/wiki/RoadMap) ou dans la liste des tickets ouverts dans [Issues](http://code.google.com/p/freetuxtv/issues/list).
Si ce n'est pas le cas, alors créez un ticket sur [cette page (Issues)](http://code.google.com/p/freetuxtv/issues/entry) ou si vous ne savez pas comment remplir ce dernier alors créez un article sur le forum de FreetuxTV sur [cette autre page (Forums)](http://groups.google.com/group/freetuxtv).
Et vraiement, et faute de mieux, en faisant un commentaire dans un article traitant de la sortie d'une nouvelle version de FreetuxTV sur le Blog d'Eric le développeur [sur son site web](http://freetuxtv.blogspot.com/).

### Où faut-il déclarer les bogues (bugs) ou problèmes rencontrés avec FreetuxTV ? ###

Pour déclarer les bogues (bugs) ou problèmes rencontrés avec FreetuxTV, vérifiez d'abord qu'il n'y a pas de ticket créé sur [cette page (Issues)](http://code.google.com/p/freetuxtv/issues/entry) et alors créez-en un nouveau directement [sur cette page](http://code.google.com/p/freetuxtv/issues/entry) ou si vous ne savez pas comment remplir ce dernier alors créez un article sur le forum de FreetuxTV sur [cette autre page (Forums)](http://groups.google.com/group/freetuxtv).
Et vraiement, et faute de mieux, en faisant un commentaire dans un article traitant de la sortie d'une nouvelle version de FreetuxTV sur le Blog d'Eric le développeur [sur son site web](http://freetuxtv.blogspot.com/).

## Problèmes fréquents ##

### Mes groupes de chaînes ne contiennent aucunes chaînes ###

Pour mettre à jour la liste des chaînes, effectuer un clic droit sur le nom du groupe et choisissez l'option "Actualiser depuis la playlist".

### Comment rajouter un groupe supprimé ? ###

Pour ajouter un groupe, cliquez sur le menu "Chaînes" puis "Ajouter un groupe". Saisissez alors le nom sur groupe et l'URI du fichier M3U. Vous pouvez aussi supprimer une partie du nom des chaînes dans la playlist en utilisant les expressions régulières.

### Je n'ai pas de son ni d'image ###

Si vous n'arrivez pas à obtenir l'image avec FreetuxTV, la première chose à faire est de voir si vous arrivez à lire la chaîne dans VLC. Pour récupérer le lien de la chaîne, ouvrez FreetuxTV en mode console et regarder dans les logs pour extraire le lien (en attendant une solution plus pratique).

Sinon, il peut s'agir d'un codec video ou audio qui n'est pas supporté ou dont le support n'a pas été compilé avec VLC comme pour le H264 ou AAC qui sont utilisés dans les chaines "Bas débit" ou "Haut Débit" chez Free.
Voir le [ticket "pas de son"](http://code.google.com/p/freetuxtv/issues/detail?id=42) pour avoir quelques informations utiles.

### Les chaines de télévision fournies par le FAI Orange ne fonctionnent pas ###

Comme nous n'avons pas Orange comme FAI mais Free, nous vous conseillons de consulter le fil suivant ["La TV d'Orange sur Mac ?, en tous cas ça marche sur Ubuntu"](http://forum.macbidouille.com/index.php?showtopic=299703) du Forum macbidouille.com qui indique les opérations à réaliser pour que celà fonctionne.

Cette page indique aussi comment configurer Orange avec VLC, ce qui devrait marché aussi avec FreetuxTV : http://www.porciello.com/inventel/tv_vlc.htm

### Problème lors de l'enregistrement avec le transcodage ###

Il peut que certaines chaînes ne s'enregistre pas correctement, si on utilise un transcodage particulier.

C'est le cas de nombreuse chaînes de FreeboxTV. En effet, celles ci utilisent un codec mp4a (AAC) et à l'heure actuelle VLC ne dispose pas d'un filtre de conversion depuis ce format pour le transcoder dans un autre. Du coup, il est impossible de faire une conversion de l'audio des chaînes de FreeboxTV.