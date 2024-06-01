importPackage(Packages.org.csstudio.opibuilder.scriptUtil);

var flagName = "firstRun";
//Avoid running this script if the script is triggered during opi startup.
if(widgetController.getExternalObject(flagName) == null){
	widgetController.setExternalObject(flagName, false);	
}else{
	var macroInput = DataUtil.createMacrosInput(true);
	macroInput.put("SECS", PVUtil.getLong(pvArray[0]));
	macroInput.put("ETRG", PVUtil.getLong(pvArray[1]));
	macroInput.put("FCLK", PVUtil.getString(pvArray[2]));
	macroInput.put("STORE", PVUtil.getLong(pvArray[3]));		
	var opi = "" + widget.getPropertyValue("name") + ".opi";
    
	ScriptUtil.openOPI(widgetController,  opi, 0, macroInput);
}
