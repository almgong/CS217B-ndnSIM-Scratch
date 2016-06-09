#ifndef CONSUMER_PRT_CBR_H_
#define CONSUMER_PRT__CBR_H_

#include "ns3/ndnSIM/apps/ndn-consumer-cbr.hpp"

#include <set>
#include <map>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include <memory>

/**
 * Make your life easier, inherit defaults of ns3::ndn::ConsumerCBR
***/

namespace ns3 {
namespace ndn {
		class ConsumerPRTCBR : public ConsumerCbr {
			public:
				static TypeId GetTypeId();
				void OnData(std::shared_ptr<const Data> data);
				void SendPacket();
				void SendInterest();
				void HandleInvalidPacket();	//temp
		};

}
}
#endif
