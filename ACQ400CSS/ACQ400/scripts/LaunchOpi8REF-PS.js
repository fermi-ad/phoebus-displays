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
	macroInput.put("UUT", PVUtil.getString(pvArray[0]));						// [0] : UUT
	macroInput.put("SITE", PVUtil.getLong(pvArray[1]));						// [1] : SITE
																			// [2] : trigger (not used)
	var plot_time = 0;
	if (pvArray[3] != null){
		plot_time = PVUtil.getLong(pvArray[3]);								// [3] : PLOT TIME?
	}
	       
	// parseInt() is WRONG, but seems getLong was returning a string..
	var ch0 = parseInt(PVUtil.getLong(pvArray[4]));							// [4] : ch0
	var ch1 = parseInt(PVUtil.getLong(pvArray[5]));							// [5] : ch1 limit (or ch0+7 if ch1==ch0)
	var ref0 = parseInt(PVUtil.getLong(pvArray[6]));						// [6] : ref0
	var ref1 = parseInt(PVUtil.getLong(pvArray[7]));						// [7] : ref1 (usually ref==ref1 ie plot 1 ref)
	
	if (ref1 == ref0 && ch0 == ch1){
		ch1 += 7;
	}
	for (var ch = ch0, nc = 1; ch <= ch1; ++ch){
		var ch0x = pad(ch, 2);
		for (var ref = ref0; ref <= ref1; ++ref){
			var _pv = "CH"+ ch0x + ":REF" + ref;
			macroInput.put("CH"+pad(nc++, 2), _pv + ":PS");
		}		 		
	}
	
	var chx = "CH"+pad(ch0, 2);
	if (ch1 - ch0 > 0){
		chx = chx + "..CH"+pad(ch1, 2)
	}
	
	chx = chx + " REF"+ref0;
	if (ref1 - ref0 > 0){
		chx = chx + "..REF"+ref1;
	}
	//acq1001_084:DSP:LIA:REF:FREQ:1
	macroInput.put("REF", ref0);

	/* plot_time != 0, use embedded tbx */
	macroInput.put("tbx", plot_time != 0? "$(TB)": "");
	macroInput.put("xtitle", plot_time==0? "Samples": plot_time == 2? "Hz": "Seconds"); 
	macroInput.put("CHX", chx);
	
	var opibase = "" + widget.getPropertyValue("name");
	var name_ext = opibase.indexOf("__");
	if (name_ext > 0) opibase = opibase.substr(0, name_ext);
	var opi = "./opi/" + opibase + ".opi";	       
	ScriptUtil.openOPI(widgetController,  opi, 0, macroInput);
}
