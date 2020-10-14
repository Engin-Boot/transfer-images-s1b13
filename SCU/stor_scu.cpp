/*************************************************************************
 *
 *       System: Merge DICOM Toolkit
 *
 *       Author: Merge Healthcare
 *
 *  Description: This is a sample Service Class User application
 *               for the Storage Service Class and the Storage Commitment
 *               service class.  The application has a number of features:
 *               - It can read in images in both the DICOM Part 10 format
 *                 and the DICOM "stream" format.
 *               - The application determines the format of the object
 *                 before reading in.
 *               - The application supports DICOM "Asychronous Window
 *                 negotiation" and will transfer asychronously if
 *                 negotiated.
 *               - The AE Title, host name, and port number of the
 *                 system being connected to can be specified on the
 *                 command line.
 *               - A verbose mode can be specified where detailed
 *                 information is displayed as the application functions.
 *               - The local AE title can be specified on the command
 *                 line.
 *               - The service list (found in the mergecom.app
 *                 configuration file) used by the application to
 *                 determine what services are negotiated can be specified
 *                 on the command line.
 *               - The application will support DICOM Part 10 formated
 *                 compressed/encapsulated if specified on the command
 *                 line.  One note, however, the standard service lists
 *                 found in the mergecom.app file must be extended with
 *                 a transfer syntax list to support these transfer
 *                 syntaxes.
 *               - If specified on the command line, the application will
 *                 send a storage commitment request to the same SCP as
 *                 it is sending images.  The storage commitment request
 *                 will be for the images included on the command line.
 *
 *************************************************************************
 *
 *                      (c) 2012 Merge Healthcare
 *            900 Walnut Ridge Drive, Hartland, WI 53029
 *
 *                      -- ALL RIGHTS RESERVED --
 *
 *  This software is furnished under license and may be used and copied
 *  only in accordance with the terms of such license and with the
 *  inclusion of the above copyright notice.  This software or any other
 *  copies thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of the software is hereby
 *  transferred.
 *
 ************************************************************************/

 /*
  * Standard OS Includes
  */
#include "stor_scu.h"
  /****************************************************************************
   *
   *  Function    :   Main
   *
   *  Description :   Main routine for DICOM Storage Service Class SCU
   *
   ****************************************************************************/
