__all__ = ['ShowerShapes']

from Gaugi  import StatusCode, EnumStringification
from Gaugi import EDM
import numpy as np
import sys
from copy import copy

class ShowerShapes(EDM):

  __eventBranches = [ 
                    ]
                      
  def __init__(self):
    EDM.__init__(self)


  def initialize(self):
    return StatusCode.SUCCESS



  def execute(self):

    from copy import deepcopy
    from EventSimulator import CaloGAN_Definitions as Layer
    # reconstruction step, get the cell container
    roi = self.getContext().getHandler("CaloCellsContainer")
    layers = [ 
                # Eletromagnetic cells
                roi.getCollection( Layer.FIRST_EM_LAYER  ),
                roi.getCollection( Layer.SECOND_EM_LAYER ),
                roi.getCollection( Layer.THIRD_EM_LAYER  ),
                # Hadronic cells
                roi.getCollection( Layer.THIRD_HAD_LAYER  ),
                roi.getCollection( Layer.THIRD_HAD_LAYER  ),
                roi.getCollection( Layer.THIRD_HAD_LAYER  ),

             ] 

    # Shower shape: Eratio
    # Eratio = (E1max - E2max)/(E1max + E2max) where E1max and E2max is the first and second cell in
    # the first EM (strips) layer with most energy
    def eratio( cells ):
      energies = deepcopy(cells)
      E1max, index = self._maxCell( energies ); energies.pop(index)
      E2max, _  = self._maxCell( energies )
      return (E1max.energy()-E2max.energy()) / float(E1max.energy()+E2max.energy()) if (E1max.energy()+E1max.energy())>0.0 else 0.0

    self._eratio1 = eratio( layers[0] )
    self._eratio2 = eratio( layers[1] )
    self._eratio3 = eratio( layers[2] )



    # Shower shape: Reta
    # The second EM layers is 12 X 12 cells. To calculate the Reta we must do:
    # Reta = E_3X7 / E_7X7 where E_aXb is the sum energy in this region
    E_max, _ = self._maxCell( layers[1] );
    E_3X7 = self._sumCells( layers[1], E_max.eta() - 1, E_max.eta() + 1, E_max.phi() - 3, E_max.phi() + 3) 
    E_7X7 = self._sumCells( layers[1], E_max.eta() - 3, E_max.eta() + 3, E_max.phi() - 3, E_max.phi() + 3) 
    self._reta = E_3X7 / float(E_7X7) if E_7X7>0.0 else 0.0

    # Shower shape: Rphi
    E_3X3 = self._sumCells( layers[1], E_max.eta() - 1, E_max.eta() + 1, E_max.phi() - 1, E_max.phi() + 1) 
    self._rphi = E_3X3 / float(E_3X7) if E_3X7>0.0 else 0.0


    # E total
    E1 = self._sumCells( layers[0] )
    E2 = self._sumCells( layers[1] )
    E3 = self._sumCells( layers[2] )

    # energy layers
    self._etot = E1+E2+E3
    self._e1 = E1
    self._e2 = E2
    self._e3 = E3
    self._f1 = E1/float(self._etot)
    self._f2 = E2/float(self._etot)
    self._f3 = E3/float(self._etot)


    # Hadronic energy layers
    self._ehad1 = self._sumCells( layers[3] )
    self._ehad2 = self._sumCells( layers[4] )
    self._ehad3 = self._sumCells( layers[5] )
    self._ehadtot = self._ehad1 + self._ehad2 + self._ehad3
    self._rhad = self._etot/float(self._ehadtot)


    return StatusCode.SUCCESS

  # Eletromagnetic information

  def eratio1(self):
    return self._eratio1

  def eratio2(self):
    return self._eratio2

  def eratio3(self):
    return self._eratio3

  def e1(self):
    return self._e1

  def e2(self):
    return self._e2

  def e3(self):
    return self._e3

  def etot(self):
    return self._etot

  def f1(self):
    return self._f1

  def f2(self):
    return self._f1

  def f3(self):
    return self._f3

  def reta(self):
    return self._reta

  def rphi(self):
    return self._rphi

  # Hadronic information

  def rhad(self):
    return self._rhad

  def e1had(self):
    return self._e1had

  def e2had(self):
    return self._e2had

  def e3had(self):
    return self._e3had



  def _maxCell( self, containers ):
    energy=-999; max_cell_object=None; index=None
    for idx, c in enumerate(containers):
      if c.energy() > energy:
        max_cell_object=copy(c); energy=c.energy(); index=idx
    return max_cell_object, index

 
  def _sumCells( self, containers, etamin=-999, etamax=999, 
                                   phimin=-999, phimax=999):
    energy=0.0
    for c in containers:
      if c.eta() >= etamin and c.eta() <= etamax:
        if c.phi() >= phimin and c.phi() <= phimax:
          energy += c.energy()
    return energy

 




