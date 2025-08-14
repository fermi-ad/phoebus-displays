import time
from java.util.logging import Logger
from org.csstudio.display.builder.runtime.script import ScriptUtil

""" Action button handler """

# Startup
logger = Logger.getLogger('handle_action_button')
widget = locals()['widget']
display = widget.getTopDisplayModel()

#Functions
def get_macros(widget):
    macros = widget.getEffectiveMacros()
    extracted = {}
    for key in macros.names:
        extracted[key] = macros.getValue(key)
    return extracted

def get_target(widget, propname="tooltip"):
    prop = widget.getPropertyValue(propname).split(' ')
    if len(prop) > 2:
        prop[2] = dict(kv.split('=') for kv in prop[2].split('/'))
    else:
        prop.append({})
    return prop[0], prop[1], prop[2]

logger.info('Pressed {}'.format(widget))

cmd, target, macros = get_target(widget)
macros.update(get_macros(display)) #merge widget macros
macros.update(get_macros(widget)) #merge display macros
macros['timestamp'] = str(int(time.time()))

if cmd == 'open':
    
    #from pprint import pprint
    #pprint(macros)

    #open a new opi display
    opi = "opi/{}".format(target)
    ScriptUtil.openDisplay(display, opi, "TAB", macros)