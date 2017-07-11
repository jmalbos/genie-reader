#include <Ntuple/NtpMCEventRecord.h>

#include <TFile.h>
#include <TTree.h>

#include <iostream>


void PrintUsage()
{
  std::cerr << "Usage: GenieReader <input_file>" << std::endl;
  std::exit(EXIT_FAILURE);
}


int main(int argc, char const *argv[])
{
  if (argc < 2) PrintUsage();

  // Open ROOT file
  TFile input_file(argv[1]);
  TTree* tree = dynamic_cast<TTree*>(input_file.Get("gtree"));
  genie::NtpMCEventRecord* gmcrec = 0;
  tree->GetBranch("gmcrec")->SetAddress(&gmcrec);

  for (Long64_t i=0; i<tree->GetEntries(); ++i) {
    tree->GetEntry(i);
    std::cout << *gmcrec << std::endl;
  }

  return 0;
}
