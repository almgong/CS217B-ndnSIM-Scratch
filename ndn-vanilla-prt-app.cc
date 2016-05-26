#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include <iostream>


namespace ns3 {

	/**
	* Driver program, for vanilla PRT logic
	* Note we need to implement logic of consumer and producer types
	* (e.g., regular consumers, "bad producers") before running this.
	**/
	int main(int argc, char**argv) {
		std::cout << "hello\n";
		return 0;
	}

}

int
main(int argc, char** argv) {
	return ns3::main(argc, argv);
}
