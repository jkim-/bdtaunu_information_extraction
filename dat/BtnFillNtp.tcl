# $Id: BtnFillNtp.tcl,v 1.4 2014/08/12 21:24:18 dchao Exp $ 
# Fill and dump the ntuple using BtaTupleMaker

sequence create BtnNtupleSequence
path append Everything BtnNtupleSequence

module clone BtuTupleMaker BtnUpsilonTupleMaker
sequence append BtnNtupleSequence BtnUpsilonTupleMaker

talkto BtnUpsilonTupleMaker {
  
  listToDump set BtnUpsilonList
  ntupleName set "Upsilon Candidates"

  if { $ConfigPatch == "MC" } {
      fillMC set true
  } else {
      fillMC set false
  }

  eventBlockContents set "EventID CMp4 BeamSpot UsrData(EventFeatures)"
  eventTagsInt       set "nTracks nGoodTrkLoose"
  eventTagsFloat     set "R2All R2"
  
  mcBlockContents    set "Mass CMMomentum Momentum Vertex"

  ntpBlockConfigs set "Upsilon(4S) Y 2   800"
  ntpBlockConfigs set "B+    B      4    400"
  ntpBlockConfigs set "B0    B      4    400"
  ntpBlockConfigs set "D*+   D      5    200"
  ntpBlockConfigs set "D*0   D      5    200"
  ntpBlockConfigs set "D0    D      5    200"
  ntpBlockConfigs set "D+    D      5    200"
  ntpBlockConfigs set "K_S0  C      2    100"
  ntpBlockConfigs set "rho+  C      2    100"
  ntpBlockConfigs set "pi0   C      2    100"
  ntpBlockConfigs set "K+    h      2    100"
  ntpBlockConfigs set "pi+   h      0    100"
  ntpBlockConfigs set "e+    l      3    100"
  ntpBlockConfigs set "mu+   l      2    100"
  ntpBlockConfigs set "gamma gamma  0    100"

  ntpBlockContents set "Y    : MCIdx Mass Momentum CMMomentum Vertex VtxChi2 nDaughters UsrData(TagBFeatures,SigBFeatures,EvtCandFeatures)"
  ntpBlockContents set "B    : MCIdx Mass Momentum CMMomentum Vertex VtxChi2 nDaughters"
  ntpBlockContents set "D    : MCIdx Mass Momentum CMMomentum Vertex VtxChi2 Doca DocaXY Poca PocaXY PocaErr PocaCov nDaughters"
  ntpBlockContents set "C    : MCIdx Mass Momentum CMMomentum Vertex VtxChi2 Doca DocaXY Poca PocaXY PocaErr PocaCov nDaughters"
  ntpBlockContents set "h    : MCIdx Mass Momentum CMMomentum Vertex VtxChi2 Doca DocaXY Poca PocaXY PocaErr PocaCov nDaughters PIDWeight(piKMVeryLoose,KKMSuperLoose,KKMTight)"
  ntpBlockContents set "l    : MCIdx Mass Momentum CMMomentum Vertex VtxChi2 Doca DocaXY Poca PocaXY PocaErr PocaCov nDaughters PIDWeight(LooseKMElectronMicro,TightKMElectronMicro,muBDTLooseFakeRate,muBDTTightFakeRate)"
  ntpBlockContents set "gamma: MCIdx Mass Momentum CMMomentum nDaughters"

  ntpBlockToTrk set "h l"
  wantATrkBlock set true

  trkExtraContents set HOTS:detailSVT,detailDCH
  trkExtraContents set Eff:ave,charge
  trkExtraContents set BitMap:KSelectorsMap,eSelectorsMap,muSelectorsMap,piSelectorsMap,pSelectorsMap
  trkExtraContents set Dirc
  gamExtraContents set EMC
}

