importPackage(Packages.org.csstudio.opibuilder.scriptUtil);

var sitelist = PVUtil.getString(pvArray[0]);
var dx = PVUtil.getLong(pvArray[1]);
//loc://clktree_site_${SITE}x("[0-9]")
var re = new RegExp(PVUtil.getString(pvArray[2]), 'gm');

var site=dx-1;
// sitelist typ 21637,1=423,3=423,5=423
var sites = sitelist.split(",");
var isVisible = false;

for (var kv in sites) {
	var kva = sites[kv].split("=");
	if (site == kva[0]){
	    if (kva[1] && kva[1].search(re) > -1){
	    	isVisible = true;	    	
	    }
	    break;
	}
}

widget.setPropertyValue("visible", isVisible);
