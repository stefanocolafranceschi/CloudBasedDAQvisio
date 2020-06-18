#include <TFile.h>
#include <TMemFile.h>
#include <TNtuple.h>
#include <TH2.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TFrame.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TRandom3.h>
#include <TBenchmark.h>
#include <TInterpreter.h>
#include <THttpServer.h>


// USAGE
// http://localhost:8082/?item=Files/raw.root/h1rate&nobrowser&monitoring=1000

void process(const char* jobname = "raw", Long64_t maxcnt = 0) {
   THttpServer* serv = new THttpServer("http:8082");

   TString filename = Form("%s.root", jobname);
   TFile* file = TFile::Open(filename);

   //while(1) {
// press Ctrl-C to stop macro
   while (!gSystem->ProcessEvents()) {

       file->ReadKeys();
       delete file->FindObject("h1rate");
       TH1F *h1rate;
       file->GetObject("h1rate",h1rate);
       //serv->Register("h1rate", h1rate);
       sleep(1);
   }

   //TFile *hfile = new TFile(filename,"READ","Demo ROOT file with histograms");
   //TH1F * h1 = new TH1F(“h1”,“h1 title” , 100, 0, 4);
   //h1 = (TH1F*)hfile.Get("h1f");
   ////TH1F* m_hist;
   ////file->GetObject("h1rate",m_hist); 
}