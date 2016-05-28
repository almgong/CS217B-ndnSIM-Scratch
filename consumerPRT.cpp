#include "consumerPRT.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

NS_LOG_COMPONENT_DEFINE("ConsumerPRT");

namespace ns3 {
	NS_OBJECT_ENSURE_REGISTERED(Consumer-PRT);

	TypeId 
	ConsumerPRT::GetTypeId()
	{
		static TypeId tid = TypeId("ConsumerPRT").SetParent<ndn::App>().AddConstructor<CustomApp>();
  		return tid;
	}

	//what to do on startup
	void
	ConsumerPRT::StartApplication()
	{
		ndn::App::StartApplication();
		ndn::FibHelper::Addroute(GetNode(), "/prefix/sub", m_face, 0);
		Simulator::Schedule(Seconds(1.0), &ConsumerPRT::SendInterest);
	}

	// Processing when application is stopped
	void
	CustomApp::StopApplication()
	{
	  // cleanup ndn::App
	  ndn::App::StopApplication();
	}

	void
	CustomApp::SendInterest()
	{
	  /////////////////////////////////////
	  // Sending one Interest packet out //
	  /////////////////////////////////////

	  // Create and configure ndn::Interest
	  auto interest = std::make_shared<ndn::Interest>("/prefix/sub");
	  Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
	  interest->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
	  interest->setInterestLifetime(ndn::time::seconds(1));

	  NS_LOG_DEBUG("Sending Interest packet for " << *interest);

	  // Call trace (for logging purposes)
	  m_transmittedInterests(interest, this, m_face);

	  m_face->onReceiveInterest(*interest);
	}

	// Callback that will be called when Data arrives
	void
	CustomApp::OnData(std::shared_ptr<const ndn::Data> data)
	{
	  NS_LOG_DEBUG("Receiving Data packet for " << data->getName());

	  std::cout << "DATA received for name " << data->getName() << std::endl;
	}
}//eo namespace ns3

