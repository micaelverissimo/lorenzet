


__all__ = ["CollectorTool"]

from Gaugi import Algorithm
from Gaugi import StatusCode, NotSet, retrieve_kw
from Gaugi.messenger.macros import *
import numpy as np




class CollectorTool( Algorithm ):

  def __init__(self, name, **kw):
    Algorithm.__init__(self, name)
    self._outputname  =retrieve_kw( kw , 'outputname', 'collection.pic')
    self._event = []
    self._event_label = []

  def initialize(self):
  
    self._event_label.extend( [ 'ring_%d'%r for r in range(56) ] )
    self._event_label.extend( [ 'eratio1',
                                'eratio2',
                                'eratio3',
                                'reta',
                                'rphi',
                                'f1',
                                'f2',
                                'f3',
                                'e1',
                                'e2',
                                'e3',
                                'etot',
                                'cells_layer1',
                                'cells_layer2',
                                'cells_layer3',
                                ] )


    return StatusCode.SUCCESS

  def fill( self, event ):
    self._event.append(event)

  def execute(self, context):
    
    cells = self.getContext().getHandler("CaloCellsContainer")
    showers = self.getContext().getHandler( "ShowerShapesContainer" )
    rings = self.getContext().getHandler("CaloRingsContainer").ringsE()
    event_row = list() 
    event_row.extend( rings )
    event_row.append( showers.eratio1() )
    event_row.append( showers.eratio2() )
    event_row.append( showers.eratio3() )
    event_row.append( showers.reta() )
    event_row.append( showers.rphi() )
    event_row.append( showers.f1() )
    event_row.append( showers.f2() )
    event_row.append( showers.f3() )
    event_row.append( showers.e1() )
    event_row.append( showers.e2() )
    event_row.append( showers.e3() )
    event_row.append( showers.etot() )

    
    # Get cells and reshape to square matrix but preserve 
    # the calo layer resolution and dimensions
    from utilities import reshape_to_square_array
    from EventSimulator import CaloGAN_Definitions as Layer
    c1 = cells.get_raw_cells( Layer.FIRST_LAYER  ) 
    c2 = cells.get_raw_cells( Layer.SECOND_LAYER ) 
    c3 = cells.get_raw_cells( Layer.THIRD_LAYER  ) 
    event_row.append( c1 )
    event_row.append( c2 )
    event_row.append( c3 )
    
    # Fill the event
    self.fill(event_row)


    return StatusCode.SUCCESS
  

  def finalize( self ):

    d = { "labels"  : self._event_label ,
          "events"  : np.array( self._event )
          }

    import pickle
    f = open( self._outputname, 'w' )
    pickle.dump(d, f)
    f.close()
    return StatusCode.SUCCESS



  

