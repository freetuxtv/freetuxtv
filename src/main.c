#include <gtk/gtk.h>

#include <vlc/libvlc.h>

   #include <gdk/gdkx.h>
  
gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
   g_print ("le signal delete_event est survenu.\n");

   /* Si l'on renvoit FALSE dans le gestionnaire du signal "delete_event",
    * GTK émettra le signal "destroy". Retourner TRUE signifie que l'on
    * ne veut pas que la fenêtre soit détruite. 
    * Utilisé pour faire apparaître des boîtes de dialogue du type
    * « Êtes-vous sûr de vouloir quitter ? » */

   /* Remplacez TRUE par FALSE et la fenêtre principale sera détruite par
    * un signal « delete_event ». */

   return FALSE; 
}

/* Autre fonction de rappel */
void destroy (GtkWidget *widget, gpointer data)
{
   gtk_main_quit ();
}

int main (int argc, char *argv[])
{
   GtkWidget *window;
	 GtkWidget *button;

   gtk_init (&argc, &argv);

   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_signal_connect (GTK_OBJECT (window), "delete_event",
                       GTK_SIGNAL_FUNC (delete_event), NULL);
   gtk_signal_connect (GTK_OBJECT (window), "destroy",
                       GTK_SIGNAL_FUNC (destroy), NULL);


   button = gtk_button_new_with_label ("Bonjour tout le monde");
   gtk_container_add (GTK_CONTAINER (window), button);
   gtk_widget_show (button);

   /* Configuration de la largeur du contour de la fenêtre. */
   gtk_container_border_width (GTK_CONTAINER (window), 10);

	 gtk_widget_show (window);

   libvlc_exception_t _vlcexcep; //déclaration de la structure
	 libvlc_exception_init (&_vlcexcep); //initialisation de la structure
   
   libvlc_instance_t *_vlcinstance;
	 _vlcinstance=libvlc_new(argc,argv,&_vlcexcep);

	 //XID xid = gdk_x11_drawable_get_xid (GDK_DRAWABLE (button));
	 //Window xid = gtk_widget_get_root_window(window);
	 XID xid = gdk_x11_drawable_get_xid(gtk_widget_get_parent_window(button));

	 libvlc_video_set_parent(_vlcinstance ,xid,&_vlcexcep); 
   
   

   /* Toutes les applications GTK doivent avoir un gtk_main(). 
    * Le déroulement du programme se termine là et attend qu'un
    * événement survienne (touche pressée ou événement souris). */

	 libvlc_playlist_clear(_vlcinstance,&_vlcexcep); //on vide la playlist
	 libvlc_playlist_add (_vlcinstance,"file://./tshirt_japonais.mpg",NULL,&_vlcexcep);//on ajoute le nouveau flux

	 libvlc_playlist_play (_vlcinstance , -1, 0, NULL, &_vlcexcep);

   gtk_main ();

   return 0;
}
