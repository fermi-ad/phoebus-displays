from java.util.logging import Logger
from org.csstudio.display.builder.runtime.script import PVUtil
from org.phoebus.framework.macros import Macros

""" On pv change update macros """

# Startup
logger = Logger.getLogger('handle_macro')
widget = locals()['widget']
pvs = locals()['pvs']
display = widget.getTopDisplayModel()

# Functions
def set_macros(widget, macros):
    new_macros = Macros()
    for key in macros:
        new_macros.add(key, str(macros[key]))
    widget.setPropertyValue("macros", new_macros)

# Main
macros = {}
for pv in pvs:
    key = pv.name.replace('loc://', '').split('_WD')[0]
    value = PVUtil.getString(pv)
    macros[key] = str(value)
    logger.info("Updated {} macro to {}".format(key, value))

set_macros(widget, macros)