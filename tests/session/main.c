#include "compute.h"

sc_status
TestSessionForHost(void)
{
	scTest(NULL, "Testing session for host...\n");
	scTest(NULL, "%s\n", SC_LOG_SEPARATOR);

	sc_session session = scCreateSessionForHost();
	if(scIsValidSession(session))
	{
		scReleaseSession(session);
		scTest(NULL, "PASSED!\n");
		return SC_SUCCESS;
	}

	scTest(NULL, "FAILED!\n");
	return SC_INVALID_SESSION;
}

sc_status
TestSessionForDeviceType(sc_uint type)
{
	sc_status status;
	scTest(NULL, "Testing session for device type '%s'...\n", scGetDeviceTypeString(type, &status));
	scTest(NULL, "%s\n", SC_LOG_SEPARATOR);

	sc_session session = scCreateSessionForDeviceType(type, 0, &status);
	if(scIsValidSession(session))
	{
		scReleaseSession(session);
		scTest(NULL, "PASSED!\n");
		return SC_SUCCESS;
	}
	
	scTest(NULL, "FAILED!\n");
	return SC_INVALID_SESSION;
}

int
main(int argc, char *argv[])
{
	char *progname;
	progname = argv[0];
		  
	scTest(NULL, "%s\n", SC_LOG_SEPARATOR);
	
	scAssert(TestSessionForHost() == SC_SUCCESS);
	scTest(NULL, "%s\n", SC_LOG_SEPARATOR);
	
	scAssert(TestSessionForDeviceType(SC_DEVICE_TYPE_ALL) == SC_SUCCESS);
	scTest(NULL, "%s\n", SC_LOG_SEPARATOR);
	
	scAssert(TestSessionForDeviceType(SC_DEVICE_TYPE_GPU) == SC_SUCCESS);
	scTest(NULL, "%s\n", SC_LOG_SEPARATOR);
	
	scAssert(TestSessionForDeviceType(SC_DEVICE_TYPE_CPU) == SC_SUCCESS);
	scTest(NULL, "%s\n", SC_LOG_SEPARATOR);
  	
	return 0;
}