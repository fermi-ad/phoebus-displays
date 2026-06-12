from java.util.logging import Logger
from org.csstudio.display.builder.runtime.script import PVUtil, ScriptUtil

from pprint import pprint

""" Builds and loads new scope"""

# Startup
logger = Logger.getLogger('init_scope')
widget = locals()['widget']
display = widget.getTopDisplayModel()
timeout = 2000

#Functions
def get_macros(*widgets):
    class DotDict(dict):
        __getattr__ = dict.__getitem__
        __setattr__ = dict.__setitem__
    macros = DotDict()
    def macro_func(key, value): macros[key] = value
    for widget in widgets:
        widget.getEffectiveMacros().forEachSpec(macro_func)
    return macros

def string_to_channels(chanstr):
    result = []
    for part in chanstr.split(','):
        if '-' in part:
            start, end = map(int, part.split('-'))
            result.extend(range(start, end + 1))
            continue
        result.append(int(part))
    return result

def read_pv(pvname):
    try: pv = PVUtil.createPV(pvname, timeout)
    except: return False
    value = PVUtil.getVType(pv).getValue()
    PVUtil.releasePV(pv)
    return value

def logical_to_physical(uut, site, chans, cache={}):
    """Convert logical (chan 33) to physical (site 2 chan 1) """
    if site != 0: return [(uut, site, chan) for chan in chans]
    cache.setdefault(uut, {"nchan": [], "sites": []})

    if len(cache[uut]["sites"]) == 0:
        sitelist = read_pv("{}:SITELIST".format(uut))
        cache[uut]["sites"] = [int(site.split('=')[0]) for site in sitelist.split(',', 1)[1].split(',')]

    physical_chans = []
    for chan in chans:
        prev_chans = 0
        for idx, site in enumerate(cache[uut]["sites"]):

            if idx >= len(cache[uut]["nchan"]):
                nchan = read_pv("{}:{}:NCHAN".format(uut, site))
                cache[uut]["nchan"].append(nchan if nchan else 0)

            nchan = cache[uut]["nchan"][idx]
            if nchan == 0: continue
            max_chan = nchan + prev_chans
            if chan <= max_chan:
                physical_chans.append((uut, site, chan - prev_chans))
                break
            prev_chans = max_chan
    return physical_chans


def format_channels(chans):
    chans = sorted(chans)
    result = []
    start = prev = chans[0]
    for chan in chans[1:]:
        if chan == prev + 1:
            prev = chan
        else:
            result.append("{}-{}".format(start, prev) if start != prev else str(start))
            start = prev = chan
    result.append("{}-{}".format(start, prev) if start != prev else str(start))
    return ",".join(result)

# Main

macros = get_macros(display, widget)

SCOPE_MAP = [item.strip() for item in macros.SCOPE_MAP.split('/') if len(item.strip()) > 0]
SCOPE_MODE = macros.SCOPE_MODE #LIVE, POST, STRIP
SCOPE_XTYPE = macros.SCOPE_XTYPE #SAMPLES, TIME
SCOPE_YTYPE = macros.SCOPE_YTYPE #CODES, VOLTS

#Default Formats
XTRACE_FMT =""
YTRACE_FMT = "{uut}:{site}:AI:{mode}:{chan:02d}"
NAME_FMT = YTRACE_FMT
TAB_FMT = "SCOPE({SCOPE_MODE}) {chan_str}"
TITLE_FMT = "SCOPE({SCOPE_MODE}) {chan_str}"

XTRACE_FMT = macros.get('XTRACE_FMT_{}'.format(SCOPE_XTYPE), XTRACE_FMT)
YTRACE_FMT = macros.get('YTRACE_FMT_{}'.format(SCOPE_YTYPE), YTRACE_FMT)
NAME_FMT = macros.get('NAME_FMT', YTRACE_FMT)
TAB_FMT = macros.get('TAB_FMT', TAB_FMT)
TITLE_FMT = macros.get('TITLE_FMT', TITLE_FMT)

