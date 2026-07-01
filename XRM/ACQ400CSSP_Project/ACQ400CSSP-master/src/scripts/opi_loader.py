import time
from java.util.logging import Logger
from org.phoebus.framework.macros import Macros
from org.csstudio.display.builder.runtime.script import PVUtil, ScriptUtil

#Convert PVs to macros and load OPI

t0 = time.time()
logger = Logger.getLogger("opi_loader")
widget = locals()['widget']
pvs = locals()['pvs']
display = widget.getTopDisplayModel()

def get_macros(widget):
    macros = widget.getEffectiveMacros()
    return {key: macros.getValue(key) for key in macros.names}

def set_macros(widget, macros):
    new_macros = Macros()
    for key in macros:
        new_macros.add(key, str(macros[key]))
    widget.setPropertyValue("macros", new_macros)

def convert_to_macros(pvs, macros, prefix):
    logger.info('macro prefix: {}'.format(prefix))
    for pv in pvs:
        try:
            pvname = pv.name.replace(prefix, '').replace(':', '_')
            if pvname[0].isdigit(): pvname = 's{}'.format(pvname)
            pvvalue = PVUtil.getVType(pv).getValue()
            macros[pvname] = pvvalue
            logger.info('new macro {} > ({}={})'.format(pv.name, pvname, pvvalue))
        except:
            logger.warning("Failed to convert PV {}".format(pv.name))
    return macros

macros = get_macros(widget)
prefix = macros.get('PREFIX', '{}:'.format(macros.get('UUT', 'uutname')))
macros = convert_to_macros(pvs, macros, prefix)

set_macros(widget, macros)
logger.info('load OPI: {}'.format(macros['FILE']))
widget.setPropertyValue("file", "")
widget.setPropertyValue("file", macros['FILE'])