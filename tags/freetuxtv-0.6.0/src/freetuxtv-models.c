/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4-*- */
/*
 * FreetuxTV is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 *
 * FreetuxTV is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Glade; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include <glib/gstdio.h>
#include <gtk/gtk.h>

#include "freetuxtv-models.h"

#include "freetuxtv-app.h"
#include "freetuxtv-i18n.h"


typedef struct _CBXMLData
{
	FreetuxTVApp *app;
	GtkTreeModel *model;
	GtkTreeIter iter1;
	GtkTreeIter iter2;	
} CBXMLData;

static void 
xml_start_cb(GMarkupParseContext *context, const gchar *element_name,
	     const gchar **attribute_names, const gchar **attribute_values,
	     gpointer data, GError **error);

void
load_model_channels_group_from_file(FreetuxTVApp* app, GtkTreeModel *model, GError** error)
{
	g_return_if_fail(error != NULL);
	g_return_if_fail(*error == NULL);
	
	// Get the good file to use
	gchar* file = NULL;
	file = g_build_filename(g_get_user_cache_dir(), "freetuxtv", "channels_groups.dat", NULL);
	
	struct stat file_stat;
	if(g_stat (file, &file_stat) != 0){
		g_free(file);
		file = g_build_filename(app->paths.datadir, "channels_groups.xml", NULL);
	}

	// Clear the model
	gtk_tree_store_clear (GTK_TREE_STORE(model));
		
	// Parse the file	
	gsize filelen;
	static GMarkupParser parser = { xml_start_cb, NULL, NULL, NULL, NULL};
	GMarkupParseContext *context;
	
	CBXMLData cbxmldata;
	cbxmldata.app = app;
	cbxmldata.model = model;
	context = g_markup_parse_context_new (&parser, 
					      G_MARKUP_DO_NOT_USE_THIS_UNSUPPORTED_FLAG,
					      &cbxmldata, NULL);
	
	gchar *xml_data;
	
	g_file_get_contents (file, &xml_data, &filelen, error);		
	
	if(*error == NULL){
		g_markup_parse_context_parse (context, xml_data, -1, error);
	}
	
	if(file != NULL){
		g_free(file);
	}	
}


static void 
xml_start_cb(GMarkupParseContext *context, const gchar *element_name,
	     const gchar **attribute_names, const gchar **attribute_values,
	     gpointer data, GError **error)
{

	CBXMLData* cbxmldata = (CBXMLData*)data;

	int nb_attrs;
	int i;
	nb_attrs = g_strv_length((gchar **)attribute_names);

	if(g_ascii_strcasecmp(element_name, "language") == 0){
		const gchar* lang = NULL;
		for(i=0; i<nb_attrs; i++){
			if(g_ascii_strcasecmp(attribute_names[i], "lang") == 0){
				lang = _(attribute_values[i]);
			}
		}

		gtk_tree_store_append (GTK_TREE_STORE(cbxmldata->model), &(cbxmldata->iter1), NULL);
		gtk_tree_store_set (GTK_TREE_STORE(cbxmldata->model), &(cbxmldata->iter1),
				    MODEL_CHANNELSGROUP_NAME, lang, -1);
		
	}

	if(g_ascii_strcasecmp(element_name, "channels_group") == 0){
		const gchar* name = NULL;
		const gchar* uri = NULL;
		const gchar* required_isp = NULL;
		const gchar* bregex = NULL;
		const gchar* eregex = NULL;		
		for(i=0; i<nb_attrs; i++){
			if(g_ascii_strcasecmp(attribute_names[i], "name") == 0){
				name = _(attribute_values[i]);
			}
			if(g_ascii_strcasecmp(attribute_names[i], "uri") == 0){
				uri = attribute_values[i];
			}
			if(g_ascii_strcasecmp(attribute_names[i], "required_isp") == 0){
				required_isp = attribute_values[i];
			}
			if(g_ascii_strcasecmp(attribute_names[i], "bregex") == 0){
				bregex = attribute_values[i];
			}
			if(g_ascii_strcasecmp(attribute_names[i], "eregex") == 0){
				eregex = attribute_values[i];
			}
		}

		gtk_tree_store_append (GTK_TREE_STORE(cbxmldata->model), &(cbxmldata->iter2), &(cbxmldata->iter1));
		gtk_tree_store_set (GTK_TREE_STORE(cbxmldata->model), &(cbxmldata->iter2),
				    MODEL_CHANNELSGROUP_NAME, name, MODEL_CHANNELSGROUP_URI, uri,
				    MODEL_CHANNELSGROUP_REQUIREDISP, required_isp, 
				    MODEL_CHANNELSGROUP_BREGEX, bregex,
				    MODEL_CHANNELSGROUP_EREGEX, eregex, -1);
		
	}
}
