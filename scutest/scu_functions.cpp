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
SAMP_BOOLEAN scu::traverseimages(STORAGE_OPTIONS* A_options, InstanceNode** instanceList, char fname[])
{
	SAMP_BOOLEAN sampBool = SAMP_FALSE;

	int imageCurrent = 0;
	for (imageCurrent = A_options->StartImage; imageCurrent <= A_options->StopImage; imageCurrent++)
	{
		sprintf(fname, "%d.img", imageCurrent);
		sampBool = scu::AddFileToList(&(*instanceList), fname);
		
	}
	return sampBool;
}
 SAMP_BOOLEAN scu::AddFileToList(InstanceNode** A_list, char* A_fname)
{
	InstanceNode*    newNode;
	

	newNode = (InstanceNode*)malloc(sizeof(InstanceNode));
	if (!newNode)
	{
		//PrintError("Unable to allocate object to store instance information", MC_NORMAL_COMPLETION);
		return (SAMP_FALSE);
	}

	memset(newNode, 0, sizeof(InstanceNode));

	strncpy(newNode->fname, A_fname, sizeof(newNode->fname));
	newNode->fname[sizeof(newNode->fname) - 1] = '\0';

	newNode->responseReceived = SAMP_FALSE;
	newNode->failedResponse = SAMP_FALSE;
	newNode->imageSent = SAMP_FALSE;
	newNode->msgID = -1;
	newNode->transferSyntax = IMPLICIT_LITTLE_ENDIAN;

	scu::update_list(A_list, newNode);

	return (SAMP_TRUE);
}
void scu::update_list(InstanceNode** A_list, InstanceNode* newNode)
{
	InstanceNode* listNode;
	if (!*A_list)
	{
		/*

		  * Nothing in the list

		  */
		newNode->Next = *A_list;

		*A_list = newNode;

	}
	else
	{
		/*

		 * Add to the tail of the list

		 */
		listNode = *A_list;
		while (listNode->Next)
			listNode = listNode->Next;
		listNode->Next = newNode;

	}

}
int scu::GetNumNodes(InstanceNode*       A_list)
{
	int            numNodes = 0;
	InstanceNode*  node;

	node = A_list;
	while (node)
	{
		numNodes++;
		node = node->Next;
	}

	return numNodes;
}
