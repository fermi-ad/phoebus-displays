from java.util.logging import Logger
from org.csstudio.display.builder.runtime.script import PVUtil

""" Hide or show traces from checkboxes """

# Startup
logger = Logger.getLogger('handle_trace_visibility')
widget = locals()['widget']
pvs = locals()['pvs']
display = widget.getTopDisplayModel()

# Functions

def get_bits(value):
    bits = [(value >> i) & 1 for i in range(8)]
    return bits

# Main
mask = get_bits(int(PVUtil.getLong(pvs[0])))
logger.info('Trace Visible {}'.format(mask))

for idx, bit in enumerate(mask):
    widget.setPropertyValue("traces[{}].visible".format(idx), int(bit))