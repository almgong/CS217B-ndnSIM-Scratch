#include "ConsumerPRTCBR.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

NS_LOG_COMPONENT_DEFINE("ConsumerPRTCBR");

namespace ns3 {

	NS_OBJECT_ENSURE_REGISTERED(ConsumerPRTCBR);

	TypeId 
	ConsumerPRT::GetTypeId()
	{
		static TypeId tid = TypeId("ns3::ndn::ConsumerPRTCBR").SetParent<ndn::ConsumerCBR>().AddConstructor<ConsumerPRTCBR>();
  		return tid;
	}


}