int  main(int argc, char** argv);
int main(int argc, char** argv)
{
	SAMP_BOOLEAN            sampBool;
	STORAGE_OPTIONS         options;
	MC_STATUS               mcStatus;
	int                     applicationID = -1, associationID = -1;
	int                     totalImages = 0L;
	double                  seconds = 0.0;
	void* startTime = NULL, * imageStartTime = NULL;
	char                    fname[512] = { 0 };  /* Extra long, just in case */
	size_t                  totalBytesRead = 0L;
	InstanceNode* instanceList = NULL, * node = NULL;
	FILE* fp = NULL;

	/*
	 * Test the command line parameters, and populate the options
	 * structure with these parameters
	 */

	sampBool = TestCmdLine(argc, argv, &options);

	CheckTestCMD(sampBool);

	/* ------------------------------------------------------- */
	/* This call MUST be the first call made to the library!!! */
	/* ------------------------------------------------------- */
	mcStatus = MC_Library_Initialization(NULL, NULL, NULL);
	CheckForLibInit(mcStatus);

	/*
	 *  Register this DICOM application
	 */
	mcStatus = MC_Register_Application(&applicationID, options.LocalAE);
	if (mcStatus != MC_NORMAL_COMPLETION)
	{
		printf("Unable to register \"%s\":\n", options.LocalAE);
		printf("\t%s\n", MC_Error_Message(mcStatus));
		fflush(stdout);
		return(EXIT_FAILURE);
	}

	/*
	 * Create a linked list of all files to be transferred.
	 * Retrieve the list from a specified file on the command line,
	 * or generate the list from the start & stop numbers on the
	 * command line
	 */

	 /* Traverse through the possible names and add them to the list based on the start/stop count */
	traverseimages(&options, &instanceList, fname);

	totalImages = GetNumNodes(instanceList);

	startTime = GetIntervalStart();


	/*
	 *   Open association and override hostname & port parameters if they were supplied on the command line.
	 */
	mcStatus = MC_Open_Association(applicationID,
		&associationID,
		options.RemoteAE,
		&options.RemotePort,
		options.RemoteHostname,
		options.ServiceList);

	/*mcStatus = MC_Open_Association(applicationID,
		&associationID,
		options.RemoteAE,
		options.RemotePort != -1 ? &options.RemotePort : NULL,
		options.RemoteHostname[0] ? options.RemoteHostname : NULL,
		options.ServiceList[0] ? options.ServiceList : NULL);*/
	checkopenassociation(&options, mcStatus);



	mcStatus = MC_Get_Association_Info(associationID, &options.asscInfo);
	checkforassociation(mcStatus);



	//	/*
	//	 * Go through all of the negotiated services.  If encapsulated /
	//	 * compressed transfer syntaxes are supported, this code should be
	//	 * expanded to save the services & transfer syntaxes that are
	//	 * negotiated so that they can be matched with the transfer
	//	 * syntaxes of the images being sent.
	//	 */

	printf("Connected to remote system [%s]\n\n", options.RemoteAE);

	/*
	 * Check User Identity Negotiation and for response
	 */
	checkresponse(&options);

	fflush(stdout);

	/*
	 *   Send all requested images.  Traverse through instanceList to
	 *   get all files to send
	 */
	node = instanceList;
	traverseListandsend(&options, &node, fname, applicationID, associationID, totalImages, &totalBytesRead);



	/*
	 * A failure on close has no real recovery.  Abort the association
	 * and continue on.
	 */
	mcStatus = MC_Close_Association(&associationID);
	checkcloseassociation(mcStatus, associationID);

	/*
	 * Calculate the transfer rate.  Note, for a real performance
	 * numbers, a function other than time() to measure elapsed
	 * time should be used.
	 */

	seconds = GetIntervalElapsed(startTime);

	printf("Data Transferred: %luMB\n", (unsigned long)(totalBytesRead / (1024 * 1024)));
	printf("    Time Elapsed: %.3fs\n", seconds);
	printf("   Transfer Rate: %.1fKB/s\n", ((float)totalBytesRead / seconds) / 1024.0);
	fflush(stdout);
	/*
	 * Release the dICOM Application
	 */
	mcStatus = MC_Release_Application(&applicationID);
	checkreleaseApp(mcStatus);

	/*
	 * Free the node list's allocated memory
	 */
	FreeList(&instanceList);

	/*
	 * Release all memory used by the Merge DICOM Toolkit.
	 */
	checkmemoryrelease();

	fflush(stdout);

	return(EXIT_SUCCESS);
}
void CheckForLibInit(MC_STATUS mcStatus) {
	if (mcStatus != MC_NORMAL_COMPLETION)
	{
		PrintError("Unable to initialize library", mcStatus);
		exit(0);
	}
}
void CheckTestCMD(SAMP_BOOLEAN sampBool)
{
	if (sampBool == SAMP_FALSE)
	{
		exit(0);
	}

}
void checkopenassociation(STORAGE_OPTIONS* options, MC_STATUS mcStatus)
{
	if (mcStatus != MC_NORMAL_COMPLETION)
	{
		printf("Unable to open association with \"%s\":\n", options->RemoteAE);
		printf("\t%s\n", MC_Error_Message(mcStatus));
		fflush(stdout);
		exit(0);
	}
}

void checkforassociation(MC_STATUS mcStatus) {
	if (mcStatus != MC_NORMAL_COMPLETION)
	{
		PrintError("MC_Get_Association_Info failed", mcStatus);
	}
}


void checkmemoryrelease()
{
	if (MC_Library_Release() != MC_NORMAL_COMPLETION)
		printf("Error releasing the library.\n");
}

void checkcloseassociation(MC_STATUS mcStatus, int associationID)
{
	if (mcStatus != MC_NORMAL_COMPLETION)
	{
		PrintError("Close association failed", mcStatus);
		MC_Abort_Association(&associationID);
	}
}



void checkreleaseApp(MC_STATUS mcStatus)
{
	if (mcStatus != MC_NORMAL_COMPLETION)
	{
		PrintError("MC_Release_Application failed", mcStatus);
	}
}


