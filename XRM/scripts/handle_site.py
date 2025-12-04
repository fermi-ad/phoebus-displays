import time
from java.util.logging import Logger
from org.csstudio.display.builder.runtime.script import PVUtil, DataUtil
from org.phoebus.framework.macros import Macros

""" Updates the current site """

# Startup
logger = Logger.getLogger('handle_site')
widget = locals()['widget']
pvs = locals()['pvs']
display = widget.getTopDisplayModel()

# Main
site_pv, model_pv = pvs

site_val = PVUtil.getLong(site_pv)
model_val = display.getPropertyValue("macros").getValue('SITE_{}_MODEL'.format(site_val))

if model_val: model_pv.setValue(str(model_val))
display.getPropertyValue("macros").add("SITE", str(site_val))
display.getPropertyValue("macros").add("SITE_MODEL", str(model_val))
logger.info('Current Site: {} Model: {}'.format(site_val, model_val))
