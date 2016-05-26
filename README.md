# CS217B-ndnSIM-Scratch

This directory should be in: [...]/ndnSIM/ns-3/scratch

Put all simulations (.cc) files in this directory and running ./waf should automatically build the simulation. If you include the optimize flag then I think debugging wont show, so don't do that for testing purposes.

Check out ndnsim.net/2.1/examples/html for good examples, we probably can just do the 9 grid example, but with our own needs.

UPDATE: a great resource I am using is: ndnsim.net/2.1/applications.html
- it has good examples on how to modify the behavior or consumers and producers,
it seems like we wont be able to implement a report easily, but we should be able
to simulate the effects of the report (via interests with clever suffixes)