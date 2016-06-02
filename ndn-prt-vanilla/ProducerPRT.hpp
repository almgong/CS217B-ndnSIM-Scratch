#ifndef PRODUCER_PRT_H_
#define PRODUCER_PRT_H_

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/ndnSIM/apps/ndn-app.hpp"
#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/nstime.h"
#include "ns3/ptr.h"

/**
 * Represents a NDN producer, and sepcifically, one that gives out data
 * with signatures that do not verify (in our case, we substitute invalid
 * signatures with a value other than 1, which is the default signature).
**/

namespace ns3 {
namespace ndn {
	class ProducerPRT : public App {

		//overrides from a ns3::ndn::Producer
		public:
			static TypeId
  			GetTypeId(void);

			virtual void 
			OnInterest(std::shared_ptr<const Interest> interest);

			//cstr
			ProducerPRT();

		private:
			Name m_prefix;
			Name m_postfix;
			uint32_t m_virtualPayloadSize;
			Time m_freshness;

			uint32_t m_signature;
			Name m_keyLocator;

		protected:
			virtual void StartApplication();
			virtual void StopApplication();
	};
}
}
#endif
