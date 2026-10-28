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
	var ch_max = ch+8;
	if (ch < 0){                // -5 : count 1..5 for 5chan device..
		ch_max = -ch;
		ch = 1;
	}
	var chx = "CH";
	for (var nc = 1; nc <= 8; ++nc, ++ch){
		if (ch > ch_max) ch = ch_max;         // hack: duplicate PV until end. saves barbies.
		var ch0x = pad(ch, 2);
		macroInput.put("CH"+pad(nc, 2), ch0x);
		if (nc == 1){
			chx += ch0x;
		}
		if (nc == 8){
			chx += ":" + ch0x;
		}		 		
	}
	
	var plot_time = 0;
	if (pvArray[3] != null){
		plot_time = PVUtil.getLong(pvArray[3]);
	}
	if (pvArray.length > 4){
		macroInput.put("WF", PVUtil.getString(pvArray[4]));
		macroInput.put("FN", PVUtil.getString(pvArray[5]));
		macroInput.put("FNY_TITLE", PVUtil.getString(pvArray[6]));
		if (pvArray.length >= 8 ){
			macroInput.put("CHX", PVUtil.getString(pvArray[7]) + " " + chx);
		}else{
			macroInput.put("CHX", PVUtil.getString(pvArray[5]).substring(0,3) + " " + chx);
		}
	}else{
		macroInput.put("CHX", chx);
	}
	/* plot_time != 0, use embedded tbx */
	macroInput.put("tbx", plot_time != 0? "$(TB)": "");
	macroInput.put("xtitle", plot_time==0? "Samples": plot_time == 2? "Hz": "Seconds"); 
	
	       
	var opibase = "" + widget.getPropertyValue("name");
	var name_ext = opibase.indexOf("__");
	if (name_ext > 0) opibase = opibase.substr(0, name_ext);
	var opi = "./opi/" + opibase + ".opi";	       
	ScriptUtil.openOPI(widgetController,  opi, 0, macroInput);	
}
