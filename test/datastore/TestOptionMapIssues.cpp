#include <pulsar/testing/CppTester.hpp>
#include <pulsar/datastore/OptionMap.hpp>
#include <sstream>

using namespace pulsar;

TEST_SIMPLE(TestOptionMapIssues){
 CppTester tester("Testing OptionMapIssues class");

 OptionMapIssues OMI,OMI1;
 tester.test_member_return("Default OptionMapIssue has no issues",true,true,&OptionMapIssues::ok,&OMI);
 OMI.toplevel.push_back("An Issue");
 tester.test_member_return("Adding Whole Issue",true,false,&OptionMapIssues::ok,&OMI);
 OMI1.optissues["Option"]={"Option's Issue"};
 tester.test_member_return("Adding local1 issue",true,false,&OptionMapIssues::ok,&OMI1);

 std::string mssg=
 "OptionMap has some issues\n"
 "    OptionMap top level issues:\n"
 "        An Issue\n";
 tester.test_member_return("To string works",
                           true,mssg,&OptionMapIssues::to_string,&OMI);



 tester.print_results();
 return tester.nfailed();
}
