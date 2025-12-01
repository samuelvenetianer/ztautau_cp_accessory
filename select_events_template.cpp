// Cullen's template to get started
// This script will select a specific number of random events from an ntuple

TTree* tree /* */;                              // create tree with name "tree" (or maybe a pointer?)
// Branch named "energy" has type double.

double energyBuffer;                            // create buffer with same type as branch
tree->SetBranchAddress("energy",&energyBuffer); // link branch to buffer

TRandom3 rand;                                  // create random variable called "rand"

for ( int randCounter = 0 ; randCounter < N_RAND_EVENTS ; randCounter++ ) { // iterate from 0 to the max number of events you want to select
  int i = rand.Uniform(tree->GetEntries());     // get the index of the event selected

  tree->GetEvent(i);                            // get the branch information for that index

  std::cout << "Energy for event " << i << ": " << energyBuffer << std::endl; // print branch information for that index
}

