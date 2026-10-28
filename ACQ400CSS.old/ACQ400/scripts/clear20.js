importPackage(Packages.org.csstudio.opibuilder.scriptUtil);


var flagName = "firstRun";
//Avoid running this script if the script is triggered during opi startup.
if(widgetController.getExternalObject(flagName) == null){
        widgetController.setExternalObject(flagName, false);
}else{
		var ii;
		for (ii = 0; ii < 20; ++ii){
        	pvArray[ii].setValue(0);
        }
}
