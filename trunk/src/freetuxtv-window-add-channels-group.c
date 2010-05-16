/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2010 <eric.beuque@gmail.com>
 * 
 * freetuxtv is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * freetuxtv is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "freetuxtv-window-add-channels-group.h"

#include "freetuxtv-fileutils.h"
#include "freetuxtv-models.h"
#include "freetuxtv-db-sync.h"
#include "freetuxtv-channels-list.h"



typedef struct _FreetuxTVWindowAddChannelsGroupPrivate FreetuxTVWindowAddChannelsGroupPrivate;
struct _FreetuxTVWindowAddChannelsGroupPrivate
{
	FreetuxTVApp* app;

	GtkWidget* add_button;

	int allowedType;

	FreetuxTVChannelsGroupInfos* pLastAddedChannelsGroupInfos;
    GtkTreePath* pLastAddedChannelsGroupPath;
};

#define FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), FREETUXTV_TYPE_WINDOW_ADD_CHANNELS_GROUP, FreetuxTVWindowAddChannelsGroupPrivate))



G_DEFINE_TYPE (FreetuxTVWindowAddChannelsGroup, freetuxtv_window_add_channels_group, G_TYPE_OBJECT);


static gboolean
dialog_init (FreetuxTVWindowAddChannelsGroup *pWindowAddChannelsGroup);

static void
on_buttonrefresh_clicked (GtkButton *button, gpointer user_data);

static void
on_buttonadd_clicked (GtkButton *button, gpointer user_data);

static void
on_dialog_response (GtkDialog *dialog, gint response_id, gpointer user_data);

static gboolean
on_dialog_close (GtkWidget *widget, GdkEvent  *event, gpointer user_data);

static void
gtk_notebook_set_page_visible(GtkNotebook* notebook, int page_num, gboolean visible);

static void
freetuxtv_window_add_channels_group_init (FreetuxTVWindowAddChannelsGroup *object)
{
	FreetuxTVWindowAddChannelsGroupPrivate* priv;

	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(object);
	priv->app = NULL;

	priv->add_button = NULL;
	priv->allowedType = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_ALL;

	priv->pLastAddedChannelsGroupInfos = NULL;
    priv->pLastAddedChannelsGroupPath = NULL;
}

static void
freetuxtv_window_add_channels_group_finalize (GObject *object)
{
	G_OBJECT_CLASS (freetuxtv_window_add_channels_group_parent_class)->finalize (object);

	FreetuxTVWindowAddChannelsGroupPrivate* priv;
	FreetuxTVWindowAddChannelsGroupClass* klass;

	GtkWidget* widget;
	
	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(object);

	if(priv->add_button){
		gtk_widget_destroy (priv->add_button);
		priv->add_button = NULL;
	}

	klass = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_GET_CLASS(object);

	if(klass->on_buttonrefresh_clicked_hid != -1){
		widget = (GtkWidget *)gtk_builder_get_object (priv->app->gui,
		    "dialogaddgroup_buttonrefresh");
		g_signal_handler_disconnect (widget, klass->on_buttonrefresh_clicked_hid);
		klass->on_buttonrefresh_clicked_hid = -1;
	}
	if(klass->on_dialog_response_hid != -1){
		widget = (GtkWidget *) gtk_builder_get_object (priv->app->gui, "dialogaddgroup");
		g_signal_handler_disconnect (widget, klass->on_dialog_response_hid);
		klass->on_dialog_response_hid = -1;
	}

	priv->app = NULL;
}

static void
freetuxtv_window_add_channels_group_class_init (FreetuxTVWindowAddChannelsGroupClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	//GObjectClass* parent_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (FreetuxTVWindowAddChannelsGroupPrivate));

	object_class->finalize = freetuxtv_window_add_channels_group_finalize;

	klass->initialized = FALSE;
	klass->on_buttonrefresh_clicked_hid = -1;
	klass->on_dialog_response_hid = -1;
}

