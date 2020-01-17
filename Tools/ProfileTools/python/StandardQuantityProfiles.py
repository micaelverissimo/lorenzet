
__all__ = ['StandardQuantityProfiles']


from Gaugi import Algorithm
from Gaugi.messenger.macros import *
from Gaugi import StatusCode, retrieve_kw
import numpy as np

class StandardQuantityProfiles( Algorithm ):


  def __init__( self, name, **kw ):
    Algorithm.__init__( self, name )
    self._basepath = retrieve_kw( kw, 'basepath', '')


  def initialize(self):

    Algorithm.initialize(self)
    sg = self.getStoreGateSvc()
    
    from ROOT import TH1F, TH2F

    sg.mkdir( self._basepath+"/ShowerShapes" )
    sg.addHistogram( TH1F("reta", ";;;", 100, 0, 1) )
    sg.addHistogram( TH1F("rphi", ";;;", 100, 0, 1) )
    sg.addHistogram( TH1F("eratio1", ";;;", 100, 0, 1) )
    sg.addHistogram( TH1F("eratio2", ";;;", 100, 0, 1) )
    sg.addHistogram( TH1F("eratio3", ";;;", 100, 0, 1) )
    sg.addHistogram( TH1F("f1", ";;;", 100, 0, 1) )
    sg.addHistogram( TH1F("f2", ";;;", 100, 0, 1) )
    sg.addHistogram( TH1F("f3", ";;;", 100, 0, 1) )
    sg.addHistogram( TH1F("rhad", ";;;", 100, 0, 2) )

    self.init_lock()
    return StatusCode.SUCCESS 
 
  def execute(self, context):
   
    from EventSimulator import CaloGAN_Definitions
    sg = self.getStoreGateSvc()
    showers = self.getContext().getHandler( "ShowerShapesContainer" )

    if showers.reta()    > 0.0: sg.histogram( self._basepath+"/ShowerShapes/reta" ).Fill( showers.reta() )
    if showers.rphi()    > 0.0: sg.histogram( self._basepath+"/ShowerShapes/rphi" ).Fill( showers.rphi() )
    if showers.eratio1() > 0.0: sg.histogram( self._basepath+"/ShowerShapes/eratio1" ).Fill( showers.eratio1() )
    if showers.eratio2() > 0.0: sg.histogram( self._basepath+"/ShowerShapes/eratio2" ).Fill( showers.eratio2() )
    if showers.eratio3() > 0.0: sg.histogram( self._basepath+"/ShowerShapes/eratio3" ).Fill( showers.eratio3() )
    if showers.f1()      > 0.0: sg.histogram( self._basepath+"/ShowerShapes/f1" ).Fill( showers.f1() )
    if showers.f2()      > 0.0: sg.histogram( self._basepath+"/ShowerShapes/f2" ).Fill( showers.f2() )
    if showers.f3()      > 0.0: sg.histogram( self._basepath+"/ShowerShapes/f3" ).Fill( showers.f3() )
    if showers.rhad()    > 0.0: sg.histogram( self._basepath+"/ShowerShapes/rhad" ).Fill( showers.rhad() )
  

    return StatusCode.SUCCESS 




  def finalize(self):

    self.fina_lock()
    return StatusCode.SUCCESS 




