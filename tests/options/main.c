#include "compute.h"

typedef struct _test_values_t {
	const char* flag;
	ce_value value;
} test_value_t;

int
main(int argc, char *argv[])
{
	char *progname;
	progname = argv[0];
	
	ce_uint i = 0;
	ce_uint count = 0;
  	ce_option options[16];

	ce_uint test_argc = 0;
	const char* test_argv[16];
	
	ce_session session = ceCreateSessionForHost();
	
	test_value_t values[] = {
		{ "--symbol=symbol",  	ceCreateSymbolValue(session, ceCreateSymbol(session, "symbol", strlen("symbol")) ),  },
		{ "--bool=true", 		ceCreateBoolValue(session, CE_TRUE)   	},
		{ "--char=42", 	 		ceCreateCharValue(session, 		42)  	},
		{ "--uchar=42",  		ceCreateUCharValue(session, 	42)  	},
		{ "--short=42", 		ceCreateShortValue(session, 	42)  	},
		{ "--ushort=42",  		ceCreateUShortValue(session, 	42)  	},
		{ "--int=42", 	 		ceCreateIntValue(session, 		42)  	},
		{ "--uint=42",  		ceCreateUIntValue(session, 		42)  	},
		{ "--long=42", 	 		ceCreateLongValue(session, 		42)  	},
		{ "--ulong=42",  		ceCreateULongValue(session, 	42)  	},
		{ "--float=42.0", 		ceCreateFloatValue(session, 	42.0f)  },
		{ "--double=42.0",  	ceCreateDoubleValue(session, 	42.0) 	},
	};
	test_argc = sizeof(values) / sizeof(test_value_t) - 1;
	
	for(i = 0; i < test_argc; i++)
		test_argv[i] = values[i].flag;

	i = 0;
  	options[i++] = ceCreateOption(session, "symbol",	CE_TYPE_SYMBOL, "Test symbol option");
  	options[i++] = ceCreateOption(session, "bool",	CE_TYPE_BOOL, "Test boolean option");
  	options[i++] = ceCreateOption(session, "char",	CE_TYPE_CHAR, "Test char option");
  	options[i++] = ceCreateOption(session, "uchar",	CE_TYPE_UCHAR, "Test unsigned char option");
  	options[i++] = ceCreateOption(session, "short",	CE_TYPE_SHORT, "Test short option");
  	options[i++] = ceCreateOption(session, "ushort",	CE_TYPE_USHORT, "Test unsigned short option");
  	options[i++] = ceCreateOption(session, "int",	CE_TYPE_INT, "Test int option");
  	options[i++] = ceCreateOption(session, "uint",	CE_TYPE_UINT, "Test unsigned int option");
  	options[i++] = ceCreateOption(session, "long",	CE_TYPE_LONG, "Test long option");
  	options[i++] = ceCreateOption(session, "ulong",	CE_TYPE_ULONG, "Test unsigned long option");
  	options[i++] = ceCreateOption(session, "float",	CE_TYPE_FLOAT, "Test float option");
  	options[i++] = ceCreateOption(session, "double",	CE_TYPE_DOUBLE, "Test double option");
  	count = i;

  	ceParseCommandLineOptions(CE_DEFAULT_SESSION, test_argc, test_argv, count, options);
  	
  	ce_status status = 0;
  	for(i = 0; i < count; i++)
  	{
  		ce_value parsed = ceGetOptionValue(options[i], NULL);
		ce_type type = ceGetValueType(parsed, &status);
  		if(type == CE_TYPE_SYMBOL)
  			ceDebug(CE_DEFAULT_SESSION, "Option[%3d]: name='%s' type='%s' parsed='%s' expected='%s'\n", i, 
				ceGetOptionName(options[i], &status), ceGetTypeString(type, &status), ceGetSymbolName(parsed.data.as_symbol), ceGetSymbolName(expected.data.as_symbol));
 
  		else
			ceDebug(CE_DEFAULT_SESSION, "Option[%3d]: name='%s' type='%d' parsed='%d/%f' expected='%d/%f'\n", i, 
				ceGetOptionName(options[i]), ceGetTypeName(type, &status), parsed.data.as_long, parsed.data.as_float, values[i].value.data.as_long, values[i].value.data.as_float);

  		ceAssert(ceIsValueEqual(parsed, expected));
  	}
  	
  	for(i = 0; i < count; i++)
  		ceReleaseOption(options[i]);
  	
	return 0;
}