FreetuxTVWindowAddChannelsGroup*
freetuxtv_window_add_channels_group_new (FreetuxTVApp* app)
{
	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroups;
	FreetuxTVWindowAddChannelsGroupPrivate* priv;
	
	pWindowAddChannelsGroups = g_object_new (FREETUXTV_TYPE_WINDOW_ADD_CHANNELS_GROUP, NULL);
	
	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(pWindowAddChannelsGroups);

	priv->app = app;

	dialog_init (pWindowAddChannelsGroups);

	return pWindowAddChannelsGroups;
}

void
freetuxtv_window_add_channels_group_set_allowed_type (
	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup,
    int allowedType)
{
	g_return_if_fail(pWindowAddChannelsGroup != NULL);
	g_return_if_fail(FREETUXTV_IS_WINDOW_ADD_CHANNELS_GROUP(pWindowAddChannelsGroup));
	
	FreetuxTVWindowAddChannelsGroupPrivate* priv;
	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(pWindowAddChannelsGroup);

	priv->allowedType = allowedType;
}

gint
freetuxtv_window_add_channels_group_run (
	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup)
{
	g_return_val_if_fail(pWindowAddChannelsGroup != NULL, GTK_RESPONSE_NONE);
	g_return_val_if_fail(FREETUXTV_IS_WINDOW_ADD_CHANNELS_GROUP(pWindowAddChannelsGroup), GTK_RESPONSE_NONE);

	FreetuxTVWindowAddChannelsGroupPrivate* priv;
	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(pWindowAddChannelsGroup);
	
	GtkBuilder *builder;
	builder = priv->app->gui;

	gint res;

	GtkDialog *dialog;
	dialog = (GtkDialog *) gtk_builder_get_object (builder, "dialogaddgroup");

	// Show the group allowed
	GtkNotebook *notebook;
	notebook = (GtkNotebook *) gtk_builder_get_object (priv->app->gui,
	    "dialogaddgroup_notebook");
	if((priv->allowedType & FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_EXISTING) 
	    == FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_EXISTING){
		gtk_notebook_set_page_visible(notebook, 0, TRUE);
	}else{
		gtk_notebook_set_page_visible(notebook, 0, FALSE);
	}
	if((priv->allowedType & FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_CUSTOM) 
	    == FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_CUSTOM){
		gtk_notebook_set_page_visible(notebook, 1, TRUE);
	}else{
		gtk_notebook_set_page_visible(notebook, 1, FALSE);
	}
	if((priv->allowedType & FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_FAVOURITES) 
	    == FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_FAVOURITES){
		gtk_notebook_set_page_visible(notebook, 2, TRUE);
	}else{
		gtk_notebook_set_page_visible(notebook, 2, FALSE);
	}

	// Display the dialog
	res = gtk_dialog_run(dialog);

	return res;
}

gboolean
freetuxtv_window_add_channels_group_get_last_added(
    FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup,
	FreetuxTVChannelsGroupInfos** ppChannelsGroupInfos,
    GtkTreePath** ppTreePath
    )
{
	g_return_val_if_fail(pWindowAddChannelsGroup != NULL, GTK_RESPONSE_NONE);
	g_return_val_if_fail(FREETUXTV_IS_WINDOW_ADD_CHANNELS_GROUP(pWindowAddChannelsGroup), GTK_RESPONSE_NONE);

	FreetuxTVWindowAddChannelsGroupPrivate* priv;
	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(pWindowAddChannelsGroup);	
	
	gboolean res = FALSE;

	res = priv->pLastAddedChannelsGroupInfos != NULL
		&& priv->pLastAddedChannelsGroupPath != NULL;

	if(ppChannelsGroupInfos){
		*ppChannelsGroupInfos = priv->pLastAddedChannelsGroupInfos;
	}

	if(ppTreePath){
		*ppTreePath = priv->pLastAddedChannelsGroupPath;
	}
	
	return res;
}

