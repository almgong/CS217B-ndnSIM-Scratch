#ifndef BAD_CONSUMER_PRT__CBR_H_
#define BAD_CONSUMER_PRT__CBR_H_

#include "ConsumerPRTCBR.hpp"

#include <set>
#include <map>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include <memory>

/**
 *  A bad consumer, simply responds to any incoming data packet with a
 *  report interest.
***/

namespace ns3 {
namespace ndn {
		class BadConsumerPRTCBR : public ConsumerPRTCBR {
			public:
				static TypeId GetTypeId();
				void OnData(std::shared_ptr<const Data> data);
				void HandleInvalidPacket();	//temp
		};

}
}
#endif
