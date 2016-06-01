#include "ProducerPRT.hpp"

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

namespace ns3 {
namespace ndn {
	NS_LOG_COMPONENT_DEFINE("ProducerPRT");

	TypeId
	Producer::GetTypeId (void)
	{
		static TypeId tid = TypeId ("ns3::ndn::ProducerPRT")
			.SetGroupName ("Ndn")
			.SetParent<App> ()
			.AddConstructor<Producer> ()
			.AddAttribute ("Prefix","Prefix, for which producer has the data",
			                StringValue ("/"),
			                MakeNameAccessor (&Producer::m_prefix),
			                MakeNameChecker ())
			.AddAttribute ("Postfix", "Postfix that is added to the output data (e.g., for adding producer-uniqueness)",
			                StringValue ("/"),
			                MakeNameAccessor (&Producer::m_postfix),
			                MakeNameChecker ())
			.AddAttribute ("PayloadSize", "Virtual payload size for Content packets",
			               UintegerValue (1024),
			               MakeUintegerAccessor (&Producer::m_virtualPayloadSize),
			               MakeUintegerChecker<uint32_t> ())
			.AddAttribute ("Freshness", "Freshness of data packets, if 0, then unlimited freshness",
			                TimeValue (Seconds (0)),
			                MakeTimeAccessor (&Producer::m_freshness),
			                MakeTimeChecker ())
			.AddAttribute ("Signature", "Fake signature, 0 valid signature (default), other values application-specific",
			               UintegerValue (0),
			               MakeUintegerAccessor (&Producer::m_signature),
			               MakeUintegerChecker<uint32_t> ())
			.AddAttribute ("KeyLocator", "Name to be used for key locator.  If root, then key locator is not used",
			                NameValue (),
			                MakeNameAccessor (&Producer::m_keyLocator),
			                MakeNameChecker ())
			;
		return tid;
	}

	void
	ProducerPRT::OnInterest(std::shared_ptr<const Interest> interest) {
		App::OnInterest(interest); // tracing inside

		NS_LOG_FUNCTION(this << interest);

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
	}
}
}