static gboolean
dialog_init (FreetuxTVWindowAddChannelsGroup *pWindowAddChannelsGroup)
{
	gboolean res = TRUE;
	
	FreetuxTVWindowAddChannelsGroupPrivate* priv;
	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(pWindowAddChannelsGroup);

	FreetuxTVWindowAddChannelsGroupClass* klass;

	GtkDialog *dialog;
	GtkWidget *widget;

	GtkBuilder *builder;
	builder = priv->app->gui;

	klass = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_GET_CLASS(pWindowAddChannelsGroup);

	dialog = (GtkDialog *) gtk_builder_get_object (builder, "dialogaddgroup");
	
	if(!klass->initialized){
		// Connect signal to the UI
		gtk_dialog_add_buttons (GTK_DIALOG(dialog),
			"gtk-close", GTK_RESPONSE_CLOSE, NULL);
		g_signal_connect(G_OBJECT(dialog),
			"delete-event",
			G_CALLBACK(on_dialog_close),
			NULL);

		// Set the tree selection mode
		widget = (GtkWidget *)gtk_builder_get_object (builder,
			"dialogaddgroup_treeviewchannelsgroups");
		GtkTreeSelection *selection;
		selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(widget));
		gtk_tree_selection_set_mode (selection, GTK_SELECTION_MULTIPLE);
		
		klass->initialized = TRUE;
	}

	// Signal to connect for each instance
	priv->add_button = gtk_button_new_from_stock ("gtk-add");
	g_signal_connect(G_OBJECT(priv->add_button),
		"clicked", G_CALLBACK(on_buttonadd_clicked), pWindowAddChannelsGroup);
	gtk_box_pack_end(GTK_BOX(dialog->action_area), priv->add_button, FALSE, FALSE, 0);
	gtk_widget_show(priv->add_button);
	
	widget = (GtkWidget *)gtk_builder_get_object (builder,
		"dialogaddgroup_buttonrefresh");
	klass->on_buttonrefresh_clicked_hid = g_signal_connect(G_OBJECT(widget),
		"clicked",
		G_CALLBACK(on_buttonrefresh_clicked),
		pWindowAddChannelsGroup);

	klass->on_dialog_response_hid = g_signal_connect(G_OBJECT(dialog),
		"response",
		G_CALLBACK(on_dialog_response),
		pWindowAddChannelsGroup);

	return res;
}

static void
gtk_notebook_set_page_visible(GtkNotebook* notebook, int page_num, gboolean visible)
{
	GtkWidget* widget;
	widget = gtk_notebook_get_nth_page (notebook, page_num);

	if(widget){
		if(visible){
			gtk_widget_show(widget);
		}else{
			gtk_widget_hide(widget);
		}
	}
}

static void
on_buttonrefresh_clicked (GtkButton *button, gpointer user_data)
{
	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroups;
	FreetuxTVWindowAddChannelsGroupPrivate* priv;
	
	FreetuxTVApp *app;

	pWindowAddChannelsGroups = (FreetuxTVWindowAddChannelsGroup*)user_data;
	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(pWindowAddChannelsGroups);
	app = priv->app;

	GError* error = NULL;

	GtkWidget* widget;
	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	    "dialogaddgroup_entrychannelsgroupfile");
	gchar *url;
	gchar *dst_file;
	url = (gchar*)gtk_entry_get_text (GTK_ENTRY(widget));
	dst_file = g_strdup_printf("%s/FreetuxTV/cache/channels_groups.dat",
	    g_get_user_config_dir());
	freetuxtv_fileutils_get_file (url, dst_file, &error);

	if(error == NULL){
		load_model_channels_group_from_file(app, &error);
	}

	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}

	g_free(dst_file);
}

