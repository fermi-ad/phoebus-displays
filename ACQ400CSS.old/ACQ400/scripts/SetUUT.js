importPackage(Packages.org.csstudio.opibuilder.scriptUtil);

function pad(number, length) {
    var str = '' + number;
    while (str.length < length) {
        str = '0' + str;
    }   
    return str;
}

var flagName = "firstRun";
//Avoid running this script if the script is triggered during opi startup.
if(widgetController.getExternalObject(flagName) == null){
	widgetController.setExternalObject(flagName, false);	
}else{	
	var model = PVUtil.getString(pvArray[0]);		
	var sn = PVUtil.getLong(pvArray[1]);
	var uut = model+"_"+pad(sn, 3);
	widget.setPropertyValue("text", uut);
}
