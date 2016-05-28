#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include <iostream>
#include "ConsumerPRT.hpp"

namespace ns3 {

	/**
	* Driver program, for vanilla PRT logic
	* Note we need to implement logic of consumer and producer types
	* (e.g., regular consumers, "bad producers") before running this.
	**/
	int main(int argc, char**argv) {
		std::cout << "hello\n";
		// Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
		CommandLine cmd;
		cmd.Parse(argc, argv);

		// Creating nodes
		Ptr<Node> node = CreateObject<Node>();

		// Install NDN stack on all nodes
		ndn::StackHelper ndnHelper;
		ndnHelper.SetDefaultRoutes(true);
		ndnHelper.InstallAll();

		// App1
		ndn::AppHelper app1("ConsumerPRT");
		app1.Install(node);

		// App2
		ndn::AppHelper app2("Hijacker");
		app2.Install(node); // last node

		Simulator::Stop(Seconds(20.0));

		Simulator::Run();
		Simulator::Destroy();

		return 0;
	}

}

int
main(int argc, char** argv) {
	return ns3::main(argc, argv);
}
