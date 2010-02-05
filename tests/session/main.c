#include "compute.h"

ce_status
TestSessionForHost(void)
{
	ceTest(NULL, "Testing session for host...\n");
	ceTest(NULL, "%s\n", CE_LOG_SEPARATOR);

	ce_session session = ceCreateSessionForHost();
	if(ceIsValidSession(session))
	{
		ceReleaseSession(session);
		ceTest(NULL, "PASSED!\n");
		return CE_SUCCESS;
	}

	ceTest(NULL, "FAILED!\n");
	return CE_INVALID_SESSION;
}

ce_status
TestSessionForDeviceType(ce_uint type)
{
	ce_status status;
	ceTest(NULL, "Testing session for device type '%s'...\n", ceGetDeviceTypeString(type, &status));
	ceTest(NULL, "%s\n", CE_LOG_SEPARATOR);

	ce_session session = ceCreateSessionForDeviceType(type, 0);
	if(ceIsValidSession(session))
	{
		ceReleaseSession(session);
		ceTest(NULL, "PASSED!\n");
		return CE_SUCCESS;
	}
	
	ceTest(NULL, "FAILED!\n");
	return CE_INVALID_SESSION;
}

int
main(int argc, char *argv[])
{
	char *progname;
	progname = argv[0];
		  
	ceTest(NULL, "%s\n", CE_LOG_SEPARATOR);
	
	ceAssert(TestSessionForHost() == CE_SUCCESS);
	ceTest(NULL, "%s\n", CE_LOG_SEPARATOR);
	
	ceAssert(TestSessionForDeviceType(CL_DEVICE_TYPE_ALL) == CE_SUCCESS);
	ceTest(NULL, "%s\n", CE_LOG_SEPARATOR);
	
	ceAssert(TestSessionForDeviceType(CL_DEVICE_TYPE_GPU) == CE_SUCCESS);
	ceTest(NULL, "%s\n", CE_LOG_SEPARATOR);
	
	ceAssert(TestSessionForDeviceType(CL_DEVICE_TYPE_CPU) == CE_SUCCESS);
	ceTest(NULL, "%s\n", CE_LOG_SEPARATOR);
  	
	return 0;
}