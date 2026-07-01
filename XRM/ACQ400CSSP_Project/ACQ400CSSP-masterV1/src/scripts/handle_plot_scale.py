import time
from java.util.logging import Logger
from org.csstudio.display.builder.runtime.script import PVUtil

""" Set the plot max scale """

# Startup
logger = Logger.getLogger('handle_plot_scale')
widget = locals()['widget']
pvs = locals()['pvs']
display = widget.getTopDisplayModel()

plot_scale = PVUtil.getString(pvs[0])

autoscale = True
maximum = 11
minimum = 11

if plot_scale != 'AUTO':
    autoscale = False

if plot_scale == '10V':
    maximum = 10
    minimum = -10

if plot_scale == '5V':
    maximum = 5
    minimum = -5

if plot_scale == '2.5V':
    maximum = 2.5
    minimum = -2.5

if plot_scale == '1.25V':
    maximum = 1.25
    minimum = -1.25

logger.info("Setting plot scale to {}".format(plot_scale))
widget.setPropertyValue("y_axes[0].autoscale",  autoscale)
time.sleep(1) # Let autoscale take effect before setting max/min
widget.setPropertyValue("y_axes[0].maximum",  maximum)
widget.setPropertyValue("y_axes[0].minimum",  minimum)
