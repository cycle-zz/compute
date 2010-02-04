#include "compute.h"


int
main(int argc, char *argv[])
{
	char *progname;
	progname = argv[0];
		  
  	ce_option options[] = {
  		{ "bool", 		CE_TYPE_BOOL, 	{ 0 } },
  		{ "char", 		CE_TYPE_CHAR, 	{ 0 } },
  		{ "uchar", 		CE_TYPE_UCHAR, 	{ 0 } },
  		{ "short", 		CE_TYPE_SHORT,  { 0 } },
  		{ "ushort", 	CE_TYPE_USHORT, { 0 } },
  		{ "int", 		CE_TYPE_INT, 	{ 0 } },
  		{ "uint", 		CE_TYPE_UINT, 	{ 0 } },
  		{ "long", 		CE_TYPE_LONG, 	{ 0 } },
  		{ "ulong", 		CE_TYPE_ULONG, 	{ 0 } },
  		{ "float", 		CE_TYPE_FLOAT, 	{ 0 } },
  		{ "double", 	CE_TYPE_DOUBLE, { 0 } },
  		{ "symbol", 	CE_TYPE_SYMBOL, { 0 } },
  		{ 0, 0, { 0 }  },
  	};
  	ce_uint expected = sizeof(options) / sizeof(ce_option) - 1;
  	
  	ceParseCommandLineOptions(CE_DEFAULT_SESSION, expected, options, argc, argv);
  	
	return 0;
}