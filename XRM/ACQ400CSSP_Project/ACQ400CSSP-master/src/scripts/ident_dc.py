from java.util.logging import Logger
from org.csstudio.display.builder.runtime.script import PVUtil, ScriptUtil

""" Ident all site DCs """

# Startup
logger = Logger.getLogger('ident_dc')
widget = locals()['widget']
display = widget.getTopDisplayModel()
timeout = 1000

# Functions
def get_macros(widget):
    macros = widget.getEffectiveMacros()
    return {key: macros.getValue(key) for key in macros.names}

# Main
nchan = PVUtil.getLong(ScriptUtil.getPrimaryPV(widget))
macros = get_macros(display)


pvfmts = [
    "{UUT}:{SITE}:AO:SLOW_SET:CH:{chan}",
    "{UUT}:{SITE}:AO:SLOW_SET:CH:{chan:02d}"
]
amount = 0.125
macros['chan'] = 1
try:
    pvfmt = pvfmts[0]
    pvname = pvfmt.format(**macros)
    PVUtil.writePV(pvname, amount, timeout)
except:
    pvfmt = pvfmts[1]
    pvname = pvfmt.format(**macros)
    PVUtil.writePV(pvname, amount, timeout)
print("{} set to {}".format(pvname, amount))

for chan in range(2, nchan + 1):
    amount += 0.125
    macros['chan'] = chan
    pvname = pvfmt.format(**macros)
    print("{} set to {}".format(pvname, amount))
    PVUtil.writePV(pvname, amount, timeout)