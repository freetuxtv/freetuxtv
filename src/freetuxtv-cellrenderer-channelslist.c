/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8-*- */
/*
 * freetuxtv
 * Copyright (C) FreetuxTV Team's 2008
 * Project homepage : http://code.google.com/p/freetuxtv/
 * 
 * freetuxtv is free software.
 * 
 */

#include "freetuxtv-cellrenderer-channelslist.h"

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

static void
freetuxtv_cellrenderer_channelslist_get_size (GtkCellRenderer *cell,
					      GtkWidget       *widget,
					      GdkRectangle    *cell_area,
					      gint            *x_offset,
					      gint            *y_offset,
					      gint            *width,
					      gint            *height)
{

	FreetuxTVCellRendererChannelsList *self = FREETUXTV_CELLRENDERER_CHANNELSLIST (cell);

	gint calc_width;
	gint calc_height;
	
	g_print("widget : %d\n", widget->allocation.width);
	calc_width  = (gint) cell->xpad * 2 + widget->allocation.width;
	calc_height = (gint) cell->ypad * 2;
	if(self->type == CELLRENDERER_TYPE_CHANNEL){
		calc_height+=40;
	}else{
		calc_height+=20;
	}

	if (cell_area){
		// calc_width -= cell_area->x + cell_area->width;
	}

	if (width)
		*width = calc_width;
	
	if (height)
		*height = calc_height;
	
	if (cell_area){
		if (x_offset){
			*x_offset = cell->xalign * (cell_area->width - calc_width);
			*x_offset = MAX (*x_offset, 0);
		}
		
		if (y_offset){
			*y_offset = cell->yalign * (cell_area->height - calc_height);
			*y_offset = MAX (*y_offset, 0);
		}
		
		g_print("size [%d,%d] [%d,%d] [%d,%d] [%f, %f] [%d,%d]\n", 
			cell_area->x, cell_area->y, 
			cell_area->width, cell_area->height,
			*x_offset, *y_offset,
			cell->xalign, cell->yalign, *width, *height);
	}
}


static void
freetuxtv_cellrenderer_channelslist_render (GtkCellRenderer *cell,
					    GdkWindow       *window,
					    GtkWidget       *widget,
					    GdkRectangle    *background_area,
					    GdkRectangle    *cell_area,
					    GdkRectangle    *expose_area,
					    guint            flags)
{
	FreetuxTVCellRendererChannelsList *self = FREETUXTV_CELLRENDERER_CHANNELSLIST (cell);
	GtkStateType state;
	gint width, height;
	gint x_offset, y_offset;
	PangoLayout *layout;

	if (GTK_WIDGET_HAS_FOCUS (cell)){			
		state = GTK_STATE_ACTIVE;
	}else{
		state = GTK_STATE_NORMAL;
	}

	freetuxtv_cellrenderer_channelslist_get_size (cell, widget, cell_area,
						      &x_offset, &y_offset,
						      &width, &height);
	
	layout = gtk_widget_create_pango_layout (widget,
						 self->name);
	//pango_layout_set_width(layout, 30);
	//pango_layout_set_height(layout, 20);
	pango_layout_set_ellipsize (layout, PANGO_ELLIPSIZE_END);
	/*
	gtk_paint_flat_box (widget->style, window, GTK_STATE_SELECTED,
			    GTK_SHADOW_NONE , NULL, widget,
			    NULL,
			    cell_area->x, cell_area->y,
			    cell_area->width, cell_area->height);

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
	if(self->type == CELLRENDERER_TYPE_CHANNEL){

		if (flags & GTK_CELL_RENDERER_SELECTED != 0){
			state = GTK_STATE_SELECTED;
		}

		// Backgroung of the cell
		if(self->is_playing){
			state = GTK_STATE_SELECTED;		
			gtk_style_set_background (widget->style, window, state);			
			gtk_style_apply_default_background  (widget->style, window, TRUE,
							     state, NULL,
							     0, cell_area->y, cell_area->x+width , height);
		
			// g_print("cell : %d %d\n", cell_area->x, width);
		}
		
		logo = gdk_pixbuf_new_from_file(self->logo, NULL);
		
		gdk_draw_pixbuf (GDK_DRAWABLE(window), NULL, logo,
				 0, 0, cell->xpad + 1, cell_area->y + cell->ypad + 1,
				 -1, -1,
				 GDK_RGB_DITHER_NONE, 0,0);

		gtk_paint_layout (widget->style, window, state,
				  TRUE, NULL, widget, NULL,
				  cell->xpad * 2 + gdk_pixbuf_get_width(logo) + 5, cell_area->y + 15,
				  layout);
	}else{
		
		gtk_style_set_background (widget->style, window, state);
		
		gtk_style_apply_default_background  (widget->style, window, TRUE,
						     state, NULL,
						     0, cell_area->y, cell_area->x + cell_area->width, height);
		
		// g_print("cell : %d %d\n", cell_area->x, width);

		gtk_paint_layout (widget->style, window, state,
				  TRUE, NULL, widget, NULL,
				  cell_area->x, cell_area->y + 3, layout);
	}
}


static void
freetuxtv_cellrenderer_channelslist_finalize (GObject *object)
{

	FreetuxTVCellRendererChannelsList *self = (FreetuxTVCellRendererChannelsList*)object;
	
	GObjectClass *parent_class;  
	parent_class = G_OBJECT_CLASS (freetuxtv_cellrenderer_channelslist_parent_class);
	(* G_OBJECT_CLASS (parent_class)->finalize) (object);
}

static void
freetuxtv_cellrenderer_channelslist_class_init (FreetuxTVCellRendererChannelsListClass *klass)
{
      
	GtkCellRendererClass *cell_class   = GTK_CELL_RENDERER_CLASS(klass);
	GObjectClass         *object_class = G_OBJECT_CLASS(klass);
	
	GObjectClass *parent_class         = g_type_class_peek_parent (klass);
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
	GTK_CELL_RENDERER(self)->mode = GTK_CELL_RENDERER_MODE_INERT;
	GTK_CELL_RENDERER(self)->xpad = 2;
	GTK_CELL_RENDERER(self)->ypad = 2;
}
