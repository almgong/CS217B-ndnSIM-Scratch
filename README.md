# CS217B-ndnSIM-Scratch

The CONTENTS of this repo should be in: [...]/ndnSIM/ns-3/scratch

./waf should automatically build the simulation. If you include the optimize flag then I think debugging wont show, so don't do that for testing purposes.

Things to keep in mind:

- All cpp source files should have the .cc extension (who knows why the compiler refuses
to ackownledge .cpp).

- for each SCENARIO, make a new directory under the [...]/scratch/ directory, such as
ndn-prt-vanilla, and put all relevant .cc and .hpp files here. The compiler and linker
treats this entire directory as ONE scenario, and thus, only one instance of a main
function should be in here (across all files).

- ./waf compiles and links everything as long as your follow the above

- ./waf --run <scenario name (e.g. ndn-prt-vanilla) runs the scenario

Check out ndnsim.net/2.1/examples/html for good examples, we probably can just do the 9 grid example, but with our own needs.

UPDATE: a great resource I am using is: ndnsim.net/2.1/applications.html
- it has good examples on how to modify the behavior or consumers and producers,
it seems like we wont be able to implement a report easily, but we should be able
to simulate the effects of the report (via interests with clever suffixes)

--------------------------------------------------------------------------------

Example:

While in the scratch/ directory, you can do NS_LOG=ndn.Consumer:ndn.Producer ../waf --run ndn-prt-vanilla to run the simulation with all NS_LOG_DEBUG prints printed to your terminal. You can also use --vis to see the simulation in a new tab (will need XQuartz if on OSX).