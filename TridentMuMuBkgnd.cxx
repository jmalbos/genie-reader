#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>
#include <GHEP/GHepParticle.h>

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include <fstream>
#include <iostream>


struct Particle {
  int pdg_code;
  double momentum [3];
  double energy;
  double mass;
};


void PrintUsage()
{
  std::cerr << "Usage: GenieReader <input_file>" << std::endl;
  std::exit(EXIT_FAILURE);
}


int main(int argc, char const *argv[])
{
  //if (argc < 2) PrintUsage();

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

    //std::cout << *gmcrec << std::endl;
    
    genie::EventRecord* evtrec = gmcrec->event;

    bool muon = false;
    bool pion = false;

    std::vector<Particle> partv;
    genie::GHepParticle* gpart = 0;

    // Loop through the particles in the GHEP record

    TIter iter(evtrec);
    while ((gpart = dynamic_cast<genie::GHepParticle*>(iter.Next()))) {
     
      //std::cout << "Reading a particle" << std::endl;
 
      // We only care about final-state particles (status 1)
      if (gpart->Status() != 1) continue;

      Particle prtcl;
      prtcl.pdg_code = gpart->Pdg();
      
      // Skip GENIE's pseudo particles
      if (prtcl.pdg_code >= 2000000000) continue;

      // Raise the appropiate flags if we see a muon or a pion
      if (std::abs(prtcl.pdg_code) ==  13) muon = true;
      else if (std::abs(prtcl.pdg_code) == 211) pion = true;

      prtcl.mass = gpart->Mass();
      
      prtcl.energy = gpart->E();

      prtcl.momentum[0] = gpart->Px();
      prtcl.momentum[1] = gpart->Py();
      prtcl.momentum[2] = gpart->Pz();

      partv.push_back(prtcl);
    }

    // If both a muon and a charged pion were present in the event
    // (i.e. the flags are true), we'll store it in the output file
    if (muon && pion) { 
      ofile << "<event>" << std::endl;
      for (Particle p: partv) {
	ofile << p.pdg_code    << " "
	      << "1"           << " "
	      << p.momentum[0] << " "
	      << p.momentum[1] << " "
	      << p.momentum[2] << " "
	      << p.energy      << " "
	      << p.mass        << std::endl;
      }
      ofile << "</event>" << std::endl;
    }
    
    std::cout << "gmcrec->Clear()" << std::endl;
    gmcrec->Clear(); // DO THIS OR THE DESTRUCTOR WILL SEG FAULT
  }

  return 0;
}
