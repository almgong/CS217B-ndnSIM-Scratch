#ifndef CONSUMER_PRT_CBR_H_
#define CONSUMER_PRT__CBR_H_

#include "ns3/ndnSIM/apps/ndn-producer.hpp"
/**
 * Represents a NDN producer, and sepcifically, one that gives out data
 * with signatures that do not verify (in our case, we substitute invalid
 * signatures with a value other than 1, which is the default signature).
**/

namespace ns3 {
	class ProducerPRT : public ndn::Producer {

		//overrides from a regular producer
		void OnInterest(shared_ptr<const ndn::Interest> interest);

	};
}

#endif