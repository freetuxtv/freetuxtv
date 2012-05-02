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

#ifndef _DB_EVOLUTION_INSTANCE_H_
#define _DB_EVOLUTION_INSTANCE_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define DB_EVOLUTION_TYPE_INSTANCE             (db_evolution_instance_get_type ())
#define DB_EVOLUTION_INSTANCE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), DB_EVOLUTION_TYPE_INSTANCE, DbEvolutionInstance))
#define DB_EVOLUTION_INSTANCE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), DB_EVOLUTION_TYPE_INSTANCE, DbEvolutionInstanceClass))
#define DB_EVOLUTION_IS_INSTANCE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DB_EVOLUTION_TYPE_INSTANCE))
#define DB_EVOLUTION_IS_INSTANCE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), DB_EVOLUTION_TYPE_INSTANCE))
#define DB_EVOLUTION_INSTANCE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), DB_EVOLUTION_TYPE_INSTANCE, DbEvolutionInstanceClass))

typedef struct _DbEvolutionInstanceClass DbEvolutionInstanceClass;
typedef struct _DbEvolutionInstance DbEvolutionInstance;

struct _DbEvolutionInstanceClass
{
	GObjectClass parent_class;
};

struct _DbEvolutionInstance
{
	GObject parent_instance;

	gchar* (*get_current_db_version_func) (gpointer user_data, GError** error);
	void (*set_current_db_version_func) (gchar* szVersion, gpointer user_data,
	                                     GError** error);
	
	int (*compare_db_version_func) (gchar* szVersion1, gchar* szVersion2, 
	                                gpointer user_data, GError** error);
	gboolean (*exec_query_func) (const gchar* szQuery, gpointer user_data,
	                             GError** error);
};

#define DBEVOLUTION_LASTDBVERSION_TAGS  "-- @libdbevolution-lastdbversion="
#define DBEVOLUTION_DBVERSION_TAGS		"-- @libdbevolution-dbversion="

GType db_evolution_instance_get_type (void) G_GNUC_CONST;

DbEvolutionInstance*
db_evolution_instance_new(const gchar* szScriptFilename, GLogFunc log_func);

gboolean
db_evolution_instance_do_creation(DbEvolutionInstance* pDbEvolutionInstance,
                                  gpointer user_data, GError** error);

gboolean
db_evolution_instance_do_evolution(DbEvolutionInstance* pDbEvolutionInstance,
                                   gpointer user_data, GError** error);

G_END_DECLS

#endif /* _DB_EVOLUTION_INSTANCE_H_ */
