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
	NS_LOG_COMPONENT_DEFINE("ConsumerPRTCBR");


	void
	ProducerPRT::OnInterest(Ptr<const Interest> interest) {
		App::OnInterest(interest); // tracing inside

		NS_LOG_FUNCTION(this << interest);

		if (!m_active)
		return;

		Name dataName(interest->getName());
		// dataName.append(m_postfix);
		// dataName.appendVersion();

		auto data = make_shared<Data>();
		data->setName(dataName);
		data->setFreshnessPeriod(::ndn::time::milliseconds(m_freshness.GetMilliSeconds()));

		data->setContent(make_shared< ::ndn::Buffer>(m_virtualPayloadSize));

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

