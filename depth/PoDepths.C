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

//Atom Depths:
TH1F* hD218;
TH1F* hD214;
TH1F* hD210;
TH1F* hD206;

//Recoil Energies:
TH1F* hE214;
TH1F* hE210;
TH1F* hE206;

//Recoil Ionization deposited:
TH1F* hC214;
TH1F* hC210;
TH1F* hC206;

TF1* f;

enum STATE {gas, surface, buried};

void PoDepths(int nEvents = 100000000)
{
  TFile *f = new TFile("DepthTree.root","RECREATE");
  TTree *tree = new TTree("Tree","");
  tree->Branch("Time",&time.year,"year/I:month:day:hour:minute:second:millisecond:date:clock");

  TH1F* hD218 = new TH1F("D218", "Depth of Pb-218", 201, -10, 2000);
  TH1F* hD214 = new TH1F("D214", "Depth of Pb-214", 201, -10, 2000);
  TH1F* hD210 = new TH1F("D210", "Depth of Pb-210", 201, -10, 2000);
  TH1F* hD206 = new TH1F("D206", "Depth of Pb-206", 201, -10, 2000);

  TH1F* hE214 = new TH1F("E214", "E Frac of Pb-214 RPR", 200, -0.5, 1.5);
  TH1F* hE210 = new TH1F("E210", "E Frac of Pb-210 RPR", 200, -0.5, 1.5);
  TH1F* hE206 = new TH1F("E206", "E Frac of Pb-206 RPR", 200, -0.5, 1.5);

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

    //Rn-222 decay produces Po-218:

    if (tr->Rndm() < q218) { depth = 0; state = surface; }
    hD218->Fill(depth);

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