from java.util.logging import Logger
from org.csstudio.display.builder.runtime.script import PVUtil


"""Combines all trace pv values into one pv"""

# Startup
logger = Logger.getLogger('handle_traces')
pvs = locals()['pvs']


chanmap = []
chanmappv = pvs[0]

traces = list(zip(pvs[1::2], pvs[2::2]))
for uutpv, chanpv in traces:
    uut = PVUtil.getString(uutpv)
    if uut.upper() == 'NONE': continue
    chan = PVUtil.getInt(chanpv)
    chanmap.append("{}:{}".format(uut, chan))

chanmap = '/ '.join(chanmap)
logger.info("CHANMAP: {}".format(chanmap))
chanmappv.write(chanmap)