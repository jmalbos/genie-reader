#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>
#include <GHEP/GHepParticle.h>

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include <fstream>
#include <iostream>

int main(int argc, char const *argv[])
{
  if (argc<2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // Open output text file
  std::ofstream ofile;
  ofile.open("output.txt", std::ofstream::out);

  // Open ROOT file
  //TFile input_file(argv[1]);
  //TTree* tree = dynamic_cast<TTree*>(input_file.Get("gtree"));
  TChain chain("gtree");
  chain.Add("*.neutrino.*.ghep.root");
  genie::NtpMCEventRecord* gmcrec = 0;
  //tree->GetBranch("gmcrec")->SetAddress(&gmcrec);
  chain.SetBranchAddress("gmcrec", &gmcrec);

  std::cout << "Entries: " << chain.GetEntries() << std::endl;

  for (Long64_t i=0; i<chain.GetEntries(); ++i) {

    chain.GetEntry(i);

    genie::EventRecord* evtrec = gmcrec->event;

    // Store now statistics about the target nucleus
    genie::Interaction* interaction = evtrec->Summary();
    const genie::Target& tgt = interaction->InitState().Tgt();

    ofile << tgt.Z() << "   "
          << evtrec ->Vertex()->X() << "   "
          << evtrec ->Vertex()->Y() << "   "
          << evtrec ->Vertex()->Z() << "   "
          << std::endl;

    gmcrec->Clear(); // DO THIS OR THE DESTRUCTOR WILL SEG FAULT
  }

  ofile.close();

  return 0;
}
