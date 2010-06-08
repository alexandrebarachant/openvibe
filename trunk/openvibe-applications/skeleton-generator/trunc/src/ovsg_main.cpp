#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <iostream>

#include "ovsgCDriverSkeletonGenerator.h"
#include "ovsgCBoxAlgorithmSkeletonGenerator.h"
#include "ovsgCAlgorithmSkeletonGenerator.h"

#if defined TARGET_OS_Windows
 #define OVSG_OS_Windows
#elif defined TARGET_OS_Linux
 #define OVSG_OS_Linux
#elif defined TARGET_OS_MacOS
 #define OVSG_OS_MacOS
#else
 #warning "No target operating system defined !"
#endif

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBESkeletonGenerator;

using namespace std;


int main(int argc, char** argv)
{

	CKernelLoader l_oKernelLoader;

	cout<<"[  INF  ] Created kernel loader, trying to load kernel module"<<endl;
	CString m_sError;
#if defined OVSG_OS_Windows
	if(!l_oKernelLoader.load("../bin/OpenViBE-kernel-dynamic.dll", &m_sError))
#else
	if(!l_oKernelLoader.load("../lib/libOpenViBE-kernel-dynamic.so", &m_sError))
#endif
	{
			cout<<"[ FAILED ] Error loading kernel ("<<m_sError<<")"<<endl;
	}
	else
	{
		cout<<"[  INF  ] Kernel module loaded, trying to get kernel descriptor"<<endl;
		IKernelDesc* l_pKernelDesc=NULL;
		IKernelContext* l_pKernelContext=NULL;
		l_oKernelLoader.initialize();
		l_oKernelLoader.getKernelDesc(l_pKernelDesc);
		if(!l_pKernelDesc)
		{
			cout<<"[ FAILED ] No kernel descriptor"<<endl;
		}
		else
		{
			cout<<"[  INF  ] Got kernel descriptor, trying to create kernel"<<endl;
			l_pKernelContext=l_pKernelDesc->createKernel("skeleton-generator", "../share/openvibe.conf");
			if(!l_pKernelContext)
			{
				cout<<"[ FAILED ] No kernel created by kernel descriptor"<<endl;
			}
			else
			{
				
				//initialise Gtk before 3D context
				gtk_init(&argc, &argv);
	
				//CSkeletonGenerator l_Generator(*l_pKernelContext,"../share/openvibe-applications/skeleton-generator/generator-interface.glade");
				
				::GladeXML * l_pGladeInterface = glade_xml_new("../share/openvibe-applications/skeleton-generator/generator-interface.glade", NULL, NULL);

				::GtkWidget * l_pDialog = glade_xml_get_widget(l_pGladeInterface, "sg-selection-dialog");
	
				gtk_dialog_add_button (GTK_DIALOG (l_pDialog),
					GTK_STOCK_OK,
					GTK_RESPONSE_OK);

				gtk_dialog_add_button (GTK_DIALOG (l_pDialog),
					GTK_STOCK_CANCEL,
					GTK_RESPONSE_CANCEL);

				::GtkWidget * l_pRadioDriver = glade_xml_get_widget(l_pGladeInterface, "sg-driver-selection-radio-button");
				::GtkWidget * l_pRadioAlgo = glade_xml_get_widget(l_pGladeInterface, "sg-algo-selection-radio-button");
				::GtkWidget * l_pRadioBox = glade_xml_get_widget(l_pGladeInterface, "sg-box-selection-radio-button");

				CDriverSkeletonGenerator l_DriverGenerator(*l_pKernelContext,l_pGladeInterface);

				gint resp = gtk_dialog_run(GTK_DIALOG(l_pDialog)); 
	
				if(resp== GTK_RESPONSE_OK)
				{
					gtk_widget_hide(GTK_WIDGET(l_pDialog));

					if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(l_pRadioDriver)))
					{
						l_DriverGenerator.initialize();
					}
					if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(l_pRadioAlgo)))
					{
						std::cout<< "NOT YET AVAILABLE." <<std::endl;
						return 0;
					}
					if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(l_pRadioBox)))
					{
						std::cout<< "NOT YET AVAILABLE." <<std::endl;
						return 0;					
					}
					gtk_main();
				}
				else
				{
					std::cout<< "User cancelled. Exit." <<std::endl;
					return 0 ;
				}
			}
		}
	}

	return 0;
}