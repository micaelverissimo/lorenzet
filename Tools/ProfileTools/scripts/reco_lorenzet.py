



from prometheus import EventSimulatorLoop
from prometheus.enumerations import Dataframe as DataframeEnum
from Gaugi.messenger import LoggingLevel
from prometheus import ToolSvc, ToolMgr


datapath = '/eos/user/j/jodafons/CERN-DATA/data/g4_simulation/g4sim_lorez.200k_events.3to150GeV/pi+.root'
#outputfile = 'collection.e+.root'
outputfile = 'collection.pi+.root'
#basepath = 'positron'

ToolMgr += EventSimulatorLoop(  "EventLorenzetLoop",
                                inputFiles = datapath, 
                                treePath = 'fancy_tree', 
                                nov = -1,
                                dataframe = DataframeEnum.Lorenzet, 
                                outputFile = outputfile,
                                level = LoggingLevel.INFO
                              )


from LorenzetTools import *
#ToolSvc += CaloView( "CaloView" , basepath = basepath)
#ToolSvc += RingProfiles( "RingerShape" , basepath = basepath)
#ToolSvc += StandardQuantityProfiles( "ShowerShapes" , basepath = basepath)
ToolSvc += Collector( "Collector")




from prometheus import job
job.initialize()
job.execute()
job.finalize()


