from java.io import File, FileInputStream
from org.yaml.snakeyaml import Yaml
from org.csstudio.display.builder.runtime.script import ScriptUtil
import os

display = widget.getTopDisplayModel()
source = display.getUserData(display.USER_DATA_INPUT_FILE)

display_file = File(str(source))
yaml_file = File(display_file.getParentFile(), "config.yaml")


widget.setPropertyValue("text", str(yaml_file).strip())


existing = widget.getTopDisplayModel().getEffectiveMacros()
initialized = widget.getTopDisplayModel().getEffectiveMacros().getValue("QPM_INITIALIZED")

#if str(initialized) == "1":
#    return

device = widget.getTopDisplayModel().getEffectiveMacros().getValue("DEVICE")
ioc_prefix = widget.getTopDisplayModel().getEffectiveMacros().getValue("IOC_PREFIX")

if not device:
    widget.setPropertyValue("text", "No device selected")
else:
    
    stream = FileInputStream(yaml_file)
    try:
        config = Yaml().load(stream)
    finally:
        stream.close()

    if device in config:
        qpc1 = config[device]["QPC1"]
        widget.setPropertyValue("text", str(qpc1))
    else:
        widget.setPropertyValue("text", "Unknown device: " + device)

ioc_prefix = str(ioc_prefix or "").strip()
device = str(device).strip()
ioc = ioc_prefix + device
macros = {"DEVICE": str(ioc)}
macros["DEV_NAME"] = str(device).upper().strip()
#macros["QPM_INITIALIZED"] = "1".strip()


for i in range(1, 7):
    key = "QPC%d" % i
    macros[key] = str(config[device][key])


ScriptUtil.openDisplay(
    widget,
    "main.bob",
    "REPLACE",
    macros
)
