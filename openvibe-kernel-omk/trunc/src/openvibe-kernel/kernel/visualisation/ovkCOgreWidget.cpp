/*
// Copyright (c) 2006 Christian Lindequist Larsen
//
// This software is provided 'as-is', without any express or implied warranty. In no event will
// the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose, including commercial
// applications, and to alter it and redistribute it freely, subject to the following
// restrictions:
//
//     1. The origin of this software must not be misrepresented; you must not claim that you
//        wrote the original software. If you use this software in a product, an acknowledgment
//        in the product documentation would be appreciated but is not required.
//
//     2. Altered source versions must be plainly marked as such, and must not be
//        misrepresented as being the original software.
//
//     3. This notice may not be removed or altered from any source distribution.
//
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreRenderSystem.h"
#include "OgreStringConverter.h"

#include "ovkCOgreWidget.h"
//#include "gtkogre.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#  include <gdk/gdkwin32.h>
#else
#  include <gdk/gdkx.h>
#endif

gboolean button_press_event_in_frame_cb(GtkWidget *widget, GdkEventButton *bevent, gpointer user_data)
{
	GtkOgre* l_pGtkOgre = (GtkOgre*)user_data;
	l_pGtkOgre->mX = bevent->x;
	l_pGtkOgre->mY = bevent->y;			

	if(bevent->button == 1) //left button
		l_pGtkOgre->mLeftButtonPressed = true;
	else if(bevent->button == 2) 
		l_pGtkOgre->mMiddleButtonPressed = true;
	else if(bevent->button == 3) //right button
		l_pGtkOgre->mRightButtonPressed = true;

	return true;
}

gboolean motion_notify_event_in_frame_cb(GtkWidget *widget, GdkEventButton *bevent, gpointer user_data)
{	
	GtkOgre* l_pGtkOgre = (GtkOgre*)user_data;
	
	const Ogre::Vector3& l_rCamPos = l_pGtkOgre->mCamera->getPosition();
	Ogre::Vector3 l_oCamPos = l_rCamPos;

	if(l_pGtkOgre->mCamType == CAM_PERSPECTIVE)
	{
		if(l_pGtkOgre->mLeftButtonPressed == true)
		{
			l_oCamPos.x = l_rCamPos.x - ((float)bevent->x - l_pGtkOgre->mX);
			l_oCamPos.y = l_rCamPos.y + ((float)bevent->y - l_pGtkOgre->mY);
			l_oCamPos.z = l_rCamPos.z;
		}
		else if(l_pGtkOgre->mRightButtonPressed == true)
		{
			l_oCamPos = l_rCamPos;
			Ogre::Vector3 dir = l_pGtkOgre->mCamera->getDirection();
			dir.normalise();
			l_oCamPos += ((float)bevent->y - l_pGtkOgre->mY)*dir;
		}
		else if(l_pGtkOgre->mMiddleButtonPressed == true)
		{	
			//Ogre::Quaternion orientation = l_pGtkOgre->mCamera->getOrientation();			 
			//l_pGtkOgre->mCamera->yaw(Ogre::Radian(0.01 * ((float)bevent->x - l_pGtkOgre->mX)));
			//l_pGtkOgre->mCamera->pitch(Ogre::Radian(0.01 * ((float)bevent->y - l_pGtkOgre->mY)));			
			
			l_pGtkOgre->mEntityNode->yaw(Ogre::Radian(0.01 * ((float)bevent->x - l_pGtkOgre->mX)));
			l_pGtkOgre->mEntityNode->pitch(Ogre::Radian(0.01 * ((float)bevent->y - l_pGtkOgre->mY)));
		}
	}
	else if(l_pGtkOgre->mCamType == CAM_ORTHO_X)
	{
		if(l_pGtkOgre->mLeftButtonPressed == true)
		{
			l_oCamPos.x = l_rCamPos.x;
			l_oCamPos.y = l_rCamPos.y + ((float)bevent->y - l_pGtkOgre->mY);					
			l_oCamPos.z = l_rCamPos.z - ((float)bevent->x - l_pGtkOgre->mX);					
		}
		else if(l_pGtkOgre->mRightButtonPressed == true)
		{
			l_oCamPos = l_rCamPos;
			Ogre::Vector3 dir = l_pGtkOgre->mCamera->getDirection();
			dir.normalise();
			l_oCamPos += ((float)bevent->x - l_pGtkOgre->mX)*dir;
		}
	}
	else if(l_pGtkOgre->mCamType == CAM_ORTHO_Y)
	{
		if(l_pGtkOgre->mLeftButtonPressed == true)
		{
			l_oCamPos.x = l_rCamPos.x - ((float)bevent->x - l_pGtkOgre->mX);
			l_oCamPos.y = l_rCamPos.y;
			l_oCamPos.z = l_rCamPos.z - ((float)bevent->y - l_pGtkOgre->mY);					
		}
		else if(l_pGtkOgre->mRightButtonPressed == true)
		{
			l_oCamPos = l_rCamPos;
			Ogre::Vector3 dir = l_pGtkOgre->mCamera->getDirection();
			dir.normalise();
			l_oCamPos += ((float)bevent->x - l_pGtkOgre->mX)*dir;
		}
	}
	else if(l_pGtkOgre->mCamType == CAM_ORTHO_Z)
	{
		if(l_pGtkOgre->mLeftButtonPressed == true)
		{
			l_oCamPos.x = l_rCamPos.x - ((float)bevent->x - l_pGtkOgre->mX);
			l_oCamPos.y = l_rCamPos.y + ((float)bevent->y - l_pGtkOgre->mY);
			l_oCamPos.z = l_rCamPos.z;
		}
		else if(l_pGtkOgre->mRightButtonPressed == true)
		{
			l_oCamPos = l_rCamPos;
			Ogre::Vector3 dir = l_pGtkOgre->mCamera->getDirection();
			dir.normalise();
			l_oCamPos += ((float)bevent->x - l_pGtkOgre->mX)*dir;
		}
	}
	
	l_pGtkOgre->mCamera->setPosition(l_oCamPos);				
	l_pGtkOgre->mRenderWindow->update(true);
	l_pGtkOgre->mX = (float)bevent->x;
	l_pGtkOgre->mY = (float)bevent->y;
				
	return true;
}

gboolean button_release_event_in_frame_cb(GtkWidget *widget, GdkEventButton *bevent, gpointer user_data)
{
	GtkOgre* l_pGtkOgre = (GtkOgre*)user_data;

	if(bevent->button == 1) //left button
		l_pGtkOgre->mLeftButtonPressed = false;
	else if(bevent->button == 2) 
		l_pGtkOgre->mMiddleButtonPressed = false;
	else if(bevent->button == 3) //right button
		l_pGtkOgre->mRightButtonPressed = false;
	
	return true;
}

int s_wid = 0;

static void gtk_ogre_class_init    (GtkOgreClass   *klass);
static void gtk_ogre_init          (GtkOgre        *ogre);

// GtkObject functions 
static void gtk_ogre_destroy       (GtkObject      *object);

// GtkWidget functions 
static void gtk_ogre_realize       (GtkWidget      *widget);
static void gtk_ogre_unrealize     (GtkWidget      *widget);

static void gtk_ogre_size_request(GtkWidget *widget, GtkRequisition* requisition);
static void gtk_ogre_size_allocate (GtkWidget      *widget, GtkAllocation  *allocation);
static gint gtk_ogre_expose        (GtkWidget      *widget, GdkEventExpose *event);

static GtkWidgetClass *parent_class = NULL;

guint gtk_ogre_get_type(void)
{
  static guint ogre_type = 0;

  if (!ogre_type)
    {
      static const GtkTypeInfo ogre_info =
      {
        "GtkOgre",
        sizeof(GtkOgre),
        sizeof(GtkOgreClass),
        (GtkClassInitFunc)gtk_ogre_class_init,
        (GtkObjectInitFunc)gtk_ogre_init,
        NULL,
        NULL,
        (GtkClassInitFunc)NULL,
      };

      ogre_type = gtk_type_unique(gtk_widget_get_type(), &ogre_info);
    }

  return ogre_type;
}

static void gtk_ogre_class_init(GtkOgreClass *klass)
{
  GtkObjectClass *object_class;
  GtkWidgetClass *widget_class;

  object_class = (GtkObjectClass*)klass;
  widget_class = (GtkWidgetClass*)klass;

  parent_class = (GtkWidgetClass*)gtk_type_class(gtk_widget_get_type());

  object_class->destroy = gtk_ogre_destroy;

  widget_class->realize = gtk_ogre_realize;
  widget_class->unrealize = gtk_ogre_unrealize;

	widget_class->size_request = gtk_ogre_size_request;
  widget_class->size_allocate = gtk_ogre_size_allocate;
	
  widget_class->expose_event = gtk_ogre_expose;
}

static void gtk_ogre_init(GtkOgre *ogre)
{
  / Initialize instance members 
  ogre->mRenderWindowName = NULL;
  ogre->mRenderWindow = NULL;
}

GtkWidget* gtk_ogre_new(const Ogre::String& renderWindowName)
{
  GtkOgre *ogre;

  ogre = (GtkOgre*)gtk_type_new(gtk_ogre_get_type());
	
	//GTK_WIDGET_SET_FLAGS(GTK_WIDGET(ogre), GTK_NO_WINDOW); //unset in realize

  ogre->mRenderWindowName = new Ogre::String(renderWindowName);

  return GTK_WIDGET(ogre);
}

// GtkObject functions 
static void gtk_ogre_destroy(GtkObject *object)
{
  GtkOgre* ogre;

  g_return_if_fail(object != NULL);
  g_return_if_fail(GTK_IS_OGRE(object));

  ogre = GTK_OGRE(object);

  // Destroy instance members 
  delete ogre->mRenderWindowName;
  ogre->mRenderWindowName = NULL;
  ogre->mRenderWindow = NULL;

  // Chain up 
  if (GTK_OBJECT_CLASS(parent_class)->destroy)
    (*GTK_OBJECT_CLASS(parent_class)->destroy)(object);
}

// GtkWidget functions 
static void gtk_ogre_realize(GtkWidget *widget)
{
	GtkOgre* ogre;

	g_return_if_fail(widget != NULL);
	g_return_if_fail(GTK_IS_OGRE(widget));

	ogre = GTK_OGRE(widget);

	g_return_if_fail(ogre->mRenderWindowName != NULL);
	g_return_if_fail(ogre->mRenderWindow == NULL);

	GdkWindow* parent = gtk_widget_get_parent_window(widget);

	//Create the GdkWindow:
	GdkWindowAttr attributes;
	memset(&attributes, 0, sizeof(attributes));

	//Set initial position and size of the Gdk::Window:
	attributes.x = widget->allocation.x;
	attributes.y = widget->allocation.y;
	attributes.width = widget->allocation.width;
	attributes.height = widget->allocation.height;

	//Set event mask as needed here
	attributes.event_mask = GDK_ALL_EVENTS_MASK; //0xFFFFFFFF; //get_events () | Gdk::EXPOSURE_MASK | Gdk::ALL_EVENTS_MASK;
	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.wclass = GDK_INPUT_OUTPUT;

	widget->window = GDK_DRAWABLE(gdk_window_new(parent, &attributes, GDK_WA_X | GDK_WA_Y));
	Ogre::NameValuePairList params; 
	params["externalWindowHandle"] = Ogre::StringConverter::toString((unsigned long)GDK_WINDOW_HWND(widget->window));

	ogre->mRenderWindow = Ogre::Root::getSingleton().createRenderWindow("GtkOgreWidget" + Ogre::StringConverter::toString(++s_wid),
				widget->allocation.width, widget->allocation.height, false, &params);

	GTK_WIDGET_UNSET_FLAGS(widget, GTK_NO_WINDOW);

	//Set event mask as needed here
	//gdk_window_set_events(widget->window, GDK_ALL_EVENTS_MASK);
	g_signal_connect(G_OBJECT(widget), "button-press-event", G_CALLBACK(button_press_event_in_frame_cb), ogre);			
	g_signal_connect(G_OBJECT(widget), "button-release-event", G_CALLBACK(button_release_event_in_frame_cb), ogre);			
	g_signal_connect(G_OBJECT(widget), "motion-notify-event", G_CALLBACK(motion_notify_event_in_frame_cb), ogre);			

	gtk_widget_set_double_buffered(widget, FALSE);
	gdk_window_set_user_data(widget->window, G_OBJECT(widget)); 
	gdk_window_set_back_pixmap(widget->window, NULL, FALSE);

	//Set realized flag 
	GTK_WIDGET_SET_FLAGS(widget, GTK_REALIZED);
}

static void gtk_ogre_unrealize(GtkWidget *widget)
{
  GtkOgre* ogre;

  g_return_if_fail(widget != NULL);
  g_return_if_fail(GTK_IS_OGRE(widget));

  ogre = GTK_OGRE(widget);

  //Hide all windows 
  //TODO: Is this necessary?
  if (GTK_WIDGET_MAPPED(widget))
    gtk_widget_unmap(widget);

  GTK_WIDGET_UNSET_FLAGS(widget, GTK_MAPPED);

  //Destroy the render window, but only if it's not the primary. Ogre will destroy the primary window on shutdown.
  if (!ogre->mRenderWindow->isPrimary())
  {
		Ogre::Root::getSingleton().getRenderSystem()->destroyRenderWindow(ogre->mRenderWindow->getName());
  }
  ogre->mRenderWindow = NULL;

  //This destroys widget->window and unsets the realized flag
  if (GTK_WIDGET_CLASS(parent_class)->unrealize)
    (*GTK_WIDGET_CLASS(parent_class)->unrealize)(widget);
}

static void gtk_ogre_size_request(GtkWidget *widget, GtkRequisition* requisition)
{
  g_return_if_fail(widget != NULL);

	widget->requisition = *requisition;
}

static void gtk_ogre_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
  GtkOgre* ogre;

  g_return_if_fail(widget != NULL);
  g_return_if_fail(GTK_IS_OGRE(widget));

  ogre = GTK_OGRE(widget);
	
  // Chain up - this will adjust the size of the actual window
  if (GTK_WIDGET_CLASS(parent_class)->size_allocate)
    (*GTK_WIDGET_CLASS(parent_class)->size_allocate)(widget, allocation);
  
	widget->allocation = *allocation;

	if(widget->window != NULL)
		gdk_window_move_resize(widget->window, allocation->x, allocation->y, allocation->width, allocation->height); 

  if (ogre->mRenderWindow)
  {		
#ifdef _WIN32
		ogre->mRenderWindow->resize(widget->allocation.width, widget->allocation.height);
		ogre->mRenderWindow->windowMovedOrResized();
#else
		ogre->mRenderWindow->resize(widget->allocation.width, widget->allocation.height);
#endif      
    gtk_ogre_expose(widget, NULL);
  }
}

static gint gtk_ogre_expose(GtkWidget *widget, GdkEventExpose *event)
{
  // Update the render window and swap buffers
  GTK_OGRE(widget)->mRenderWindow->update(true);

  return TRUE;
}

// GtkOgre specific functions
Ogre::RenderWindow* gtk_ogre_get_render_window(GtkOgre* ogre)
{
  g_return_val_if_fail(ogre != NULL, NULL);

  return ogre->mRenderWindow;
}
*/
