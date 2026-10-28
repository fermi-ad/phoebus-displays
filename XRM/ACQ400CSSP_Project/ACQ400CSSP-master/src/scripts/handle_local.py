from java.util.logging import Logger
from org.phoebus.framework.macros import Macros
from org.csstudio.display.builder.runtime.script import PVUtil

""" On local pv change update macros"""

# Startup
logger = Logger.getLogger('handle_local')
pvs = locals()['pvs']
widget = locals()['widget']

# Functions
class DotDict(dict):
    __delattr__ = dict.__delitem__
    __getattr__ = dict.__getitem__
    __setattr__ = dict.__setitem__

def get_macros(widget):
    macros = widget.getPropertyValue("macros")
    return DotDict({key: macros.getValue(key) for key in macros.names})

def set_macros(widget, macros):
    new_macros = Macros()
    for key in macros: new_macros.add(key, str(macros[key]))
    widget.setPropertyValue("macros", new_macros)

# Main
macros = get_macros(widget)
if 'OPI_KEYS' not in macros:
    macros.OPI_KEYS = ','.join(macros.keys())

locked = macros.OPI_KEYS.split(',')
for pv in pvs:
    key = pv.name.replace('loc://', '').split('_WD')[0]
    value = str(PVUtil.getString(pv))
    if key in locked: continue
    if macros.get(key, None) != value:
        macros[key] = value
        logger.info("Updated {} macro to {}".format(key, value))

set_macros(widget, macros)
