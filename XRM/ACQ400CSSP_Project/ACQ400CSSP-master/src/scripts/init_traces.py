import time
from java.util.logging import Logger
from org.csstudio.display.builder.runtime.script import PVUtil
from  org.csstudio.display.builder.model.properties import WidgetColor

""" Init Strip Plot from macros """

# Startup
logger = Logger.getLogger('init_traces')
widget = locals()['widget']
pvs = locals()['pvs']
display = widget.getTopDisplayModel()
timeout = 1 * 1000

colors = (
    (21, 21, 196),      # Blue
    (242, 26, 26),      # Red
    (33, 179, 33),      # Green
    (0, 0, 0),          # Black
    (128, 0, 255),      # Purple
    (255, 170, 0),      # Orange
    (255, 0, 240),      # Magenta
    (243, 132, 132),    # Coral
)

# Functions
class DotDict(dict):
    __delattr__ = dict.__delitem__
    __getattr__ = dict.__getitem__
    __setattr__ = dict.__setitem__

def get_macros(widget):
    macros = widget.getEffectiveMacros()
    return DotDict({key: macros.getValue(key) for key in macros.names})

def list_of_channels(chanstr):
    result = []
    for part in chanstr.split(','):
        if '-' in part:
            start, end = map(int, part.split('-'))
            result.extend(range(start, end + 1))
            continue
        result.append(int(part))
    return result

def read_pv(pvname):
    timeout = 1000
    try: PV = PVUtil.createPV(pvname, timeout)
    except: return False
    return PVUtil.getVType(PV).getValue()


# Main
macros = get_macros(widget)

UUT = macros.UUT                            #Target UUT
SITE = int(macros.SITE)                     #Target Site (-1 for all)
SITES = macros.SITES                        #Valid Sites
NSITES = len(SITES)                         #Total Sites
CHANS = list_of_channels(macros.CHANS)      #Target Channels
PV_FMT  = macros.PV_FMT                     #Format of trace pv
NAME_FMT = macros.NAME_FMT                  #Format of trace name
TITLE_FMT = macros.TITLE_FMT                #Scope title fmt
MAX_TRACES = int(macros.MAX_TRACES)         #Max traces

chanmap = {SITE: CHANS}
if SITE < 0:
    macros.SITE = ""
    current = 0
    chanmap = {}
    for site_idx in range(1, NSITES + 1):
        if not len(CHANS) > 0: break
        nchan_pv = "{}:{}:NCHAN".format(UUT, site_idx)
        nchan = current + read_pv(nchan_pv)
        chanmap[site_idx] = []
        while len(CHANS) > 0:
            if CHANS[0] > nchan: break
            chanmap[site_idx].append(CHANS.pop(0) - current)
        current = nchan

widget.propTitle().setValue(TITLE_FMT.format(**macros))

logger.info("Chanmap: {}".format(repr(chanmap)))
current = 0
for site in chanmap:
    for chan in chanmap[site]:
        if current >= MAX_TRACES: break
        macros.site = site
        macros.chan = chan
        macros.index = int(chan) - 1
        trace_pv = PV_FMT.format(**macros)
        trace_name = NAME_FMT.format(**macros)
        trace_color = WidgetColor(*colors[current])

        logger.info("Trace[{}] Site[{}] CH[{}] PV {}".format(current, site, chan, trace_pv, current))
        widget.setPropertyValue("traces[{}].color".format(current), trace_color)
        widget.setPropertyValue("traces[{}].name".format(current), trace_name)
        widget.setPropertyValue("traces[{}].y_pv".format(current), trace_pv)

        current += 1

