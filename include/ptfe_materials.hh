#ifndef ptfe_materials_h
#define ptfe_materials_h 1

class G4Material;

class ptfe_materials
{
  public:
    ptfe_materials();
    virtual ~ptfe_materials();

    virtual G4Material* MakeTeflon();
    virtual G4Material* MakeLiquidXe();
    virtual G4Material* MakeCopper();
};

#endif