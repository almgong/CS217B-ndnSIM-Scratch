#include "ProducerPRT.hpp"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"

#include "ns3/ndnSIM/model/ndn-app-face.hpp"
#include "ns3/ndnSIM/model/ndn-ns3.hpp"
#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include <memory>


NS_LOG_COMPONENT_DEFINE("ndn.ProducerPRT");

namespace ns3 {
namespace ndn {

	NS_OBJECT_ENSURE_REGISTERED(ProducerPRT);

	ProducerPRT::ProducerPRT ()
	{
	  NS_LOG_FUNCTION_NOARGS ();
	  std::cout << "Constructor of bad producer" << std::endl;
	}

	TypeId
	ProducerPRT::GetTypeId (void)
	{
		std::cout << "Got type id of bad producer" << std::endl;
		static TypeId tid = TypeId ("ns3::ndn::ProducerPRT")
			.SetGroupName ("Ndn")
			.SetParent<App> ()
			.AddConstructor<ProducerPRT> ()
			.AddAttribute ("Prefix","Prefix, for which producer has the data",
			                StringValue ("/"),
			                MakeNameAccessor (&ProducerPRT::m_prefix),
			                MakeNameChecker ())
			.AddAttribute ("Postfix", "Postfix that is added to the output data (e.g., for adding producer-uniqueness)",
			                StringValue ("/"),
			                MakeNameAccessor (&ProducerPRT::m_postfix),
			                MakeNameChecker ())
			.AddAttribute ("PayloadSize", "Virtual payload size for Content packets",
			               UintegerValue (1024),
			               MakeUintegerAccessor (&ProducerPRT::m_virtualPayloadSize),
			               MakeUintegerChecker<uint32_t> ())
			.AddAttribute ("Freshness", "Freshness of data packets, if 0, then unlimited freshness",
			                TimeValue (Seconds (0)),
			                MakeTimeAccessor (&ProducerPRT::m_freshness),
			                MakeTimeChecker ())
			.AddAttribute ("Signature", "Fake signature, 0 valid signature (default), other values application-specific",
			               UintegerValue (1),
			               MakeUintegerAccessor (&ProducerPRT::m_signature),
			               MakeUintegerChecker<uint32_t> ())
			.AddAttribute ("KeyLocator", "Name to be used for key locator.  If root, then key locator is not used",
			                NameValue (),
			                MakeNameAccessor (&ProducerPRT::m_keyLocator),
			                MakeNameChecker ())
			;
		return tid;
	}

	// inherited from Application base class.
	void
	ProducerPRT::StartApplication ()
	{
		NS_LOG_FUNCTION_NOARGS();
		App::StartApplication();
		std::cout << "Started bad producer" << std::endl;
		FibHelper::AddRoute(GetNode(), m_prefix, m_face, 0);
	}

	void
	ProducerPRT::StopApplication ()
	{
		NS_LOG_FUNCTION_NOARGS ();
		App::StopApplication ();
		std::cout << "Stopped bad producer" << std::endl;
	}



	void
	ProducerPRT::OnInterest(std::shared_ptr<const Interest> interest) {
		App::OnInterest(interest); // tracing inside

		NS_LOG_FUNCTION(this << interest);
		//NS_LOG_DEBUG("got an interest in baddie");
		//std::cout << "interest received by baddie" << std::endl;
		if (!m_active)
		return;

		Name dataName(interest->getName());
		// dataName.append(m_postfix);
		// dataName.appendVersion();

		auto data = std::make_shared<Data>();
		data->setName(dataName);
		data->setFreshnessPeriod(::ndn::time::milliseconds(m_freshness.GetMilliSeconds()));

		data->setContent(std::make_shared< ::ndn::Buffer>(m_virtualPayloadSize));

		Signature signature;
		SignatureInfo signatureInfo(static_cast< ::ndn::tlv::SignatureTypeValue>(255));

		if (m_keyLocator.size() > 0) {
		signatureInfo.setKeyLocator(m_keyLocator);
		}

		signature.setInfo(signatureInfo);
		signature.setValue(::ndn::nonNegativeIntegerBlock(::ndn::tlv::SignatureValue, m_signature));
		std::cout << "m_signature: " << m_signature << " actual signature: " << signature << std::endl;
		data->setSignature(signature);

		NS_LOG_INFO("node(" << GetNode()->GetId() << ") responding with Data: " << data->getName());

		// to create real wire encoding
		data->wireEncode();

		m_transmittedDatas(data, this, m_face);
		m_face->onReceiveData(*data);
		//std::cout << "interest replied to by baddie" << std::endl;
	}
}
}
