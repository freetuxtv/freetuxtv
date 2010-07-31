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

#include "db-evolution-instance.h"
#include <string.h>

typedef struct _DbEvolutionInstancePrivate DbEvolutionInstancePrivate;
struct _DbEvolutionInstancePrivate
{
	gchar* szScriptFilename;

	int idLogFuncHandler;
};

#define DB_EVOLUTION_INSTANCE_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), DB_EVOLUTION_TYPE_INSTANCE, DbEvolutionInstancePrivate))

G_DEFINE_TYPE (DbEvolutionInstance, db_evolution_instance, G_TYPE_OBJECT);

#define DBEVOLUTION_LOG_DOMAIN			"DBEvolution"

static void
db_evolution_instance_init (DbEvolutionInstance *object)
{	
	DbEvolutionInstancePrivate* priv;
	priv = DB_EVOLUTION_INSTANCE_PRIVATE(object);

	object->get_current_db_version_func = NULL;
	object->set_current_db_version_func = NULL;
	object->compare_db_version_func = NULL;
	object->exec_query_func = NULL;
	
	priv->szScriptFilename = NULL;
	priv->idLogFuncHandler = -1;
}

static void
db_evolution_instance_finalize (GObject *object)
{
	DbEvolutionInstancePrivate* priv;
	priv = DB_EVOLUTION_INSTANCE_PRIVATE(object);

	if(priv->szScriptFilename){
		g_free(priv->szScriptFilename);
	}

	if(priv->idLogFuncHandler >= 0){
		g_log_remove_handler(DBEVOLUTION_LOG_DOMAIN, priv->idLogFuncHandler);
	}

	G_OBJECT_CLASS (db_evolution_instance_parent_class)->finalize (object);
}

static void
db_evolution_instance_class_init (DbEvolutionInstanceClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GObjectClass* parent_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (DbEvolutionInstancePrivate));

	object_class->finalize = db_evolution_instance_finalize;
}

DbEvolutionInstance*
db_evolution_instance_new(const gchar* szScriptFilename, GLogFunc log_func)
{
	g_return_val_if_fail(szScriptFilename != NULL, NULL);

	int idLogFuncHandler = -1;
	if(log_func){
		idLogFuncHandler = g_log_set_handler (DBEVOLUTION_LOG_DOMAIN, G_LOG_LEVEL_MASK,
                                              log_func, NULL);
	}

	DbEvolutionInstance *pDbEvolutionInstance;
	pDbEvolutionInstance = g_object_new (DB_EVOLUTION_TYPE_INSTANCE, NULL);

	if(pDbEvolutionInstance){
		DbEvolutionInstancePrivate* priv;
		priv = DB_EVOLUTION_INSTANCE_PRIVATE(pDbEvolutionInstance);

		priv->szScriptFilename=g_strdup(szScriptFilename);
		priv->idLogFuncHandler = idLogFuncHandler;
	}else{
		if(idLogFuncHandler >= 0){
			g_log_remove_handler(DBEVOLUTION_LOG_DOMAIN, idLogFuncHandler);
		}
	}

	return pDbEvolutionInstance;
}