void traverseListandsend(STORAGE_OPTIONS* options, InstanceNode** node, char* fname, int applicationID, int associationID, int totalImages, size_t* totalBytesRead)
{
	void* imageStartTime = NULL;
	int imagesSent = 0;
	float seconds = 0.0;
	SAMP_BOOLEAN sampBool;
	MC_STATUS        mcStatus;
	while (*node)
	{

		imageStartTime = GetIntervalStart();
		/*
		* Determine the image format and read the image in.  If the
		* image is in the part 10 format, convert it into a message.
		*/
		sampBool = ReadImage(options, applicationID, (*node));
		checkReadImage(sampBool, *node);
		totalBytesRead += (*node)->imageBytes;

		/*
		 * Send image read in with ReadImage.
		 *
		 * Because SendImage may not have actually sent an image even though it has returned success,
		 * the calculation of performance data below may not be correct.
		 */
		sampBool = SendImage(options, associationID, (*node));
		checkSendImage(sampBool, *node, associationID, applicationID);
		/*
		 * Save image transfer information in list
		 */
		sampBool = UpdateNode((*node));
		checkImageUpdate(sampBool, *node, associationID, applicationID);


		//CONTINUE FROM HERE
		incrementImageSent(*node, &imagesSent);


		mcStatus = MC_Free_Message(&(*node)->msgID);
		if (mcStatus != MC_NORMAL_COMPLETION)
		{
			PrintError("MC_Free_Message failed for request message", mcStatus);
		}

		seconds = GetIntervalElapsed(imageStartTime);
		printf("\tSent %s image (%d of %d), elapsed time: %.3f seconds\n", (*node)->serviceName, imagesSent, totalImages, seconds);

		/*
		 * Traverse through file list
		 */
		*node = (*node)->Next;

	}   /* END for loop for each image */
}


void checkReadImage(SAMP_BOOLEAN sampBool, InstanceNode* node) {
	if (!sampBool)
	{
		(node)->imageSent = SAMP_FALSE;
		printf("Can not open image file [%s]\n", (node)->fname);

	}
}


void checkSendImage(SAMP_BOOLEAN sampBool, InstanceNode* node, int associationID, int applicationID) {
	if (!sampBool)
	{
		(node)->imageSent = SAMP_FALSE;
		printf("Failure in sending file [%s]\n", (node)->fname);
		MC_Abort_Association(&associationID);
		MC_Release_Application(&applicationID);

	}
}

void checkImageUpdate(SAMP_BOOLEAN sampBool, InstanceNode* node, int associationID, int applicationID)
{
	if (!sampBool)
	{
		printf("Warning, unable to update node with information [%s]\n", (node)->fname);

		MC_Abort_Association(&associationID);
		MC_Release_Application(&applicationID);

	}
}

void traverseimages(STORAGE_OPTIONS* A_options, InstanceNode** instanceList, char fname[])
{
	SAMP_BOOLEAN sampBool;

	int imageCurrent = 0;
	for (imageCurrent = A_options->StartImage; imageCurrent <= A_options->StopImage; imageCurrent++)
	{
		sprintf(fname, "%d.img", imageCurrent);
		sampBool = AddFileToList((instanceList), fname);
		if (!sampBool)
		{
			printf("Warning, cannot add SOP instance to File List, image will not be sent [%s]\n", fname);
		}
	}
}
void incrementImageSent(InstanceNode* node, int* imagesSent) {
	if ((node)->imageSent == SAMP_TRUE)
	{
		(*imagesSent)++;
	}
	else
	{
		(node)->responseReceived = SAMP_TRUE;
		(node)->failedResponse = SAMP_TRUE;
	}
}
void checkresponse(STORAGE_OPTIONS* options) {
	if (options->ResponseRequested)
	{
		if (!options->asscInfo.PositiveResponseReceived)
		{
			printf("WARNING: Positive response for User Identity requested from\n\tserver, but not received.\n\n");
		}
	}
}
/********************************************************************
 *
 *  Function    :   PrintCmdLine
 *
 *  Parameters  :   none
 *
 *  Returns     :   nothing
 *
 *  Description :   Prints program usage
 *
 ********************************************************************/
static void PrintCmdLine(void)
{
	printf("\nUsage stor_scu remote_ae start stop -f filename -a local_ae -b local_port -c -n remote_host -p remote_port -l service_list -v -u username -w password -q\n");
	printf("\n");
	printf("\t remote_ae       name of remote Application Entity Title to connect with\n");
	printf("\t start           start image number (not required if -f specified)\n");
	printf("\t stop            stop image number (not required if -f specified)\n");
	printf("\t -f filename     (optional) specify a file containing a list of images to transfer\n");
	printf("\t -a local_ae     (optional) specify the local Application Title (default: MERGE_STORE_SCU)\n");
	printf("\t -b local_port   (optional) specify the local TCP listen port for commitment (default: found in the mergecom.pro file)\n");
	printf("\t -c              Do storage commitment for the transferred files\n");
	printf("\t -n remote_host  (optional) specify the remote hostname (default: found in the mergecom.app file for remote_ae)\n");
	printf("\t -p remote_port  (optional) specify the remote TCP listen port (default: found in the mergecom.app file for remote_ae)\n");
	printf("\t -l service_list (optional) specify the service list to use when negotiating (default: Storage_SCU_Service_List)\n");
	printf("\t -s              Transfer the data using stream (raw) mode\n");
	printf("\t -v              Execute in verbose mode, print negotiation information\n");
	printf("\t -u username     (optional) specify a username to negotiate as defined in DICOM Supplement 99\n");
	printf("\t                 Note: just a username can be specified, or a username and password can be specified\n");
	printf("\t                       A password alone cannot be specified.\n");
	printf("\t -w password     (optional) specify a password to negotiate as defined in DICOM Supplement 99\n");
	printf("\t -q              Positive response to user identity requested from SCP\n");
	printf("\n");
	printf("\tImage files must be in the current directory if -f is not used.\n");
	printf("\tImage files must be named 0.img, 1.img, 2.img, etc if -f is not used.\n");

} /* end PrintCmdLine() */


