#include "BadConsumerPRTCBR.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/integer.h"
#include "ns3/double.h"

//#include "ns3/ndn-app-face.h"
//#include "ns3/ndn-interest.h"
//#include "ns3/ndn-data.h"

#include "utils/ndn-ns3-packet-tag.hpp"
#include "model/ndn-app-face.hpp"
#include "utils/ndn-rtt-mean-deviation.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/ref.hpp>

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

NS_LOG_COMPONENT_DEFINE("ndn.BadConsumerPRTCBR");

namespace ns3 {
	namespace ndn {

		NS_OBJECT_ENSURE_REGISTERED(BadConsumerPRTCBR);

		TypeId
	    BadConsumerPRTCBR::GetTypeId (void)
	    {
	      static TypeId tid = TypeId ("ns3::ndn::BadConsumerPRTCBR")
	        .SetGroupName ("Ndn")
	        .SetParent<ConsumerPRTCBR> ()
	        .AddConstructor<BadConsumerPRTCBR> ()	    
	        ;
	    
	      return tid;
	    }

	    void
	    BadConsumerPRTCBR::HandleInvalidPacket() {
	    	//nothing

	    }

	    void 
	    BadConsumerPRTCBR::OnData(std::shared_ptr<const Data> data) {

	    	/* regular handling + logging */
	    	if (!m_active)
		      return;
		    
		    App::OnData(data); // tracing inside

		    NS_LOG_FUNCTION(this << data);

		    // NS_LOG_INFO ("Received content object: " << boost::cref(*data));

		    // This could be a problem......
		    NS_LOG_INFO("< DATA for " << data->getName());


		    /*  Simply reply with a report  */
		    SendInterest();

	    }



	}
}