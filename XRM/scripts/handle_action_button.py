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
    return {key: macros.getValue(key) for key in macros.names}

def get_target(widget):
    action = str(widget.propActions()).split(' ')

    if len(action) > 2:
        action[2] = dict(kv.split('=') for kv in action[2].split('/'))
    else:
        action.append({})
    return action[0], action[1], action[2]

macros = {}
widget_macros = get_macros(widget)
macros.update(widget_macros)

global_widget = ScriptUtil.findWidgetByName(display, "GLOBAL_MACROS")
if global_widget:
    global_macros = get_macros(global_widget)
    macros.update(global_macros)

# Combine macros from display, widget and button
cmd, target, button_macros = get_target(widget)
macros.update(button_macros)

#macros['timestamp'] = str(int(time.time())) # remove?

logger.info('Pressed button {} {}'.format(cmd, target))
if cmd == 'open':
    ScriptUtil.openDisplay(display, target, "TAB", macros)