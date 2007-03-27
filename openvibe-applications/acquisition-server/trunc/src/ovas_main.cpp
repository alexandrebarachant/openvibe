
#include <iostream>
#include "ovasCAcquisitionServer.h"

#include <glade/glade.h>
#include <gtk/gtk.h>

int main(int argc, char** argv)
{
	OpenViBEAcquisitionServer::CAcquisitionServer l_oAcquisitionServer;

	gtk_init(&argc, &argv);
	l_oAcquisitionServer.initialize();
	gtk_main();

	return 0;
}
