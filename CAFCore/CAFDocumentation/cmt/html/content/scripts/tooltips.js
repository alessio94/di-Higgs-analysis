function detectMobileBrowser(){ return (function(a){if(/(android|bb\d+|meego).+mobile|avantgo|bada\/|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od)|iris|kindle|lge |maemo|midp|mmp|netfront|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\/|plucker|pocket|psp|series(4|6)0|symbian|treo|up\.(browser|link)|vodafone|wap|windows (ce|phone)|xda|xiino/i.test(a)||/1207|6310|6590|3gso|4thp|50[1-6]i|770s|802s|a wa|abac|ac(er|oo|s\-)|ai(ko|rn)|al(av|ca|co)|amoi|an(ex|ny|yw)|aptu|ar(ch|go)|as(te|us)|attw|au(di|\-m|r |s )|avan|be(ck|ll|nq)|bi(lb|rd)|bl(ac|az)|br(e|v)w|bumb|bw\-(n|u)|c55\/|capi|ccwa|cdm\-|cell|chtm|cldc|cmd\-|co(mp|nd)|craw|da(it|ll|ng)|dbte|dc\-s|devi|dica|dmob|do(c|p)o|ds(12|\-d)|el(49|ai)|em(l2|ul)|er(ic|k0)|esl8|ez([4-7]0|os|wa|ze)|fetc|fly(\-|_)|g1 u|g560|gene|gf\-5|g\-mo|go(\.w|od)|gr(ad|un)|haie|hcit|hd\-(m|p|t)|hei\-|hi(pt|ta)|hp( i|ip)|hs\-c|ht(c(\-| |_|a|g|p|s|t)|tp)|hu(aw|tc)|i\-(20|go|ma)|i230|iac( |\-|\/)|ibro|idea|ig01|ikom|im1k|inno|ipaq|iris|ja(t|v)a|jbro|jemu|jigs|kddi|keji|kgt( |\/)|klon|kpt |kwc\-|kyo(c|k)|le(no|xi)|lg( g|\/(k|l|u)|50|54|\-[a-w])|libw|lynx|m1\-w|m3ga|m50\/|ma(te|ui|xo)|mc(01|21|ca)|m\-cr|me(rc|ri)|mi(o8|oa|ts)|mmef|mo(01|02|bi|de|do|t(\-| |o|v)|zz)|mt(50|p1|v )|mwbp|mywa|n10[0-2]|n20[2-3]|n30(0|2)|n50(0|2|5)|n7(0(0|1)|10)|ne((c|m)\-|on|tf|wf|wg|wt)|nok(6|i)|nzph|o2im|op(ti|wv)|oran|owg1|p800|pan(a|d|t)|pdxg|pg(13|\-([1-8]|c))|phil|pire|pl(ay|uc)|pn\-2|po(ck|rt|se)|prox|psio|pt\-g|qa\-a|qc(07|12|21|32|60|\-[2-7]|i\-)|qtek|r380|r600|raks|rim9|ro(ve|zo)|s55\/|sa(ge|ma|mm|ms|ny|va)|sc(01|h\-|oo|p\-)|sdk\/|se(c(\-|0|1)|47|mc|nd|ri)|sgh\-|shar|sie(\-|m)|sk\-0|sl(45|id)|sm(al|ar|b3|it|t5)|so(ft|ny)|sp(01|h\-|v\-|v )|sy(01|mb)|t2(18|50)|t6(00|10|18)|ta(gt|lk)|tcl\-|tdg\-|tel(i|m)|tim\-|t\-mo|to(pl|sh)|ts(70|m\-|m3|m5)|tx\-9|up(\.b|g1|si)|utst|v400|v750|veri|vi(rg|te)|vk(40|5[0-3]|\-v)|vm40|voda|vulc|vx(52|53|60|61|70|80|81|83|85|98)|w3c(\-| )|webc|whit|wi(g |nc|nw)|wmlb|wonu|x700|yas\-|your|zeto|zte\-/i.test(a.substr(0,4)))return true})(navigator.userAgent||navigator.vendor||window.opera); }

// determine the browser type 
var browser = undefined;
var chromium = false;
var firefox = false;
var opera = false;
var internetexplorer = false;
var browserwarning=true;
var mobilebrowser = detectMobileBrowser();
function determineBrowser(){
    var infobox=document.getElementById('browsername');
    if (/Firefox[\/\s](\d+\.\d+)/.test(navigator.userAgent)){ //test for Firefox/x.x or Firefox x.x (ignoring remaining digits);
	var ffversion=new Number(RegExp.$1) // capture x.x portion and store as a number
	if (ffversion>=5) browser = "Mozilla Firefox " + ffversion;
	else browser = "Mozilla Firefox ";
	firefox = true;
	if (ffversion>=16) browserwarning=false; 
    } else  if (/MSIE (\d+\.\d+);/.test(navigator.userAgent)){ //test for MSIE x.x;
	var ieversion=new Number(RegExp.$1) // capture x.x portion and store as a number
	if (ieversion>=5) browser = "Internet Explorer " + ieversion;
	else browser = "Internet Explorer";
	internetexplorer = true;
    } else if (/Opera[\/\s](\d+\.\d+)/.test(navigator.userAgent)){ //test for Opera/x.x or Opera x.x (ignoring remaining decimal places);
	var oprversion=new Number(RegExp.$1) // capture x.x portion and store as a number
	if (oprversion>=7) browser = "Opera "+oprversion;
	else browser = "Opera";
	opera = true;
    } else if (/Chrome[\/\s](\d+\.\d+)/.test(navigator.userAgent)){ //test for Opera/x.x or Opera x.x (ignoring remaining decimal places);
	var chrversion=new Number(RegExp.$1) // capture x.x portion and store as a number
	if(chrversion>=1) browser = "Chrome "+chrversion;
	else browser = "Google Chrome";
	chromium = true;
	if (chrversion>=23) browserwarning=false; 
    } else {
	console.log("could not detect browser!");
	console.log(navigator.userAgent);
    }
    if(browser){
	infobox.innerHTML="Du scheinst " + browser + " zu verwenden";
    }
    var warningbox=document.getElementById('browserwarning');
    if(!browserwarning){
	warningbox.style.display="none";
    } else {
	warningbox.style.display="block";
    }
}

