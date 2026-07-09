importPackage(Packages.org.csstudio.opibuilder.scriptUtil);

function pad(ii, nchars)
{
	if (nchars == 2 && ii < 10){
		return "0"+ii;
	}else{
		return ""+ii;
	}
}


var flagName = "firstRun";
//Avoid running this script if the script is triggered during opi startup.
if(widgetController.getExternalObject(flagName) == null){
	widgetController.setExternalObject(flagName, false);	
}else{
	var macroInput = DataUtil.createMacrosInput(true);
	macroInput.put("UUT", PVUtil.getString(pvArray[0]));
	macroInput.put("SITE", PVUtil.getLong(pvArray[1]));	
       
	var ch = PVUtil.getLong(pvArray[2]);
	var chx = "CH";
	for (var nc = 1; nc <= 1; ++nc, ++ch){
		var ch0x = pad(ch, 2);
		macroInput.put("CH"+pad(nc, 2), ch0x);
		if (nc == 1){
			chx += ch0x;
		}	 		
	}
	
	var plot_time = 0;
	if (pvArray[3] != null){
		plot_time = PVUtil.getLong(pvArray[3]);
	}
	/* plot_time != 0, use embedded tbx */
	macroInput.put("tbx", plot_time != 0? "$(TB)": "");
	macroInput.put("xtitle", plot_time==0? "Samples": plot_time == 2? "Hz": "Seconds"); 
	macroInput.put("CHX", chx);
	       
	var opi = "./opi/" + widget.getPropertyValue("name") + ".opi";	       
	ScriptUtil.openOPI(widgetController,  opi, 0, macroInput);
}
