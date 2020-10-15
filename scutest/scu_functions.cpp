#include "scutest.h"
#include "../mc3inc/mc3msg.h"
using namespace std;

int scu::CheckHostDetails(STORAGE_OPTIONS* A_options)
{
	if (A_options->RemoteHostname[0] && (A_options->RemotePort != -1))
	{
		return 1;
	}
	return 0;
}
void scu::CopyPatientFirstName(int *i, string A_argv[],Patient* p)
{

	if (!strcmp(A_argv[*i].c_str(), "-fname"))
	{
		(*i)++;
		p->FirstName = A_argv[*i];
		
	}
	
}
void scu::CopyPatientLastName(int* i, string A_argv[],Patient* p)
{
	if (!strcmp(A_argv[*i].c_str(), "-lname"))
	{
		(*i)++;
		p->LastName = A_argv[*i];
	}
}
void scu::CopyPatientId(int* i, string A_argv[],Patient* p)
{
	if (!strcmp(A_argv[*i].c_str(), "-id"))
	{
		(*i)++;
		p->PatientId = A_argv[*i];
	}
}
void scu::CopyPatientSOPInstanceUID(int* i, string A_argv[],Patient* p)
{
	if (!strcmp(A_argv[*i].c_str(), "-SID"))
	{
		(*i)++;
		p->SOPInstanceUID = A_argv[*i];

	}
}
void scu::CopyHostName(int *i, string A_argv[], STORAGE_OPTIONS* A_options)
{
	if (!strcmp(A_argv[*i].c_str(), "-n") || !strcmp(A_argv[*i].c_str(), "-N"))
	{
		/*
		 * Remote Host Name
		 */
		(*i)++;
		A_options->RemoteHostname = A_argv[*i];
	}
}
void scu::CopyPortName(int* i, string A_argv[], STORAGE_OPTIONS* A_options)
{
	if (!strcmp(A_argv[*i].c_str(), "-p") || !strcmp(A_argv[*i].c_str(), "-P"))
	{
		/*
		 * Remote Port Number
		 */
		(*i)++;
		A_options->RemotePort = atoi(A_argv[*i].c_str());

	}
}
