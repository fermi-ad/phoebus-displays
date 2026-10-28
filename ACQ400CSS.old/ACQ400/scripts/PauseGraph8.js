importPackage(Packages.org.csstudio.opibuilder.scriptUtil);

var pause = display.getVar("pause");
var xygraph=display.getWidget("XY Graph");

if(pause == null || pause==false){
	display.setVar("pause", true);
	for (var tr = 0; tr < 8; ++tr){
		var key = "trace_"+tr+"_y_pv";
		display.setVar(key, xygraph.getPropertyValue(key));	
		xygraph.getPV(key).stop();
	}
	widget.setPropertyValue("text", "Resume");
}else{
	display.setVar("pause", false);
	for (var tr = 0; tr < 8; ++tr){
		var key = "trace_"+tr+"_y_pv";
		
		xygraph.setPropertyValue(key, display.getVar(key));
		xygraph.getPV(key).start();
	}
	widget.setPropertyValue("text", "Pause");
}

