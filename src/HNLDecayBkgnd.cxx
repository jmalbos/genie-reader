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

//### Numero de evento
//M_weight		N_weight		MN		USquared		L_lab_N		x_0		y_0		z_0		thetaN		phiN		M_width_CM		BR_M		N_width_CM		BR_N		time_N


int main(int argc, char const *argv[])
{
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // Open input ROOT file.
  // Let's assume the second command-line parameter is the input filename.
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

    // Ignore inteactions happening on elements other than argon
    const genie::Target& tgt = evtrec->Summary()->InitState().Tgt();
    if (tgt.Z() != 18) continue;

    // Loop through the particles in the GHEP record
    int number_mulike = 0;
    std::vector<Particle> prtv;
    genie::GHepParticle* gprt = 0;
    TIter iter(evtrec);
    while ((gprt = dynamic_cast<genie::GHepParticle*>(iter.Next()))) {

      // We only care about final-state particles (i.e. status 1)
      if (gprt->Status() != 1) continue;

      // Ignore GENIE's pseudo-particles
      if (gprt->Pdg() >= 2000000000) continue;

      Particle prt;
      prt.pdg = gprt->Pdg();

      // Count the particle as mu-like if it's a muon or a charged pion
      if (std::abs(prt.pdg) == 13 || std::abs(prt.pdg) == 211) ++number_mulike;

      // Store the particle in the particle vector
      prt.mass   = gprt->Mass();
      prt.energy = gprt->E();
      prt.momentum[0] = gprt->Px();
      prt.momentum[1] = gprt->Py();
      prt.momentum[2] = gprt->Pz();
      prtv.push_back(prt);
    }

    // If the event contains 2 or more mu-like particles,
    // store it in the output file
    if (number_mulike >= 2) {

      ofile << "### " << i << std::endl;

      ofile << 0 << "   "
            << 0 << "   "
            << 0 << "   "
            << 0 << "   "
            << 0 << "   "
            << evtrec->Vertex()->X() << "   "
            << evtrec->Vertex()->Y() << "   "
            << evtrec->Vertex()->Z() << "   "
            << 0 << "   "
            << 0 << "   "
            << 0 << "   "
            << 0 << "   "
            << 0 << "   "
            << 0 << "   "
            << 0 << "   "
            << std::endl;

      for (Particle prt: prtv) {
        ofile << prt.pdg         << "   "
              << prt.energy      << "   "
              << prt.momentum[0] << "   "
              << prt.momentum[1] << "   "
              << prt.momentum[2] << "   "
              << std::endl;
      }
    }

    gmcrec->Clear(); // DO THIS OR THE DESTRUCTOR WILL SEG FAULT
  }

  ofile.close();
  ifile.Close();

  return 0;
}
