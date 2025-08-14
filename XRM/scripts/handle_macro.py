import time
from java.util.logging import Logger
from org.csstudio.display.builder.runtime.script import PVUtil

""" Update Macro when changed """

# Startup
logger = Logger.getLogger('handle_macro')
widget = locals()['widget']
pvs = locals()['pvs']
display = widget.getTopDisplayModel()
t0 = time.time()

def is_stale(pv):
    timestamp = float(PVUtil.getTimeInMilliseconds(pv)) / 1000
    return t0 - timestamp > 1

for pv in pvs:
    #if is_stale(pv): continue # fix me
    pvname = pv.getName().split('://')[-1]
    pvvalue = PVUtil.getString(pv)
    display.getEffectiveMacros().add(pvname, str(pvvalue))
    logger.info("Updated {} macro to {}".format(pvname, pvvalue))
