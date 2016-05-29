#include "ConsumerPRTCBR.hpp"

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

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

NS_LOG_COMPONENT_DEFINE("ConsumerPRTCBR");

namespace ns3 {

	NS_OBJECT_ENSURE_REGISTERED(ConsumerPRTCBR);

	TypeId
    ConsumerPRTCBR::GetTypeId (void)
    {
      static TypeId tid = TypeId ("ns3::ndn::ConsumerPRTCBR")
        .SetGroupName ("Ndn")
        .SetParent<Consumer> ()
        .AddConstructor<ConsumerPRTCBR> ()
    
       .AddAttribute ("Frequency", "Frequency of interest packets",
                       StringValue ("1.0"),
                       MakeDoubleAccessor (&ConsumerPRTCBR::m_frequency),
                       MakeDoubleChecker<double> ())
        
        .AddAttribute ("Randomize", "Type of send time randomization: none (default), uniform, exponential",
                       StringValue ("none"),
                       MakeStringAccessor (&ConsumerPRTCBR::SetRandomize, &ConsumerCbr::GetRandomize),
                       MakeStringChecker ())
    
        .AddAttribute ("MaxSeq",
                       "Maximum sequence number to request",
                       IntegerValue (std::numeric_limits<uint32_t>::max ()),
                       MakeIntegerAccessor (&ConsumerPRTCBR::m_seqMax),
                       MakeIntegerChecker<uint32_t> ())
    
        ;
    
      return tid;
    }


}