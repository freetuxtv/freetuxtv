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

#include "freetuxtv-gladexml.h"
#include "freetuxtv-fileutils.h"
#include "freetuxtv-models.h"
#include "freetuxtv-db-sync.h"
#include "freetuxtv-channels-list.h"
#include "freetuxtv-window-main.h"


typedef struct _FreetuxTVWindowAddChannelsGroupPrivate FreetuxTVWindowAddChannelsGroupPrivate;
struct _FreetuxTVWindowAddChannelsGroupPrivate
{
	FreetuxTVApp* app;

	int allowedType;

	GtkTreeModel* pModel;
};

#define FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), FREETUXTV_TYPE_WINDOW_ADD_CHANNELS_GROUP, FreetuxTVWindowAddChannelsGroupPrivate))

G_DEFINE_TYPE (FreetuxTVWindowAddChannelsGroup, freetuxtv_window_add_channels_group, GTK_TYPE_BUILDER_WINDOW);

enum {
   SIGNAL_CHANNELS_GROUP_ADDED,
   SIGNAL_CHANNELS_ADDED,
   NB_SIGNALS
};

static guint g_signals [NB_SIGNALS] = { 0, 0 };

static void
on_buttonrefresh_clicked (GtkButton *button, gpointer user_data);

static void
on_buttonadd_clicked (GtkButton *button, gpointer user_data);

static void
on_buttonclose_clicked (GtkButton *button, gpointer user_data);

static void
gtk_notebook_set_page_visible(GtkNotebook* notebook, int page_num, gboolean visible);

static void
db_on_group_added (
    FreetuxTVApp *app, DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    gpointer user_data, GError** error);

static void
db_on_channels_added (
    FreetuxTVApp *app, DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    gpointer user_data, GError** error);

static void
freetuxtv_window_add_channels_group_init (FreetuxTVWindowAddChannelsGroup *object)
{
	FreetuxTVWindowAddChannelsGroupPrivate* priv;

	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(object);
	priv->app = NULL;

	priv->allowedType = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_ALLOW_ALL;
}

static void
freetuxtv_window_add_channels_group_finalize (GObject *object)
{
	G_OBJECT_CLASS (freetuxtv_window_add_channels_group_parent_class)->finalize (object);

	FreetuxTVWindowAddChannelsGroupPrivate* priv;
	
	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(object);

	priv->app = NULL;
}

#ifdef G_ENABLE_DEBUG
#define g_marshal_value_peek_boolean(v)  g_value_get_boolean (v)
#define g_marshal_value_peek_char(v)     g_value_get_char (v)
#define g_marshal_value_peek_uchar(v)    g_value_get_uchar (v)
#define g_marshal_value_peek_int(v)      g_value_get_int (v)
#define g_marshal_value_peek_uint(v)     g_value_get_uint (v)
#define g_marshal_value_peek_long(v)     g_value_get_long (v)
#define g_marshal_value_peek_ulong(v)    g_value_get_ulong (v)
#define g_marshal_value_peek_int64(v)    g_value_get_int64 (v)
#define g_marshal_value_peek_uint64(v)   g_value_get_uint64 (v)
#define g_marshal_value_peek_enum(v)     g_value_get_enum (v)
#define g_marshal_value_peek_flags(v)    g_value_get_flags (v)
#define g_marshal_value_peek_float(v)    g_value_get_float (v)
#define g_marshal_value_peek_double(v)   g_value_get_double (v)
#define g_marshal_value_peek_string(v)   (char*) g_value_get_string (v)
#define g_marshal_value_peek_param(v)    g_value_get_param (v)
#define g_marshal_value_peek_boxed(v)    g_value_get_boxed (v)
#define g_marshal_value_peek_pointer(v)  g_value_get_pointer (v)
#define g_marshal_value_peek_object(v)   g_value_get_object (v)
#else /* !G_ENABLE_DEBUG */
/* WARNING: This code accesses GValues directly, which is UNSUPPORTED API.
 *          Do not access GValues directly in your code. Instead, use the
 *          g_value_get_*() functions
 */
