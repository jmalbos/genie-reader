#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>
#include <GHEP/GHepParticle.h>

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include <fstream>
#include <iostream>


struct Particle {
  int pdg;
  double mass;
  double energy;
  double momentum[3];
};


int main(int argc, char const *argv[])
{
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // Open input ROOT file.
  // Let's assume the second command-line parameters is the filename.
  TFile ifile(argv[1]);
  TTree* tree = dynamic_cast<TTree*>(ifile.Get("gtree"));
  genie::NtpMCEventRecord* gmcrec = 0;
  tree->GetBranch("gmcrec")->SetAddress(&gmcrec);

  // Do the following if you want to use a TChain and not a TTree
  //TChain filechain("gtree");
  //filechain.Add("*.neutrino.*.ghep.root");
  //genie::NtpMCEventRecord* gmcrec = 0;
  //filechain.SetBranchAddress("gmcrec", &gmcrec);

  // Open output text file
  std::ofstream ofile;
  ofile.open("output.txt", std::ofstream::out);

  // Origin of coordinates for vertices
  double x_0 = 0.00;
  double y_0 = 3.06;
  double z_0 = 2.59;

  auto entries = tree->GetEntries();
  //auto entries = filechain.GetEntries();
  //std::cout << "Number of events: " << entries << std::endl;

  for (auto i=0; i<entries; ++i) {

    tree->GetEntry(i);
    //chain.GetEntry(i);

    genie::EventRecord* evtrec = gmcrec->event;

    int number_mupi = 0;

    std::vector<Particle> prtv;
    genie::GHepParticle* gprt = 0;

    // Loop through the particles in the GHEP record
    TIter iter(evtrec);
    while ((gprt = dynamic_cast<genie::GHepParticle*>(iter.Next()))) {

      // We only care about final-state particles (i.e. status 1)
      if (gprt->Status() != 1) continue;

      // Ignore GENIE's pseudo-particles
      if (gprt->Pdg() >= 2000000000) continue;

      Particle prt;
      prt.pdg = gprt->Pdg();

      // Count the particle as mu-like if it's a muon or a charged pion
      if (std::abs(prt.pdg) == 13 || std::abs(prt.pdg) == 211) ++number_mupi;

      prt.mass   = gprt->Mass();
      prt.energy = gprt->E();
      prt.momentum[0] = gprt->Px();
      prt.momentum[1] = gprt->Py();
      prt.momentum[2] = gprt->Pz();

      prtv.push_back(prt);
    }

    // If the event contains 2 or more mu-like particles,
    // store it in the output file
    if (number_mupi >= 2) {

      ofile << "<event>" << std::endl;

      ofile << (evtrec->Vertex()->X() - x_0) << " "
            << (evtrec->Vertex()->Y() - y_0) << " "
            << (evtrec->Vertex()->Z() - z_0) << " "
            << std::endl;

      for (Particle prt: prtv) {
        ofile << prt.pdg << " "
              << "1"     << " "
              << prt.momentum[0] << " "
              << prt.momentum[1] << " "
              << prt.momentum[2] << " "
              << prt.energy      << " "
              << prt.mass        << std::endl;
      }

      ofile << "</event>" << std::endl;
    }

    gmcrec->Clear(); // DO THIS OR THE DESTRUCTOR WILL SEG FAULT
  }

  ofile.close();
  ifile.Close();

  return 0;
}
