
__all__ = ['RingProfiles']


from Gaugi import Algorithm
from Gaugi.messenger.macros import *
from Gaugi import StatusCode, retrieve_kw
import numpy as np

class RingProfiles( Algorithm ):


  def __init__( self, name, **kw ):
    Algorithm.__init__( self, name )
    self._basepath = retrieve_kw( kw, 'basepath' , '')


  def initialize(self):

    Algorithm.initialize(self)
    sg = self.getStoreGateSvc()
    
    from ROOT import TH1F, TH2F
    sg.mkdir(self._basepath+'/Rings')
    for ring in range(66):
      sg.addHistogram( TH1F( ("ring_%d")%(ring), ";Energy;Count", 500, 0, 1000 ) ) 
    sg.addHistogram( TH2F( ("rings"), ";Energy;Count", 66,0,66,500, 0, 1000 ) ) 
    sg.addHistogram( TH1F( "ring_profile", ";Energy;Count", 66, 0, 66 ) ) 
    self.init_lock()
    
    return StatusCode.SUCCESS 
 
  def execute(self, context):
   
    sg = self.getStoreGateSvc()
    rings = self.getContext().getHandler("CaloRingsContainer").ringsE()
    for r, energy in enumerate(rings):
      sg.histogram(self._basepath+'/Rings/ring_%d' % r).Fill(energy)
      sg.histogram(self._basepath+'/Rings/rings' ).Fill(r,energy)

    return StatusCode.SUCCESS 




  def finalize(self):

    sg = self.getStoreGateSvc()
    for r in range(66):
      mean_energy = sg.histogram(self._basepath+'/Rings/ring_%d' % r).GetMean()
      sg.histogram(self._basepath+'/Rings/ring_profile').SetBinContent( r+1, mean_energy )
    self.fina_lock()
    return StatusCode.SUCCESS 



