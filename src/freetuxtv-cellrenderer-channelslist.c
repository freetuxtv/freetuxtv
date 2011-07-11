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

#include "freetuxtv-cellrenderer-channelslist.h"

#include <config.h>

#include <gdk/gdk.h>

G_DEFINE_TYPE (FreetuxTVCellRendererChannelsList, freetuxtv_cellrenderer_channelslist, GTK_TYPE_CELL_RENDERER);

enum
{
	PROP_TYPE = 1,
	PROP_NAME = 2,
	PROP_LOGO = 3,
	PROP_ISPLAYING = 4
};

GtkCellRenderer *
freetuxtv_cellrenderer_channelslist_new (void)
{
	return g_object_new(FREETUXTV_TYPE_CELLRENDERER_CHANNELSLIST, NULL);
}


static GType
freetuxtv_cellrenderer_channelslist_type_get_type (void)
{
	static GType cellrenderer_channelslist_type_type = 0;
	static GEnumValue cellrenderer_channelslist_type[] = {
		{CELLRENDERER_TYPE_CHANNELS_GROUP, "0", "Channels group"},
		{CELLRENDERER_TYPE_CHANNEL, "1", "Channel"},
		{0, NULL, NULL},
	};
	
	if (!cellrenderer_channelslist_type_type) {
		cellrenderer_channelslist_type_type =
			g_enum_register_static ("FreetuxTVCellRendererChannelsListType",
						cellrenderer_channelslist_type);
	}
	return cellrenderer_channelslist_type_type;
}

static void
freetuxtv_cellrenderer_channelslist_get_property (GObject    *object,
						  guint       param_id,
						  GValue     *value,
						  GParamSpec *psec)
{
	FreetuxTVCellRendererChannelsList *self = FREETUXTV_CELLRENDERER_CHANNELSLIST(object);
	
	switch (param_id){
	case PROP_TYPE:
		g_value_set_enum (value, self->type);
		break;
	case PROP_NAME:
		g_value_set_string(value, self->name);
		break;
	case PROP_LOGO:
		g_value_set_string(value, self->logo);
		break;
	case PROP_ISPLAYING:
		g_value_set_boolean(value, self->is_playing);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, psec);
		break;
	}
}

static void
freetuxtv_cellrenderer_channelslist_set_property (GObject      *object,
						  guint         param_id,
						  const GValue *value,
						  GParamSpec   *pspec)
{	
	FreetuxTVCellRendererChannelsList *self = FREETUXTV_CELLRENDERER_CHANNELSLIST(object);
	
	switch (param_id){
	case PROP_TYPE:
		self->type = g_value_get_enum(value);
		break;
	case PROP_NAME:
		self->name = g_strdup(g_value_get_string(value));
		break;
	case PROP_LOGO:
		self->logo = g_strdup(g_value_get_string(value));
		break;
	case PROP_ISPLAYING:
		self->is_playing = g_value_get_boolean(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, param_id, pspec);
		break;
	}
}

#if GTK_API_VERSION == 3
static void
freetuxtv_cellrenderer_channelslist_get_size (GtkCellRenderer *cell,
                                              GtkWidget *widget,
                                              const GdkRectangle *cell_area,
                                              gint *x_offset,
                                              gint *y_offset,
                                              gint *width,
                                              gint *height)
#else
static void
freetuxtv_cellrenderer_channelslist_get_size (GtkCellRenderer *cell,
					      GtkWidget       *widget,
					      GdkRectangle    *cell_area,
					      gint            *x_offset,
					      gint            *y_offset,
					      gint            *width,
					      gint            *height)
#endif
{

	FreetuxTVCellRendererChannelsList *self = FREETUXTV_CELLRENDERER_CHANNELSLIST (cell);

	gint calc_width;
	gint calc_height;

	gint cell_xpad;
	gint cell_ypad;
	gfloat cell_xalign;
	gfloat cell_yalign;
	GtkAllocation allocation;

#if GTK_API_VERSION == 3
	gtk_cell_renderer_get_padding(cell, &cell_xpad, &cell_ypad);
	gtk_cell_renderer_get_alignment (cell, &cell_xalign, &cell_yalign);
	gtk_widget_get_allocation(widget, &allocation);
#else
	cell_xpad = cell->xpad;
	cell_ypad = cell->ypad;
	cell_xalign = cell->xalign;
	cell_yalign = cell->yalign;
	allocation = widget->allocation;
#endif
	
	calc_width  = (gint) cell_xpad * 2;
	calc_height = (gint) cell_xpad * 2;
	if(self->type == CELLRENDERER_TYPE_CHANNEL){
		calc_height+=40;
	}else{
		calc_height+=20;
	}
	
	if (cell_area){
		calc_width += allocation.width;
	}

	if (width)
		*width = calc_width;
	
	if (height)
		*height = calc_height;
	
	if (cell_area){
		if (x_offset){
			*x_offset = cell_xalign * (cell_area->width - calc_width);
			*x_offset = MAX (*x_offset, 0);
		}
		
		if (y_offset){
			*y_offset = cell_yalign * (cell_area->height - calc_height);
			*y_offset = MAX (*y_offset, 0);
		}
		/*
		g_print("size [%d,%d] [%d,%d] [%d,%d] [%f, %f] [%d,%d]\n", 
			cell_area->x, cell_area->y, 
			cell_area->width, cell_area->height,
			*x_offset, *y_offset,
			cell->xalign, cell->yalign, *width, *height);
		*/
	}
}

