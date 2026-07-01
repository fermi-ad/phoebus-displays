import time
from java.util.logging import Logger
from org.phoebus.framework.macros import Macros
from org.csstudio.display.builder.runtime.script import PVUtil, ScriptUtil

""" Reloads display when UUT value changes """

# Startup
t0 = time.time()
logger = Logger.getLogger('handle_uut')
widget = locals()['widget']
pvs = locals()['pvs']
display = widget.getTopDisplayModel()
timeout = 2 * 1000

# Functions
class DotDict(dict):
    __delattr__ = dict.__delitem__
    __getattr__ = dict.__getitem__
    __setattr__ = dict.__setitem__

def get_macros(widget):
    macros = widget.getEffectiveMacros()
    return DotDict({key: macros.getValue(key) for key in macros.names})

def set_macros(widget, macros):
    new_macros = Macros()
    for key in macros: new_macros.add(key, str(macros[key]))
    widget.setPropertyValue("macros", new_macros)

# Main
macros = get_macros(display)
uut_val = PVUtil.getString(pvs[0])

if uut_val != '0':
    if macros.get('UUT') != uut_val:
        logger.info("Set UUT to {}".format(uut_val))
        macros.UUT = uut_val
        set_macros(display, macros)
        ScriptUtil.openDisplay(display, macros.LAUNCHER, "REPLACE", macros)