import time
from java.util.logging import Logger
from org.phoebus.framework.macros import Macros
from org.csstudio.display.builder.runtime.script import PVUtil

""" Init macros and local pvs """

# Startup
t0 = time.time()
logger = Logger.getLogger('init_main')
widget = locals()['widget']
pvs = locals()['pvs']
display = widget.getTopDisplayModel()
timeout = 1

# Functions
def get_pv_value(pvname):
    timeout = 1000
    try: PV = PVUtil.createPV(pvname, timeout)
    except:
        return False
    return PVUtil.getVType(PV).getValue()

def get_model(uutname):
    uut_model = get_pv_value("{}:0:MODEL".format(uutname))
    if uut_model:
        for model in model_def:
            if uut_model.startswith(model):
                return model
    return None

model_def = {
    'acq1001': {
        'sites': 1,
    },
    'acq1002': {
        'sites': 2,
    },
    'acq1102': {
        'sites': 2,
    },
    'acq2106': {
        'sites': 6,
    },
    'acq2206': {
        'sites': 6,
    },
    'z7io'   : {
        'sites': 2,
    },
}

# Main
logger.info("Start")

uutname = PVUtil.getString(pvs[0]).lower()
model = get_model(uutname)


if model:
    time.sleep(0.1)
    display.getEffectiveMacros().add('UUT', uutname)
    PVUtil.writePV('loc://MODEL', model, timeout)

    nsites = model_def[model]['sites']
    PVUtil.writePV('loc://NSITES', nsites, timeout)

    sites = {}
    sitelist = get_pv_value("{}:SITELIST".format(uutname))

    if sitelist:
        sites = {int(k): v for k, v in (site.split('=') for site in sitelist.split(',', 1)[1].split(','))}
        site_str = ','.join(map(str, sites.keys()))
        PVUtil.writePV('loc://SITES', site_str, timeout)

    for site in range(1, 6 + 1):
        pvname = "loc://SITE_{}_MODEL".format(site)
        value = sites[site] if site in sites else "none"
        PVUtil.writePV(pvname, str(value), timeout)

logger.info("Complete {:0.2f}s".format(time.time() - t0))
