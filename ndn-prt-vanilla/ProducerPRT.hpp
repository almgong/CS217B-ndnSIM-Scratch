#ifndef CONSUMER_PRT_CBR_H_
#define CONSUMER_PRT__CBR_H_

#include "ns3/ndnSIM/apps/ndn-producer.hpp"
/**
 * Represents a NDN producer, and sepcifically, one that gives out data
 * with signatures that do not verify (in our case, we substitute invalid
 * signatures with a value other than 1, which is the default signature).
**/

namespace ns3 {
namespace ndn {
	class ProducerPRT : public Producer {

		//overrides from a ns3::ndn::Producer
		public:
			static TypeId
  			GetTypeId(void);

			virtual void 
			OnInterest(std::shared_ptr<const Interest> interest);

		private:
			Name m_prefix;
			Name m_postfix;
			uint32_t m_virtualPayloadSize;
			Time m_freshness;

			uint32_t m_signature;
			Name m_keyLocator;

	};
}
}
#endif
