from java.util.logging import Logger
from org.csstudio.display.builder.runtime.script import PVUtil

""" On local pv change update macros """

# Startup
logger = Logger.getLogger('handle_local')
pvs = locals()['pvs']
widget = locals()['widget']
display = widget.getTopDisplayModel()

# Functions
def get_macros(*widgets):
    class DotDict(dict):
        __getattr__ = dict.__getitem__
        __setattr__ = dict.__setitem__
    macros = DotDict()
    def macro_func(key, value): macros[key] = value
    for widget in widgets:
        widget.getEffectiveMacros().forEachSpec(macro_func)
    return macros


# Main
macros = get_macros(display, widget)
for pv in pvs:
    key = pv.name.replace('loc://', '').split('_WD')[0]
    value = str(PVUtil.getString(pv))
    if macros.get(key, None) != value:
        macros[key] = value
        logger.info("Updated {} macro to {}".format(key, value))
    
display.setPropertyValue("macros", macros)
