#ifndef CONSUMER_PRT_H_
#define CONSUMER_PRT_H_

#include "ns3/ndnSIM/apps/ndn-app.hpp"

/**
 * Represents a consumer under the PRT scheme (ideally sends a report or something
 * like that on receipt of invalid data).
**/

namespace ns3 {

	class ConsumerPRT : public ndn::App {
		public:
		  // register NS-3 type "CustomApp"
		  static TypeId
		  GetTypeId();

		  // (overridden from ndn::App) Processing upon start of the application
		  virtual void
		  StartApplication();

		  // (overridden from ndn::App) Processing when application is stopped
		  virtual void
		  StopApplication();

		  // (overridden from ndn::App) Callback that will be called when Interest arrives
		  virtual void
		  OnInterest(std::shared_ptr<const ndn::Interest> interest);

		  // (overridden from ndn::App) Callback that will be called when Data arrives
		  virtual void
		  OnData(std::shared_ptr<const ndn::Data> contentObject);

		private:
		  void
		  SendInterest();
};

	};

}

#endif