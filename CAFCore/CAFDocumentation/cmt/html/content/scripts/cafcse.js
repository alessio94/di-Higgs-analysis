var callback = function() {		   
    var customSearchOptions = {};  
    if(cseid.length < 1)
	// this is the default cse that points to wwwhep.physik.uni-freiburg.de/~cburgard/CAF-doc
	// if you want to setup your documentation elsewhere, you need to create your own 
	// custom google search engine and save the ID in cseid.txt in this directory
	// it will then be read by the PHP script and used automatically
	cseid = "004966404739141243895:afc8onancwc"; 
    var customSearchControl = new google.search.CustomSearchControl(cseid,
								    customSearchOptions);
    customSearchControl.setResultSetSize(google.search.Search.FILTERED_CSE_RESULTSET);
    customSearchControl.draw('cse');
    if(query.length > 0)
	customSearchControl.execute(query);
};
google.load('search', '1', {language : 'en', style : google.loader.themes.V2_DEFAULT});
google.setOnLoadCallback(callback, true);
