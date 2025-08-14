import time
from java.util.logging import Logger
from  org.csstudio.display.builder.model.properties import WidgetColor

from org.csstudio.display.builder.runtime.script import PVUtil

""" Init XY Plot from macros """

# Startup
logger = Logger.getLogger('init_xy_plot')
widget = locals()['widget']
pvs = locals()['pvs']
display = widget.getTopDisplayModel()

max_traces = 8
timestamp = int(time.time())
trace_pv_fmt = "{uut}:{site}:AI:{pmode}:{chan:02d}{dtype}"

pmodes = {
    'LIVE': 'WF',
    'POST': 'TW',
}
ptypes = {
    'RAW': '',
    'VOLTS': ':V.VALA',
}

colors = (
    (21, 21, 196), # Blue
    (242, 26, 26), # Red
    (33, 179, 33), # Green
    (0, 0, 0), # Black
    (128, 0, 255), # Purple
    (255, 170, 0), # Orange
    (255, 0, 240), #Magenta
    (243, 132, 132), #Coral
)

# Functions
def list_of_channels(chanstr):
    chans = []
    for chan in chanstr.split(','):
        if '-' in chan:
            chan = list(map(int, chan.split('-')))
            chans.extend(list(range(chan[0], chan[1] + 1)))
            continue
        chans.append(int(chan))
    return chans

def get_pv_value(pvname):
    timeout = 1000
    try: PV = PVUtil.createPV(pvname, timeout)
    except:
        return False
    return PVUtil.getVType(PV).getValue()

# Main
uut = widget.getEffectiveMacros().getValue('UUT')
site = widget.getEffectiveMacros().getValue('SITE')

pchans = widget.getEffectiveMacros().getValue('pchans')
pmode = widget.getEffectiveMacros().getValue('pmode')
ptype = widget.getEffectiveMacros().getValue('ptype')
ptarget = widget.getEffectiveMacros().getValue('ptarget')

chans = list_of_channels(pchans)
chanmap = {site: chans}

if ptarget == 'ALL':
    current = 0
    chanmap = {}

    for site_i in [1, 2, 3, 4, 5, 6]:
        pvname = "{}:{}:NCHAN".format(uut, site_i)
        nchan = get_pv_value(pvname) + current
        while len(chans) > 0:
            if chans[0] > nchan: break
            chanmap.setdefault(site_i, [])
            chanmap[site_i].append(chans.pop(0) - current)
        current = nchan

env = {
    'uut': uut,
    'dtype': ptypes.get(ptype,''),
    'pmode': pmodes.get(pmode, 'WF'),
}

current = 0
for site_i in chanmap:
    for chan in chanmap[site_i]:
        if current >= max_traces: break

        env['chan'] = chan
        env['site'] = site_i
        trace_pv = trace_pv_fmt.format(**env)
        color = WidgetColor(*colors[current])

        logger.info("CH{} Trace Pv {}".format(chan, trace_pv))
        widget.setPropertyValue("traces[{}].color".format(current), color)
        widget.setPropertyValue("traces[{}].name".format(current), trace_pv)
        widget.setPropertyValue("traces[{}].y_pv".format(current), trace_pv)
        current += 1

widget.propTitle().setValue("{} Site {} CH{} {} {}".format(uut, ptarget, pchans, pmode, ptype))
widget.setPropertyValue("y_axes[0].title", 'Volts (V)' if ptype == 'VOLTS' else 'Codes')