/**************************************
 *
 * PoDepths.C
 * A macro to model the depth of 210Po atoms present 
 * after 222Rn exposure.  
 *
 * Atoms begin in the gas after a 222Rn decay.  
 * Some fraction (user input) attach to the surface.  
 * For each subsequent decay:
 * -Calculate recoil direction and length
 * -Atoms in gas stay in gas, others migrate within target (and can leave)
 * 
 * Created  Nov 19 2014  - Eric H. Miller
 * Updated for PTFE work July 31 2017 - EHM
 * Updated for Roughness studies May 4 2020 - Davide Porzio
 *
 ***************************************/


/*** Recoil Energies ***
* 214Pb: 6114.7 - 6002.4 keV = 112.3 keV 
* 210Pb: 7833.5 - 7686.8 keV = 146.7 keV
* 206Pb: 5407.5 - 5304.4 keV = 103.1 keV
************************/

TFile *f = new TFile("DepthTree.root","RECREATE");

const double L206 = 458;
const double L206e = 74;
const double L210 = 579;
const double L210e = 89;
const double L214 = 484;
const double L214e = 77;
const double L218 = 1;   //Unused
const double L218e = 1;  //Unused

// Collected fraction after decay.  Relevant if in gas.  
const double q218 = 0.88;
const double q214 = 1.0;
const double q210 = 1.0;
const double q206 = 0.77;  //???? Use 218 value. Not used. 

enum STATE {gas, surface, buried};

void PoDepths(int nEvents = 100000000)
{

  TTree *Pb218 = new TTree("Pb218","Lead-218");
  double D_Pb218, E_Pb218, C_Pb218;
  Pb218->Branch("depth",&D_Pb218); // Atom depth
  Pb218->Branch("energyFrac",&E_Pb218); // Recoil energy
  Pb218->Branch("ionDeposited",&E_Pb218); // Recoil ionisation deposited

  TTree *Pb214 = new TTree("Pb214","Lead-214");
  double D_Pb214, E_Pb214, C_Pb214;
  Pb214->Branch("depth",&D_Pb214);
  Pb214->Branch("energyFrac",&E_Pb214);
  Pb214->Branch("ionDeposited",&E_Pb214);

  TTree *Pb210 = new TTree("Pb210","Lead-210");
  double D_Pb210, E_Pb210, C_Pb210;
  Pb210->Branch("depth",&D_Pb210);
  Pb210->Branch("energyFrac",&E_Pb210);
  Pb210->Branch("ionDeposited",&E_Pb210);

  TTree *Pb206 = new TTree("Pb206","Lead-206");
  double D_Pb206, E_Pb206, C_Pb206;
  Pb206->Branch("depth",&D_Pb206);
  Pb206->Branch("energyFrac",&E_Pb206);
  Pb206->Branch("ionDeposited",&E_Pb206);

  TRandom* tr = new TRandom3();

  //initialize counters:
  //gas recoils from Al:
  int b206 =0;
  int b210 =0;
  int b214 =0;
  //gas recoils from surface:
  int s206=0;
  int s210=0;
  int s214=0;

  for (int i=0; i<nEvents; i++) {
    double R;
    STATE state = gas;
    double depth = -10000; //begins in gas

    // Rn-222 decay produces Po-218:
    // It's either in the gas, OR just on the surface.
    if (tr->Rndm() < q218) { depth = 0; state = surface; }
    D_Pb218 = depth;
    Pb218->Fill();

    //Po-218 decay produces Pb-214:
    R = tr->Gaus(L214, L214e);
    R = R*2.0*(tr->Rndm()-0.5);
    depth += R;

    if (state == surface && depth < 0) { hE214->Fill(depth/R); s214++; }
    if (depth > 0) state = buried;
    if (depth < 0) { state = gas; depth = -10000; }
    if (state == gas && tr->Rndm() < q214) { depth = 0; state = surface; }
    hD214->Fill(depth);

    //Po-214 decay produces Pb-210:
    R = tr->Gaus(L210, L210e);
    R = R*2.0*(tr->Rndm()-0.5);
    depth += R;

    if (state == surface && depth < 0) { hE210->Fill(depth/R); s210++; }
    if (state == buried && depth < 0) { b210++; hE210->Fill(depth/R); }
    if (depth > 0) state = buried;
    if (depth < 0) { state = gas; depth = -10000; }
    if (state == gas && tr->Rndm() < q210) { depth = 0; state = surface; }
    hD210->Fill(depth);

    //Po-210 decay produces Pb-206:
    R = tr->Gaus(L206, L206e);
    R = R*2.0*(tr->Rndm()-0.5);
    depth += R;

    if (state == surface && depth < 0) { hE206->Fill(depth/R); s206++; }
    if (state == buried && depth < 0) { b206++; hE206->Fill(depth/R); }
    if (depth > 0) state = buried;
    if (depth < 0) { state = gas; depth = -10000; }
    if (state == gas && tr->Rndm() < q206) { depth = 0; state = surface; }
    hD206->Fill(depth);
  }

  printf("     surface/buried   \n");
  printf(" Pb-214     Pb-210     Pb-206 \n");
  printf("%i/%i  %i/%i  %i/%i\n", s214,b214,s210,b210,s206,b206);

  hE206->Draw();

}