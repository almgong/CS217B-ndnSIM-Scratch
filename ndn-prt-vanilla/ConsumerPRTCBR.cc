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

#include "utils/ndn-ns3-packet-tag.hpp"
#include "model/ndn-app-face.hpp"
#include "utils/ndn-rtt-mean-deviation.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/ref.hpp>

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

NS_LOG_COMPONENT_DEFINE("ndn.ConsumerPRTCBR");

namespace ns3 {
namespace ndn {
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
                       MakeStringAccessor (&ConsumerPRTCBR::SetRandomize, &ConsumerPRTCBR::GetRandomize),
                       MakeStringChecker ())
    
        .AddAttribute ("MaxSeq",
                       "Maximum sequence number to request",
                       IntegerValue (std::numeric_limits<uint32_t>::max ()),
                       MakeIntegerAccessor (&ConsumerPRTCBR::m_seqMax),
                       MakeIntegerChecker<uint32_t> ())
    
        ;
    
      return tid;
    }

  //Process incoming packets 
  //override ndn-consumer            
  void
  ConsumerPRTCBR::OnData(std::shared_ptr<const Data> data)
  {
    if (!m_active)
      return;
    
    App::OnData(data); // tracing inside

    NS_LOG_FUNCTION(this << data);

    // NS_LOG_INFO ("Received content object: " << boost::cref(*data));

    // This could be a problem......
    uint32_t seq = data->getName().at(-1).toSequenceNumber();
    NS_LOG_INFO("< DATA for " << data->getName());


    //signature verification, if it isn't 1 then we have a problem
    //std::cout << "Signature " << data->getSignature() << std::endl;

    //data->getSignature().getValue().parse();
    //std::cout << "value: " << data->getSignature().hasKeyLocator() << std::endl;
    
    if(data->getSignature().hasKeyLocator()) {
      //std::cout << "Skipped " << std::endl;

      //TODO now just send a report and we are good to go
      SendInterest();       //temporary
      HandleInvalidPacket();
      return;
    }

    int hopCount = 0;
    auto ns3PacketTag = data->getTag<Ns3PacketTag>();
    if (ns3PacketTag != nullptr) { // e.g., packet came from local node's cache
      FwHopCountTag hopCountTag;
      if (ns3PacketTag->getPacket()->PeekPacketTag(hopCountTag)) {
        hopCount = hopCountTag.Get();
        NS_LOG_DEBUG("Hop count: " << hopCount);
      }
    }

    SeqTimeoutsContainer::iterator entry = m_seqLastDelay.find(seq);
    if (entry != m_seqLastDelay.end()) {
      m_lastRetransmittedInterestDataDelay(this, seq, Simulator::Now() - entry->time, hopCount);
    }

    entry = m_seqFullDelay.find(seq);
    if (entry != m_seqFullDelay.end()) {
      m_firstInterestDataDelay(this, seq, Simulator::Now() - entry->time, m_seqRetxCounts[seq], hopCount);
    }

    m_seqRetxCounts.erase(seq);
    m_seqFullDelay.erase(seq);
    m_seqLastDelay.erase(seq);

    m_seqTimeouts.erase(seq);
    m_retxSeqs.erase(seq);

    m_rtt->AckSeq(SequenceNumber32(seq));
  }


  //send next packet, but really, send the report
  //overried ndn-consumer
  void
  ConsumerPRTCBR::SendPacket()
  {
    if (!m_active)
      return;

    NS_LOG_FUNCTION_NOARGS();

    uint32_t seq = std::numeric_limits<uint32_t>::max(); // invalid

    while (m_retxSeqs.size()) {
      seq = *m_retxSeqs.begin();
      m_retxSeqs.erase(m_retxSeqs.begin());
      break;
    }

    if (seq == std::numeric_limits<uint32_t>::max()) {
      if (m_seqMax != std::numeric_limits<uint32_t>::max()) {
        if (m_seq >= m_seqMax) {
          return; // we are totally done
        }
      }

      seq = m_seq++;
    }

    //
    std::shared_ptr<Name> nameWithSequence = std::make_shared<Name>(m_interestName);
    nameWithSequence->appendSequenceNumber(seq);
    std::cout << *nameWithSequence << std::endl;
    //

    // shared_ptr<Interest> interest = make_shared<Interest> ();
    std::shared_ptr<Interest> interest = std::make_shared<Interest>();
    interest->setNonce(m_rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
    interest->setName(*nameWithSequence);
    time::milliseconds interestLifeTime(m_interestLifeTime.GetMilliSeconds());
    interest->setInterestLifetime(interestLifeTime);

    // NS_LOG_INFO ("Requesting Interest: \n" << *interest);
    NS_LOG_INFO("> Interest for " << seq);

    WillSendOutInterest(seq);

    m_transmittedInterests(interest, this, m_face);
    m_face->onReceiveInterest(*interest);

    ScheduleNextPacket();
  }


  //meant to send a single interest packet, can use as alternative to report
  void
  ConsumerPRTCBR::SendInterest()
  {
    /////////////////////////////////////
    // Sending one Interest packet out //
    /////////////////////////////////////

    // Create and configure ndn::Interest
    auto interest = std::make_shared<Interest>("/report/");
    Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
    interest->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
    interest->setInterestLifetime(time::seconds(1));

    NS_LOG_DEBUG("Sending Interest packet for " << *interest);

    // Call trace (for logging purposes)
    m_transmittedInterests(interest, this, m_face);

    m_face->onReceiveInterest(*interest);
  }

  //implement or override in subclass to handle case where packet is invalid
  void
  ConsumerPRTCBR::HandleInvalidPacket() {
    std::cout << "Invalid packet handled" << std::endl;
  }


}
}