#if GTK_API_VERSION == 3
static void
freetuxtv_cellrenderer_channelslist_render (GtkCellRenderer *cell,
                                            cairo_t *cr,
                                            GtkWidget *widget,
                                            const GdkRectangle *background_area,
                                            const GdkRectangle *cell_area,
                                            GtkCellRendererState flags)
#else
static void
freetuxtv_cellrenderer_channelslist_render (GtkCellRenderer *cell,
					    GdkWindow       *window,
					    GtkWidget       *widget,
					    GdkRectangle    *background_area,
					    GdkRectangle    *cell_area,
					    GdkRectangle    *expose_area,
					    guint            flags)
#endif
{
	FreetuxTVCellRendererChannelsList *self = FREETUXTV_CELLRENDERER_CHANNELSLIST (cell);
	GtkStateType state;
	gint width, height;
	gint x_offset, y_offset;
	PangoLayout *layout;

	gboolean bHasFocus;

#if GTK_API_VERSION == 3
	GtkStyleContext *pStyleContext;
	pStyleContext = gtk_widget_get_style_context (GTK_WIDGET(cell));
	GdkWindow* window = gtk_widget_get_window  (GTK_WIDGET(cell));
	bHasFocus = gtk_widget_has_focus (GTK_WIDGET(cell));
#else
	GtkStyle* pStyle;
	pStyle = widget->style;
	bHasFocus = GTK_WIDGET_HAS_FOCUS (cell);
#endif

	if (bHasFocus){
		state = GTK_STATE_ACTIVE;
	}else{
		state = GTK_STATE_NORMAL;
	}

	freetuxtv_cellrenderer_channelslist_get_size (cell, widget, cell_area,
						      &x_offset, &y_offset,
						      &width, &height);
	
	layout = gtk_widget_create_pango_layout (widget,
						 self->name);
	//pango_layout_set_width(layout, 700);
	//pango_layout_set_height(layout, 20);
	pango_layout_set_ellipsize (layout, PANGO_ELLIPSIZE_END);
	/*
	gtk_paint_flat_box (widget->style, window, GTK_STATE_SELECTED,
			    GTK_SHADOW_NONE , NULL, widget,
			    NULL,
			    0, cell_area->y,
			    cell_area->x + width, cell_area->height);
	*/
	/*
	gtk_paint_hline (widget->style, window, state,
                         NULL, widget, NULL,
			 0, widget->allocation.width, cell_area->y);
	gtk_paint_hline (widget->style, window, state,
                         NULL, widget, NULL,
			 cell_area->x, cell_area->width, cell_area->y + 2);
	gtk_paint_hline (widget->style, window, state,
                         NULL, widget, NULL,
			 0, 176, cell_area->y + 3);
	*/

	GdkPixbuf* logo;

	if ((flags & GTK_CELL_RENDERER_SELECTED) != 0){
		state = GTK_STATE_SELECTED;
	}

	if(self->type == CELLRENDERER_TYPE_CHANNEL){

		// Backgroung of the cell
		if(self->is_playing){
			state = GTK_STATE_SELECTED;		

#if GTK_API_VERSION == 3
			gtk_style_context_set_state (pStyleContext, state);
			gtk_style_context_set_background (pStyleContext, window);

			gtk_render_background (pStyleContext, cr,
			                       0, cell_area->y,
			                       cell_area->x + width, cell_area->height);
#else
			gtk_style_set_background (pStyle, window, state);
			gtk_style_apply_default_background  (pStyle, window, TRUE, state, NULL,
			                                     0, cell_area->y, cell_area->x + width, cell_area->height);
#endif			
			
			// g_print("cell : %d %d\n", cell_area->x, width);
		}
		
		logo = gdk_pixbuf_new_from_file(self->logo, NULL);

		int cell_xpad;
		int cell_ypad;
		
#if GTK_API_VERSION == 3
		gtk_cell_renderer_get_padding (cell, &cell_xpad, &cell_ypad);

		gdk_cairo_set_source_pixbuf(cr, logo, (double)cell_xpad, (double)cell_ypad);
		cairo_paint(cr);

		gtk_render_layout (pStyleContext, cr,
		                   cell_xpad * 2 + gdk_pixbuf_get_width(logo) + 5,
		                   cell_area->y + 15,
		                   layout);
#else
		cell_xpad = cell->xpad;
		cell_ypad = cell->ypad;
		
		gdk_draw_pixbuf (GDK_DRAWABLE(window), NULL, logo,
				 0, 0, cell_xpad + 1, cell_area->y + cell_ypad + 1,
				 -1, -1,
				 GDK_RGB_DITHER_NONE, 0,0);

		gtk_paint_layout (widget->style, window, state,
				  TRUE, NULL, widget, NULL,
				  cell->xpad * 2 + gdk_pixbuf_get_width(logo) + 5, cell_area->y + 15,
				  layout);
#endif
		
		if(logo){
			g_object_unref(logo);
			logo = NULL;
		}
	}else{

#if GTK_API_VERSION == 3
		gtk_style_context_set_state (pStyleContext, state);
		gtk_style_context_set_background (pStyleContext, window);

		gtk_render_background (pStyleContext, cr,
		                       0, cell_area->y,
		                       cell_area->x + width, cell_area->height);

		gtk_render_layout (pStyleContext, cr,
		                   cell_area->x, cell_area->y + 3,
		                   layout);
#else
		gtk_style_set_background (widget->style, window, state);
		gtk_style_apply_default_background  (widget->style, window, TRUE,
						     state, NULL,
						     0, cell_area->y, cell_area->x + width, cell_area->height);
		
		gtk_paint_layout (widget->style, window, state,
				  TRUE, NULL, widget, NULL,
				  cell_area->x, cell_area->y + 3, layout);
#endif
	}
}