gboolean
db_evolution_instance_do_creation(DbEvolutionInstance* pDbEvolutionInstance,
                                  gpointer user_data, GError** error)
{
	g_return_val_if_fail(pDbEvolutionInstance != NULL, FALSE);
	g_return_val_if_fail(DB_EVOLUTION_IS_INSTANCE(pDbEvolutionInstance), FALSE);
	g_return_val_if_fail(error != NULL, FALSE);
	g_return_val_if_fail(*error == NULL, FALSE);

	g_return_val_if_fail(pDbEvolutionInstance->set_current_db_version_func != NULL, FALSE);
	g_return_val_if_fail(pDbEvolutionInstance->exec_query_func != NULL, FALSE);

	gboolean bRes = FALSE;

	DbEvolutionInstancePrivate* priv;
	priv = DB_EVOLUTION_INSTANCE_PRIVATE(pDbEvolutionInstance);

	gchar* szTmpVersion = NULL;
	gchar* szNextVersion = NULL;
	g_log(DBEVOLUTION_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Starting database creation\n");

	GIOChannel *pIOChannelIn = NULL;
	gchar *szScriptFilename;
	if(*error == NULL){
		pIOChannelIn = g_io_channel_new_file(priv->szScriptFilename,
		                                     "r", error);	
	}

	if(*error == NULL){
		GIOStatus iostatus;
		gchar* line;
		gsize  linelenght;
		GString *query;
		gsize tagslen;
		gboolean foundVersion;
		gboolean installVersion;

		tagslen = strlen(DBEVOLUTION_DBVERSION_TAGS);
		
		query = g_string_new ("");

		// Parse the file, line by line, to check the database version
		do {
			installVersion = FALSE;
			foundVersion = FALSE;
			szNextVersion = NULL;
			iostatus = g_io_channel_read_line (pIOChannelIn, &line, &linelenght,
				                               NULL, error);
			if(iostatus == G_IO_STATUS_NORMAL && *error == NULL){
				// Check if we detect a new version if the file
				if(g_ascii_strncasecmp (line, DBEVOLUTION_DBVERSION_TAGS, tagslen) == 0){
					foundVersion = TRUE;
					// If we have already a version, we run the update script
					if(szTmpVersion){
						installVersion = TRUE;
					}
					szNextVersion = g_strndup(line+tagslen, linelenght-tagslen-1); // -1 remove the \n
				}else{
					g_string_append(query, line);
				}
			}

			if(iostatus == G_IO_STATUS_EOF && *error == NULL){
				if(szTmpVersion){
					installVersion = TRUE;
				}
			}

			if(installVersion && *error == NULL){
				if(*error == NULL){
					g_log(DBEVOLUTION_LOG_DOMAIN, G_LOG_LEVEL_INFO,
					      "Installing database version %s\n", szTmpVersion);
					// We run the script to migrate the database
					pDbEvolutionInstance->exec_query_func(query->str, user_data, error);
				}
				if(*error == NULL){
					pDbEvolutionInstance->set_current_db_version_func(szTmpVersion, user_data, error);
				}
						
				g_string_erase (query, 0, -1);
				g_free(szTmpVersion);
				szTmpVersion = NULL;
			}

			if(foundVersion){
				szTmpVersion = szNextVersion;
			}
			
		}while(iostatus == G_IO_STATUS_NORMAL && *error == NULL);		

		if(query){
			g_string_free(query, TRUE);
			query = NULL;
		}
	}
	
	if(pIOChannelIn){
       g_io_channel_shutdown(pIOChannelIn, TRUE, error);
	}
	
	return bRes;
}

gboolean
db_evolution_instance_do_evolution(DbEvolutionInstance* pDbEvolutionInstance,
                                   gpointer user_data, GError** error)
{
	g_return_val_if_fail(pDbEvolutionInstance != NULL, FALSE);
	g_return_val_if_fail(DB_EVOLUTION_IS_INSTANCE(pDbEvolutionInstance), FALSE);
	g_return_val_if_fail(error != NULL, FALSE);
	g_return_val_if_fail(*error == NULL, FALSE);

	g_return_val_if_fail(pDbEvolutionInstance->get_current_db_version_func != NULL, FALSE);
	g_return_val_if_fail(pDbEvolutionInstance->set_current_db_version_func != NULL, FALSE);
	g_return_val_if_fail(pDbEvolutionInstance->compare_db_version_func != NULL, FALSE);
	g_return_val_if_fail(pDbEvolutionInstance->exec_query_func != NULL, FALSE);

	gboolean bRes = FALSE;

	DbEvolutionInstancePrivate* priv;
	priv = DB_EVOLUTION_INSTANCE_PRIVATE(pDbEvolutionInstance);

	gchar* szInitialeVersion;
	gchar* szCurrentVersion = NULL;
	gchar* szTmpVersion = NULL;
	gchar* szNextVersion = NULL;
	g_log(DBEVOLUTION_LOG_DOMAIN, G_LOG_LEVEL_INFO,
	      "Starting database evolution\n");

	// Get the current version of the database
	szInitialeVersion = pDbEvolutionInstance->get_current_db_version_func(user_data, error);
	if(*error == NULL){
		szCurrentVersion = g_strdup(szInitialeVersion);
		g_log(DBEVOLUTION_LOG_DOMAIN, G_LOG_LEVEL_INFO,
		      "Current database version is %s\n", szCurrentVersion);
	}

	GIOChannel *pIOChannelIn = NULL;
	gchar *szScriptFilename;
	if(*error == NULL){
		pIOChannelIn = g_io_channel_new_file(priv->szScriptFilename,
		                                     "r", error);	
	}

	if(*error == NULL){
		GIOStatus iostatus;
		gchar* line;
		gsize  linelenght;
		GString *query;
		gsize tagslen;
		gboolean updateVersion;
		gboolean foundVersion;

		tagslen = strlen(DBEVOLUTION_DBVERSION_TAGS);
		
		query = g_string_new ("");

		// Parse the file, line by line, to check the database version
		do {
			updateVersion = FALSE;
			foundVersion = FALSE;
			szNextVersion = NULL;
			iostatus = g_io_channel_read_line (pIOChannelIn, &line, &linelenght,
				                               NULL, error);
			if(iostatus == G_IO_STATUS_NORMAL && *error == NULL){
				// Check if we detect a new version in the file
				if(g_ascii_strncasecmp (line, DBEVOLUTION_DBVERSION_TAGS, tagslen) == 0){
					foundVersion = TRUE;
					// If we have already a version, we run the update script
					if(szTmpVersion){
						updateVersion = TRUE;
					}
					szNextVersion = g_strndup(line+tagslen, linelenght-tagslen-1); // -1 remove the \n
				}else{
					g_string_append(query, line);
				}
			}

			if(iostatus == G_IO_STATUS_EOF && *error == NULL){
				if(szTmpVersion){
					updateVersion = TRUE;
				}
			}

			if(updateVersion && *error == NULL){
				if(pDbEvolutionInstance->compare_db_version_func(szCurrentVersion, szTmpVersion, user_data, error) < 0){
					if(*error == NULL){
						g_log(DBEVOLUTION_LOG_DOMAIN, G_LOG_LEVEL_INFO,
						      "Migrate from database version %s to %s\n", szCurrentVersion, szTmpVersion);
						// We run the script to migrate the database
						pDbEvolutionInstance->exec_query_func(query->str, user_data, error);
					}
					if(*error == NULL){
						pDbEvolutionInstance->set_current_db_version_func(szTmpVersion, user_data, error);
					}
					if(*error == NULL){
						if(szCurrentVersion){
							g_free(szCurrentVersion);
							szCurrentVersion = NULL;
						}
						szCurrentVersion = g_strdup(szTmpVersion);
					}
				}
						
				g_string_erase (query, 0, -1);
				g_free(szTmpVersion);
				szTmpVersion = NULL;
			}

			if(foundVersion){
				szTmpVersion = szNextVersion;
			}
			
		}while(iostatus == G_IO_STATUS_NORMAL && *error == NULL);

		if(query){
			g_string_free(query, TRUE);
			query = NULL;
		}
	}

	if(szCurrentVersion){
		g_free(szCurrentVersion);
		szCurrentVersion = NULL;
	}
	
	if(pIOChannelIn){
       g_io_channel_shutdown(pIOChannelIn, TRUE, error);
	}
	
	return bRes;
}