function mousePosition(event){
    if(internetexplorer)
	return {
	    left: event.clientX + document.documentElement.scrollLeft,
	    top: event.clientY + document.documentElement.scrollTop
	}
    else 
	return {
	    left: event.pageX,
	    top: event.pageY
	}
}

function pageBox(){
    var y = 0;
    var x = 0;
    if (self.innerHeight){
        x = self.innerWidth;
        y = self.innerHeight;
    } else if (document.documentElement && document.documentElement.clientHeight){
        x = document.documentElement.clientWidth;
        y = document.documentElement.clientHeight;
    } else if (document.body){
        x = document.body.clientWidth;
        y = document.body.clientHeight;
    }
    return {x:x,y:y};
}

function boundingBox(elem) {
    var box = elem.getBoundingClientRect();
    var body = document.body;
    var docElem = document.documentElement;
    var scrollTop = window.pageYOffset || docElem.scrollTop || body.scrollTop;
    var scrollLeft = window.pageXOffset || docElem.scrollLeft || body.scrollLeft;
    var clientTop = docElem.clientTop || body.clientTop || 0;
    var clientLeft = docElem.clientLeft || body.clientLeft || 0;
    return { 
	top: Math.round(box.top +  scrollTop - clientTop), 
	left: Math.round(box.left + scrollLeft - clientLeft),
	bottom: Math.round(box.bottom +  scrollTop - clientTop), 
	right: Math.round(box.right +  scrollTop - clientTop), 
	width: Math.round(box.right - box.left),
	height: Math.round(box.bottom - box.top)
    };
}

var tooltip=function(){
    var id = 'tt';
    var top = 3;
    var left = 3;
    var maxw = 300;
    var speed = 20;
    var timer = 20;
    var endalpha = 95;
    var alpha = 0;
    var tt,t,c,b,height,width;
    return{
	show:function(v,w){
	    if(mobilebrowser) return;
	    if(!tt){
		tt = document.createElement('div');
		tt.setAttribute('id',id);
		tt.style.position="absolute";
		tt.style.paddingLeft="1em";
		tt.style.paddingRight="1em";
		tt.style.backgroundColor="white";
		tt.style.borderRadius="25px";
		tt.style.zIndex = 20;
		document.body.appendChild(tt);
	    }
	    tt.style.display = 'block';
	    if(!c){
		c = document.createElement('div');
		c.setAttribute('id',id + 'cont');
		tt.appendChild(c);
	    }
	    if(internetexplorer)
		tt.style.filter = 'alpha(opacity=0)';
	    else
		tt.style.opacity = 0;
	    document.onmousemove = this.pos;
	    c.innerHTML = v;
	    tt.style.width = w ? w + 'px' : 'auto';
	    if(!w && internetexplorer){
		t.style.display = 'none';
		b.style.display = 'none';
		tt.style.width = tt.offsetWidth;
		t.style.display = 'block';
		b.style.display = 'block';
	    }
	    height = parseInt(tt.offsetHeight) + top;
	    width = Math.min(parseInt(tt.offsetWidth),maxw);
	    clearInterval(tt.timer);
	    tt.timer = setInterval(function(){tooltip.fade(1)},timer);
	},
	pos:function(e){
	    var mpos = mousePosition(e);
	    console.log(mpos);
	    var bb = boundingBox(tt);
	    var limits = pageBox();
	    var rightmargin = 12; // width of scrollbar
	    if(mpos.top - height < 0)
		tt.style.top = mpos.top;
	    else
		tt.style.top = mpos.top -height;
	    if(mpos.left + width > limits.x - rightmargin)
		tt.style.left = mpos.left - width;
	    else
		tt.style.left = mpos.left;
	},
	fade:function(d){
	    if((alpha < endalpha && d > 0) || (alpha > 0 && d < 0)){
		var i = speed;
		if(endalpha - alpha < speed && d == 1){
		    i = endalpha - alpha;
		}else if(alpha < speed && d == -1){
		    i = alpha;
		}
		alpha = alpha + (i * d);
		tt.style.opacity = alpha * .01;
		if(internetexplorer)
		    tt.style.filter = 'alpha(opacity=' + alpha + ')';
	    } else {
		window.clearInterval(tt.timer);
		if(d < 0){
		    tt.style.display="none";
		}
	    }
	},
	hide:function(){
	    if(mobilebrowser) return;
	    clearInterval(tt.timer);
	    tt.timer = setInterval(function(){tooltip.fade(-1)},timer);
	},
	reset:function(){
	    alpha = 0;
	    d = -1;
	    if(tt){
		purge(tt);
		clearInterval(tt.timer);
	    }
	},
	make:function(title, content){
	    return (function(){tooltip.show("<b>"+title+"</b><br>"+content);});
	}
    };
}();
