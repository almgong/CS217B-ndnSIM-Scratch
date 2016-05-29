#ifndef CONSUMER_PRT_CBR_H_
#define CONSUMER_PRT__CBR_H_

#include "ns3/ndnSIM/apps/ndn-consumer-cbr.hpp"

/**
 * Make your life easier, inherit defaults of ns3::ndn::ConsumerCBR
***/

namespace ns3 {


	class ConsumerPRTCBR : public ndn::ConsumerCbr {
		public:
			static TypeId GetTypeId();
	};


}

#endif