static void
freetuxtv_cellrenderer_channelslist_finalize (GObject *object)
{	
	GObjectClass *parent_class;  
	parent_class = G_OBJECT_CLASS (freetuxtv_cellrenderer_channelslist_parent_class);
	(* G_OBJECT_CLASS (parent_class)->finalize) (object);
}

static void
freetuxtv_cellrenderer_channelslist_class_init (FreetuxTVCellRendererChannelsListClass *klass)
{
      
	GtkCellRendererClass *cell_class   = GTK_CELL_RENDERER_CLASS(klass);
	GObjectClass         *object_class = G_OBJECT_CLASS(klass);
	
	object_class->finalize = freetuxtv_cellrenderer_channelslist_finalize;
	
	object_class->get_property = freetuxtv_cellrenderer_channelslist_get_property;
	object_class->set_property = freetuxtv_cellrenderer_channelslist_set_property;
	
	cell_class->get_size = freetuxtv_cellrenderer_channelslist_get_size;
	cell_class->render   = freetuxtv_cellrenderer_channelslist_render;
	
	g_object_class_install_property (object_class,
					 PROP_TYPE,
					 g_param_spec_enum ("type",
							    "Type",
							    "The type of data in the cell",
							    FREETUXTV_TYPE_CELLRENDERER_CHANNELSLIST_TYPE,
							    CELLRENDERER_TYPE_CHANNELS_GROUP,
							    G_PARAM_READWRITE));
  
	g_object_class_install_property (object_class,
					 PROP_NAME,
					 g_param_spec_string ("name",
							      "Name",
							      "The name to display in the cell",
							      NULL,
							      G_PARAM_READWRITE));
	
	g_object_class_install_property (object_class,
					 PROP_LOGO,
					 g_param_spec_string ("logo",
							      "Logo",
							      "Logo to display in the cell",
							      NULL,
							      G_PARAM_READWRITE));
	
	g_object_class_install_property (object_class,
					 PROP_ISPLAYING,
					 g_param_spec_boolean ("isplaying",
							       "Channel is playing",
							       "Say if the channel is playing",
							       FALSE,
							       G_PARAM_READWRITE));
}

static void
freetuxtv_cellrenderer_channelslist_init (FreetuxTVCellRendererChannelsList *self)
{
#if GTK_API_VERSION == 3
	gtk_cell_renderer_set_padding (GTK_CELL_RENDERER(self), 2, 2);
#else
	GTK_CELL_RENDERER(self)->mode = GTK_CELL_RENDERER_MODE_INERT;
	GTK_CELL_RENDERER(self)->xpad = 2;
	GTK_CELL_RENDERER(self)->ypad = 2;
#endif
}
