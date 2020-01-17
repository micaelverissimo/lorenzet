


from monet.PlotFunctions import *
from monet.TAxisFunctions import *
from monet.utilities import *
 


def PlotShowers(  hist_1, hist_2, name_1, name_2, varname, outname ,drawopt='hist', doLogY=True, ylabel='Count'):
  
  from ROOT import TCanvas, kBlack, kAzure
  #h1 = NormHist(h1, 1/h1.GetMaximum())
  #h2 = NormHist(h2, 1/h2.GetMaximum())
  canvas = TCanvas( 'canvas', "", 500, 500 ) 
  if doLogY: canvas.SetLogy()


  hist_1.SetLineColor(kAzure-5)
  hist_1.SetFillColor(kAzure-4)
  hist_1.SetMarkerColor(kAzure-4)
  hist_2.SetLineColor(kBlack)
  AddHistogram(canvas,hist_1,drawopt=drawopt) 
  AddHistogram(canvas,hist_2,drawopt=drawopt) 
  #h1.SetStats(False)
  #h2.SetStats(False)
  MakeLegend(canvas,.5,.77,.89,.97,option='p',textsize=14, names=[name_1,name_2], ncolumns=1, 
             squarebox=False, doFixLength=False)
  AutoFixAxes(canvas,ignoreErrors=False)
  FormatCanvasAxes(canvas, XLabelSize=18, YLabelSize=18, XTitleOffset=0.87, YTitleOffset=1.5)
  SetAxisLabels(canvas,varname,ylabel)
  FixYaxisRanges(canvas, ignoreErrors=True, yminc=-eps )
  canvas.SaveAs(outname)





def PlotDeposit( hist, outname ):

  from ROOT import TCanvas, gStyle, kBird,TLine
  gStyle.SetPalette(kBird)

  canvas = TCanvas('canvas','canvas',800, 500)
  canvas.SetRightMargin(0.15)
  hist.GetXaxis().SetTitle("Depth from Calorimeter Center [mm]")
  hist.GetYaxis().SetTitle("#eta direction [mm]")
  hist.GetZaxis().SetTitle("Local Energy Deposit [MeV]")
  hist.Draw( 'colz' )

  # taken from: https://github.com/jodafons/lorenzet/blob/master/src/SteppingAction.cc
  vertical_lines = [ (150,0, 150,480) ,  ( 450,0, 450,480 ), (480,0, 480,480), (880,0,880,480), (1280,0,1280,480) ]
  first_em_layer = []; second_em_layer = []; third_em_layer = []
  first_had_layer = []; second_had_layer = []; third_had_layer = []
  
  # horizontal lines
  for y in range( 0, 480, int(480/96) ):
    # (x1,y,x2,y)
    first_em_layer.append( (0, y, 150, y) )
  for y in range( 0, 480, int(480/12) ):
    # (x1,y,x2,y)
    second_em_layer.append( (150, y, 450, y) )
  for y in range( 0, 480, int(480/6) ):
    # (x1,y,x2,y)
    third_em_layer.append( (450, y, 480, y) )

  # horizontal lines
  for y in range( 0, 480, int(480/8) ):
    # (x1,y,x2,y)
    first_had_layer.append( (480, y, 880, y) )
  for y in range( 0, 480, int(480/8) ):
    # (x1,y,x2,y)
    second_had_layer.append( (880, y, 1280, y) )
  for y in range( 0, 480, int(480/4) ):
    # (x1,y,x2,y)
    third_had_layer.append( (1280, y, 1480, y) )

  objects = []
  for l in vertical_lines+first_em_layer+second_em_layer+third_em_layer+first_had_layer+second_had_layer+third_had_layer:
    obj = TLine(l[0],l[1],l[2],l[3])
    obj.Draw('sames')
    objects.append(obj)

  #hist.GetXaxis().SetLimits(0,480)
  #hist.GetYaxis().SetLimits(0,480)
  canvas.SaveAs(outname)




def PlotCells( hist, outname, zlabel = "Energy Deposit Average [MeV]" ):

  from ROOT import TCanvas, gStyle, kBird,TLine
  gStyle.SetPalette(kBird)
  canvas = TCanvas('canvas','canvas',500, 500)
  canvas.SetRightMargin(0.15)
  hist.GetXaxis().SetTitle("#phi Cell")
  hist.GetYaxis().SetTitle("#eta Cell")
  hist.GetZaxis().SetTitle(zlabel)
  hist.Draw( 'colz' )
  canvas.SaveAs(outname)

  




