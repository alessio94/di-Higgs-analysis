var m_ = "mailto:";
var a_ = "@";
var d_ = ".";

function mail(name, dom){
    var s = e(name,dom);
    document.write('<a href="'+m_+s+'">'+s+'</a>');
}

function mail2(name, dom, display){
    document.write('<a href="'+m_+e(name,dom,display)+'">'+display+'</a>');
}

function e(name, dom){
    return swapper(name)+a_+swapper(dom);
}
function swapper(d){
    var s = "";
    for (var i=0; i<d.length; i+=2)
	if (i+1==d.length)
	    s+= d.charAt(i)
else
    s+= d.charAt(i+1)+d.charAt(i);
    return s.replace(/\?/g,'.');
}
