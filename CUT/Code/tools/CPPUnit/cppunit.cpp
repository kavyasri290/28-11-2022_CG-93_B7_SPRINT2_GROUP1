#include <iostream>
#include <list>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <netinet/in.h>
#include "common.h"
#include "sch_client.h"

using namespace CppUnit;
using namespace std;

class TestM:public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestM);
    CPPUNIT_TEST(testSockfd); 
    CPPUNIT_TEST(testPort);
    CPPUNIT_TEST(testAddress);
    CPPUNIT_TEST_SUITE_END();

    public:
    	void setUp(void);
    	void tearDown(void);

    protected:
    	void testSockfd(void);
	void testPort(void); 
	void testAddress(void);
    private:
    	TcpClient *client;
};


void TestM::testSockfd(void)
{	
	CPPUNIT_ASSERT(4==client->getSockFd());
}

void TestM::testPort(void)
{
	CPPUNIT_ASSERT(65123==client->getPort());
}
void TestM::testAddress(void)
{
	CPPUNIT_ASSERT("127.0.0.1"==client->getAddress());
}

void TestM::setUp(void)
{
	client=new TcpClient(65123,11,"127.0.01");
}

void TestM::tearDown(void)
{
	delete client;
	}

CPPUNIT_TEST_SUITE_REGISTRATION( TestM );

int main(int argc, char* argv[])
{
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // Output XML for Jenkins CPPunit plugin
    ofstream xmlFileOut("scheduler.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}