MAX_TRACES = macros.get('MAX_TRACES', 8)
OPI = macros.get('{}_OPI'.format(SCOPE_MODE), None)
UUT = macros.get('UUT', None)
SITE = int(macros.get('SITE', 1))
FIRST_SITE = 0
CROSS_SITE = int(macros.get('CROSS_SITE', 0))
SIZE = macros.get('SCOPE_SIZE', None)
macros.mode = macros.get('MODE_{}'.format(SCOPE_MODE), 'WF')

if SCOPE_MODE == 'STRIP': #Force strip chart to use volts. TODO fix
    SCOPE_YTYPE = "VOLTS"
    YTRACE_FMT = "{uut}:{site}:AI:{mode}:{chan:02d}"


trace = 0
new_macros = {}
active_channels = {}
cache = {}

for i in range(MAX_TRACES):
    new_macros['TRACE_{}_YPV'.format(i)] = ""
    new_macros['TRACE_{}_XPV'.format(i)] = ""
    new_macros['TRACE_{}_NAME'.format(i)] = ""

for uutchans in SCOPE_MAP:
    uutchans = uutchans.split(':')
    if len(uutchans) == 2:
        if uutchans[0].upper() == 'NONE': continue
        uut = uutchans[0]
        chans = uutchans[1]
    else:
        uut = UUT
        chans = uutchans[0]

    chans = string_to_channels(chans)

    site = 0 if CROSS_SITE else SITE
    chans = logical_to_physical(uut, site, chans, cache)

    for uut, site, chan in chans:
        if trace >= MAX_TRACES: break

        if FIRST_SITE == 0: FIRST_SITE = site
        macros.uut = uut
        macros.site = site
        macros.chan = chan
        macros.index = chan - 1

        macro_value = NAME_FMT.format(**macros)
        macro_key = 'TRACE_{}_NAME'.format(trace)
        logger.info("{} = {}".format(macro_key, macro_value))
        new_macros[macro_key] = macro_value

        macro_value = XTRACE_FMT.format(**macros)
        macro_key = 'TRACE_{}_XPV'.format(trace)
        logger.info("{} = {}".format(macro_key, macro_value))
        new_macros[macro_key] = macro_value

        macro_value = YTRACE_FMT.format(**macros)
        macro_key = 'TRACE_{}_YPV'.format(trace)
        logger.info("{} = {}".format(macro_key, macro_value))
        new_macros[macro_key] = macro_value

        active_channels.setdefault(uut, {})
        active_channels[uut].setdefault(site, [])
        active_channels[uut][site].append(chan)

        trace += 1

chan_str = []
for uut, sitechans in active_channels.items():
    for site, chans in sitechans.items():
        uut_num = uut[-3::]
        chans = format_channels(chans)
        chan_str.append("{}:{}:({})".format(uut_num, site, chans))
chan_str = ' '.join(chan_str)
macros.chan_str = chan_str

new_macros['SCOPE_TITLE'] = TITLE_FMT.format(**macros)
new_macros['SCOPE_TAB'] = TITLE_FMT.format(**macros)
new_macros['XAXIS_LABEL'] = macros.get('XAXIS_{}_LABEL'.format(SCOPE_XTYPE), SCOPE_XTYPE)
new_macros['YAXIS_LABEL'] = macros.get('YAXIS_{}_LABEL'.format(SCOPE_YTYPE), SCOPE_YTYPE)
new_macros['UUT'] = active_channels.keys()[0]
new_macros['SITE'] = max(FIRST_SITE, 1)
new_macros['WIDTH'] = macros.get('WIDTH_{}'.format(SIZE), 800)
new_macros['HEIGHT'] = macros.get('HEIGHT_{}'.format(SIZE), 600)
new_macros['TRACE_WIDTH'] = macros.get('TRACE_WIDTH', 1)

new_macros = dict((k, str(v)) for k, v in new_macros.items())

if OPI: 
    ScriptUtil.openDisplay(display, OPI, "TAB", new_macros)
else:
    logger.severe("{} OPI not found".format(OPI))
