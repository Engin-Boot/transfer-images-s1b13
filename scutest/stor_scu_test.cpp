#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <iostream>
#include "scutest.h"
#include<string.h>
using namespace std;

TEST_CASE("When we pass -fname and patient first name in CMD arguments, then patient First name is stored in the stucture's FirstName"){
    Patient p;
    string colour[5] = { "./scu-test", "-fname", "chandan"};
    int i = 1;
    scu::CopyPatientFirstName(&i,colour,&p);
    REQUIRE(p.FirstName == "chandan");
}
TEST_CASE("When we don't pass -fname  in CMD arguments,then in the Place of FirstName in stucture NULL value will be stored"){
    Patient p;
    string colour[5] = { "./scu-test", "-name", "chandan"};
    int i = 1;
    scu::CopyPatientFirstName(&i,colour,&p);
    REQUIRE(p.FirstName == "");
}
TEST_CASE("When we pass -lname and patient Last name in CMD arguments,then patient Last name is stored in the stucture's LastName"){
    Patient p;
    string colour[3] = { "./scu-test", "-lname","nayak"};
    int i = 1;
    scu::CopyPatientLastName(&i,colour,&p);
    REQUIRE(p.LastName == "nayak");
}
TEST_CASE("When we don't pass -lname in CMD arguments,then in the Place of LastName in stucture NULL value will be stored"){
    Patient p;
    string colour[3] = { "./scu-test", "-name","nayak"};
    int i = 1;
    scu::CopyPatientLastName(&i,colour,&p);
    REQUIRE(p.LastName == "");
}
TEST_CASE("When we pass -id and patient ID in CMD arguments,then patient ID is stored in the stucture's PatientId"){
    Patient p;
    string colour[3] = { "./scu-test", "-id","id123"};
    int i = 1;
    scu::CopyPatientId(&i,colour,&p);
    REQUIRE(p.PatientId == "id123");
}
TEST_CASE("When we don't pass -id in CMD arguments,then in the Place of PatientId in stucture NULL value will be stored"){
    Patient p;
    string colour[3] = { "./scu-test", "-id1","id123"};
    int i = 1;
    scu::CopyPatientId(&i,colour,&p);
    REQUIRE(p.PatientId == "");
}
TEST_CASE("When we pass -SID and patient SOPInstanceUID in CMD arguments,then patient SOPInstanceUID is stored in the stucture's SOPInstanceUID"){
    Patient p;
    string colour[3] = { "./scu-test", "-SID","2.16.840.1.113669.11.0.0.20201004.2143131358"};
    int i = 1;
    scu::CopyPatientSOPInstanceUID(&i,colour,&p);
    REQUIRE(p.SOPInstanceUID == "2.16.840.1.113669.11.0.0.20201004.2143131358");
}
TEST_CASE("When we pass -n to this function the name of the RemoteHost is stored in the structure"){
    STORAGE_OPTIONS s;
    string CMDargument[3] = {"./scu-test","-n","localhost"};
    int i=1;
    scu::CopyHostName(&i,CMDargument,&s);
    REQUIRE(s.RemoteHostname == "localhost");
}
TEST_CASE("When we pass -N to this function the name of the RemoteHost is stored in the structure"){
    STORAGE_OPTIONS s;
    string CMDargument[3] = {"./scu-test","-N","localhost"};
    int i=1;
    scu::CopyHostName(&i,CMDargument,&s);
    REQUIRE(s.RemoteHostname == "localhost");
}
TEST_CASE("When we don't pass -N or -n to this function the name of the RemoteHost is not stored in the structure"){
    STORAGE_OPTIONS s;
    string CMDargument[3] = {"./scu-test","-N1","localhost"};
    int i=1;
    scu::CopyHostName(&i,CMDargument,&s);
    REQUIRE(s.RemoteHostname == "");
}
TEST_CASE("When we pass -p to this function the PortNumber of the RemoteHost is stored in the structure"){
    STORAGE_OPTIONS s;
    string CMDargument[3] = {"./scu-test","-p","6677"};
    int i=1;
    scu::CopyPortName(&i,CMDargument,&s);
    REQUIRE(s.RemotePort == 6677);
}
TEST_CASE("When we pass -P to this function the PortNumber of the RemoteHost is stored in the structure"){
    STORAGE_OPTIONS s;
    string CMDargument[3] = {"./scu-test","-P","6677"};
    int i=1;
    scu::CopyPortName(&i,CMDargument,&s);
    REQUIRE(s.RemotePort == 6677);
}
TEST_CASE("When we don't pass -P or -p to this function the PortNumber of the RemoteHost is not stored in the structure"){
    STORAGE_OPTIONS s;
    string CMDargument[3] = {"./scu-test","-P1","6677"};
    int i=1;
    scu::CopyPortName(&i,CMDargument,&s);
    REQUIRE(s.RemotePort == 0);
}
TEST_CASE("travese node if start and stop numbers are correct"){
 STORAGE_OPTIONS A_options;
 char fname[512] = { 0 };
 A_options.StartImage=0;
 A_options.StopImage=0;
 	InstanceNode* instanceList = NULL;

   REQUIRE(scu::traverseimages(&A_options, &instanceList, fname)==SAMP_TRUE);
}
TEST_CASE("donot travese node if start and stop numbers are incorrect"){
 STORAGE_OPTIONS A_options;
 char fname[512] = { 0 };
 A_options.StartImage=10;
 A_options.StopImage=0;
 	InstanceNode* instanceList = NULL;

   REQUIRE(scu::traverseimages(&A_options, &instanceList, fname) == SAMP_FALSE);
}
TEST_CASE("when no of nodes is empty the function getnumnodes returns 0")
{STORAGE_OPTIONS A_options;
 char fname[512] = { 0 };
 A_options.StartImage=10;
 A_options.StopImage=0;
 	InstanceNode* instanceList = NULL;
scu::traverseimages(&A_options, &instanceList, fname);

    REQUIRE(scu::GetNumNodes(instanceList)==0);

}
TEST_CASE("when no of nodes is non  empty the function getnumnodes returns no of nodes present")
{STORAGE_OPTIONS A_options;
 char fname[512] = { 0 };
 A_options.StartImage=0;
 A_options.StopImage=0;
 	InstanceNode* instanceList = NULL;
scu::traverseimages(&A_options, &instanceList, fname);

    REQUIRE(scu::GetNumNodes(instanceList)==1);

}
TEST_CASE("add new file node to empty nodelist")
{ 
    InstanceNode*    newNode;
InstanceNode* instanceList = NULL;
	newNode = (InstanceNode*)malloc(sizeof(InstanceNode));
	memset(newNode, 0, sizeof(InstanceNode));
	strncpy(newNode->fname, "0.img", sizeof(newNode->fname));
	newNode->fname[sizeof(newNode->fname) - 1] = '\0';

	newNode->responseReceived = SAMP_FALSE;
	newNode->failedResponse = SAMP_FALSE;
	newNode->imageSent = SAMP_FALSE;
	newNode->msgID = -1;
	newNode->transferSyntax = IMPLICIT_LITTLE_ENDIAN;
    scu::update_list(&instanceList,newNode);
    REQUIRE(scu::GetNumNodes(instanceList)==1);

}
TEST_CASE("add new file node to existing nodelist")
{ 
    InstanceNode*    newNode1,*newNode2;
InstanceNode* instanceList = NULL;
	newNode1= (InstanceNode*)malloc(sizeof(InstanceNode));
	memset(newNode1, 0, sizeof(InstanceNode));
	strncpy(newNode1->fname, "0.img", sizeof(newNode1->fname));
	newNode1->fname[sizeof(newNode1->fname) - 1] = '\0';
	newNode2 = (InstanceNode*)malloc(sizeof(InstanceNode));
	memset(newNode2, 0, sizeof(InstanceNode));
	strncpy(newNode2->fname, "1.img", sizeof(newNode2->fname));
	newNode2->fname[sizeof(newNode2->fname) - 1] = '\0';

	
    scu::update_list(&instanceList,newNode1);
     scu::update_list(&instanceList,newNode2);

    REQUIRE(scu::GetNumNodes(instanceList)==2);

}
TEST_CASE("when we pass storage options to the function, then it has to check whether RemoteHostName and RemotePort is set"){
    STORAGE_OPTIONS A_options;
REQUIRE(scu::CheckHostDetails(&A_options) == 0);
}


