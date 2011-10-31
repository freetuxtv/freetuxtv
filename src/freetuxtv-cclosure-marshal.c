/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * freetuxtv
 * Copyright (C) Eric Beuque 2011 <eric.beuque@gmail.com>
 * 
freetuxtv is free software: you can redistribute it and/or modify it
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

#include "freetuxtv-cclosure-marshal.h"

void
g_cclosure_marshal_VOID__OBJECT_POINTER_POINTER (
    GClosure     *closure,
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
