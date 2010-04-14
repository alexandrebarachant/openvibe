#include "ovavrdCOgreVRApplication.h"

#include "HandballBCI/ovavrdCHandballBCI.h"
#include "TieFighterBCI/ovavrdCTieFighterBCI.h"

#if 0
#if defined OVA_OS_Linux
namespace CEGUI
{
	Exception::Exception(const String& s)
	{
		Exception(s, "", "", 0);
	}
}
#endif
#endif

int main(int argc, char **argv)
{
	if(argc!=2)
	{
		printf("Syntax: %s demo-name\n", argv[0]);
		printf("\n");
		printf("where demo-name could be one of the following :\n");
		printf("  - tie-fighter\n");
		printf("  - handball\n");
		return 1;
	}

	OpenViBEVRDemos::COgreVRApplication * app;
	if(strcmp(argv[1],"tie-fighter") == 0)
	{
		printf("\n");
		printf("  _                       _  \n");
		printf(" | |                     | | \n");
		printf(" | |                     | | \n");
		printf(" | |                     | | \n");
		printf(" | |                     | | \n");
		printf(" | |        .---.        | | \n");
		printf(" | |     _.'\\   /'._     | | \n");
		printf("/  b__--- | .'\"'. | ---__d  \\\n");
		printf("\\  p\"\"---_| '._.' |_---\"\"q  /\n");
		printf(" | |       ./   \\.       | | \n");
		printf(" | |        '---'        | | \n");
		printf(" | |                     | | \n");
		printf(" | |                     | | \n");
		printf(" |_|                     |_| \n\n");
		printf("Tie-fighter application started ! \nMay the Force be with you, young padawan.\n");
		app = new OpenViBEVRDemos::CTieFighterBCI();
	}
	else if(strcmp(argv[1],"handball") == 0)
	{
		printf("Handball application started !\n");
		app = new OpenViBEVRDemos::CHandballBCI();
	}
	else
	{
		printf("ERROR: the application specified does not exist (%s).\n",argv[1]);
		return 2;
	}

	app->go();
	delete app;

	return 0;
}
