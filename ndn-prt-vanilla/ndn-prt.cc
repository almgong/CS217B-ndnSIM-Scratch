#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/log.h"

#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"

#include <iostream>
#include "ConsumerPRT.hpp"

namespace ns3 {

	/**
	* Driver program, for vanilla PRT logic
	* Note we need to implement logic of consumer and producer types
	* (e.g., regular consumers, "bad producers") before running this.
	**/
	int main(int argc, char**argv) {
		/*std::cout << "hello\n";
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
		ndn::AppHelper app1("ns3::ndn::ConsumerPRT");
		app1.Install(node);

		// App2
		ndn::AppHelper app2("ns3::ndn::ConsumerPRT");
		app2.Install(node); // last node

		*/


		//3x3 ndn grid
		// Setting default parameters for PointToPoint links and channels
		Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
		Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
		Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("10"));

		// Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
		CommandLine cmd;
		cmd.Parse(argc, argv);

		// Creating 3x3 topology
		PointToPointHelper p2p;
		PointToPointGridHelper grid(4, 4, p2p);
		grid.BoundingBox(100, 100, 200, 200);

		// Install NDN stack on all nodes
		ndn::StackHelper ndnHelper;
		ndnHelper.InstallAll();

		// Set BestRoute strategy
		ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route");

		// Installing global routing interface on all nodes
		ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
		ndnGlobalRoutingHelper.InstallAll();

		// Getting containers for the consumer/producer
		Ptr<Node> producer = grid.GetNode(3, 3);
		Ptr<Node> badProducer = grid.GetNode(2,2);
		NodeContainer consumerNodes;
		consumerNodes.Add(grid.GetNode(0, 0));
		consumerNodes.Add(grid.GetNode(0,3));

		// Install NDN applications
		std::string prefix = "/ndn/prefix";

		ndn::AppHelper consumerHelper("ns3::ndn::ConsumerPRTCBR");
		consumerHelper.SetPrefix(prefix);
		consumerHelper.SetAttribute("Frequency", StringValue("10")); // 10 interests a second
		consumerHelper.Install(consumerNodes);

		ndn::AppHelper producerHelper("ns3::ndn::Producer");
		producerHelper.SetPrefix(prefix);
		producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
		producerHelper.Install(producer);

		ndn::AppHelper badProducerHelper("ns3::ndn::Producer");
		badProducerHelper.SetPrefix(prefix);
		badProducerHelper.SetAttribute("PayloadSize", StringValue("1024"));
		badProducerHelper.Install(badProducer);

		//producerHelper.SetPrefix("/prefix/report/1");
		//producerHelper.Install(producer);

		// Add /prefix origins to ndn::GlobalRouter
		ndnGlobalRoutingHelper.AddOrigins(prefix, producer);
		ndnGlobalRoutingHelper.AddOrigins(prefix, badProducerHelper);

		// Calculate and install FIBs
		ndn::GlobalRoutingHelper::CalculateRoutes();
		
		Simulator::Stop(Seconds(10.0));

		Simulator::Run();
		Simulator::Destroy();

		return 0;
	}

}

int
main(int argc, char** argv) {
	return ns3::main(argc, argv);
}