static void CopyHostName(int* i, char* A_argv[], STORAGE_OPTIONS* A_options)
{

	if (!strcmp(A_argv[*i], "-n") || !strcmp(A_argv[*i], "-N"))
	{
		/*
		 * Remote Host Name
		 */
		(*i)++;
		strcpy(A_options->RemoteHostname, A_argv[*i]);
	}

}
static void CopyPortName(int* i, char* A_argv[], STORAGE_OPTIONS* A_options)
{

	if (!strcmp(A_argv[*i], "-p") || !strcmp(A_argv[*i], "-P"))
	{
		/*
		 * Remote Port Number
		 */
		(*i)++;
		A_options->RemotePort = atoi(A_argv[*i]);

	}

}
static void CopyPatientSOPInstanceUID(int* i, char* A_argv[])
{
	if (!strcmp(A_argv[*i], "-SID"))
	{
		(*i)++;
		PatientDetails.SOPInstanceUID = A_argv[*i];

	}
}
static void CopyPatientFirstName(int* i, char* A_argv[])
{
	if (!strcmp(A_argv[*i], "-fname"))
	{
		(*i)++;
		PatientDetails.FirstName = A_argv[*i];

	}

}
static void CopyPatientLastName(int* i, char* A_argv[])
{
	if (!strcmp(A_argv[*i], "-lname"))
	{
		(*i)++;
		PatientDetails.LastName = A_argv[*i];
	}
}
static void CopyPatientId(int* i, char* A_argv[])
{
	if (!strcmp(A_argv[*i], "-id"))
	{
		(*i)++;
		PatientDetails.PatientId = A_argv[*i];
	}
}
static void CopyCmdArguments(int A_argc, char* A_argv[], STORAGE_OPTIONS* A_options)
{
	int       i = 0, argCount = 0;
	for (i = 1; i < A_argc; i++)
	{

		CopyHostName(&i, A_argv, A_options);
		CopyPortName(&i, A_argv, A_options);
		CopyPatientFirstName(&i, A_argv);
		CopyPatientLastName(&i, A_argv);
		CopyPatientSOPInstanceUID(&i, A_argv);
		CopyPatientId(&i, A_argv);
		if (!strcmp(A_argv[i], "-h"))
		{
			PrintCmdLine();
			//return SAMP_FALSE;
		}
		else
		{
			/*
			 * Parse through the rest of the options
			 */

			argCount++;
			Parsethroughrestofoptions(argCount, &i, A_argv, A_options);

		}
	}
}
static void Parsethroughrestofoptions(int argCount, int* i, char* A_argv[], STORAGE_OPTIONS* A_options)
{
	/*switch (argCount)
	{
	case 1:
		strcpy(A_options->RemoteAE, A_argv[*i]);
		break;
	case 2:
		A_options->StartImage = A_options->StopImage = atoi(A_argv[*i]);
		break;

	default:
		A_options->StopImage = atoi(A_argv[*i]);
		break;

	}*/
	if (argCount == 1) {
		strcpy(A_options->RemoteAE, A_argv[*i]);
	}
	else {
		AssignStartStopOptions(argCount, A_argv, A_options, i);
	}

}
static void AssignStartStopOptions(int argCount, char* A_argv[], STORAGE_OPTIONS* A_options, int* i)
{
	switch (argCount) {
	case 2:
		A_options->StartImage = A_options->StopImage = atoi(A_argv[*i]);
		break;

	case 3:
		A_options->StopImage = atoi(A_argv[*i]);
		break;
		// default:
		// 	printf("Unkown option: %s\n", A_argv[*i]);
		// 	break;
	}
}

