
__all__ = ['ResolutionTool']

from Gaugi import Algorithm
from Gaugi.messenger.macros import *
from Gaugi import StatusCode, retrieve_kw
import numpy as np

class ResolutionTool( Algorithm ):


  def __init__( self, name, **kw ):
    Algorithm.__init__( self, name )
    self._basepath = retrieve_kw( kw, 'basepath' , 'particle')
    self._energy_bins = retrieve_kw( kw, 'energy_bins' , [])


  def initialize(self):

    Algorithm.initialize(self)
    sg = self.getStoreGateSvc()


    from ROOT import TGraph, TH1F
    sg.mkdir(self._basepath+'/Resolution')

    for energy in self._energy_bins:
      sg.addHistogram( TH1F( "total_energy_"+str(energy)+"GeV", ";Energy;Count", 100, energy-int(np.sqrt(energy))-1, energy+int(np.sqrt(energy))+1 ) ) 
    self.init_lock()
    
    return StatusCode.SUCCESS 
 


  def execute(self, context):
   
    sg = self.getStoreGateSvc()

    # E total
    from copy import deepcopy
    from EventSimulator import CaloGAN_Definitions as Layer
    # reconstruction step, get the cell container
    roi = self.getContext().getHandler("CaloCellsContainer")
    layers = [ 
                roi.getCollection( Layer.FIRST_LAYER  ),
                roi.getCollection( Layer.SECOND_LAYER ),
                roi.getCollection( Layer.THIRD_LAYER  ),
             ] 

    E1 = self.__sumCells( layers[0] )
    E2 = self.__sumCells( layers[1] )
    E3 = self.__sumCells( layers[2] )

    E_total = (E1+E2+E3)/1000
    
    xbin = self.__findBin( roi.totalEnergy() )
    if xbin:
      sg.histogram(self._basepath+"/Resolution/total_energy_"+str(self._energy_bins[xbin])+"GeV" ).Fill(E_total)

    return StatusCode.SUCCESS 




  def finalize(self):
    self.fina_lock()
    return StatusCode.SUCCESS 



  def __findBin( self, energy ):
    # Let's considere a monocromatic energy spectrum +-1GeV
    for xbin, e in enumerate(self._energy_bins):
      if energy >= e-0.2 and energy <= e+0.2:
        return xbin
    return None


 
  def __sumCells( self, containers):
    energy=0.0
    for c in containers:
      energy += c.energy()
    return energy





  def plot( self, sg, outname ):
 
    x = []
    y = [] 
    for energy in self._energy_bins:
      print(energy)
      x.append(float(energy))
      #y.append(sg.histogram(self._basepath+"/Resolution/total_energy_"+str(energy)+"GeV" ).GetStdDev()/ np.sqrt( float(energy)) )
      y.append(sg.histogram(self._basepath+"/Resolution/total_energy_"+str(energy)+"GeV" ).GetStdDev()/ float(energy) )
    print(y)
    
    #for i in range(100):
    #  x_.append(i+1); y.append( 1/np.sqrt(i+1)


    from ROOT import TGraph,TCanvas
    from monet.AtlasStyle import SetAtlasStyle
    SetAtlasStyle()
    canvas = TCanvas( 'canvas', "", 500, 500 ) 
    g = TGraph( len(x),np.array(x),np.array(y) )
    g.GetXaxis().SetTitle( 'Energy [GeV]' )
    g.GetYaxis().SetTitle( '#sigma/Energy' )
    g.SetMarkerStyle( 20 )
    g.SetMarkerColor( 1 )
    g.SetMarkerSize( 2 )
    g.Draw( 'AP' )
    canvas.SaveAs("reso.pdf") 



