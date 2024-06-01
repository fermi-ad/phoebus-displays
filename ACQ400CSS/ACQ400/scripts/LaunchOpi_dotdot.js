importPackage(Packages.org.csstudio.opibuilder.scriptUtil);

var flagName = "firstRun";
//Avoid running this script if the script is triggered during opi startup.
if(widgetController.getExternalObject(flagName) == null){
	widgetController.setExternalObject(flagName, false);	
}else{
	var macroInput = DataUtil.createMacrosInput(true);
	macroInput.put("UUT", PVUtil.getString(pvArray[0]));		
	macroInput.put("SITE", PVUtil.getLong(pvArray[1]));
		
	var opi = "" + widget.getPropertyValue("name") + ".opi";

// change for ACQ1014 breaks other launchers on UUT change
// revert to original, TODO must re-test ACQ1014 for breakage	
	if (true) { 
//	if (widgetController.getExternalObject(opi) == null){
		ScriptUtil.openOPI(widgetController,  opi, 0, macroInput);
		widgetController.setExternalObject(opi, false);
	}
}
