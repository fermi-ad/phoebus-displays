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
	var suffix = ".len" + pvArray.length;
	macroInput.put("UUT", PVUtil.getString(pvArray[0]));
	macroInput.put("SITE", PVUtil.getLong(pvArray[1]));	

	var plot_time = 0;
	if (pvArray[3] != null){
		plot_time = PVUtil.getLong(pvArray[3]);
	}
	       
	// parseInt() is WRONG, but seems getLong was returning a string..
	var ch0 = parseInt(PVUtil.getLong(pvArray[4]));
	var ch1 = parseInt(PVUtil.getLong(pvArray[5]));
	var ref0 = parseInt(PVUtil.getLong(pvArray[6]));
	var ref1 = parseInt(PVUtil.getLong(pvArray[7]));
	if (pvArray.length > 8){
		suffix = PVUtil.getString(pvArray[8]);		
	}
	
	if (ref1 == ref0 && ch0 == ch1){
		ch1 += 7;
	}
	var chx = "CH";
	var nc = 1;
	for (var ch = ch0; ch <= ch1; ++ch){
		var ch0x = pad(ch, 2);
		for (var ref = ref0; ref <= ref1; ++ref, ++nc){
			macroInput.put("CH"+pad(nc, 2), "CH" + ch0x + ":REF" + ref + suffix);	
		}		 		
	}
	
	chx = "CH"+pad(ch0, 2);
	if (ch1 - ch0 > 0){
		chx = chx + "..CH"+pad(ch1, 2)
	}
	
	chx = chx + " REF"+ref0;
	if (ref1 - ref0 > 0){
		chx = chx + "..REF"+ref1;
	}
	
	

	/* plot_time != 0, use embedded tbx */
	macroInput.put("tbx", plot_time != 0? "$(TB)": "");
	macroInput.put("xtitle", plot_time==0? "Samples": plot_time == 2? "Hz": "Seconds"); 
	macroInput.put("CHX", chx);
	
	var opibase = widget.getPropertyValue("name");
	var opi = "./opi/" + opibase.replace("/__IR1/","") + ".opi";	       
	ScriptUtil.openOPI(widgetController,  opi, 0, macroInput);
}
