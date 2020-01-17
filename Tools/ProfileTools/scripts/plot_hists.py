



f = 'hists.root'


from SimulatorTools.drawers import *
from Gaugi.storage import  restoreStoreGate
#store1 =  restoreStoreGate( f1 )
sg =  restoreStoreGate( f )

from monet.AtlasStyle import SetAtlasStyle
SetAtlasStyle()



PlotCells( sg.histogram("electron/Cells/layer_0_2D"), "cells_electron_first_layer.pdf")
PlotCells( sg.histogram("electron/Cells/layer_1_2D"), "cells_electron_second_layer.pdf")
PlotCells( sg.histogram("electron/Cells/layer_2_2D"), "cells_electron_third_layer.pdf")


PlotCells( sg.histogram("piminus/Cells/layer_0_2D"), "cells_piminus_first_layer.pdf")
PlotCells( sg.histogram("piminus/Cells/layer_1_2D"), "cells_piminus_second_layer.pdf")
PlotCells( sg.histogram("piminus/Cells/layer_2_2D"), "cells_piminus_third_layer.pdf")




PlotShowers( sg.histogram("electron/ShowerShapes/eratio"), sg.histogram("piminus/ShowerShapes/eratio"), 'e^{-}','#pi^{-}','E_{ratio}_{EM1}','eratio_em1.pdf')
PlotShowers( sg.histogram("electron/ShowerShapes/reta"), sg.histogram("piminus/ShowerShapes/reta"), 'e^{-}','#pi^{-}','R_{#eta}','reta.pdf')
PlotShowers( sg.histogram("electron/ShowerShapes/rphi"), sg.histogram("piminus/ShowerShapes/rphi"), 'e^{-}','#pi^{-}','R_{#phi}','rphi.pdf')
PlotShowers( sg.histogram("electron/ShowerShapes/f1"), sg.histogram("piminus/ShowerShapes/f1"), 'e^{-}','#pi^{-}','f_{1}','f1.pdf')

PlotShowers( sg.histogram("electron/Rings/ring_profile"), sg.histogram("piminus/Rings/ring_profile"), 'e^{-}','#pi^{-}','Ring#','rings.pdf',ylabel='Energy Average [MeV]')
PlotShowers( sg.histogram("electron/Rings/ring_profile"), sg.histogram("piminus/Rings/ring_profile"), 'e^{-}','#pi^{-}','Ring#','rings_nolog.pdf', 
    doLogY=False,ylabel='Energy Average [MeV]')