#define g_marshal_value_peek_boolean(v)  (v)->data[0].v_int
#define g_marshal_value_peek_char(v)     (v)->data[0].v_int
#define g_marshal_value_peek_uchar(v)    (v)->data[0].v_uint
#define g_marshal_value_peek_int(v)      (v)->data[0].v_int
#define g_marshal_value_peek_uint(v)     (v)->data[0].v_uint
#define g_marshal_value_peek_long(v)     (v)->data[0].v_long
#define g_marshal_value_peek_ulong(v)    (v)->data[0].v_ulong
#define g_marshal_value_peek_int64(v)    (v)->data[0].v_int64
#define g_marshal_value_peek_uint64(v)   (v)->data[0].v_uint64
#define g_marshal_value_peek_enum(v)     (v)->data[0].v_long
#define g_marshal_value_peek_flags(v)    (v)->data[0].v_ulong
#define g_marshal_value_peek_float(v)    (v)->data[0].v_float
#define g_marshal_value_peek_double(v)   (v)->data[0].v_double
#define g_marshal_value_peek_string(v)   (v)->data[0].v_pointer
#define g_marshal_value_peek_param(v)    (v)->data[0].v_pointer
#define g_marshal_value_peek_boxed(v)    (v)->data[0].v_pointer
#define g_marshal_value_peek_pointer(v)  (v)->data[0].v_pointer
#define g_marshal_value_peek_object(v)   (v)->data[0].v_pointer
#endif /* !G_ENABLE_DEBUG */

void
g_cclosure_marshal_VOID__OBJECT_POINTER_POINTER (GClosure     *closure,
    GValue       *return_value,
    guint         n_param_values,
    const GValue *param_values,
    gpointer      invocation_hint,
    gpointer      marshal_data)
{
	typedef void (*GMarshalFunc_VOID__OBJECT_POINTER_POINTER) (gpointer     data1,
	    gpointer     arg_1,
	    gpointer     arg_2,
	    gpointer     arg_3,
	    gpointer     data2);
	register GMarshalFunc_VOID__OBJECT_POINTER_POINTER callback;
	register GCClosure *cc = (GCClosure*) closure;
	register gpointer data1, data2;

	g_return_if_fail (n_param_values == 4);

	if (G_CCLOSURE_SWAP_DATA (closure))
	{
		data1 = closure->data;
		data2 = g_value_peek_pointer (param_values + 0);
	}
	else
	{
		data1 = g_value_peek_pointer (param_values + 0);
		data2 = closure->data;
	}
	callback = (GMarshalFunc_VOID__OBJECT_POINTER_POINTER) (marshal_data ? marshal_data : cc->callback);

	callback (data1,
	    g_marshal_value_peek_object (param_values + 1),
	    g_marshal_value_peek_pointer (param_values + 2),
	    g_marshal_value_peek_pointer (param_values + 3),
	    data2);
}

static void
freetuxtv_window_add_channels_group_class_init (FreetuxTVWindowAddChannelsGroupClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (FreetuxTVWindowAddChannelsGroupPrivate));
	
	object_class->finalize = freetuxtv_window_add_channels_group_finalize;

	g_signals [SIGNAL_CHANNELS_GROUP_ADDED] = g_signal_new (
	    "channels-group-added",
	    G_TYPE_FROM_CLASS (klass),
	    G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
	    G_STRUCT_OFFSET (FreetuxTVWindowAddChannelsGroupClass, channels_group_added),
	    NULL, NULL,
	    g_cclosure_marshal_VOID__OBJECT_POINTER_POINTER,
	    G_TYPE_NONE,
	    3,
	    FREETUXTV_TYPE_CHANNELS_GROUP_INFOS,
	    G_TYPE_POINTER,
	    G_TYPE_POINTER
	    );

	g_signals [SIGNAL_CHANNELS_ADDED] = g_signal_new (
	    "channels-added",
	    G_TYPE_FROM_CLASS (klass),
	    G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
	    G_STRUCT_OFFSET (FreetuxTVWindowAddChannelsGroupClass, channels_added),
	    NULL, NULL,
	    g_cclosure_marshal_VOID__OBJECT_POINTER_POINTER,
	    G_TYPE_NONE,
	    3,
	    FREETUXTV_TYPE_CHANNELS_GROUP_INFOS,
	    G_TYPE_POINTER,
	    G_TYPE_POINTER
	    );
}

