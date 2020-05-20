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


/*** Recoil Energies ****
* 214Pb (from 218Po): 6114.7 - 6002.4 keV = 112.3 keV 
* 210Pb (from 214Po): 7833.5 - 7686.8 keV = 146.7 keV
* 206Pb: 5407.5 - 5304.4 keV = 103.1 keV
************************/

// Lengths, and straggling, in Angstroms, from SRIM.
// Then converted to nm (/10.)
// Values for Teflon
const double L206 = 458/10.;
const double L206e = 74/10.;
const double L210 = 579/10.;
const double L210e = 89/10.;
const double L214 = 484/10.;
const double L214e = 77/10.;
const double L218 = 1/10.;   //Unused
const double L218e = 1/10.;  //Unused

// Collected fraction after decay.  Relevant if in gas.  
// Probability that ion is charged. If ion is charged, electric field
// in gas immediately transfer ion to surface.
const double q218 = 0.88;
const double q214 = 1.0;
const double q210 = 1.0;
const double q206 = 0.77;  //???? Use 218 value. Not used. 

enum STATE {gas, surface, buried};
TFile *f = new TFile("DepthTree.root","RECREATE");

void PoDepths(int nEvents = 100000000)
{

  TTree *Po218 = new TTree("Po218","Polonium-218");
  double D_Po218, E_Po218, C_Po218;
  Po218->Branch("depth",&D_Po218); // Atom depth
  // Po218->Branch("energyFrac",&E_Po218); // Recoil energy
  // Po218->Branch("ionDeposited",&C_Po218); // Recoil ionisation deposited

  TTree *Po214 = new TTree("Po214","Polonium-214");
  double D_Po214, E_Po214, C_Po214;
  Po214->Branch("depth",&D_Po214); // Atom depth
  // Po214->Branch("energyFrac",&E_Po214); // Recoil energy
  // Po214->Branch("ionDeposited",&C_Po214); // Recoil ionisation deposited

  TTree *Po210 = new TTree("Po210","Polonium-210");
  double D_Po210, E_Po210, C_Po210;
  Po210->Branch("depth",&D_Po210); // Atom depth
  // Po210->Branch("energyFrac",&E_Po210); // Recoil energy
  // Po210->Branch("ionDeposited",&C_Po210); // Recoil ionisation deposited

  TTree *Pb214 = new TTree("Pb214","Lead-214");
  double D_Pb214, E_Pb214, C_Pb214;
  Pb214->Branch("depth",&D_Pb214);
  // Pb214->Branch("energyFrac",&E_Pb214);
  // Pb214->Branch("ionDeposited",&C_Pb214);

  TTree *Pb210 = new TTree("Pb210","Lead-210");
  double D_Pb210, E_Pb210, C_Pb210;
  Pb210->Branch("depth",&D_Pb210);
  // Pb210->Branch("energyFrac",&E_Pb210);
  // Pb210->Branch("ionDeposited",&C_Pb210);

  TTree *Pb206 = new TTree("Pb206","Lead-206");
  double D_Pb206, E_Pb206, C_Pb206;
  Pb206->Branch("depth",&D_Pb206);
  // Pb206->Branch("energyFrac",&E_Pb206);
  // Pb206->Branch("ionDeposited",&C_Pb206);

  TRandom* tr = new TRandom3();

  //initialize counters:
  //gas recoils from Al:
  int b_Pb206 =0;
  int b_Pb210 =0;
  int b_Pb214 =0;
  //gas recoils from surface:
  int s_Pb206=0;
  int s_Pb210=0;
  int s_Pb214=0;

  for (int i=0; i<nEvents; i++) {
    double R;
    STATE state = gas;
    double depth = -10000; //begins in gas

    // Rn-222 decay produces Po-218:
    // It's either in the gas, OR just on the surface.
    if (tr->Rndm() < q218) {depth = 0; state = surface;}
    D_Po218 = depth;
    Po218->Fill();

    // Po-218 decay produces Pb-214:
    R = tr->Gaus(L214, L214e);
    R = R*2.0*(tr->Rndm()-0.5);
    depth += R;
    // If it bounced back from surface or inside, increase counter
    if (state == surface && depth < 0) s_Pb214++;
    if (state == buried && depth < 0) b_Pb214++;
    // If it goes inside surface, tag it as buried
    if (depth > 0) state = buried;
    // If outside the surface, tag it in gas. Reset depth.
    if (depth < 0) {state = gas; depth = -10000;}
    // If already inside gas, calculate random prob. to be charged 
    // and be transferred on surface
    if (state == gas && tr->Rndm() < q214) {depth = 0; state = surface;}
    // Fill tree
    D_Pb214 = depth;
    Pb214->Fill();

    // Pb-214 decay produces Po-214, but distance travelled is negligible
    D_Po214 = D_Pb214;
    Po214->Fill();

    // Po-214 decay produces Pb-210:
    R = tr->Gaus(L210, L210e);
    R = R*2.0*(tr->Rndm()-0.5);
    depth += R;
    // If it bounced back from surface or inside, increase counter
    if (state == surface && depth < 0) s_Pb210++;
    if (state == buried && depth < 0) b_Pb210++;
    // If it goes inside surface, tag it as buried
    if (depth > 0) state = buried;
    // If outside the surface, tag it in gas. Reset depth.
    if (depth < 0) {state = gas; depth = -10000;}
    // If already inside gas, calculate random prob. to be charged 
    // and be transferred on surface
    if (state == gas && tr->Rndm() < q210) {depth = 0; state = surface;}
    // Fill tree
    D_Pb210 = depth;
    Pb210->Fill();

    // Pb-210 decay produces Po-210, but distance travelled is negligible
    D_Po210 = D_Pb210;
    Po210->Fill();

    // Po-210 decay produces Pb-206:
    R = tr->Gaus(L206, L206e);
    R = R*2.0*(tr->Rndm()-0.5);
    depth += R;
    // If it bounced back from surface or inside, increase counter
    if (state == surface && depth < 0) s_Pb206++;
    if (state == buried && depth < 0) b_Pb206++;
    // If it goes inside surface, tag it as buried
    if (depth > 0) state = buried;
    // If outside the surface, tag it in gas. Reset depth.
    if (depth < 0) {state = gas; depth = -10000;}
    // If already inside gas, calculate random prob. to be charged 
    // and be transferred on surface
    if (state == gas && tr->Rndm() < q206) {depth = 0; state = surface;}
    // Fill tree
    D_Pb206 = depth;
    Pb206->Fill();
  }

  printf("     surface/buried   \n");
  printf(" Pb-214     Pb-210     Pb-206 \n");
  printf("%i/%i  %i/%i  %i/%i\n", s_Pb214,b_Pb214,s_Pb210,b_Pb210,s_Pb206,b_Pb206);

  f->Write();
  f->Close();

}