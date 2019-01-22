// -*- C++ -*-
//
// Package:    MetAnalyzer
// Class:      MetAnalyzer
// 

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

//classes to extract PFMET information
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"

//classes to save data
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"

//class to save the histograms
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include<vector>

//
// class declaration
//

class MetAnalyzer : public edm::EDAnalyzer {
   public:
      explicit MetAnalyzer(const edm::ParameterSet&);
      ~MetAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);

      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
//declare a function to do the met analysis
      void analyzeMets(const edm::Event& iEvent, const edm::Handle<reco::PFMETCollection> &mets);


//se declara el input tag de tipo PFMETCollection         
      edm::InputTag metInput;

	  // ----------member data ---------------------------

	int nummet; //number of mets in the event
	TH1D *methisto;
	TH1D *hist_e;
	TH1D *hist_pt;
	TH1D *hist_px;
	TH1D *hist_py;
	//TH1D *hist_pz;
	//TH1D *hist_eta;
	TH1D *hist_phi;
	TH1D *hist_ch;
	TFile *mfile;
	TTree *mtree;

	std::vector<float> _e;
  	std::vector<float> _pt;
  	std::vector<float> _px;
  	std::vector<float> _py;
  	//std::vector<float> _pz;
  	//std::vector<float> _eta;
  	std::vector<float> _phi;
  	std::vector<float> _ch;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

MetAnalyzer::MetAnalyzer(const edm::ParameterSet& iConfig)

{
//now do what ever initialization is needed
	edm::Service<TFileService> fs;

// se crean los histogramas
	hist_e = fs->make <TH1D>("hist_energy", "Electron energy", 100, 0, 5000);
	hist_pt = fs->make <TH1D>("hist_pt", "Electron pt ", 100,0,5000 );
	hist_px = fs->make <TH1D>("hist_px", "Electron px ", 100, 0, 5000 );
	hist_py = fs->make <TH1D>("hist_py", "Electron py ", 100, 0, 5000 );
	//hist_pz = fs->make <TH1D>("hist_pz", "Electron pz ", 100, 0, 5000 );
	//hist_eta = fs->make <TH1D>("hist_eta", "Electron eta ", 100, 0, 5000 );
	hist_phi = fs->make <TH1D>("hist_phi", "Electron phi ", 100, 0, 5000 );
	hist_ch =  fs->make <TH1D>("hist_ch", "Electron ch ", 100,0,5000 );
	methisto = fs->make <TH1D>("methisto", "Met histo", 100, 0, 5000);

	metInput = iConfig.getParameter<edm::InputTag>("InputCollection");

}


MetAnalyzer::~MetAnalyzer()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void
MetAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;


   Handle<reco::PFMETCollection> mymets;
   iEvent.getByLabel(metInput, mymets);

   analyzeMets(iEvent,mymets);

   mtree->Fill();
   return;

}

//************************************************************************

void 
MetAnalyzer::analyzeMets(const edm::Event& iEvent, const edm::Handle<reco::PFMETCollection> &mets)
{
	  nummet = 0;
	  _e.clear();
	  _pt.clear();
	  _px.clear();
	  _py.clear();
	  //_pz.clear();
	  //_eta.clear();
	  _phi.clear();
	  _ch.clear();

  if(mets.isValid()){
     // get the number of mets in the event
     nummet=(*mets).size();
     methisto->Fill(mets->size());
        for (reco::PFMETCollection::const_iterator itmet=mets->begin(); itmet!=mets->end(); ++itmet){

	    _e.push_back(itmet->energy());
	    _pt.push_back(itmet->pt());
	    _px.push_back(itmet->px());
	    _py.push_back(itmet->py());
	    //_pz.push_back(itmet->pz());
	    //_eta.push_back(itmet->eta());
	    _phi.push_back(itmet->phi());
	    _ch.push_back(itmet->charge());

	    hist_e->Fill(itmet->energy());
	    hist_pt->Fill(itmet->pt());
	    hist_px->Fill(itmet->px());
	    hist_py->Fill(itmet->py());
	    //hist_pz->Fill(itmet->pz());
	    //hist_eta->Fill(itmet->eta());
	    hist_phi->Fill(itmet->phi());
	    hist_ch->Fill(itmet->charge());

        }
  }
}

//*************************************************************************


// ------------ method called once each job just before starting event loop  ------------
void
MetAnalyzer::beginJob()
{

mfile = new TFile("MetInfo.root","RECREATE");
mtree = new TTree("mtree","Met information");

  mytree->Branch("met_e",&_e);
  mytree->Branch("met_pt",&_pt);
  mytree->Branch("met_px",&_px);
  mytree->Branch("met_py",&_py);
  //mytree->Branch("met_pz",&_pz);
  //mytree->Branch("met_eta",&_eta);
  mytree->Branch("met_phi",&_phi);
  mytree->Branch("met_ch",&_ch);

}

// ------------ method called once each job just after ending the event loop  ------------
void
MetAnalyzer::endJob()
{
  mfile->Write();
}

// ------------ method called when starting to processes a run  ------------
void
MetAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void
MetAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
// ------------ method called when starting to processes a luminosity block  ------------
void
MetAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void
MetAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MetAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
 //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MetAnalyzer);