static void
on_buttonadd_clicked (GtkButton *button, gpointer user_data)
{
	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup;
	FreetuxTVWindowAddChannelsGroupPrivate* priv;
	
	FreetuxTVApp *app;

	pWindowAddChannelsGroup = (FreetuxTVWindowAddChannelsGroup*)user_data;
	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(pWindowAddChannelsGroup);
	
	app = priv->app;
	
	GError* error = NULL;
	gchar *errmsg = NULL;

	GtkWidget* widget;
	GtkProgressDialog* pProgressDialog = NULL;
	
	const gchar *sgroupname = NULL;
	const gchar *sgroupuri = NULL;
	const gchar *sbregex = NULL;
	const gchar *seregex = NULL;

	gboolean has_process = FALSE;

	gchar *tmptext;

	GtkTreePath* pTreePathTmp;
	
	DBSync dbsync;
	dbsync_open_db (&dbsync, &error);

	GtkDialog *dialog;
	dialog = (GtkDialog *) gtk_builder_get_object (app->gui, "dialogaddgroup");

	widget = (GtkWidget *) gtk_builder_get_object (app->gui,
	    "dialogaddgroup_notebook");

	if(error == NULL){
		int page;
		page = gtk_notebook_get_current_page (GTK_NOTEBOOK(widget));
		switch(page){
		case 0:
			// Add one or many groups in the list
			widget = (GtkWidget *)gtk_builder_get_object (app->gui,
								      "dialogaddgroup_treeviewchannelsgroups");
			GtkTreeSelection *selection;
			selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(widget));
			
			GtkTreeModel* model;
			model = (GtkTreeModel *) gtk_builder_get_object (app->gui,
									 "treestore_channelsgroup");
			GList *list;
			list = gtk_tree_selection_get_selected_rows (selection, &model);

			int count;
			int i = 0;
			int nb_added = 0;
			count = gtk_tree_selection_count_selected_rows(selection);

			
			GList* iterator = NULL;
			iterator = g_list_first (list);
			
			if(iterator != NULL){
				has_process = TRUE;
			}else{
				errmsg = g_strdup_printf(_("Please select a least one group !"));
			}

			// Initialize the dialog progress
			if(has_process){
				pProgressDialog = gtk_progress_dialog_new (GTK_WINDOW(dialog));
				gtk_progress_dialog_set_title(pProgressDialog, _("Adding channels groups"));
				gtk_widget_show(GTK_WIDGET(pProgressDialog));
			}
			
			GtkTreePath* last_ppath = NULL;
			FreetuxTVChannelsGroupInfos *pChannelsGroupInfos;
			
			while(iterator != NULL && error == NULL){
				GtkTreePath* path;
				GtkTreePath* parent_path;
				
				GtkTreeIter iter;
				GtkTreeIter iter_parent;
				gboolean valid = TRUE;	
				
				// Get the next path
				path = (GtkTreePath*)iterator->data;

				i++;

				if(gtk_tree_path_get_depth (path) == 1){
					last_ppath = path;
					
					if(gtk_tree_model_get_iter (model, &iter_parent, path)){
						if(gtk_tree_model_iter_children (model, &iter, &iter_parent)){
							
							do {
								gtk_tree_model_get (model, &iter, MODEL_CHANNELSGROUP_NAME, &sgroupname,
										    MODEL_CHANNELSGROUP_URI, &sgroupuri,
										    MODEL_CHANNELSGROUP_BREGEX, &sbregex,
										    MODEL_CHANNELSGROUP_EREGEX, &seregex, -1);
								
								pChannelsGroupInfos = freetuxtv_channels_group_infos_new ((gchar*)sgroupname,
									FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST);
								freetuxtv_channels_group_infos_set_uri (pChannelsGroupInfos,
									(gchar*)sgroupuri);
								freetuxtv_channels_group_infos_set_regex (pChannelsGroupInfos,
									(gchar*)sbregex, (gchar*)seregex);
								
								// Update progress dialog
								tmptext = g_strdup_printf(_("Adding channels group : <i>%s</i>"),
									pChannelsGroupInfos->name);
								gtk_progress_dialog_set_text(pProgressDialog, tmptext);
								g_free(tmptext);

								channels_list_add_channels_group (app, pChannelsGroupInfos, NULL, &dbsync, &error);
								nb_added++;
								
							}while (gtk_tree_model_iter_next(model, &iter) && error == NULL);
							
						}
					}
				}else{
					parent_path = gtk_tree_path_copy (path);
					gtk_tree_path_up(parent_path);
					
					if(last_ppath != NULL){
						if(gtk_tree_path_compare (parent_path, last_ppath) == 0){
							valid = FALSE;
						}
					}
					gtk_tree_path_free (parent_path);
					
					if(valid){
						if(gtk_tree_model_get_iter (model, &iter, path)){
							gtk_tree_model_get (model, &iter, MODEL_CHANNELSGROUP_NAME, &sgroupname,
									    MODEL_CHANNELSGROUP_URI, &sgroupuri,
									    MODEL_CHANNELSGROUP_BREGEX, &sbregex,
									    MODEL_CHANNELSGROUP_EREGEX, &seregex, -1);
							
							pChannelsGroupInfos = freetuxtv_channels_group_infos_new ((gchar*)sgroupname,
								FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST);
							freetuxtv_channels_group_infos_set_uri (pChannelsGroupInfos,
								(gchar*)sgroupuri);
							freetuxtv_channels_group_infos_set_regex (pChannelsGroupInfos, (gchar*)sbregex, (gchar*)seregex);
							
							// Update progress dialog
							tmptext = g_strdup_printf(_("Adding channels group : <i>%s</i>"),
								pChannelsGroupInfos->name);
							gtk_progress_dialog_set_text(pProgressDialog, tmptext);
							g_free(tmptext);
							
							channels_list_add_channels_group (app, pChannelsGroupInfos, NULL, &dbsync, &error);
							nb_added++;
						}			
					}	
				}
				
				// Update the progress bar
				gdouble percent;
				percent = (gdouble)i/(gdouble)count;
				gtk_progress_dialog_set_percent(pProgressDialog, percent);

				iterator = g_list_next(iterator);
			}
			g_list_free (list);

			if(has_process){
				tmptext = g_strdup_printf(_("%d channels group(s) have been successfully added."), nb_added);
				gtk_progress_dialog_set_text(pProgressDialog, tmptext);
				g_free(tmptext);
			}
			
			break;
		case 1:
			// Add custom group
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialogaddgroup_name");
			sgroupname = gtk_entry_get_text(GTK_ENTRY(widget));
			
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
								       "dialogaddgroup_uri");
			sgroupuri = gtk_entry_get_text(GTK_ENTRY(widget));
			
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
								       "dialogaddgroup_bregex");
			sbregex = gtk_entry_get_text(GTK_ENTRY(widget));
			
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
								       "dialogaddgroup_eregex");
			seregex = gtk_entry_get_text(GTK_ENTRY(widget));			
			
			/* Check the field */
			if(g_ascii_strcasecmp(sgroupname,"") == 0 && errmsg == NULL){
				errmsg = g_strdup_printf(_("Please enter the group's name !"));
			}
			if(g_ascii_strcasecmp(sgroupuri,"") == 0 && errmsg == NULL){
				errmsg = g_strdup_printf(_("Please enter the group's URI !"));
			}
			
			if(errmsg == NULL){
				pProgressDialog = gtk_progress_dialog_new (GTK_WINDOW(dialog));
				gtk_progress_dialog_set_title(pProgressDialog, _("Adding channels groups"));
				gtk_widget_show(GTK_WIDGET(pProgressDialog));
			
				if(g_ascii_strcasecmp(sbregex,"") == 0 ){
					sbregex = NULL;
				}
				if(g_ascii_strcasecmp(seregex,"") == 0 ){
					seregex = NULL;
				}
				
				FreetuxTVChannelsGroupInfos *pChannelsGroupInfos;		
				pChannelsGroupInfos = freetuxtv_channels_group_infos_new ((gchar*)sgroupname,
					FREETUXTV_CHANNELSGROUP_TYPEGROUP_PLAYLIST);
				freetuxtv_channels_group_infos_set_uri (pChannelsGroupInfos,
					(gchar*)sgroupuri);
				freetuxtv_channels_group_infos_set_regex (pChannelsGroupInfos, (gchar*)sbregex, (gchar*)seregex);
				
				// Update progress dialog
				tmptext = g_strdup_printf(_("Adding channels group : <i>%s</i>"),
					pChannelsGroupInfos->name);
				gtk_progress_dialog_set_text(pProgressDialog, tmptext);
				gtk_progress_dialog_set_percent(pProgressDialog, 0.0);
				g_free(tmptext);

				channels_list_add_channels_group (app, pChannelsGroupInfos, NULL, &dbsync, &error);

				tmptext = g_strdup_printf(_("%d channels group(s) have been successfully added."), 1);
				gtk_progress_dialog_set_text(pProgressDialog, tmptext);
				gtk_progress_dialog_set_percent(pProgressDialog, 1.0);
				
				g_free(tmptext);
			}
			
			break;
		case 2:
			// Add custom group
			widget = (GtkWidget *) gtk_builder_get_object (app->gui,
							       "dialogaddgroup_specialgroupname");
			sgroupname = gtk_entry_get_text(GTK_ENTRY(widget));
			
			/* Check the field */
			if(g_ascii_strcasecmp(sgroupname,"") == 0 && errmsg == NULL){
				errmsg = g_strdup_printf(_("Please enter the group's name !"));
			}
			
			if(errmsg == NULL){
				pProgressDialog = gtk_progress_dialog_new (GTK_WINDOW(dialog));
				gtk_progress_dialog_set_title(pProgressDialog, _("Adding channels groups"));
				gtk_widget_show(GTK_WIDGET(pProgressDialog));

				FreetuxTVChannelsGroupInfos *pChannelsGroupInfos;		
				pChannelsGroupInfos = freetuxtv_channels_group_infos_new ((gchar*)sgroupname,
					FREETUXTV_CHANNELSGROUP_TYPEGROUP_FAVORITES);
				
				// Update progress dialog
				tmptext = g_strdup_printf(_("Adding channels group : <i>%s</i>"),
					pChannelsGroupInfos->name);
				gtk_progress_dialog_set_text(pProgressDialog, tmptext);
				gtk_progress_dialog_set_percent(pProgressDialog, 0.0);
				g_free(tmptext);

				channels_list_add_channels_group (app, pChannelsGroupInfos, &pTreePathTmp, &dbsync, &error);
				priv->pLastAddedChannelsGroupInfos = pChannelsGroupInfos;
				if(priv->pLastAddedChannelsGroupPath){
					gtk_tree_path_free(priv->pLastAddedChannelsGroupPath);
				}
				priv->pLastAddedChannelsGroupPath = pTreePathTmp;
				g_print("added %s\n", gtk_tree_path_to_string (pTreePathTmp));

				tmptext = g_strdup_printf(_("%d channels group(s) have been successfully added."), 1);
				gtk_progress_dialog_set_text(pProgressDialog, tmptext);
				gtk_progress_dialog_set_percent(pProgressDialog, 1.0);
				
				g_free(tmptext);
			}
			break;
		}
	}
	dbsync_close_db(&dbsync);

	if(errmsg != NULL){
		windowmain_show_error (app, errmsg);
		g_free(errmsg);
	}
	if(error != NULL){
		windowmain_show_gerror (app, error);
		g_error_free (error);
	}

	if(pProgressDialog){
		// TODO g_object_unref(pProgressDialog);
		pProgressDialog = NULL;
	}
}

static void
on_dialog_response (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup;
	FreetuxTVWindowAddChannelsGroupPrivate* priv;

	pWindowAddChannelsGroup = (FreetuxTVWindowAddChannelsGroup*)user_data;
	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(pWindowAddChannelsGroup);

	GtkBuilder *builder;
	builder = priv->app->gui;

	gtk_widget_hide(GTK_WIDGET(dialog));
}

static gboolean
on_dialog_close (GtkWidget *widget, GdkEvent  *event, gpointer user_data)
{
	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup;

	pWindowAddChannelsGroup = (FreetuxTVWindowAddChannelsGroup*)user_data;
	
	return gtk_widget_hide_on_delete(widget);
}