static void DefaultServiceList(STORAGE_OPTIONS* A_options)
{
	if (CheckHostDetails(A_options)
		&& !A_options->ServiceList[0])

	{
		strcpy(A_options->ServiceList, "Storage_SCU_Service_List");
	}
}
static int CheckHostDetails(STORAGE_OPTIONS* A_options)
{
	if (A_options->RemoteHostname[0] && (A_options->RemotePort != -1))
	{
		return 1;
	}
	return 0;
}
/*************************************************************************
 *
 *  Function    :   TestCmdLine
 *
 *  Parameters  :   Aargc   - Command line arguement count
 *                  Aargv   - Command line arguements
 *                  A_options - Local application options read in.
 *
 *  Return value:   SAMP_TRUE
 *                  SAMP_FALSE
 *
 *  Description :   Test command line for valid arguements.  If problems
 *                  are found, display a message and return SAMP_FALSE
 *
 *************************************************************************/
static SAMP_BOOLEAN TestCmdLine(int A_argc, char* A_argv[], STORAGE_OPTIONS* A_options)
{

	if (A_argc < 3)
	{
		PrintCmdLine();
		return SAMP_FALSE;
	}

	/*
	 * Set default values
	 */
	A_options->StartImage = 0;
	A_options->StopImage = 0;

	strcpy(A_options->LocalAE, "MERGE_STORE_SCU");
	strcpy(A_options->RemoteAE, "MERGE_STORE_SCP");
	strcpy(A_options->ServiceList, "Storage_SCU_Service_List");


	A_options->RemotePort = -1;
	A_options->RemoteHostname[0] = '\0';
	A_options->Verbose = SAMP_FALSE;
	A_options->StorageCommit = SAMP_FALSE;
	A_options->ListenPort = 1115;
	A_options->ResponseRequested = SAMP_FALSE;
	A_options->StreamMode = SAMP_FALSE;
	A_options->Username[0] = '\0';
	A_options->Password[0] = '\0';

	A_options->UseFileList = SAMP_FALSE;
	A_options->FileList[0] = '\0';

	/*
	 * Loop through each arguement
	 */
	CopyCmdArguments(A_argc, A_argv, A_options);

	/*
	 * If the hostname & port are specified on the command line,
	 * the user may not have the remote system configured in the
	 * mergecom.app file.  In this case, force the default service
	 * list, so we can attempt to make a connection, or else we would
	 * fail.
	 */
	 /*if ( A_options->RemoteHostname[0]
	 &&  !A_options->ServiceList[0]
	  && ( A_options->RemotePort != -1))
	 {
		 strcpy(A_options->ServiceList, "Storage_SCU_Service_List");
	 }*/
	DefaultServiceList(A_options);

	if (A_options->StopImage < A_options->StartImage)
	{
		printf("Image stop number must be greater than or equal to image start number.\n");
		PrintCmdLine();
		return SAMP_FALSE;
	}

	return SAMP_TRUE;

}/* TestCmdLine() */

static void update_list(InstanceNode** A_list, InstanceNode* newNode)
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

/****************************************************************************
 *
 *  Function    :   AddFileToList
 *
 *  Parameters  :   A_list     - List of nodes.
 *                  A_fname    - The name of file to add to the list
 *
 *  Returns     :   SAMP_TRUE
 *                  SAMP_FALSE
 *
 *  Description :   Create a node in the instance list for a file to be sent
 *                  on this association.  The node is added to the end of the
 *                  list.
 *
 ****************************************************************************/
static SAMP_BOOLEAN AddFileToList(InstanceNode** A_list, char* A_fname)
{
	InstanceNode* newNode;


	newNode = (InstanceNode*)malloc(sizeof(InstanceNode));
	if (!newNode)
	{
		PrintError("Unable to allocate object to store instance information", MC_NORMAL_COMPLETION);
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

	update_list(A_list, newNode);

	return (SAMP_TRUE);
}


/****************************************************************************
 *
 *  Function    :   UpdateNode
 *
 *  Parameters  :   A_node     - node to update
 *
 *  Returns     :   SAMP_TRUE
 *                  SAMP_FALSE
 *
 *  Description :   Update an image node with info about a file transferred
 *
 ****************************************************************************/
static SAMP_BOOLEAN UpdateNode(InstanceNode* A_node)
{
	MC_STATUS        mcStatus;

	/*
	 * Get DICOM msgID for tracking of responses
	 */
	mcStatus = MC_Get_Value_To_UInt(A_node->msgID, MC_ATT_MESSAGE_ID, &(A_node->dicomMsgID));
	if (mcStatus != MC_NORMAL_COMPLETION)
	{
		PrintError("MC_Get_Value_To_UInt for Message ID failed", mcStatus);
		A_node->responseReceived = SAMP_TRUE;
		return(SAMP_FALSE);
	}

	A_node->responseReceived = SAMP_FALSE;
	A_node->failedResponse = SAMP_FALSE;
	A_node->imageSent = SAMP_TRUE;

	return (SAMP_TRUE);
}
