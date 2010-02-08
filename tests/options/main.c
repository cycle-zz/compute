#include "compute.h"

typedef struct _test_values_t {
	const char* flag;
	sc_value value;
} test_value_t;

int
main(int argc, char *argv[])
{
	char *progname;
	progname = argv[0];
	
	sc_uint i = 0;
	sc_uint count = 0;
  	sc_option options[16];

	sc_uint test_argc = 0;
	const char* test_argv[16];
	
	sc_session session = scCreateSessionForHost();
	
	test_value_t values[] = {
		{ "--symbol=symbol",  	scCreateSymbolValue(session, scCreateSymbol(session, "symbol", strlen("symbol")) ),  },
		{ "--bool=true", 		scCreateBoolValue(session, SC_TRUE)   	},
		{ "--char=42", 	 		scCreateCharValue(session, 		42)  	},
		{ "--uchar=42",  		scCreateUCharValue(session, 	42)  	},
		{ "--short=42", 		scCreateShortValue(session, 	42)  	},
		{ "--ushort=42",  		scCreateUShortValue(session, 	42)  	},
		{ "--int=42", 	 		scCreateIntValue(session, 		42)  	},
		{ "--uint=42",  		scCreateUIntValue(session, 		42)  	},
		{ "--long=42", 	 		scCreateLongValue(session, 		42)  	},
		{ "--ulong=42",  		scCreateULongValue(session, 	42)  	},
		{ "--float=42.0", 		scCreateFloatValue(session, 	42.0f)  },
		{ "--double=42.0",  	scCreateDoubleValue(session, 	42.0) 	},
	};
	test_argc = sizeof(values) / sizeof(test_value_t);
	
	for(i = 0; i < test_argc; i++)
		test_argv[i] = values[i].flag;

	i = 0;
  	options[i++] = scCreateOption(session, "symbol",	SC_TYPE_SYMBOL, "Test symbol option");
  	options[i++] = scCreateOption(session, "bool",		SC_TYPE_BOOL, "Test boolean option");
  	options[i++] = scCreateOption(session, "char",		SC_TYPE_CHAR, "Test char option");
  	options[i++] = scCreateOption(session, "uchar",		SC_TYPE_UCHAR, "Test unsigned char option");
  	options[i++] = scCreateOption(session, "short",		SC_TYPE_SHORT, "Test short option");
  	options[i++] = scCreateOption(session, "ushort",	SC_TYPE_USHORT, "Test unsigned short option");
  	options[i++] = scCreateOption(session, "int",		SC_TYPE_INT, "Test int option");
  	options[i++] = scCreateOption(session, "uint",		SC_TYPE_UINT, "Test unsigned int option");
  	options[i++] = scCreateOption(session, "long",		SC_TYPE_LONG, "Test long option");
  	options[i++] = scCreateOption(session, "ulong",		SC_TYPE_ULONG, "Test unsigned long option");
  	options[i++] = scCreateOption(session, "float",		SC_TYPE_FLOAT, "Test float option");
  	options[i++] = scCreateOption(session, "double",	SC_TYPE_DOUBLE, "Test double option");
  	count = i;

  	scParseCommandLineOptions(session, count, test_argv, count, options);
  	
  	sc_status status = 0;
  	for(i = 0; i < count; i++)
  	{
  		sc_value parsed = scGetOptionValue(options[i], NULL);
  		sc_value expected = values[i].value;
	
		scInfo(session, "Option[%3d]: name='%s' parsed='%s'(%s) expected='%s'(%s)\n", i, 
			scGetOptionName(options[i], &status), 
			scGetSymbolName(scCreateSymbolFromValue(session, parsed,    &status)), 
			scGetTypeString(scGetValueType(parsed, &status), &status), 
			scGetSymbolName(scCreateSymbolFromValue(session, expected,  &status)),
			scGetTypeString(scGetValueType(expected, &status), &status));
  		
  		scAssert(scIsValueEqual(parsed, expected, &status));
  	}
  	
  	for(i = 0; i < count; i++)
  		scReleaseOption(options[i]);
  	
	return 0;
}