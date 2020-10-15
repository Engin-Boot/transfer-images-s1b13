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
