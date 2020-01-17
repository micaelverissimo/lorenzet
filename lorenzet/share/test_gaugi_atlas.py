



from prometheus import EventATLASLoop
from prometheus.enumerations import Dataframe as DataframeEnum
from Gaugi.messenger import LoggingLevel
from prometheus import ToolSvc, ToolMgr


datapath = '/home/asp-calo/CERN-DATA-jodafons/data/data17_13TeV/PhysVal_v2/EGAM1/after_ts1/user.jodafons.data17_13TeV.00329835.physics_Main.deriv.DAOD_EGAM1.f843_m1824_p3336.Physval.GRL_v97.r7000_GLOBAL'
datapath = '/afs/cern.ch/work/j/jodafons/public/data_samples/PhysVal/user.jodafons.data17_13TeV.00329835.physics_Main.deriv.DAOD_EGAM1.f843_m1824_p3336.Physval.GRL_v97.r7000_GLOBAL'
ToolMgr += EventATLASLoop(  "EventATLASLoop",
                            inputFiles = datapath, 
                            treePath = '*/HLT/Physval/Egamma/probes', 
                            nov = 1000,
                            dataframe = DataframeEnum.PhysVal_v2, 
                            outputFile = 'test_output.root',
                            level = LoggingLevel.DEBUG
                          )


from prometheus import Algorithm
ToolSvc += Algorithm( "AlgTest" )

from prometheus import job
job.initialize()
job.execute()
job.finalize()



