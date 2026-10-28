importPackage(Packages.org.csstudio.opibuilder.scriptUtil);

var flagName = "firstRun";
//Avoid running this script if the script is triggered during opi startup.
if(widgetController.getExternalObject(flagName) == null){
	widgetController.setExternalObject(flagName, false);	
}else{
	var macroInput = DataUtil.createMacrosInput(true);
	macroInput.put("UUT", PVUtil.getString(pvArray[0]));		
	macroInput.put("SITE", PVUtil.getLong(pvArray[1]));
		
	
	var typ = PVUtil.getString(pvArray[0]).split("_")[0];
	macroInput.put("TYPE", typ);
    var opi = "" + typ + widget.getPropertyValue("name") + ".opi";		
    
	ScriptUtil.openOPI(widgetController,  opi, 0, macroInput);
}
