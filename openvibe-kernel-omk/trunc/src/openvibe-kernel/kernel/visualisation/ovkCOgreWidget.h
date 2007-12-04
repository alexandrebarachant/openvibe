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
#ifndef __GTK_OGRE_H__
#define __GTK_OGRE_H__

#include "OgrePrerequisites.h"

#include "../ovkTKernelObject.h"
#include <gtk/gtkwidget.h>

#define GTK_OGRE(obj)         GTK_CHECK_CAST(obj, gtk_ogre_get_type(), GtkOgre)
#define GTK_OGRE_CLASS(klass) GTK_CHECK_CLASS_CAST(klass, gtk_ogre_get_type(), GtkOgreClass)
#define GTK_IS_OGRE(obj)      GTK_CHECK_TYPE(obj, gtk_ogre_get_type())

typedef struct _GtkOgre       GtkOgre;
typedef struct _GtkOgreClass  GtkOgreClass;

enum eCamType
{
	CAM_PERSPECTIVE,
	CAM_ORTHO_X,
	CAM_ORTHO_Y,
	CAM_ORTHO_Z
};

struct _GtkOgre
{
  GtkWidget widget;

  Ogre::String* mRenderWindowName;
  Ogre::RenderWindow* mRenderWindow;
	Ogre::Camera* mCamera;
	eCamType mCamType;
	Ogre::Viewport* mViewport;
	Ogre::SceneNode* mEntityNode;
	OpenViBE::boolean mLeftButtonPressed;
	OpenViBE::boolean mMiddleButtonPressed;
	OpenViBE::boolean mRightButtonPressed;
	OpenViBE::float32 mX;
	OpenViBE::float32 mY;	
};

struct _GtkOgreClass
{
  GtkWidgetClass parent_class;
};

guint      gtk_ogre_get_type (void);
GtkWidget* gtk_ogre_new      (const Ogre::String& renderWindowName);

// GtkOgre specific functions
Ogre::RenderWindow* gtk_ogre_get_render_window (GtkOgre* ogre);

#endif // __GTK_OGRE_H__
*/