FreetuxTVWindowAddChannelsGroup*
freetuxtv_window_add_channels_group_new (GtkWindow *parent, FreetuxTVApp* app, GError** error)
{
	g_return_val_if_fail(parent != NULL, NULL);
	g_return_val_if_fail(GTK_IS_WINDOW(parent), NULL);
	g_return_val_if_fail(app != NULL, NULL);
	g_return_val_if_fail(error != NULL, NULL);
	g_return_val_if_fail(*error == NULL, NULL);
	
	
	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroups;
	FreetuxTVWindowAddChannelsGroupPrivate* priv;
	GtkBuilder* builder;
	
	gchar* szUiFile = NULL;
	szUiFile = g_build_filename (app->paths.szPathGladeXml, FREETUXTV_GUIFILE_ADDCHANNELSGROUPS, NULL);
	
	pWindowAddChannelsGroups = g_object_new (FREETUXTV_TYPE_WINDOW_ADD_CHANNELS_GROUP,
	    "ui-file", szUiFile,
	    "toplevel-widget-name", "dialogaddgroup",
	    NULL);

	builder = gtk_builder_window_get_builder(GTK_BUILDER_WINDOW(pWindowAddChannelsGroups));

	// Private members
	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(pWindowAddChannelsGroups);
	priv->app = app;
	priv->pModel = (GtkTreeModel*) gtk_builder_get_object (builder,
	    "treestore_channelsgroup");
	
	// Set the parent
	gtk_window_set_transient_for (GTK_WINDOW(pWindowAddChannelsGroups), parent);
	gtk_window_set_position (GTK_WINDOW(pWindowAddChannelsGroups), GTK_WIN_POS_CENTER_ON_PARENT);

	// Set the tree selection mode
	GtkWidget *widget;
	widget = (GtkWidget *)gtk_builder_get_object (builder,
		"treeview_channelsgroups");
	GtkTreeSelection *selection;
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(widget));
	gtk_tree_selection_set_mode (selection, GTK_SELECTION_MULTIPLE);

	// Signal to connect instance
	widget = (GtkWidget*) gtk_builder_get_object (builder, "button_add");
	g_signal_connect(G_OBJECT(widget),
		"clicked",
	    G_CALLBACK(on_buttonadd_clicked),
	    pWindowAddChannelsGroups);
	
	widget = (GtkWidget *)gtk_builder_get_object (builder, "button_refresh");
	g_signal_connect(G_OBJECT(widget),
		"clicked",
		G_CALLBACK(on_buttonrefresh_clicked),
		pWindowAddChannelsGroups);
	
	widget = (GtkWidget *)gtk_builder_get_object (builder, "button_close");
	g_signal_connect(G_OBJECT(widget),
		"clicked",
		G_CALLBACK(on_buttonclose_clicked),
	    pWindowAddChannelsGroups);
	
	load_model_channels_group_from_file (priv->app, priv->pModel, error);

	if(*error != NULL){
		g_object_unref (G_OBJECT(pWindowAddChannelsGroups));
		pWindowAddChannelsGroups = NULL;
	}


	if(szUiFile){
		g_free(szUiFile);
		szUiFile = NULL;
	}

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
	
	GtkBuilder *builder;
	builder = gtk_builder_window_get_builder(GTK_BUILDER_WINDOW(pWindowAddChannelsGroup));

	// Show the group allowed
	GtkNotebook *notebook;
	notebook = (GtkNotebook *) gtk_builder_get_object (builder, "notebook_dialogaddgroup");
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

	GtkBuilder *builder;
	builder = gtk_builder_window_get_builder(GTK_BUILDER_WINDOW(pWindowAddChannelsGroups));

	GError* error = NULL;

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Starting update of the channel's groups list\n");

	GtkWidget* widget;
	widget = (GtkWidget *) gtk_builder_get_object (builder,
	    "dialogaddgroup_entrychannelsgroupfile");
	gchar *url;
	gchar *dst_file;
	url = (gchar*)gtk_entry_get_text (GTK_ENTRY(widget));
	dst_file = g_build_filename(g_get_user_cache_dir(), "freetuxtv", "channels_groups.dat", NULL);

	g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Downloading the file '%s'\n", url);
	freetuxtv_fileutils_get_file (url, dst_file, &(app->prefs.proxy), app->prefs.timeout, &error);

	if(error == NULL){
		g_log(FREETUXTV_LOG_DOMAIN, G_LOG_LEVEL_INFO,
			  "Updating the list of channel's groups\n");
		load_model_channels_group_from_file(app, priv->pModel, &error);
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
	GtkBuilder *builder;
	builder = gtk_builder_window_get_builder(GTK_BUILDER_WINDOW(pWindowAddChannelsGroup));
	
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
	
	DBSync dbsync;
	dbsync_open_db (&dbsync, &error);

	widget = (GtkWidget *) gtk_builder_get_object (builder,
	    "notebook_dialogaddgroup");

	if(error == NULL){
		int page;
		page = gtk_notebook_get_current_page (GTK_NOTEBOOK(widget));
		switch(page){
		case 0:
			// Add one or many groups from the list of existing group
			widget = (GtkWidget *)gtk_builder_get_object (builder,
								      "treeview_channelsgroups");
			GtkTreeSelection *selection;
			selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(widget));
			
			GtkTreeModel* model;
			model = (GtkTreeModel *) gtk_builder_get_object (builder,
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
				pProgressDialog = gtk_progress_dialog_new (GTK_WINDOW(pWindowAddChannelsGroup));
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
								tmptext = g_strdup_printf(_("Adding channels group: <i>%s</i>"),
									pChannelsGroupInfos->name);
								gtk_progress_dialog_set_text(pProgressDialog, tmptext);
								g_free(tmptext);

								// Add the group in the database
								if(error == NULL){
									channels_list_db_add_channels_group (app, &dbsync, pChannelsGroupInfos,
									    db_on_group_added, db_on_channels_added, pWindowAddChannelsGroup, &error);
								}

								if(error == NULL){
									nb_added++;
								}
								
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
							tmptext = g_strdup_printf(_("Adding channels group: <i>%s</i>"),
								pChannelsGroupInfos->name);
							gtk_progress_dialog_set_text(pProgressDialog, tmptext);
							g_free(tmptext);

							// Add the group in the database
							if(error == NULL){
								channels_list_db_add_channels_group (app, &dbsync, pChannelsGroupInfos,
								    db_on_group_added, db_on_channels_added, pWindowAddChannelsGroup, &error);
							}
							
							if(error == NULL){
								nb_added++;
							}
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
			widget = (GtkWidget *) gtk_builder_get_object (builder,
							       "dialogaddgroup_name");
			sgroupname = gtk_entry_get_text(GTK_ENTRY(widget));
			
			widget = (GtkWidget *) gtk_builder_get_object (builder,
								       "dialogaddgroup_uri");
			sgroupuri = gtk_entry_get_text(GTK_ENTRY(widget));
			
			widget = (GtkWidget *) gtk_builder_get_object (builder,
								       "dialogaddgroup_bregex");
			sbregex = gtk_entry_get_text(GTK_ENTRY(widget));
			
			widget = (GtkWidget *) gtk_builder_get_object (builder,
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
				pProgressDialog = gtk_progress_dialog_new (GTK_WINDOW(pWindowAddChannelsGroup));
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
				tmptext = g_strdup_printf(_("Adding channels group: <i>%s</i>"),
					pChannelsGroupInfos->name);
				gtk_progress_dialog_set_text(pProgressDialog, tmptext);
				gtk_progress_dialog_set_percent(pProgressDialog, 0.0);
				g_free(tmptext);

				// Add the group in the database
				if(error == NULL){
					channels_list_db_add_channels_group (app, &dbsync, pChannelsGroupInfos,
					    db_on_group_added, db_on_channels_added, pWindowAddChannelsGroup, &error);
				}

				if(error == NULL){
					tmptext = g_strdup_printf(_("%d channels group(s) have been successfully added."), 1);
					gtk_progress_dialog_set_text(pProgressDialog, tmptext);
					gtk_progress_dialog_set_percent(pProgressDialog, 1.0);
				
					g_free(tmptext);
				}
			}
			
			break;
		case 2:
			// Add custom group
			widget = (GtkWidget *) gtk_builder_get_object (builder,
							       "dialogaddgroup_specialgroupname");
			sgroupname = gtk_entry_get_text(GTK_ENTRY(widget));
			
			/* Check the field */
			if(g_ascii_strcasecmp(sgroupname,"") == 0 && errmsg == NULL){
				errmsg = g_strdup_printf(_("Please enter the group's name !"));
			}
			
			if(errmsg == NULL){
				pProgressDialog = gtk_progress_dialog_new (GTK_WINDOW(pWindowAddChannelsGroup));
				gtk_progress_dialog_set_title(pProgressDialog, _("Adding channels groups"));
				gtk_widget_show(GTK_WIDGET(pProgressDialog));

				FreetuxTVChannelsGroupInfos *pChannelsGroupInfos;		
				pChannelsGroupInfos = freetuxtv_channels_group_infos_new ((gchar*)sgroupname,
					FREETUXTV_CHANNELSGROUP_TYPEGROUP_FAVORITES);
				
				// Update progress dialog
				tmptext = g_strdup_printf(_("Adding channels group: <i>%s</i>"),
					pChannelsGroupInfos->name);
				gtk_progress_dialog_set_text(pProgressDialog, tmptext);
				gtk_progress_dialog_set_percent(pProgressDialog, 0.0);
				g_free(tmptext);

				// Add the group in the database
				if(error == NULL){
					channels_list_db_add_channels_group (app, &dbsync, pChannelsGroupInfos,
					    db_on_group_added, db_on_channels_added, pWindowAddChannelsGroup, &error);
				}

				if(error == NULL){
					tmptext = g_strdup_printf(_("%d channels group(s) have been successfully added."), 1);
					gtk_progress_dialog_set_text(pProgressDialog, tmptext);
					gtk_progress_dialog_set_percent(pProgressDialog, 1.0);
				
					g_free(tmptext);
				}
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

	/*
	// TODO : Destroy the dialog
	if(pProgressDialog){
		g_object_unref(pProgressDialog);
		pProgressDialog = NULL;
	}
	*/
}

static void
db_on_group_added (
    FreetuxTVApp *app, DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    gpointer user_data, GError** error)
{
	g_return_if_fail(app != NULL);
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(pChannelsGroupInfos != NULL);
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);

	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup;
	pWindowAddChannelsGroup = (FreetuxTVWindowAddChannelsGroup*)user_data;
	
	g_return_if_fail(pWindowAddChannelsGroup != NULL);
	g_return_if_fail(FREETUXTV_IS_WINDOW_ADD_CHANNELS_GROUP(pWindowAddChannelsGroup));

	// Send signal that a group is added
	if(*error == NULL){
		g_signal_emit (
		    G_OBJECT (pWindowAddChannelsGroup),
		    g_signals [SIGNAL_CHANNELS_GROUP_ADDED],
		    0, pChannelsGroupInfos, dbsync, error
		    );
	}
}

static void
db_on_channels_added (
    FreetuxTVApp *app, DBSync *dbsync,
    FreetuxTVChannelsGroupInfos* pChannelsGroupInfos,
    gpointer user_data, GError** error)
{
	g_return_if_fail(app != NULL);
	g_return_if_fail(dbsync != NULL);
	g_return_if_fail(pChannelsGroupInfos != NULL);
	g_return_if_fail(error != NULL);

	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup;
	pWindowAddChannelsGroup = (FreetuxTVWindowAddChannelsGroup*)user_data;
	
	g_return_if_fail(pWindowAddChannelsGroup != NULL);
	g_return_if_fail(FREETUXTV_IS_WINDOW_ADD_CHANNELS_GROUP(pWindowAddChannelsGroup));

	// Send signal that a group is added
	if(*error == NULL){
		g_signal_emit (
		    G_OBJECT (pWindowAddChannelsGroup),
		    g_signals [SIGNAL_CHANNELS_ADDED],
		    0, pChannelsGroupInfos, dbsync, error
		    );
	}
}

FreetuxTVApp*
freetuxtv_window_add_channels_group_get_app(FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup)
{
	g_return_val_if_fail(pWindowAddChannelsGroup != NULL, NULL);
	g_return_val_if_fail(FREETUXTV_IS_WINDOW_ADD_CHANNELS_GROUP(pWindowAddChannelsGroup), NULL);
	
	FreetuxTVWindowAddChannelsGroupPrivate* priv;
	priv = FREETUXTV_WINDOW_ADD_CHANNELS_GROUP_PRIVATE(pWindowAddChannelsGroup);
	
	return priv->app;
}

static gboolean
do_close_clicked (gpointer user_data)
{
	FreetuxTVWindowAddChannelsGroup* pWindowAddChannelsGroup;

	pWindowAddChannelsGroup = (FreetuxTVWindowAddChannelsGroup*)user_data;
	gtk_widget_destroy (GTK_WIDGET(pWindowAddChannelsGroup));

	return FALSE;
}

static void
on_buttonclose_clicked (GtkButton *button, gpointer user_data)
{
	// We do the destruction after the button clicked event is finished
	g_idle_add (do_close_clicked, user_data);
}
