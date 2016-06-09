#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/log.h"

#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"

#include <iostream>

#include "ConsumerPRTCBR.hpp"
#include "ProducerPRT.hpp"

namespace ns3 {
	using ns3::ndn::StackHelper;
	using ns3::ndn::AppHelper;
	using ns3::ndn::GlobalRoutingHelper;
	using ns3::ndn::StrategyChoiceHelper;
	using ns3::AnnotatedTopologyReader;
	/**
	* Driver program, for vanilla PRT logic
	* Note we need to implement logic of consumer and producer types
	* (e.g., regular consumers, "bad producers") before running this.
	**/
	int main(int argc, char**argv) {
		
		// Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
		CommandLine cmd;
		cmd.Parse(argc, argv);
		/*
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
		/*
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
		ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/client-control");

		// Installing global routing interface on all nodes
		ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
		ndnGlobalRoutingHelper.InstallAll();

		// Getting containers for the consumer/producer
		Ptr<Node> producer = grid.GetNode(3, 3);
		Ptr<Node> badProducer = grid.GetNode(1,0);
		NodeContainer consumerNodes;
		consumerNodes.Add(grid.GetNode(0, 0));
		consumerNodes.Add(grid.GetNode(0,3));

		// Install NDN applications
		std::string prefix = "/prefix";

		ndn::AppHelper consumerHelper("ns3::ndn::ConsumerPRTCBR");
		consumerHelper.SetPrefix(prefix);
		consumerHelper.SetAttribute("Frequency", StringValue("10")); // 10 interests a second
		consumerHelper.Install(consumerNodes);

		ndn::AppHelper producerHelper("ns3::ndn::Producer");
		producerHelper.SetPrefix(prefix);
		producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
		producerHelper.Install(producer);

		ndn::AppHelper badProducerHelper("ns3::ndn::ProducerPRT");
		badProducerHelper.SetPrefix(prefix);
		badProducerHelper.SetAttribute("PayloadSize", StringValue("1024"));
		badProducerHelper.Install(badProducer);

		// Add /prefix origins to ndn::GlobalRouter
		ndnGlobalRoutingHelper.AddOrigins(prefix, producer);
		ndnGlobalRoutingHelper.AddOrigins(prefix, badProducer);

		// Calculate and install FIBs
		ndn::GlobalRoutingHelper::CalculateRoutes();
		*/





		/* Using custom topolgy */
		AnnotatedTopologyReader topologyReader("", 25);
		topologyReader.SetFileName("src/ndnSIM/examples/topologies/topology-single-producer.txt");
		topologyReader.Read();

		// Install NDN stack on all nodes
		StackHelper ndnHelper;
		ndnHelper.InstallAll();

		ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/client-control");

		// Installing global routing interface on all nodes
		GlobalRoutingHelper ndnGlobalRoutingHelper;
		ndnGlobalRoutingHelper.InstallAll();

		// Getting containers for the consumer/producer
		Ptr<Node> producer1 = Names::Find<Node>("producer");
		NodeContainer consumerNodes;
		consumerNodes.Add(Names::Find<Node>("group-1-node-1"));
		consumerNodes.Add(Names::Find<Node>("group-1-node-2"));

		// Install NDN applications
		std::string prefix = "/prefix";
		AppHelper consumerHelper("ns3::ndn::ConsumerPRTCBR");
		consumerHelper.SetPrefix(prefix);
		consumerHelper.SetAttribute("Frequency", StringValue("10")); // 100 interests a second
		consumerHelper.Install(consumerNodes);

		AppHelper producerHelper("ns3::ndn::ProducerPRT");
		producerHelper.SetPrefix(prefix);
		producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
		producerHelper.Install(producer1);
		//producerHelper.Install(producer2);

		// ndn::AppHelper badProducerHelper("ns3::ndn::ProducerPRT");
		// badProducerHelper.SetPrefix(prefix);
		// badProducerHelper.SetAttribute("PayloadSize", StringValue("1024"));
		// badProducerHelper.Install(badProducer);

		// Add /prefix origins to ndn::GlobalRouter
		ndnGlobalRoutingHelper.AddOrigins(prefix, producer1);
		//ndnGlobalRoutingHelper.AddOrigins(prefix, producer2);

		// Calculate and install FIBs
		GlobalRoutingHelper::CalculateRoutes();


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
